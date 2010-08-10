/*========================================================================
 Copyright (c) INRIA - ASCLEPIOS Project (http://www-sop.inria.fr/asclepios).
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice,
 this list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.

 * Neither the name of INRIA or ASCLEPIOS, nor the names of any contributors
 may be used to endorse or promote products derived from this software
 without specific prior written permission.

 * Modified source versions must be plainly marked as such, and must not be
 misrepresented as being the original software.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS ``AS IS''
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
 ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 =========================================================================*/

/*=========================================================================
 Modifications were made by the GoFigure Dev. Team.
 while at Megason Lab, Systems biology, Harvard Medical school, 2009-10

 Copyright (c) 2009-10, President and Fellows of Harvard College.
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice,
 this list of conditions and the following disclaimer.
 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.
 Neither the name of the  President and Fellows of Harvard College
 nor the names of its contributors may be used to endorse or promote
 products derived from this software without specific prior written
 permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
 OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 =========================================================================*/

#include "vtkInteractorStyleImage2D.h"

#include "vtkAbstractPropPicker.h"
#include "vtkAssemblyPath.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "vtkRenderWindowInteractor.h"
#include <vtkImageData.h>
#include <vtkRenderWindow.h>
#include <vtkCamera.h>
#include <vtkRenderer.h>
#include "vtkViewImage2DCommand.h"

vtkCxxRevisionMacro (vtkInteractorStyleImage2D, "$Revision: 490 $");
vtkStandardNewMacro (vtkInteractorStyleImage2D);

//----------------------------------------------------------------------------
vtkInteractorStyleImage2D::
vtkInteractorStyleImage2D()
  {
  this->SliceStep = 0;
  this->RequestedPosition = new int[2];
  this->RequestedPosition[0] = this->RequestedPosition[1] = 0;

  this->LeftButtonInteraction   = InteractionTypeWindowLevel;
  this->RightButtonInteraction  = InteractionTypeZoom;
  this->MiddleButtonInteraction = InteractionTypePan;
  this->WheelButtonInteraction  = InteractionTypeSlice;

  this->m_PickingModeEnabled = false;
  }
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
vtkInteractorStyleImage2D::
~vtkInteractorStyleImage2D()
  {
  delete[] this->RequestedPosition;
  }
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void
vtkInteractorStyleImage2D::
OnMouseMove()
{
  this->DefaultMoveAction();

  switch (this->State)
    {
    case VTKIS_SLICE_MOVE:
      this->SliceMove();
      this->InvokeEvent(vtkViewImage2DCommand::SyncViewsEvent, this);
      break;
    case VTKIS_PAN:
      this->InvokeEvent(vtkViewImage2DCommand::PanEvent);
      break;
    case VTKIS_SPIN:
    case VTKIS_ROTATE:
    case VTKIS_DOLLY:
    case VTKIS_ZOOM:
      this->InvokeEvent(vtkViewImage2DCommand::ZoomEvent);
      break;
    case VTKIS_FORWARDFLY:
    case VTKIS_REVERSEFLY:
      this->InvokeEvent(vtkViewImage2DCommand::CameraMoveEvent, this);
      break;
    case VTKIS_PICK:
      HighlightCurrentActor();
      break;
    case VTKIS_WINDOW_LEVEL:
      this->Superclass::OnMouseMove();
      // Send event to update Scalar bar in 3D view
      this->InvokeEvent(vtkViewImage2DCommand::WindowLevelEvent, NULL);
      break;
    default:
      this->Superclass::OnMouseMove();
      break;
    }

  //Update image information (pixel position and value)
  this->InvokeEvent(vtkViewImage2DCommand::InteractionEvent, this);
}

