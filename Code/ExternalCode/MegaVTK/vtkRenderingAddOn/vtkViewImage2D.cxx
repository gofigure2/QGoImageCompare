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

#include "vtkViewImage2D.h"

#include "vtkSmartPointer.h"

#include "vtkCamera.h"
#include "vtkCommand.h"
#include "vtkImageActor.h"
#include "vtkImageData.h"
#include "vtkImageMapToWindowLevelColors.h"
#include "vtkInteractorStyleImage.h"
#include "vtkObjectFactory.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkMatrix4x4.h"
#include "vtkTransform.h"
#include "vtkScalarBarActor.h"
#include "vtkOrientationAnnotation.h"
#include "vtkCornerAnnotation.h"
#include "vtkTextProperty.h"
#include "vtkLookupTable.h"
#include "vtkMath.h"
#include "vtkPlane.h"
#include "vtkCutter.h"
// #include "vtkQuadricLODActor.h"
#include "vtkActor.h"
#include "vtkPolyDataMapper.h"
#include "vtkProp3DCollection.h"
#include "vtkPoints.h"
#include "vtkIdList.h"
#include "vtkOutlineSource.h"
#include "vtkMatrixToLinearTransform.h"
#include "vtkPointData.h"
#include "vtkUnsignedCharArray.h"
#include "vtkIntArray.h"
#include "vtkImageAccumulate.h"
#include "vtkInteractorStyleImage2D.h"
#include "vtkViewImage2DCommand.h"
#include "vtkProperty.h"
#include <vtkOrientationAnnotation.h>
#include <vtkViewImage2DCommand.h>
#include <vtkProperty2D.h>
#include <vtkAxisActor2D.h>
#include <vtkCursor2D.h>
#include <vtkPointHandleRepresentation2D.h>

#include <vtkExtractPolyDataGeometry.h>

#include <vector>
#include <string>
#include <sstream>
#include <cmath>

vtkCxxRevisionMacro(vtkViewImage2D, "$Revision: 541 $");
vtkStandardNewMacro(vtkViewImage2D);

//----------------------------------------------------------------------------
vtkViewImage2D::vtkViewImage2D()
{
  this->ConventionMatrix = vtkMatrix4x4::New();
  this->SliceImplicitPlane = vtkSmartPointer< vtkPlane >::New();
  this->SlicePlane = vtkPolyData::New();
  this->Command = vtkViewImage2DCommand::New();
  this->OrientationAnnotation = vtkOrientationAnnotation::New();

  this->Command->SetViewer(this);

  this->SliceImplicitPlane->SetOrigin(0, 0, 0);
  this->SliceImplicitPlane->SetNormal(0, 0, 1);

  this->ViewConvention = vtkViewImage2D::VIEW_CONVENTION_RADIOLOGICAL;
  this->ViewOrientation = vtkViewImage2D::VIEW_ORIENTATION_AXIAL;
  this->ViewCenter[0] = this->ViewCenter[1] = this->ViewCenter[2] = 0;

  this->ConventionMatrix->Zero();
  this->ConventionMatrix->SetElement(2, 0, 1);
  this->ConventionMatrix->SetElement(2, 1, 1);
  this->ConventionMatrix->SetElement(1, 2, -1);
  this->ConventionMatrix->SetElement(0, 3, 1);
  this->ConventionMatrix->SetElement(1, 3, -1);
  this->ConventionMatrix->SetElement(2, 3, -1);

  this->OrientationAnnotation->SetTextProperty(this->TextProperty);
  this->Renderer->AddViewProp (this->OrientationAnnotation);

  this->CursorGenerator = vtkCursor2D::New();
  this->CursorGenerator->AllOff();
  this->CursorGenerator->AxesOn();
  this->CursorGenerator->SetRadius(3);
  this->CursorGenerator->SetModelBounds (-40, 40, -40, 40, 0, 0);

  this->Cursor = vtkPointHandleRepresentation2D::New();
  this->Cursor->ActiveRepresentationOff();
  this->Cursor->SetCursorShape( this->CursorGenerator->GetOutput() );
  this->Cursor->GetProperty()->SetColor (0.9, 0.9, 0.1);
  this->Cursor->SetVisibility (0);

  this->Renderer->AddViewProp(this->Cursor);

  this->ShowAnnotationsOn();
  this->InitializeSlicePlane();
  this->Zoom = 1.;
  this->Slice = -1;
}

//----------------------------------------------------------------------------
vtkViewImage2D::
~vtkViewImage2D()
{
  this->ConventionMatrix->Delete();
  this->SlicePlane->Delete();
  this->Command->Delete();
  this->OrientationAnnotation->Delete();

  this->Cursor->Delete();
  this->CursorGenerator->Delete();
}

