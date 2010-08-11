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

#ifdef _MSC_VER
#  pragma warning (disable : 4018)
#endif

#include <vtkAbstractPropPicker.h>
#include <vtkAssemblyPath.h>
#include <vtkCellPicker.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkImageData.h>
#include <vtkCornerAnnotation.h>
#include <vtkCamera.h>
#include <vtkMath.h>

#include "vtkViewImage2D.h"
#include "vtkInteractorStyleImage2D.h"

#include "vtkViewImage2DCommand.h"
#include "vtkInteractorStyleImage2D.h"

#include <string>
#include <sstream>

//----------------------------------------------------------------------------
vtkViewImage2DCommand::
vtkViewImage2DCommand() : Viewer(0)
  {
  }

//----------------------------------------------------------------------------
void
vtkViewImage2DCommand::
Execute(vtkObject*    caller,
                               unsigned long event,
                               void*         vtkNotUsed(callData))
{
  vtkInteractorStyleImage2D *isi =
    static_cast<vtkInteractorStyleImage2D *>(caller);

  if (!isi || !this->Viewer || !this->Viewer->GetInput())
    {
    return;
    }

  // Reset
  if (event == vtkCommand::ResetWindowLevelEvent)
    {
    this->Viewer->ResetWindowLevel();
    this->Viewer->Render();
    return;
    }
  // Reset
  if (event == vtkViewImage2DCommand::ResetViewerEvent)
    {
    this->Viewer->Reset();
    this->Viewer->Render();
    return;
    }

  // Start
  if (event == vtkCommand::StartWindowLevelEvent)
    {
    this->InitialWindow = this->Viewer->GetColorWindow();
    this->InitialLevel = this->Viewer->GetColorLevel();
    return;
    }

  // Adjust the window level here
  if (event == vtkCommand::WindowLevelEvent)
    {
    this->Windowing(isi);
    return;
    }

  if (event == vtkViewImage2DCommand::MeshPickingEvent)
    {
    //std::cout << "Mesh pick single" << std::endl;
    }

  if (event == vtkCommand::KeyPressEvent)
    {
    vtkRenderWindowInteractor *rwi =
      this->Viewer->GetRenderWindow()->GetInteractor();
    if (rwi->GetKeyCode() == 't')
      {
      this->Viewer->SetViewOrientation (
        (this->Viewer->GetViewOrientation() + 1) % 3);
      this->Viewer->Render();
      }
    else if (rwi->GetKeyCode() == 'i')
      {
      this->Viewer->SetInterpolate ((this->Viewer->GetInterpolate() + 1) % 2);
      this->Viewer->Render();
      }
    return;
    }

  if (event == vtkViewImage2DCommand::EndSliceMoveEvent)
    {
    int step = isi->GetSliceStep();
    this->Viewer->SetSlice (this->Viewer->GetSlice() + step);
    PrintInformation();
    }

  if (event == vtkViewImage2DCommand::ZoomEvent)
    {
    this->Zooming();
    }

  if (event == vtkViewImage2DCommand::PanEvent)
    {
    this->Panning();
    }

  // Move
  // Position Value requested
  if (event == vtkViewImage2DCommand::InteractionEvent)
    {
    PrintInformation();
    }

  // Position requested
  if (event == vtkViewImage2DCommand::RequestedPositionEvent)
    {
    double* position = this->Viewer->GetWorldCoordinatesFromDisplayPosition (
      isi->GetRequestedPosition ());
    this->Viewer->SetWorldCoordinates(position);
    this->Viewer->Render();
    }
}

//----------------------------------------------------------------------------
void
vtkViewImage2DCommand::
Windowing(vtkInteractorStyleImage2D* isi)
{
  int *  size = this->Viewer->GetRenderWindow()->GetSize();
  double window = this->InitialWindow;
  double level = this->InitialLevel;

  // Compute normalized delta
  double dx = 4.0 *
              (isi->GetWindowLevelCurrentPosition()[0] -
               isi->GetWindowLevelStartPosition()[0]) / size[0];
  double dy = 4.0 *
              (isi->GetWindowLevelStartPosition()[1] -
               isi->GetWindowLevelCurrentPosition()[1]) / size[1];

  // Scale by current values
  if (fabs(window) > 0.01) dx = dx * window;
  else
    dx = dx * (window < 0 ? -0.01 : 0.01);
  if (fabs(level) > 0.01) dy = dy * level;
  else
    dy = dy * (level < 0 ? -0.01 : 0.01);

  // Abs so that direction does not flip
  if (window < 0.0) dx = -1 * dx;
  if (level < 0.0) dy = -1 * dy;

  // Compute new window level
  double newWindow = dx + window;
  double newLevel = level - dy;

  // Stay away from zero and really
  if (fabs(newWindow) < 0.01) newWindow = 0.01 * (newWindow < 0 ? -1 : 1);
  if (fabs(newLevel) < 0.01) newLevel = 0.01 * (newLevel < 0 ? -1 : 1);

  this->Viewer->SetColorWindow(newWindow);
  this->Viewer->SetColorLevel(newLevel);
  this->Viewer->Render();
}