//----------------------------------------------------------------------------
void
vtkInteractorStyleImage2D::
OnLeftButtonDown()
{
  int x = this->Interactor->GetEventPosition()[0];
  int y = this->Interactor->GetEventPosition()[1];
  this->FindPokedRenderer(x, y);

  if (this->Interactor->GetRepeatCount())
    {
    this->RequestedPosition[0] = x;
    this->RequestedPosition[1] = y;
    this->InvokeEvent (vtkViewImage2DCommand::RequestedPositionEvent);
    return;
    }

  if (this->Interactor->GetShiftKey() || this->Interactor->GetControlKey())
    {
    if (this->GetLeftButtonInteraction() == InteractionTypeWindowLevel)
      {
      this->StartSliceMove();
      }
    }
  else if (this->State == VTKIS_PICK)
    {
    this->InvokeEvent(vtkViewImage2DCommand::ContourPickingEvent);
    this->Superclass::OnLeftButtonDown();
    }
  else
    {
    switch (this->GetLeftButtonInteraction())
      {
      case InteractionTypeSlice:
        this->Superclass::OnLeftButtonDown();
        break;
      case InteractionTypeWindowLevel:
        this->Superclass::OnLeftButtonDown();
        break;
      case InteractionTypeZoom:
        this->InvokeEvent(vtkViewImage2DCommand::ZoomEvent);
        this->Superclass::OnRightButtonDown();
        break;
      case InteractionTypePan:
        this->InvokeEvent(vtkViewImage2DCommand::PanEvent);
        this->Superclass::OnMiddleButtonDown();
        break;
      case InteractionTypeSeed:
        this->InvokeEvent(vtkViewImage2DCommand::SeedEvent);
        this->Superclass::OnLeftButtonDown();
        break;
      default:
        this->Superclass::OnLeftButtonDown();
        break;
      }
    }

  // Call parent to handle all other states and perform additional work
  this->Superclass::OnLeftButtonDown();

}

//----------------------------------------------------------------------------
void
vtkInteractorStyleImage2D::
OnLeftButtonUp()
{
  switch (this->State)
    {
    case VTKIS_SLICE_MOVE:
      this->EndSliceMove();
      break;
    default:
      break;
    }

  switch (this->LeftButtonInteraction)
    {
    case InteractionTypeSlice:
      this->Superclass::OnLeftButtonUp();
      break;
    case InteractionTypeZoom:
      this->Superclass::OnRightButtonDown();
      break;
    case InteractionTypePan:
      this->Superclass::OnMiddleButtonDown();
      break;
    case InteractionTypeWindowLevel:
      this->Superclass::OnLeftButtonUp();
      break;
    default:
      this->Superclass::OnLeftButtonUp();
      break;
    }

  // Call parent to handle all other states and perform additional work
  this->Superclass::OnLeftButtonUp();
}

//----------------------------------------------------------------------------
void
vtkInteractorStyleImage2D::
OnMiddleButtonDown()
{
  // bool to check
  if (this->m_PickingModeEnabled)
    {
    this->State = VTKIS_PAN;
    }

  int x = this->Interactor->GetEventPosition()[0];
  int y = this->Interactor->GetEventPosition()[1];
  this->FindPokedRenderer(x, y);

  if (this->Interactor->GetShiftKey() || this->Interactor->GetControlKey())
    {
    if (this->GetLeftButtonInteraction() == InteractionTypeWindowLevel)
      {
      this->StartSliceMove();
      }
    }
  else
    {
    switch (this->GetMiddleButtonInteraction())
      {
      case InteractionTypeSlice:
        this->Superclass::OnLeftButtonDown();
        break;
      case InteractionTypeWindowLevel:
        this->Superclass::OnLeftButtonDown();
        break;
      case InteractionTypeZoom:
        this->InvokeEvent(vtkViewImage2DCommand::ZoomEvent);
        this->Superclass::OnRightButtonDown();
        break;
      case InteractionTypePan:
        this->InvokeEvent(vtkViewImage2DCommand::PanEvent);
        this->Superclass::OnMiddleButtonDown();
        break;
      default:
        this->Superclass::OnMiddleButtonDown();
        break;
      }
    }

  // Call parent to handle all other states and perform additional work
  this->Superclass::OnMiddleButtonDown();
}