//----------------------------------------------------------------------------
void
vtkViewImage2D::SetViewConvention(int convention)
{
  if ( ( convention < vtkViewImage2D::VIEW_CONVENTION_RADIOLOGICAL )
       || convention == this->ViewConvention )
    {
    return;
    }

  this->ViewConvention = convention;

  this->ConventionMatrix->SetElement(2, 0, 1);
  this->ConventionMatrix->SetElement(2, 1, 1);
  this->ConventionMatrix->SetElement(1, 2, -1);

  int x_watcher, y_watcher, z_watcher;

  switch ( convention )
    {
    default:
    case vtkViewImage2D::VIEW_CONVENTION_RADIOLOGICAL:
      {
      x_watcher =  1;
      y_watcher = -1;
      z_watcher = -1;
      break;
      }
    case vtkViewImage2D::VIEW_CONVENTION_NEUROLOGICAL:
      {
      x_watcher = 1;
      y_watcher = 1;
      z_watcher = 1;
      break;
      }
      ///\todo why not adding cardiologic conventions with oblique points of
      // view ?
      /// actually we can't: oblique point of view implies resampling data: loss
      // of
      /// data... and we don't want that, do we ?
    }
  this->ConventionMatrix->SetElement(0, 3, x_watcher);
  this->ConventionMatrix->SetElement(1, 3, y_watcher);
  this->ConventionMatrix->SetElement(2, 3, z_watcher);

  this->UpdateOrientation();
}

//----------------------------------------------------------------------------
void
vtkViewImage2D::SetShowAnnotations(const int & iShowAnnotations)
{
  this->Superclass::SetShowAnnotations (iShowAnnotations);
  this->OrientationAnnotation->SetVisibility (iShowAnnotations);
}

//----------------------------------------------------------------------------
void
vtkViewImage2D::SetViewOrientation(int orientation)
{
  ///\todo: in terms of view orientation here we can add some cardiac specific:
  /// short axis, long axis, and 4-chambers !!! exiting !
  if ( ( orientation < vtkViewImage2D::VIEW_ORIENTATION_SAGITTAL )
       || orientation == this->ViewOrientation )
    {
    return;
    }

  this->ViewOrientation = orientation;
  unsigned int sliceorientation = 2;
  double       dot = 0;
  for ( unsigned int i = 0; i < 3; i++ )
    {
    if ( dot < fabs( this->GetOrientationMatrix()->GetElement(orientation, i) ) )
      {
      dot = fabs( this->GetOrientationMatrix()->GetElement(orientation, i) );
      sliceorientation = i;
      }
    }
  this->SetSliceOrientation(sliceorientation);
}

//----------------------------------------------------------------------------
void
vtkViewImage2D::SetOrientationMatrix(vtkMatrix4x4 *matrix)
{
  this->Superclass::SetOrientationMatrix(matrix);
  this->UpdateOrientation();
}

//----------------------------------------------------------------------------
void
vtkViewImage2D::InitializeSlicePlane(void)
{
  vtkSmartPointer< vtkPoints > points = vtkSmartPointer< vtkPoints >::New();
  this->SlicePlane->SetPoints(points);
  points->InsertNextPoint(0, 0, 0);
  points->InsertNextPoint(1, 0, 0);
  points->InsertNextPoint(0, 1, 0);
  points->InsertNextPoint(1, 1, 0);
  this->SlicePlane->Allocate(4);

  vtkIdType pts[4];
  pts[0] = 0; pts[1] = 1; pts[2] = 3; pts[3] = 2;
  this->SlicePlane->InsertNextCell(VTK_QUAD, 4, pts);
  pts[0] = 0; pts[1] = 2; pts[2] = 3; pts[3] = 1;
  this->SlicePlane->InsertNextCell(VTK_QUAD, 4, pts);

  vtkSmartPointer< vtkUnsignedCharArray > array =
    vtkSmartPointer< vtkUnsignedCharArray >::New();
  array->SetName("Colors");
  array->SetNumberOfComponents(3);
  unsigned char vals[3];
  vals[0] = 255; vals[1] = 0; vals[2] = 0;
  array->InsertNextTupleValue(vals);
  array->InsertNextTupleValue(vals);
  array->InsertNextTupleValue(vals);
  array->InsertNextTupleValue(vals);

  this->SlicePlane->GetPointData()->SetScalars(array);
}

//----------------------------------------------------------------------------
void
vtkViewImage2D::UpdateOrientation()
{
  this->Superclass::UpdateOrientation();
  this->PostUpdateOrientation();
}

//----------------------------------------------------------------------------
void
vtkViewImage2D::SetWorldCoordinates(double pos[3])
{
  this->SetSlice( this->GetSliceForWorldCoordinates(pos) );
}