void vtkViewImage2DCommand::PrintInformation()
{

  vtkRenderWindowInteractor *rwi =
    this->Viewer->GetRenderWindow()->GetInteractor();

  double* pos = this->Viewer->GetWorldCoordinatesFromDisplayPosition (
    rwi->GetLastEventPosition ());

  int* idx = this->Viewer->GetImageCoordinatesFromWorldCoordinates(pos);

  int dims[3];
  this->Viewer->GetInput()->GetDimensions(dims);

  double spacing[3];
  this->Viewer->GetInput()->GetSpacing(spacing);

  std::ostringstream os;
  ///TODO dims and spacing must be computed from the orientation of the
  /// image.
  os << "Size: " << "[ " << dims[0] << " x " << dims[1] << " x " << dims[2]
     << " ]" << std::endl;
  os << "Pixel Size: "  << "[ " << spacing[0] << " x "
     << spacing[1] << " x " << spacing[2] << " ] um" << std::endl;
  os << "Slice: " << this->Viewer->GetSlice() << " / "
     << this->Viewer->GetSliceMax() - this->Viewer->GetSliceMin() << std::endl;
  this->Viewer->GetCornerAnnotation()->SetText (2, os.str().c_str());

  std::ostringstream os2;
  os2 << "Location: " << "[ " << pos[0] << " ; " << pos[1] << " ; "
      << pos[2]  << " ] um" << std::endl;

  os2 << "Index: " << "[ " << idx[0] << " ; " << idx[1] << " ; "
      << idx[2]  << " ]" << std::endl;

  if (!this->Viewer->GetIsColor())
    {
    os2 << "Value : " << this->Viewer->GetValueAtPosition (pos);
    }
  else
    {
    os2 << "Value : [";
    for (int i = 0; i < 3; i++)
      {
      os2 << this->Viewer->GetValueAtPosition (pos, i) << ", ";
      }
    os2 << "]";
    }

  this->Viewer->GetCornerAnnotation()->SetText (3, os2.str().c_str());
  this->Viewer->Render();

  // GetImageCoordinatesFromWorldCoordinates gives a pointer
  // to allocated but not managed memory : have to cleanup :
  delete[] idx;
}

void vtkViewImage2DCommand::Zooming()
{
  vtkRenderWindowInteractor *rwi =
    this->Viewer->GetRenderWindow()->GetInteractor();

  rwi->FindPokedRenderer(rwi->GetEventPosition()[0],
                         rwi->GetEventPosition()[1]);

  vtkRenderer* ren = this->Viewer->GetRenderer();

  int* size = ren->GetSize();
  int  dy = rwi->GetEventPosition()[1] - rwi->GetLastEventPosition()[1];

  double factor = 10. * static_cast<double>(dy) / static_cast<double>(size[1]);

  double z = pow(static_cast<double>(1.1), factor);

  this->Viewer->SetZoom(z);
}

//----------------------------------------------------------------------------
void
vtkViewImage2DCommand::
Panning()
{
  vtkRenderWindowInteractor *rwi =
    this->Viewer->GetRenderWindow()->GetInteractor();

  double ViewFocus[4], focalDepth, ViewPoint[3];
  double NewPickPoint[4], OldPickPoint[4], MotionVector[3];

  // Calculate the focal depth since we'll be using it a lot
  vtkRenderer* ren = this->Viewer->GetRenderer();
  vtkCamera*   camera = ren->GetActiveCamera();
  camera->GetFocalPoint(ViewFocus);
  vtkInteractorObserver::ComputeWorldToDisplay(ren,
                                               ViewFocus[0], ViewFocus[1], ViewFocus[2], ViewFocus);

  focalDepth = ViewFocus[2];

  vtkInteractorObserver::ComputeDisplayToWorld(ren,
                                               rwi->GetEventPosition()[0],
                                               rwi->GetEventPosition()[1],
                                               focalDepth,
                                               NewPickPoint);

  vtkInteractorObserver::ComputeDisplayToWorld(ren,
                                               rwi->GetLastEventPosition()[0],
                                               rwi->GetLastEventPosition()[1],
                                               focalDepth,
                                               OldPickPoint);

  // Camera motion is reversed
  MotionVector[0] = OldPickPoint[0] - NewPickPoint[0];
  MotionVector[1] = OldPickPoint[1] - NewPickPoint[1];
  MotionVector[2] = OldPickPoint[2] - NewPickPoint[2];

  // Get the current focal point and position

  camera->GetFocalPoint(ViewFocus);
  camera->GetPosition(ViewPoint);
  camera->SetFocalPoint(MotionVector[0] + ViewFocus[0],
                        MotionVector[1] + ViewFocus[1],
                        MotionVector[2] + ViewFocus[2]);
  camera->SetPosition(MotionVector[0] + ViewPoint[0],
                      MotionVector[1] + ViewPoint[1],
                      MotionVector[2] + ViewPoint[2]);

  this->Viewer->SetCameraMotionVector(MotionVector);
}