//----------------------------------------------------------------------------
void
vtkInteractorStyleImage2D::
OnMiddleButtonUp()
{
  // bool to check
  if (this->m_PickingModeEnabled)
    {
    this->State = VTKIS_PICK;
    }

  switch (this->State)
    {
    case VTKIS_SLICE_MOVE:
      this->EndSliceMove();
      break;
    default:
      break;
    }

  switch (this->MiddleButtonInteraction)
    {
    case InteractionTypeSlice:
      this->Superclass::OnLeftButtonDown();
      break;
    case InteractionTypeZoom:
      this->Superclass::OnRightButtonUp();
      break;
    case InteractionTypePan:
      this->Superclass::OnMiddleButtonUp();
      break;
    case InteractionTypeWindowLevel:
      break;
    default:
      this->Superclass::OnMiddleButtonUp();
      break;
    }

  // Call parent to handle all other states and perform additional work
  this->Superclass::OnMiddleButtonUp();
}

//----------------------------------------------------------------------------
void
vtkInteractorStyleImage2D::
OnRightButtonDown()
{
  // bool to check
  if (this->m_PickingModeEnabled)
    {
    this->State = VTKIS_ZOOM;
    }

  //change state to zoom
  switch (this->GetRightButtonInteraction())
    {
    case InteractionTypeSlice:
      this->StartSliceMove();
      this->Superclass::OnLeftButtonDown();
      break;
    case InteractionTypeWindowLevel:
      this->Superclass::OnLeftButtonDown();
      break;
    case InteractionTypeZoom:
      // change state
      this->InvokeEvent(vtkViewImage2DCommand::ZoomEvent);
      this->Superclass::OnRightButtonDown();
      // renable state
      break;
    case InteractionTypePan:
      this->InvokeEvent(vtkViewImage2DCommand::PanEvent);
      this->Superclass::OnMiddleButtonDown();
      break;
    default:
      this->Superclass::OnRightButtonDown();
      break;
    }

  // Call parent to handle all other states and perform additional work
  this->Superclass::OnRightButtonDown();
}

//----------------------------------------------------------------------------
void
vtkInteractorStyleImage2D::
OnRightButtonUp()
{
  if (this->m_PickingModeEnabled)
    {
    this->State = VTKIS_PICK;
    }

  switch (this->State)
    {
    case VTKIS_SLICE_MOVE:
      this->EndSliceMove();
      break;
    // To avoid going into the super class we need the following return
    case VTKIS_PICK:
      return;
    default:
      break;
    }

  switch (this->RightButtonInteraction)
    {
    case InteractionTypeSlice:
      this->Superclass::OnLeftButtonUp();
      break;
    case InteractionTypePan:
      this->Superclass::OnMiddleButtonUp();
      break;
    case InteractionTypeWindowLevel:
      this->Superclass::OnLeftButtonUp();
      break;
    case InteractionTypeZoom:
      this->Superclass::OnRightButtonUp();
      break;
    default:
      this->Superclass::OnRightButtonUp();
      break;
    }

  // Call parent to handle all other states and perform additional work
  this->Superclass::OnRightButtonUp();
}

//----------------------------------------------------------------------------
void
vtkInteractorStyleImage2D::
OnMouseWheelForward()
{

  int x = this->Interactor->GetEventPosition()[0];
  int y = this->Interactor->GetEventPosition()[1];

  this->FindPokedRenderer(x, y);
  if (!this->CurrentRenderer)
    {
    return;
    }

  switch (this->GetWheelButtonInteraction())
    {
    case InteractionTypeSlice:
      this->StartSliceMove();
      this->SliceStep = static_cast<int>(this->MouseWheelMotionFactor);
      this->SliceMove();
      this->EndSliceMove();
      break;
    case InteractionTypeWindowLevel:
      break;
    case InteractionTypeZoom:
      break;
    case InteractionTypePan:
      break;
    default:
      this->Superclass::OnMouseWheelForward();
      break;
    }
}