//----------------------------------------------------------------------------
int
vtkViewImage2D::SetCameraToConvention(void)
{
  vtkCamera *cam = this->Renderer ? this->Renderer->GetActiveCamera() : NULL;

  if ( !cam )
    {
    return -1;
    }

  double position[4], focalpoint[4], viewup[4];

  double conventionposition[4];
  conventionposition[0] = 0.;
  conventionposition[1] = 0.;
  conventionposition[2] = 0.;
  conventionposition[3] = 0.;

  double                  conventionview[4];
  double                  focaltoposition[3];
  double                  abs_focaltoposition[3];
  std::vector< double * > viewupchoices;
  double                  first[3], second[3], third[3], fourth[3];

  // First recover information from the camera.
  // Recover also information from the convention matrix
  unsigned int i;
  for ( i = 0; i < 3; i++ )
    {
    position[i] = cam->GetPosition()[i];
    focalpoint[i] = cam->GetFocalPoint()[i];
    conventionposition[i] = this->ConventionMatrix->GetElement(i, 3);
    conventionview[i] = this->ConventionMatrix->GetElement(i,
                                                           this->SliceOrientation);
    }

  position[3] = 1;
  focalpoint[3] = 1;
  conventionview[3] = 0;
  viewup[3] = 0;

  // Apply the orientation matrix to all this information
  this->GetOrientationMatrix()->MultiplyPoint(position, position);
  this->GetOrientationMatrix()->MultiplyPoint(focalpoint, focalpoint);
  this->GetOrientationMatrix()->MultiplyPoint(conventionview, conventionview);
  this->GetOrientationMatrix()->MultiplyPoint(conventionposition,
                                              conventionposition);

  // Compute the vector perpendicular to the view
  for ( i = 0; i < 3; i++ )
    {
    focaltoposition[i] = position[i] - focalpoint[i];
    abs_focaltoposition[i] = fabs(focaltoposition[i]);
    }

  // Deal with the position :
  // invert it if necessary( symetry among the focal point)
  if ( vtkMath::Dot(focaltoposition, conventionposition) < 0 )
    {
    for ( i = 0; i < 3; i++ )
      {
      position[i] -= 2 * focaltoposition[i];
      }
    }

  // Now we now we have 4 choices for the View-Up information
  for ( i = 0; i < 3; i++ )
    {
    first[i] = conventionview[i];
    second[i] = -conventionview[i];
    }

  vtkMath::Cross(first, focaltoposition, third);
  vtkMath::Cross(second, focaltoposition, fourth);
  vtkMath::Normalize(third);
  vtkMath::Normalize(fourth);

  viewupchoices.push_back(first);
  viewupchoices.push_back(second);
  viewupchoices.push_back(third);
  viewupchoices.push_back(fourth);

  // To choose between these choices, first we find the axis
  // the closest to the focaltoposition vector
  unsigned int id = 0;
  double       dot = 0;
  for ( i = 0; i < 3; i++ )
    {
    if ( dot < fabs(focaltoposition[i]) )
      {
      dot = fabs(focaltoposition[i]);
      id = i;
      }
    }

  // Then we choose the convention matrix vector correspondant to the
  // one we just found
  for ( i = 0; i < 3; i++ )
    {
    conventionview[i] = this->ConventionMatrix->GetElement(i, id);
    }

  // Then we pick from the 4 solutions the closest to the
  // vector just found
  unsigned int id2 = 0;
  double       dot2 = 0;
  for ( i = 0; i < viewupchoices.size(); i++ )
    {
    if ( dot2 < vtkMath::Dot(viewupchoices[i], conventionview) )
      {
      dot2 = vtkMath::Dot(viewupchoices[i], conventionview);
      id2 = i;
      }
    }

  // We found the solution
  for ( i = 0; i < 3; i++ )
    {
    viewup[i] = viewupchoices[id2][i];
    }

  cam->SetPosition(position[0], position[1], position[2]);
  cam->SetFocalPoint(focalpoint[0], focalpoint[1], focalpoint[2]);
  cam->SetViewUp(viewup[0], viewup[1], viewup[2]);

  this->SliceImplicitPlane->SetNormal(abs_focaltoposition);

  double view_plane_normal[3];
  cam->GetViewPlaneNormal(view_plane_normal);

  // Rotation of the( Anterior-Posterior)( Ventral-Dorsal) View
  if ( this->ViewOrientation == 0 )
    {
    cam->Roll(-90.);
    }

  return id;
}

//----------------------------------------------------------------------------
void
vtkViewImage2D::SetAnnotationToConvention(void)
{
  vtkCamera *cam = this->Renderer ? this->Renderer->GetActiveCamera() : NULL;

  if ( !cam )
    {
    return;
    }

  std::string solution[4];

  ///\todo surely there is a simpler way to do all of that !

  double *viewup = cam->GetViewUp();
  double *normal = cam->GetViewPlaneNormal();
  double  rightvector[3];
  vtkMath::Cross(normal, viewup, rightvector);

  unsigned int id1 = 0;
  unsigned int id2 = 0;
  unsigned int id3 = 0;
  double       dot1 = 0;
  double       dot2 = 0;
  double       dot3 = 0;

  for ( unsigned int i = 0; i < 3; i++ )
    {
    if ( dot1 <= fabs(viewup[i]) )
      {
      dot1 = fabs(viewup[i]);
      id1 = i;
      }
    if ( dot2 <= fabs(rightvector[i]) )
      {
      dot2 = fabs(rightvector[i]);
      id2 = i;
      }
    if ( dot3 <= fabs(normal[i]) )
      {
      dot3 = fabs(normal[i]);
      id3 = i;
      }
    }

  if ( viewup[id1] > 0 )
    {
    solution[3] = this->DirectionAnnotationMatrix[id1][0];
    solution[1] = this->DirectionAnnotationMatrix[id1][1];
    }
  else
    {
    solution[3] = this->DirectionAnnotationMatrix[id1][1];
    solution[1] = this->DirectionAnnotationMatrix[id1][0];
    }
  if ( rightvector[id2] > 0 )
    {
    solution[0] = this->DirectionAnnotationMatrix[id2][0];
    solution[2] = this->DirectionAnnotationMatrix[id2][1];
    }
  else
    {
    solution[0] = this->DirectionAnnotationMatrix[id2][1];
    solution[2] = this->DirectionAnnotationMatrix[id2][0];
    }

  for ( unsigned int i = 0; i < 4; i++ )
    {
    this->OrientationAnnotation->SetText( i, solution[i].c_str() );
    }
}

