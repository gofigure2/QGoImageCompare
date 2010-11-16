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

#include "vtkOrientedBoxWidget.h"
#include "vtkMatrixToLinearTransform.h"
#include "vtkObjectFactory.h"
#include "vtkMatrix4x4.h"
#include "vtkPoints.h"
#include "vtkActor.h"
#include "vtkAssemblyNode.h"
#include "vtkAssemblyPath.h"
#include "vtkCallbackCommand.h"
#include "vtkCamera.h"
#include "vtkCellArray.h"
#include "vtkCellPicker.h"
#include "vtkDoubleArray.h"
#include "vtkFloatArray.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "vtkPlanes.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkSphereSource.h"
#include "vtkTransform.h"

vtkCxxRevisionMacro(vtkOrientedBoxWidget, "$Revision: 490 $");
vtkStandardNewMacro(vtkOrientedBoxWidget);

//----------------------------------------------------------------------------
vtkOrientedBoxWidget::vtkOrientedBoxWidget()
{
  this->OrientationMatrix = vtkMatrix4x4::New();
  this->OrientationMatrix->Identity();

  this->InvertedOrientationMatrix = vtkMatrix4x4::New();
  this->InvertedOrientationMatrix->Identity();
}

//----------------------------------------------------------------------------
vtkOrientedBoxWidget::~vtkOrientedBoxWidget()
{
  this->OrientationMatrix->Delete();
  this->InvertedOrientationMatrix->Delete();
}

//----------------------------------------------------------------------------
void vtkOrientedBoxWidget::SetOrientationMatrix(vtkMatrix4x4 *matrix)
{
  if ( matrix == this->OrientationMatrix ) { return; }

  if ( this->OrientationMatrix )
    {
    this->OrientationMatrix->UnRegister (this);
    this->OrientationMatrix = NULL;
    }

  this->OrientationMatrix = matrix;

  if ( this->OrientationMatrix )
    {
    // move all the actors according to the user-matrix
    this->HexActor->SetUserMatrix (matrix);
    this->HexFace->SetUserMatrix (matrix);
    this->HexOutline->SetUserMatrix (matrix);

    for ( unsigned int i = 0; i < 7; i++ )
      {
      this->Handle[i]->SetUserMatrix (matrix);
      }

    // initialize inverse matrix
    vtkMatrix4x4::Invert (this->OrientationMatrix, this->InvertedOrientationMatrix);
    }
}

//----------------------------------------------------------------------------
void vtkOrientedBoxWidget::OnMouseMove()
{
  // See whether we're active
  if ( this->State == vtkBoxWidget::Outside
       || this->State == vtkBoxWidget::Start )
    {
    return;
    }

  int X = this->Interactor->GetEventPosition()[0];
  int Y = this->Interactor->GetEventPosition()[1];

  // Do different things depending on state
  // Calculations everybody does
  double focalPoint[4], pickPoint[4], prevPickPoint[4];
  double z, vpn[3];

  vtkCamera *camera = this->CurrentRenderer->GetActiveCamera();
  if ( !camera )
    {
    return;
    }

  // Compute the two points defining the motion vector
  this->ComputeWorldToDisplay(this->LastPickPosition[0],
                              this->LastPickPosition[1],
                              this->LastPickPosition[2], focalPoint);
  z = focalPoint[2];
  this->ComputeDisplayToWorld(
    static_cast< double >( this->Interactor->GetLastEventPosition()[0] ),
    static_cast< double >( this->Interactor->GetLastEventPosition()[1] ),
    z, prevPickPoint);
  this->ComputeDisplayToWorld(static_cast< double >( X ), static_cast< double >( Y ),
                              z, pickPoint);

  // multiply pick points by the inverted orientation matrix
  // to recover the true movement to apply
  this->InvertedOrientationMatrix->MultiplyPoint (prevPickPoint, prevPickPoint);
  this->InvertedOrientationMatrix->MultiplyPoint (pickPoint, pickPoint);

  // Process the motion
  if ( this->State == vtkBoxWidget::Moving )
    {
    // Okay to process
    if ( this->CurrentHandle )
      {
      if ( this->RotationEnabled && this->CurrentHandle == this->HexFace )
        {
        camera->GetViewPlaneNormal(vpn);
        this->Rotate(X, Y, prevPickPoint, pickPoint, vpn);
        }
      else if ( this->TranslationEnabled
                && this->CurrentHandle == this->Handle[6] )
        {
        this->Translate(prevPickPoint, pickPoint);
        }
      else if ( this->TranslationEnabled && this->ScalingEnabled )
        {
        if ( this->CurrentHandle == this->Handle[0] )
          {
          this->MoveMinusXFace(prevPickPoint, pickPoint);
          }
        else if ( this->CurrentHandle == this->Handle[1] )
          {
          this->MovePlusXFace(prevPickPoint, pickPoint);
          }
        else if ( this->CurrentHandle == this->Handle[2] )
          {
          this->MoveMinusYFace(prevPickPoint, pickPoint);
          }
        else if ( this->CurrentHandle == this->Handle[3] )
          {
          this->MovePlusYFace(prevPickPoint, pickPoint);
          }
        else if ( this->CurrentHandle == this->Handle[4] )
          {
          this->MoveMinusZFace(prevPickPoint, pickPoint);
          }
        else if ( this->CurrentHandle == this->Handle[5] )
          {
          this->MovePlusZFace(prevPickPoint, pickPoint);
          }
        }
      }
    }
  else if ( this->ScalingEnabled && this->State == vtkBoxWidget::Scaling )
    {
    this->Scale(prevPickPoint, pickPoint, X, Y);
    }

  // Interact, if desired
  this->EventCallbackCommand->SetAbortFlag(1);
  this->InvokeEvent(vtkCommand::InteractionEvent, NULL);
  this->Interactor->Render();
}
