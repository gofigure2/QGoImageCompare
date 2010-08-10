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

#include "vtkInteractorStyleImage3D.h"

#include <vtkObjectFactory.h>
#include <vtkRenderWindowInteractor.h>
#include "vtkViewImage3DCommand.h"

#include <vtkProp.h>

// For picking
#include "vtkAbstractPropPicker.h"
#include "vtkAssemblyPath.h"

vtkCxxRevisionMacro (vtkInteractorStyleImage3D, "$Revision: 1 $");
vtkStandardNewMacro (vtkInteractorStyleImage3D);

//----------------------------------------------------------------------------
vtkInteractorStyleImage3D::
vtkInteractorStyleImage3D()
  {
  m_EnablePickMode = false;
  m_EnableZoomMode = false;
  m_EnablePanMode = false;

  this->LeftButtonInteraction   = 0;
  this->RightButtonInteraction  = 0;
  this->MiddleButtonInteraction = 0;
  this->WheelButtonInteraction  = 0;
  }

//----------------------------------------------------------------------------
vtkInteractorStyleImage3D::
~vtkInteractorStyleImage3D()
  {
  }

//----------------------------------------------------------------------------
void
vtkInteractorStyleImage3D::
OnMouseMove()
{
  switch (this->State)
    {
    case VTKIS_PICK3D:
      HighlightCurrentActor();
      break;
    }

  this->Superclass::OnMouseMove();
}

//----------------------------------------------------------------------------
void
vtkInteractorStyleImage3D::
OnLeftButtonDown()
{
  // if object is picked, send the event
  if (m_EnablePickMode)
    {
    //if no actor selected = change state
    if (this->PropPicked == 0) this->StopState();
    }

  if (m_EnableZoomMode)
    {
    this->Superclass::OnRightButtonDown();
    return;
    }

  if (m_EnablePanMode)
    {
    this->Superclass::OnMiddleButtonDown();
    return;
    }

  if (this->State == VTKIS_PICK3D)
    {
    this->SetCurrentProp();
    this->InvokeEvent(vtkViewImage3DCommand::MeshPickingEvent);
    return;
    }

  // Call parent to handle all other states and perform additional work
  this->Superclass::OnLeftButtonDown();
}

//----------------------------------------------------------------------------
void
vtkInteractorStyleImage3D::
OnLeftButtonUp()
{
  // if object is picked, send the event
  if (m_EnablePickMode)
    {
    //if no actor selected = change state
    this->StartState(VTKIS_PICK3D);
    }

  if (m_EnableZoomMode)
    {
    this->Superclass::OnRightButtonUp();
    return;
    }

  if (m_EnablePanMode)
    {
    this->Superclass::OnMiddleButtonUp();
    return;
    }

  // Call parent to handle all other states and perform additional work
  this->Superclass::OnLeftButtonUp();
}

//----------------------------------------------------------------------------
void
vtkInteractorStyleImage3D::
OnRightButtonDown()
{
  // if object is picked, send the event
  if (m_EnablePickMode)
    {
    this->StopState();
    }

  if (m_EnablePanMode)
    {
    this->Superclass::OnMiddleButtonDown();
    return;
    }

  // Call parent to handle all other states and perform additional work
  this->Superclass::OnRightButtonDown();
}

//----------------------------------------------------------------------------
void
vtkInteractorStyleImage3D::
OnRightButtonUp()
{
  // if object is picked, send the event
  if (m_EnablePickMode)
    {
    //if no actor selected = change state
    this->StartState(VTKIS_PICK3D);
    }

  if (m_EnablePanMode)
    {
    this->Superclass::OnMiddleButtonUp();
    return;
    }

  // Call parent to handle all other states and perform additional work
  this->Superclass::OnRightButtonUp();
}

//----------------------------------------------------------------------------
void
vtkInteractorStyleImage3D::
OnMiddleButtonDown()
{
  // if object is picked, send the event
  if (m_EnablePickMode)
    {
    this->StopState();
    }

  if (m_EnableZoomMode)
    {
    this->Superclass::OnRightButtonDown();
    return;
    }

  // Call parent to handle all other states and perform additional work
  this->Superclass::OnMiddleButtonDown();
}