//----------------------------------------------------------------------------
void
vtkViewImage2D::SetSlicePlaneToConvention(unsigned int axis)
{
  unsigned char vals[3] = { 0, 0, 0 };

  vals[axis] = 255;

  vtkUnsignedCharArray *array = vtkUnsignedCharArray::SafeDownCast(
    this->SlicePlane->GetPointData()->GetScalars() );

  if ( !array )
    {
    return;
    }

  array->SetTupleValue(0, vals);
  array->SetTupleValue(1, vals);
  array->SetTupleValue(2, vals);
  array->SetTupleValue(3, vals);
}

//----------------------------------------------------------------------------
void
vtkViewImage2D::SetSlice(int slice)
{
  if ( slice != this->Slice )
    {
    vtkCamera *cam = this->Renderer ? this->Renderer->GetActiveCamera() : NULL;

    if ( !cam )
      {
      return;
      }

    this->Superclass::SetSlice(slice);
    this->UpdateSlicePlane();
    }
}

//----------------------------------------------------------------------------
void
vtkViewImage2D::UpdateSlicePlane(void)
{
  vtkSmartPointer< vtkPoints > oldpoints = vtkSmartPointer< vtkPoints >::New();
  vtkSmartPointer< vtkPoints > points = vtkSmartPointer< vtkPoints >::New();
  double                       x[3];
  double *                     bounds = this->ImageActor->GetDisplayBounds();
  unsigned int                 added1;
  unsigned int                 added2;

  for ( unsigned int i = 0; i < 4; i++ )
    {
    added1 = ( !( i % 2 ) ) ? 1 : 0;
    added2 = ( i < 2 ) ? 1 : 0;
    x[( this->SliceOrientation + 1 ) % 3] =
      bounds[2 * ( ( this->SliceOrientation + 1 ) % 3 ) + added1];
    x[( this->SliceOrientation + 2 ) % 3] =
      bounds[2 * ( ( this->SliceOrientation + 2 ) % 3 ) + added2];
    x[this->SliceOrientation] = bounds[2 * this->SliceOrientation];
    oldpoints->InsertPoint(i, x);
    }
  this->OrientationTransform->TransformPoints(oldpoints, points);
  this->SlicePlane->SetPoints(points);

  points->GetPoint(0, x);
  this->SliceImplicitPlane->SetOrigin(x);
}

//----------------------------------------------------------------------------
double *
vtkViewImage2D::GetWorldCoordinatesFromDisplayPosition(const int & x, const int & y)
{
  int xy[2] = { x, y };

  return this->GetWorldCoordinatesFromDisplayPosition(xy);
}

//----------------------------------------------------------------------------
int
vtkViewImage2D::GetSliceForWorldCoordinates(double pos[3])
{
  int *indices = this->GetImageCoordinatesFromWorldCoordinates(pos);
  int  slice = indices[this->SliceOrientation];

  delete[] indices;

  return slice;
}

//----------------------------------------------------------------------------
double *
vtkViewImage2D::GetWorldCoordinatesForSlice(int slice)
{
  int indices[3] = { slice, slice, slice };

  return this->GetWorldCoordinatesFromImageCoordinates(indices);
}

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void
vtkViewImage2D::ResetPosition(void)
{
  if ( !this->GetInput() )
    {
    return;
    }

  int *range = this->GetSliceRange();
  if ( range )
    {
    this->SetSlice( ( range[1] + range[0] ) / 2 );
    }
}

//----------------------------------------------------------------------------
void
vtkViewImage2D::ResetCamera(void)
{
  this->Zoom = 1.;

  double bounds[6];
  this->GetInput()->GetBounds(bounds);

  double focal[3];
  focal[0] = 0.5 * ( bounds[0] + bounds[1] );
  focal[1] = 0.5 * ( bounds[2] + bounds[3] );
  focal[2] = 0.5 * ( bounds[4] + bounds[5] );

  double pos[3];
  pos[0] = focal[0];
  pos[1] = focal[1];
  pos[2] = focal[2];

  double length[3];
  length[0] = 0.5 * ( bounds[1] - bounds[0] );
  length[1] = 0.5 * ( bounds[3] - bounds[2] );
  length[2] = 0.5 * ( bounds[5] - bounds[4] );

  double radius = 0.75 * sqrt(length[0] * length[0]
                              + length[1] * length[1]
                              + length[2] * length[2]);

  vtkCamera *camera = this->GetRenderer()->GetActiveCamera();
  double     distance =
    radius / sin(camera->GetViewAngle() * vtkMath::Pi() / 360.0);

  double view_plane_normal[3];
  camera->GetViewPlaneNormal(view_plane_normal);
  double *vup = camera->GetViewUp();

  if ( fabs( vtkMath::Dot(vup, view_plane_normal) ) > 0.999 )
    {
    vtkWarningMacro(<< "Resetting view-up since view plane normal is parallel");
    camera->SetViewUp(-vup[2], vup[0], vup[1]);
    }

  pos[0] += distance * view_plane_normal[0];
  pos[1] += distance * view_plane_normal[1];
  pos[2] += distance * view_plane_normal[2];

  this->SetCameraFocalAndPosition(focal, pos);

  double max_length = std::max( length[0], std::max(length[1], length[2]) );
  bounds[0] = focal[0] - 50. * max_length;
  bounds[1] = focal[0] + 50. * max_length;
  bounds[2] = focal[1] - 50. * max_length;
  bounds[3] = focal[1] + 50. * max_length;
  bounds[4] = focal[2] - 50. * max_length;
  bounds[5] = focal[2] + 50. * max_length;
  this->GetRenderer()->ResetCameraClippingRange(bounds);

  camera->SetParallelScale(radius);
}