//----------------------------------------------------------------------------
void
vtkInteractorStyleImage2D::
OnMouseWheelBackward()
{
  int x = this->Interactor->GetEventPosition()[0];
  int y = this->Interactor->GetEventPosition()[1];

  this->FindPokedRenderer(x, y);
  if (!this->CurrentRenderer)
    {
    return;
    }

  switch (this->GetWheelButtonInteraction())
    {
    case InteractionTypeSlice:
      this->StartSliceMove();
      this->SliceStep = static_cast<int>(-this->MouseWheelMotionFactor);
      this->SliceMove();
      this->EndSliceMove();
      break;
    case InteractionTypeWindowLevel:
      break;
    case InteractionTypeZoom:
      break;
    case InteractionTypePan:
      break;
    default:
      this->Superclass::OnMouseWheelBackward();
      break;
    }
}

//----------------------------------------------------------------------------
void
vtkInteractorStyleImage2D::
OnChar()
{
  vtkRenderWindowInteractor *rwi = this->Interactor;

  if (!strcmp (rwi->GetKeySym(), "Up"))
    {
    this->SliceStep = 1;
    this->StartSliceMove();
    this->SliceStep = static_cast<int>(-this->MouseWheelMotionFactor);
    this->SliceMove();
    this->EndSliceMove();
    }
  else if (!strcmp (rwi->GetKeySym(), "Down"))
    {
    this->SliceStep = -1;
    this->StartSliceMove();
    this->SliceMove();
    this->EndSliceMove();
    }
  else if ((rwi->GetKeyCode() == 'r') || (rwi->GetKeyCode() == 'R'))
    {
    this->InvokeEvent (vtkViewImage2DCommand::ResetWindowLevelEvent, this);
    }
  else if ((rwi->GetKeyCode() == 'o') || (rwi->GetKeyCode() == 'O'))
    {
    this->InvokeEvent (vtkViewImage2DCommand::ResetViewerEvent, this);
    }

  this->Superclass::OnChar();
}

//----------------------------------------------------------------------------
void
vtkInteractorStyleImage2D::
OnKeyUp()
{
  vtkRenderWindowInteractor *rwi = this->Interactor;

  if (!strcmp (rwi->GetKeySym(), "Up"))
    {
    this->SliceStep = 1;
    this->StartSliceMove();
    this->SliceMove();
    this->EndSliceMove();
    }
  else if (!strcmp (rwi->GetKeySym(), "Down"))
    {
    this->SliceStep = -1;
    this->StartSliceMove();
    this->SliceMove();
    this->EndSliceMove();
    }
  else if (!strcmp (rwi->GetKeySym(), "o"))
    {
    this->InvokeEvent (vtkViewImage2DCommand::ResetViewerEvent, this);
    }
  else if (!strcmp (rwi->GetKeySym(), "r"))
    {
    this->InvokeEvent (vtkViewImage2DCommand::ResetWindowLevelEvent, this);
    }

  this->Superclass::OnKeyUp();
}

//----------------------------------------------------------------------------
void
vtkInteractorStyleImage2D::
OnKeyPress()
{
  this->Superclass::OnKeyPress();
}

//----------------------------------------------------------------------------
void
vtkInteractorStyleImage2D::
OnKeyRelease()
{
  this->Superclass::OnKeyRelease();
}

//----------------------------------------------------------------------------
void
vtkInteractorStyleImage2D::
StartSliceMove()
{
  if ((this->State != VTKIS_NONE) && (this->State != VTKIS_PICK))
    {
    return;
    }
  this->StartState(VTKIS_SLICE_MOVE);
  this->InvokeEvent(vtkViewImage2DCommand::StartSliceMoveEvent, this);
}