//----------------------------------------------------------------------------
void
vtkInteractorStyleImage3D::
OnMiddleButtonUp()
{
  // if object is picked, send the event
  if (m_EnablePickMode)
    {
    //if no actor selected = change state
    this->StartState(VTKIS_PICK3D);
    }

  if (m_EnableZoomMode)
    {
    this->Superclass::OnRightButtonUp();
    return;
    }

  // Call parent to handle all other states and perform additional work
  this->Superclass::OnMiddleButtonUp();
}

//----------------------------------------------------------------------------
void
vtkInteractorStyleImage3D::
SetLeftButtonInteraction(InteractionTypeIds interactionType)
{
  LeftButtonInteraction = interactionType;
}

//----------------------------------------------------------------------------
void
vtkInteractorStyleImage3D::
SetRightButtonInteraction(InteractionTypeIds interactionType)
{
  RightButtonInteraction = interactionType;
}

//----------------------------------------------------------------------------
void
vtkInteractorStyleImage3D::
SetMiddleButtonInteraction(InteractionTypeIds interactionType)
{
  MiddleButtonInteraction = interactionType;
}

//----------------------------------------------------------------------------
void
vtkInteractorStyleImage3D::
SetWheelButtonInteraction(InteractionTypeIds interactionType)
{
  WheelButtonInteraction = interactionType;
}

//----------------------------------------------------------------------------
void
vtkInteractorStyleImage3D::
SetCurrentProp()
{
  this->m_CurrentProp = this->CurrentProp;
}

//----------------------------------------------------------------------------
vtkProp*
vtkInteractorStyleImage3D::
GetCurrentProp()
{
  return this->m_CurrentProp;
}

//----------------------------------------------------------------------------
void
vtkInteractorStyleImage3D::
StartPick()
{
  if (this->State != VTKIS_NONE)
    {
    return;
    }
  this->StartState(VTKIS_PICK3D);
  this->InvokeEvent(vtkCommand::StartPickEvent, this);
}

//----------------------------------------------------------------------------
void
vtkInteractorStyleImage3D::
EndPick()
{
  // Remove boxes in view
  this->HighlightProp(NULL);
  this->PropPicked = 0;
  this->InvokeEvent(vtkCommand::EndPickEvent, this);
  this->StopState();
}
//----------------------------------------------------------------------------
void
vtkInteractorStyleImage3D::
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
      // Check dimensionality
      double* bounds = path->GetFirstNode()->GetViewProp()->GetBounds();
      if (bounds[0] != bounds[1] && bounds[2] != bounds[3] && bounds[4] != bounds[5])
        {
        this->HighlightProp(path->GetFirstNode()->GetViewProp());
        this->PropPicked = 1;
        }
      else
        {
        this->HighlightProp(NULL);
        this->PropPicked = 0;
        }
      }
    rwi->EndPickCallback();
    }
}
/// TODO improve it
//----------------------------------------------------------------------------
void
vtkInteractorStyleImage3D::
EnablePickMode()
{
  m_EnablePickMode = true;
}

//----------------------------------------------------------------------------
void
vtkInteractorStyleImage3D::
DisablePickMode()
{
  m_EnablePickMode = false;
}
//----------------------------------------------------------------------------
void
vtkInteractorStyleImage3D::
EnableZoomMode()
{
  m_EnableZoomMode = true;
  m_EnablePanMode = false;
}

//----------------------------------------------------------------------------
void
vtkInteractorStyleImage3D::
EnablePanMode()
{
  m_EnablePanMode = true;
  m_EnableZoomMode = false;
}

//----------------------------------------------------------------------------
void
vtkInteractorStyleImage3D::
EnableDefaultMode()
{
  m_EnablePanMode = false;
  m_EnableZoomMode = false;
  m_EnablePickMode = false;
}