//----------------------------------------------------------------------------
void
vtkViewImage2D::Reset(void)
{
  this->ResetPosition();
  this->ResetWindowLevel();
  this->ResetCamera();
}

//----------------------------------------------------------------------------
void
vtkViewImage2D::SetCameraFocalAndPosition(double focal[3], double pos[3])
{
  vtkRenderer *ren = this->GetRenderer();

  if ( !ren )
    {
    return;
    }
  vtkCamera *camera = ren->GetActiveCamera();

  if ( !camera )
    {
    return;
    }

  camera->SetFocalPoint(focal[0], focal[1], focal[2]);
  camera->SetPosition(pos[0], pos[1], pos[2]);
}

//----------------------------------------------------------------------------
void
vtkViewImage2D::GetCameraFocalAndPosition(double focal[3], double pos[3])
{
  vtkRenderer *ren = this->GetRenderer();

  if ( !ren )
    {
    return;
    }
  vtkCamera *camera = ren->GetActiveCamera();

  if ( !camera )
    {
    return;
    }

  camera->GetPosition(pos);
  camera->GetFocalPoint(focal);
}

//----------------------------------------------------------------------------
double *
vtkViewImage2D::GetWorldCoordinatesFromDisplayPosition(int xy[2])
{
  // in one case this function creates an array and don't manage it
  // in another case, it gives a pointer to a managed array :
  // MEMORY LEAKS

  if ( !this->GetInput() || !this->GetRenderer() )
    {
    // case 1 : non managed array pointer
    double *nullpos = new double[3];
    nullpos[0] = 0; nullpos[1] = 0; nullpos[2] = 0;
    return nullpos;
    }

  double *slicepos = this->GetWorldCoordinatesForSlice( this->GetSlice() );
  this->GetRenderer()->SetWorldPoint(slicepos[0], slicepos[1], slicepos[2],
                                     1.0);
  this->GetRenderer()->WorldToDisplay();
  this->GetRenderer()->SetDisplayPoint(xy[0], xy[1],
                                       this->GetRenderer()->GetDisplayPoint()[2]);
  this->GetRenderer()->DisplayToWorld();
  // think about deleting the temporary created array
  delete[] slicepos;

  // case 2 : pointer to managed array
  return this->GetRenderer()->GetWorldPoint();
}

//----------------------------------------------------------------------------
void
vtkViewImage2D::InstallPipeline()
{
  if ( this->RenderWindow && this->Renderer )
    {
    if ( !this->RenderWindow->HasRenderer(this->Renderer) )
      {
      this->RenderWindow->AddRenderer(this->Renderer);
      }
    }

  if ( this->Interactor )
    {
    if ( !this->InteractorStyle )
      {
      this->InteractorStyle = vtkInteractorStyleImage2D::New();
      this->Interactor->SetInteractorStyle(this->InteractorStyle);
      this->InteractorStyle->AddObserver(
        vtkCommand::KeyPressEvent, this->Command);
      this->InteractorStyle->AddObserver(
        vtkViewImage2DCommand::StartSliceMoveEvent, this->Command);
      this->InteractorStyle->AddObserver(
        vtkViewImage2DCommand::SliceMoveEvent, this->Command);
      this->InteractorStyle->AddObserver(
        vtkViewImage2DCommand::EndSliceMoveEvent, this->Command);
      this->InteractorStyle->AddObserver(
        vtkViewImage2DCommand::ResetViewerEvent, this->Command);
      this->InteractorStyle->AddObserver(
        vtkViewImage2DCommand::ZoomEvent, this->Command);
      this->InteractorStyle->AddObserver(
        vtkViewImage2DCommand::PanEvent, this->Command);
      this->InteractorStyle->AddObserver(
        vtkCommand::InteractionEvent, this->Command);
      this->InteractorStyle->AddObserver(
        vtkViewImage2DCommand::RequestedPositionEvent, this->Command);
      this->InteractorStyle->AddObserver(
        vtkViewImage2DCommand::ContourPickingEvent, this->Command);
      this->InteractorStyle->AddObserver(
        vtkViewImage2DCommand::MeshPickingEvent, this->Command);

      this->InteractorStyle->AddObserver(
        vtkCommand::StartWindowLevelEvent, this->Command);
      this->InteractorStyle->AddObserver(
        vtkCommand::WindowLevelEvent, this->Command);
      }
    this->InteractorStyleSwitcher = this->InteractorStyle;
    this->Interactor->SetInteractorStyle(this->InteractorStyle);
    this->Interactor->SetRenderWindow(this->RenderWindow);
    }

  if ( this->Renderer && this->ImageActor )
    {
    this->ImageActor->SetInterpolate(false);
    }

  if ( this->Renderer && this->ImageActor )
    {
    if ( !this->Renderer->GetActors()->IsItemPresent(this->ImageActor) )
      {
      this->Renderer->AddViewProp(this->ImageActor);
      }
    }

  if ( this->ImageActor && this->WindowLevel )
    {
    this->ImageActor->SetInput( this->WindowLevel->GetOutput() );
    }
}

//----------------------------------------------------------------------------
void
vtkViewImage2D::SetInterpolate(const int & val)
{
  if ( this->ImageActor )
    {
    this->ImageActor->SetInterpolate(val);
    }
}