//----------------------------------------------------------------------------
void
vtkInteractorStyleImage2D::
EndSliceMove()
{
  if (this->State != VTKIS_SLICE_MOVE)
    {
    return;
    }
  this->StopState();
  this->InvokeEvent(vtkViewImage2DCommand::EndSliceMoveEvent, this);
  // Call one more time to update views...
  this->InvokeEvent(vtkViewImage2DCommand::SyncViewsEvent, this);
}

//----------------------------------------------------------------------------
void
vtkInteractorStyleImage2D::
SliceMove()
{
  if (this->State != VTKIS_SLICE_MOVE)
    {
    return;
    }
  this->InvokeEvent(vtkViewImage2DCommand::SyncViewsEvent, this);
}

//----------------------------------------------------------------------------
void
vtkInteractorStyleImage2D::
OnKeyDown(void)
{
  // Apparently there is a problem here.
  // The event vtkCommand::CharEvent and vtkCommand::KeyPressEvent seem
  // to mix between each other.
  // tackled by calling the charevent
  // (supposed to be called at any keyboard event)
  this->OnChar();
  this->Superclass::OnKeyDown();
}

//----------------------------------------------------------------------------
void
vtkInteractorStyleImage2D::
DefaultMoveAction()
{
  this->InvokeEvent (vtkViewImage2DCommand::DefaultMoveEvent, this);
}

//----------------------------------------------------------------------------
void
vtkInteractorStyleImage2D::
SetLeftButtonInteraction(InteractionTypeIds interactionType)
{
  LeftButtonInteraction = interactionType;
}

//----------------------------------------------------------------------------
void
vtkInteractorStyleImage2D::
SetRightButtonInteraction(InteractionTypeIds interactionType)
{
  RightButtonInteraction = interactionType;
}

//----------------------------------------------------------------------------
void
vtkInteractorStyleImage2D::
SetMiddleButtonInteraction(InteractionTypeIds interactionType)
{
  MiddleButtonInteraction = interactionType;
}

//----------------------------------------------------------------------------
void
vtkInteractorStyleImage2D::
SetWheelButtonInteraction(InteractionTypeIds interactionType)
{
  WheelButtonInteraction = interactionType;
}

//----------------------------------------------------------------------------
vtkProp*
vtkInteractorStyleImage2D::
GetCurrentProp()
{
  return this->CurrentProp;
}

//----------------------------------------------------------------------------
void
vtkInteractorStyleImage2D::
HighlightCurrentActor()
{
  vtkRenderWindowInteractor *rwi = this->Interactor;
  if (this->CurrentRenderer != 0)
    {
    vtkAssemblyPath *path = NULL;
    int *            eventPos = rwi->GetEventPosition();
    this->FindPokedRenderer(eventPos[0], eventPos[1]);
    rwi->StartPickCallback();
    vtkAbstractPropPicker *picker =
      vtkAbstractPropPicker::SafeDownCast(rwi->GetPicker());
    if (picker != NULL)
      {
      picker->Pick(eventPos[0], eventPos[1],
                   0.0, this->CurrentRenderer);
      path = picker->GetPath();
      }
    if (path == NULL)
      {
      this->HighlightProp(NULL);
      this->PropPicked = 0;
      }
    else
      {
      //  if selected actor is not one of the 4 first:
      // should check some how if it is a mesh cut or not....
      //if(  )
      //  {
      //  this->HighlightProp( NULL );
      //  this->PropPicked = 0;
      //  }
      //else
      //  {
      this->HighlightProp(path->GetFirstNode()->GetViewProp());
      this->PropPicked = 1;
      //  }
      }
    rwi->EndPickCallback();
    }
}
//----------------------------------------------------------------------------
void
vtkInteractorStyleImage2D::
StartPick()
{
  this->m_PickingModeEnabled = true;
  this->Superclass::StartPick();
}
//----------------------------------------------------------------------------
void
vtkInteractorStyleImage2D::
EndPick()
{
  this->HighlightProp(NULL);
  this->PropPicked = 0;
  this->Superclass::EndPick();
  this->m_PickingModeEnabled = false;
}