//----------------------------------------------------------------------------
int
vtkViewImage2D::GetInterpolate(void)
{
  if ( this->ImageActor )
    {
    return this->ImageActor->GetInterpolate();
    }
  else
    {
    return 0;
    }
}

//----------------------------------------------------------------------------
// vtkQuadricLODActor*
vtkActor *
vtkViewImage2D::AddDataSet(vtkPolyData *dataset,
                           vtkProperty *property,
                           const bool & intersection,
                           const bool & iDataVisibility)
{
  vtkCamera *cam = NULL;

  if ( this->Renderer )
    {
    cam = this->Renderer->GetActiveCamera();
    }
  else
    {
    return NULL;
    }

  vtkSmartPointer< vtkPolyDataMapper > mapper =
    vtkSmartPointer< vtkPolyDataMapper >::New();
  mapper->SetScalarVisibility(iDataVisibility);

  vtkActor *                                    actor = vtkActor::New();
  vtkSmartPointer< vtkCutter >                  cutter = vtkSmartPointer< vtkCutter >::New();
  vtkSmartPointer< vtkExtractPolyDataGeometry > extracter =
    vtkSmartPointer< vtkExtractPolyDataGeometry >::New();

  // check if input data is 2D
  double *bounds = dataset->GetBounds();

  //  get normal
  double *normal = this->SliceImplicitPlane->GetNormal();

  // if in 2d
  if ( ( ( bounds[0] == bounds[1] ) && ( normal[1] == 0 ) && ( normal[2] == 0 ) )
       || ( ( bounds[2] == bounds[3] ) && ( normal[2] == 0 ) && ( normal[0] == 0 ) )
       || ( ( bounds[4] == bounds[5] ) && ( normal[0] == 0 ) && ( normal[1] == 0 ) ) )
    {
    //std::cout << "extract 2d" << std::endl;
    extracter->SetInput(dataset);
    extracter->SetImplicitFunction(this->SliceImplicitPlane);
    extracter->Update();
    mapper->SetInput( extracter->GetOutput() );
    }
  // i.e. if volume
  else
    {
    if ( intersection )
      {
      //std::cout << "intersection" << std::endl;
      cutter->SetInput(dataset);
      cutter->SetCutFunction(this->SliceImplicitPlane);
      cutter->Update();
      mapper->SetInput( cutter->GetOutput() );
      }
    else
      {
      //std::cout << "else" << std::endl;
      mapper->SetInput(dataset);
      }
    }

  mapper->Update();
  actor->SetMapper(mapper);

  if ( property )
    {
    actor->SetProperty(property);
    }
  actor->GetProperty()->BackfaceCullingOn();

  this->Renderer->AddViewProp(actor);
  this->Prop3DCollection->AddItem(actor);

  return actor;
}

//----------------------------------------------------------------------------
//vtkQuadricLODActor*
vtkActor *
vtkViewImage2D::AddDataSet(vtkDataSet *dataset,
                           vtkProperty *property,
                           const bool & intersection,
                           const bool & iDataVisibility)
{
  /* return this->AddDataSet( vtkPolyData::SafeDownCast( dataset ),
     property, intersection, iDataVisibility );*/
  vtkCamera *cam = NULL;

  if ( this->Renderer )
    {
    cam = this->Renderer->GetActiveCamera();
    }
  else
    {
    return NULL;
    }

  vtkSmartPointer< vtkPolyDataMapper > mapper =
    vtkSmartPointer< vtkPolyDataMapper >::New();
  mapper->SetScalarVisibility(iDataVisibility);

  //vtkQuadricLODActor* actor = vtkQuadricLODActor::New();
  vtkActor *actor = vtkActor::New();

  vtkSmartPointer< vtkCutter > cutter = vtkSmartPointer< vtkCutter >::New();

  if ( intersection )
    {
    //std::cout << "inter dataset" << std::endl;
    cutter->SetInput(dataset);
    cutter->SetCutFunction(this->SliceImplicitPlane);
    mapper->SetInput( cutter->GetOutput() );
    }
  else
    {
    //std::cout << "else dataset" << std::endl;
    mapper->SetInput( vtkPolyData::SafeDownCast(dataset) );
    }
  mapper->Update();

  actor->SetMapper(mapper);
  if ( property )
    {
    actor->SetProperty(property);
    }
  actor->GetProperty()->BackfaceCullingOn();

  this->Renderer->AddViewProp(actor);
  this->Prop3DCollection->AddItem(actor);

  return actor;
}

//----------------------------------------------------------------------------
void
vtkViewImage2D::UpdateCenter(void)
{
  if ( !this->GetInput() )
    {
    return;
    }
  vtkCamera *cam = NULL;

  if ( this->Renderer )
    {
    cam = this->Renderer->GetActiveCamera();
    }
  else
    {
    return;
    }
  int *dimensions = this->GetInput()->GetDimensions();

  int indices[3] = { 0, 0, 0 };
  for ( unsigned int i = 0; i < 3; i++ )
    {
    indices[i] = (int)( (double)dimensions[i] / 2.0 );
    }
  indices[this->SliceOrientation] = this->GetSlice();

  double *center = this->GetWorldCoordinatesFromImageCoordinates (indices);

  for ( unsigned int i = 0; i < 3; i++ )
    {
    this->ViewCenter[i] = center[i];
    }

  delete[] center;
}

//----------------------------------------------------------------------------
void
vtkViewImage2D::PostUpdateOrientation()
{
  int axis = this->SetCameraToConvention();

  this->ViewOrientation = axis;

  this->SetAnnotationToConvention();
  this->SetSlicePlaneToConvention(axis);
  this->UpdateSlicePlane();
}

//----------------------------------------------------------------------------
void
vtkViewImage2D::SetSlicePlaneFromOrientation(void)
{
  /**
     These lines tell the slice plane which color it should be
     ///\todo We should allow more colors...
  */
  unsigned char vals[3] = { 0, 0, 0 };

  vals[this->ViewOrientation] = 255;
  vtkUnsignedCharArray *array = vtkUnsignedCharArray::SafeDownCast ( this->SlicePlane->GetPointData()->GetScalars() );
  if ( !array ) { return; }
  array->SetTupleValue (0, vals);
  array->SetTupleValue (1, vals);
  array->SetTupleValue (2, vals);
  array->SetTupleValue (3, vals);

  this->UpdateSlicePlane();
}

//----------------------------------------------------------------------------
int
vtkViewImage2D::SetCameraFromOrientation(void)
{
  // The camera has already been set as if the image has no orientation.
  // So we just have to adjust its position and view up according
  // to the image orientation and conventions.

  vtkCamera *cam = this->Renderer ? this->Renderer->GetActiveCamera() : NULL;

  if ( !cam ) { return -1; }

  double                  position[4], focalpoint[4], viewup[4];
  double                  conventionposition[4];
  double                  conventionview[4];
  double                  focaltoposition[3];
  std::vector< double * > viewupchoices;
  double                  first[3], second[3], third[3], fourth[3];
  bool                    inverseposition;

  // First recover information from the camera.
  // Recover also information from the convention matrix
  for ( unsigned int i = 0; i < 3; i++ )
    {
    position[i] = cam->GetPosition()[i];
    focalpoint[i] = cam->GetFocalPoint()[i];
    conventionposition[i] = this->ConventionMatrix->GetElement (i, 3);
    conventionview[i] = this->ConventionMatrix->GetElement (i, this->SliceOrientation);
    }

  position[3] = 1;
  focalpoint[3] = 1;
  conventionview[3] = 0;
  viewup[3] = 0;

  // Apply the orientation matrix to all this information
  if ( this->GetOrientationMatrix() )
    {
    this->GetOrientationMatrix()->MultiplyPoint (position, position);
    this->GetOrientationMatrix()->MultiplyPoint (focalpoint, focalpoint);
    this->GetOrientationMatrix()->MultiplyPoint (conventionview, conventionview);
    this->GetOrientationMatrix()->MultiplyPoint (conventionposition, conventionposition);
    }

  // Compute the vector perpendicular to the view
  for ( unsigned int i = 0; i < 3; i++ )
    {
    focaltoposition[i] = position[i] - focalpoint[i];
    }

  // Deal with the position :
  // invert it if necessary (symetry among the focal point)
  inverseposition = ( vtkMath::Dot (focaltoposition, conventionposition) < 0 );
  if ( inverseposition )
    {
    for ( unsigned int i = 0; i < 3; i++ )
      {
      position[i] -= 2 * focaltoposition[i];
      }
    }

  // Now we now we have 4 choices for the View-Up information
  for ( unsigned int i = 0; i < 3; i++ )
    {
    first[i] = conventionview[i];
    second[i] = -conventionview[i];
    }

  vtkMath::Cross (first, focaltoposition, third);
  vtkMath::Cross (second, focaltoposition, fourth);
  vtkMath::Normalize (third);
  vtkMath::Normalize (fourth);

  viewupchoices.push_back (first);
  viewupchoices.push_back (second);
  viewupchoices.push_back (third);
  viewupchoices.push_back (fourth);

  // To choose between these choices, first we find the axis
  // the closest to the focaltoposition vector
  unsigned int id = 0;
  double       dot = 0;
  for ( unsigned int i = 0; i < 3; i++ )
    {
    if ( dot < std::abs (focaltoposition[i]) )
      {
      dot = std::abs (focaltoposition[i]);
      id = i;
      }
    }

  // Then we choose the convention matrix vector correspondant to the
  // one we just found
  for ( unsigned int i = 0; i < 3; i++ )
    {
    conventionview[i] = this->ConventionMatrix->GetElement (i, id);
    }

  // Then we pick from the 4 solutions the closest to the
  // vector just found
  unsigned int id2 = 0;
  double       dot2 = 0;
  for ( unsigned int i = 0; i < viewupchoices.size(); i++ )
    {
    if ( dot2 < vtkMath::Dot (viewupchoices[i], conventionview) )
      {
      dot2 = vtkMath::Dot (viewupchoices[i], conventionview);
      id2 = i;
      }
    }

  // We found the solution
  for ( unsigned int i = 0; i < 3; i++ )
    {
    viewup[i] = viewupchoices[id2][i];
    }

  cam->SetPosition(position[0], position[1], position[2]);
  cam->SetFocalPoint(focalpoint[0], focalpoint[1], focalpoint[2]);
  cam->SetViewUp(viewup[0], viewup[1], viewup[2]);

  return id;
}

//----------------------------------------------------------------------------
void
vtkViewImage2D::SetAnnotationsFromOrientation(void)
{
  // This method has to be called after the camera
  // has been set according to orientation and convention.
  // We rely on the camera settings to compute the oriention
  // annotations.

  vtkCamera *cam = this->Renderer ? this->Renderer->GetActiveCamera() : NULL;

  if ( !cam ) { return; }

  std::string matrix[3][2];
  matrix[0][0] = "R"; matrix[0][1] = "L";
  matrix[1][0] = "A"; matrix[1][1] = "P";
  matrix[2][0] = "I"; matrix[2][1] = "S";

  std::string solution[4];

  ///\todo surely there is a simpler way to do all of that !

  double *viewup = cam->GetViewUp();
  double *normal = cam->GetViewPlaneNormal();
  double  rightvector[3];
  vtkMath::Cross (normal, viewup, rightvector);

  unsigned int id1 = 0;
  unsigned int id2 = 0;
  unsigned int id3 = 0;
  double       dot1 = 0;
  double       dot2 = 0;
  double       dot3 = 0;

  for ( unsigned int i = 0; i < 3; i++ )
    {
    if ( dot1 <= std::abs (viewup[i]) )
      {
      dot1 = std::abs (viewup[i]);
      id1 = i;
      }
    if ( dot2 <= std::abs (rightvector[i]) )
      {
      dot2 = std::abs (rightvector[i]);
      id2 = i;
      }
    if ( dot3 <= std::abs (normal[i]) )
      {
      dot3 = std::abs (normal[i]);
      id3 = i;
      }
    }

  if ( viewup[id1] > 0 )
    {
    solution[3] = matrix[id1][0];
    solution[1] = matrix[id1][1];
    }
  else
    {
    solution[3] = matrix[id1][1];
    solution[1] = matrix[id1][0];
    }
  if ( rightvector[id2] > 0 )
    {
    solution[0] = matrix[id2][0];
    solution[2] = matrix[id2][1];
    }
  else
    {
    solution[0] = matrix[id2][1];
    solution[2] = matrix[id2][0];
    }

  for ( unsigned int i = 0; i < 4; i++ )
    {
    this->OrientationAnnotation->SetText ( i, solution[i].c_str() );
    }

  if ( this->GetInput() )
    {
    // naively the X and Y axes of the current view
    // correspond to the rightvector and the viewup respectively.
    // But in fact we have to put those vectors back in the image
    // coordinates and see to which xyz image axis they correspond.

    double Xaxis[4] = { 0, 0, 0, 0 };
    double Yaxis[4] = { 0, 0, 0, 0 };
    for ( unsigned int i = 0; i < 3; i++ )
      {
      Xaxis[i] = rightvector[i];
      Yaxis[i] = viewup[i];
      }

    vtkMatrix4x4 *inverse = vtkMatrix4x4::New();
    inverse->Identity();
    if ( this->GetOrientationMatrix() )
      {
      vtkMatrix4x4::Invert (this->GetOrientationMatrix(), inverse);
      }

    inverse->MultiplyPoint (Xaxis, Xaxis);
    inverse->MultiplyPoint (Yaxis, Yaxis);
    inverse->Delete();

    double dotX = 0;
    double dotY = 0;
    int    idX, idY;
    idX = idY = 0;

    for ( unsigned int i = 0; i < 3; i++ )
      {
      if ( dotX <= std::abs (Xaxis[i]) )
        {
        dotX = std::abs (Xaxis[i]);
        idX = i;
        }
      if ( dotY <= std::abs (Yaxis[i]) )
        {
        dotY = std::abs (Yaxis[i]);
        idY = i;
        }
      }

    int *   dimensions = this->GetInput()->GetDimensions();
    double *spacing = this->GetInput()->GetSpacing();

    sprintf(this->ImageInformation,
            "Image Size:  %i x %i\nVoxel Size: %g x %g mm",
            dimensions[idX], dimensions[idY],
            spacing[idX], spacing[idY]);
    sprintf(this->SliceAndWindowInformation,
            "<slice_and_max>\n<window>\n<level>");

    this->CornerAnnotation->SetText(2, this->ImageInformation);
    this->CornerAnnotation->SetText(3, this->SliceAndWindowInformation);
    }
}

//----------------------------------------------------------------------------
void
vtkViewImage2D::SetImplicitPlaneFromOrientation(void)
{
  vtkCamera *cam = this->Renderer ? this->Renderer->GetActiveCamera() : NULL;

  if ( !cam ) { return; }

  double *position = cam->GetPosition();
  double *focalpoint = cam->GetFocalPoint();

  double focaltoposition[3];

  unsigned int i;

  // Compute the vector perpendicular to the view
  for ( i = 0; i < 3; i++ )
    {
    focaltoposition[i] = fabs(position[i] - focalpoint[i]);
    }

  this->SliceImplicitPlane->SetNormal (focaltoposition);
}

//----------------------------------------------------------------------------
void
vtkViewImage2D::UpdateCursor(void)
{
  this->GetRenderer()->SetWorldPoint (this->CurrentPoint[0], this->CurrentPoint[1], this->CurrentPoint[2], 0);
  this->GetRenderer()->WorldToDisplay();

  double *xy = this->GetRenderer()->GetDisplayPoint();
  this->GetCursor()->SetDisplayPosition (xy);

  char os[128];
  sprintf(os,
          "%s\nXYZ:  %4.2f, %4.2f, %4.2f mm\nValue: %g",
          this->ImageInformation,
          this->CurrentPoint[0], this->CurrentPoint[1], this->CurrentPoint[2],
          this->GetValueAtPosition (this->CurrentPoint)
          );
  this->CornerAnnotation->SetText(2, os);
}
