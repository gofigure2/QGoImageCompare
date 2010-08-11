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

#include "vtkViewImage.h"
#include "vtkInformation.h"

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
#include "vtkScalarBarActor.h"
#include "vtkOrientationAnnotation.h"
#include "vtkCornerAnnotation.h"
#include "vtkTextProperty.h"
#include "vtkLookupTable.h"
#include "vtkMath.h"
#include "vtkPlane.h"
#include "vtkCutter.h"
#include "vtkProperty.h"
#include "vtkActor.h"
#include "vtkPolyDataMapper.h"
#include "vtkProp3DCollection.h"
#include "vtkDataSetCollection.h"
#include "vtkPoints.h"
#include "vtkIdList.h"
#include "vtkOutlineSource.h"
#include "vtkMatrixToLinearTransform.h"
#include "vtkPointData.h"
#include "vtkUnsignedCharArray.h"
#include "vtkIntArray.h"
#include "vtkImageAccumulate.h"
#include "vtkInteractorStyleImage.h"

#include <vector>
#include <string>

vtkCxxRevisionMacro(vtkViewImage, "$Revision: 546 $");
//vtkStandardNewMacro(vtkViewImage);

//----------------------------------------------------------------------------
vtkViewImage::vtkViewImage()
  {
  this->OrientationMatrix = vtkMatrix4x4::New();
  this->CornerAnnotation = vtkCornerAnnotation::New();
  this->TextProperty = vtkTextProperty::New();
  this->LookupTable = vtkLookupTable::New();
  this->ScalarBarActor = vtkScalarBarActor::New();
  this->Prop3DCollection = vtkProp3DCollection::New();
  this->DataSetCollection = vtkDataSetCollection::New();
  this->OrientationTransform = vtkMatrixToLinearTransform::New();

  this->OrientationMatrix->Identity();
  this->CornerAnnotation->SetNonlinearFontScaleFactor (0.35);
  this->CornerAnnotation->SetTextProperty (this->TextProperty);

  this->ScalarBarActor->GetLabelTextProperty()->BoldOff();
  this->ScalarBarActor->GetLabelTextProperty()->ItalicOff();
  this->ScalarBarActor->SetNumberOfLabels (3);
  this->ScalarBarActor->SetWidth (0.1);
  this->ScalarBarActor->SetHeight (0.5);
  this->ScalarBarActor->SetPosition (0.9, 0.3);
  this->LookupTable->SetTableRange (0, 1);
  this->LookupTable->SetSaturationRange (0, 0);
  this->LookupTable->SetHueRange (0, 0);
  this->LookupTable->SetValueRange (0, 1);
  this->LookupTable->Build();

  this->ShowAnnotations = true;
  this->ShowScalarBar = true;

  this->OrientationTransform->SetInput (this->OrientationMatrix);

  this->ScalarBarActor->SetLookupTable (this->LookupTable);

  this->Renderer->AddViewProp (this->CornerAnnotation);

  this->Renderer->AddViewProp (this->ScalarBarActor);

  this->IsColor = false;

  // default DirectionAnnotation
  this->DirectionAnnotationMatrix[0][0] = "R";
  this->DirectionAnnotationMatrix[0][1] = "L";
  this->DirectionAnnotationMatrix[1][0] = "A";
  this->DirectionAnnotationMatrix[1][1] = "P";
  this->DirectionAnnotationMatrix[2][0] = "V";
  this->DirectionAnnotationMatrix[2][1] = "D";

  }

//----------------------------------------------------------------------------
void vtkViewImage::SetInput(vtkImageData* in)
{
  if (in)
    {
    Superclass::SetInput(in);

    this->IsColor = (in->GetNumberOfScalarComponents() > 1);

    if (this->IsColor)
      {
      this->WindowLevel->SetLookupTable(NULL);
      this->ShowScalarBar = false;
      this->ScalarBarActor->SetVisibility(this->ShowScalarBar);
      }
    else
      {
      this->WindowLevel->SetLookupTable(this->LookupTable);
      }
    }
}

//----------------------------------------------------------------------------
vtkViewImage::~vtkViewImage()
  {
  this->OrientationTransform->SetInput(NULL);

  if (this->OrientationMatrix)
    {
    this->OrientationMatrix->Delete();
    }

  this->OrientationTransform->Delete();
  this->TextProperty->Delete();
  this->CornerAnnotation->Delete();
  this->LookupTable->Delete();
  this->ScalarBarActor->Delete();
  this->Prop3DCollection->Delete();
  this->DataSetCollection->Delete();
  }

//----------------------------------------------------------------------------
void vtkViewImage::SetOrientationMatrix(vtkMatrix4x4* matrix)
{
  vtkSetObjectMacro2Body(OrientationMatrix, vtkMatrix4x4, matrix);
  this->ImageActor->SetUserMatrix(this->OrientationMatrix);
  this->OrientationTransform->SetInput(this->OrientationMatrix);

  this->UpdateOrientation();
}

//----------------------------------------------------------------------------
void vtkViewImage::SetLookupTable(vtkLookupTable* lookuptable)
{
  if (!this->IsColor)
    {
    vtkSetObjectMacro2Body(LookupTable, vtkLookupTable, lookuptable);
    this->WindowLevel->SetLookupTable(this->LookupTable);
    this->ScalarBarActor->SetLookupTable(this->LookupTable);
    }
  else
    {
    this->ShowScalarBar = false;
    this->ScalarBarActor->SetVisibility(this->ShowScalarBar);
    }
}

//----------------------------------------------------------------------------
void vtkViewImage::SetTextProperty(vtkTextProperty* textproperty)
{
  vtkSetObjectMacro2Body (TextProperty, vtkTextProperty, textproperty);
  this->CornerAnnotation->SetTextProperty (this->TextProperty);
}

//----------------------------------------------------------------------------
void vtkViewImage::SetColorWindow(double s)
{
  double t = s;

  if (t < 0.)
    {
    t = 1.;
    }

  Superclass::SetColorWindow(t);

  if (!this->IsColor)
    {
    double level = this->GetColorLevel();
    t *= 0.5;
    double v_min = level - t;
    double v_max = level + t;
    this->LookupTable->SetRange (v_min, v_max);
    }
  else
    {
    this->ShowScalarBar = false;
    this->ScalarBarActor->SetVisibility(this->ShowScalarBar);
    }
}

//----------------------------------------------------------------------------
void vtkViewImage::SetColorLevel(double s)
{
  Superclass::SetColorLevel(s);

  if (!this->IsColor)
    {
    double t = 0.5 * this->GetColorWindow();
    double v_min = s - t;
    double v_max = s + t;
    this->LookupTable->SetRange(v_min, v_max);
    }
  else
    {
    this->ShowScalarBar = false;
    this->ScalarBarActor->SetVisibility(this->ShowScalarBar);
    }
}

//----------------------------------------------------------------------------
void vtkViewImage::SetWorldCoordinates(const double& x,
                                       const double& y, const double& z)
{
  double pos[3] = { x, y, z};
  this->SetWorldCoordinates(pos);
}
//----------------------------------------------------------------------------
double vtkViewImage::GetValueAtPosition(double worldcoordinates[3],
                                        int component)
{
// by default, value = 0.0
  double value = 0.0;

  if (!this->GetInput())
    {
    return value;
    }

  int* indices =
    this->GetImageCoordinatesFromWorldCoordinates(worldcoordinates);
  vtkImageData* input = this->GetInput();

  int* extent = input->GetWholeExtent();

  if (!((indices[0] < extent[0]) || (indices[0] > extent[1]) ||
        (indices[1] < extent[2]) || (indices[1] > extent[3]) ||
        (indices[2] < extent[4]) || (indices[2] > extent[5])))
    {
    value = input->GetScalarComponentAsDouble(indices[0], indices[1],
                                              indices[2], component);
    }
  // have to free the indices array allocated by
  // GetImageCoordinatesFromWorldCoordinates
  delete[] indices;

  return value;
}

//----------------------------------------------------------------------------
bool vtkViewImage::RemoveDataSet(vtkDataSet* dataset)
{
  unsigned int index = this->DataSetCollection->IsItemPresent(dataset);

  if (!index)
    {
    return false;
    }
  else
    {
    this->Renderer->RemoveViewProp (
      vtkProp::SafeDownCast(this->Prop3DCollection->GetItemAsObject(index)));
    this->DataSetCollection->RemoveItem(index);
    this->Prop3DCollection->RemoveItem(index);

    return true;
    }
}

void vtkViewImage::RemoveProp(vtkProp* prop)
{
  this->Renderer->RemoveViewProp(prop);

  unsigned int index = this->DataSetCollection->IsItemPresent(prop);
  this->Prop3DCollection->RemoveItem(index);
}

//----------------------------------------------------------------------------
double* vtkViewImage::GetWorldCoordinatesFromImageCoordinates(int indices[3])
{
  vtkImageData* input = this->GetInput();

  if (!input)
    {
    double* nullpos = new double[3];
    nullpos[0] = 0.;
    nullpos[1] = 0.;
    nullpos[2] = 0.;
    return nullpos;
    }

  // Get information
  double* spacing = input->GetSpacing();
  double* origin = input->GetOrigin();

  double unorientedposition[4];
  for (unsigned int i = 0; i < 3; i++)
    {
    unorientedposition[i] = origin[i] + spacing[i] * indices[i];
    }
  unorientedposition[3] = 1;

  // apply orientation matrix
  double* position = new double[4];
  this->GetOrientationMatrix()->MultiplyPoint(unorientedposition, position);

  return position;
}

//----------------------------------------------------------------------------
int* vtkViewImage::GetImageCoordinatesFromWorldCoordinates(double position[3])
{
  vtkImageData* input = this->GetInput();

  if (!input)
    {
    int* nullpos = new int[3];
    nullpos[0] = 0;
    nullpos[1] = 0;
    nullpos[2] = 0;
    return nullpos;
    }

  // Get information
  double unorientedposition[4] = { position[0], position[1], position[2], 1};
  double spacing[4] = { input->GetSpacing()[0],
                        input->GetSpacing()[1],
                        input->GetSpacing()[2],
                        0};
  double origin[4] = { input->GetOrigin()[0],
                       input->GetOrigin()[1],
                       input->GetOrigin()[2],
                       1};

  // apply inverted orientation matrix to the world-coordinate position
  vtkSmartPointer<vtkMatrix4x4> inverse = vtkSmartPointer<vtkMatrix4x4>::New();
  vtkMatrix4x4::Invert (this->GetOrientationMatrix(), inverse);
  inverse->MultiplyPoint (unorientedposition, unorientedposition);

  int* indices = new int[3];
  for (unsigned int i = 0; i < 3; i++)
    {
    if (fabs (spacing[i]) > 1e-5)
      {
      indices[i] = vtkMath::Round((unorientedposition[i] - origin[i]) / spacing[i]);
      }
    else
      {
      indices[i] = 0;
      }
    }
  return indices;
}

//----------------------------------------------------------------------------
void vtkViewImage::SetBackground(double rgb[3])
{
  if (this->Renderer)
    {
    this->Renderer->SetBackground(rgb);
    }
}
//----------------------------------------------------------------------------
void vtkViewImage::SetBackground(const double& r, const double& g, const double& b)
{
  if (this->Renderer)
    {
    this->Renderer->SetBackground(r, g, b);
    }
}

//----------------------------------------------------------------------------
double* vtkViewImage::GetBackground()
{
  if (this->Renderer)
    {
    return this->Renderer->GetBackground();
    }
  else
    {
    return NULL;
    }
}

//----------------------------------------------------------------------------
void vtkViewImage::ResetWindowLevel(void)
{
  vtkImageData* input = this->GetInput();

  if (!input)
    {
    return;
    }

  if (!this->IsColor)
    {
    double* range = input->GetScalarRange();
    double  window = range[1] - range[0];
    double  level = 0.5 * (range[1] + range[0]);

    this->SetColorWindow(window);
    this->SetColorLevel(level);
    }
}
//----------------------------------------------------------------------------
void vtkViewImage::ResetCamera(void)
{
  if (this->Renderer)
    {
    this->Renderer->ResetCamera();
    }
}

//----------------------------------------------------------------------------
void vtkViewImage::Reset(void)
{
  this->Update();
  this->ResetWindowLevel();
  this->ResetCamera();
}

//----------------------------------------------------------------------------
void vtkViewImage::Enable(void)
{
  this->Interactor->Enable();
}
//----------------------------------------------------------------------------
void vtkViewImage::Disable(void)
{
  this->Interactor->Disable();
}
//----------------------------------------------------------------------------
bool vtkViewImage::GetEnabled(void)
{
  return (this->Interactor->GetEnabled() == 1);
}

//----------------------------------------------------------------------------
void vtkViewImage::SetShowScalarBar(const bool& val)
{
  if (!this->IsColor)
    {
    this->ShowScalarBar = val;
    this->ScalarBarActor->SetVisibility(val);
    }
  else
    {
    this->ShowScalarBar = false;
    this->ScalarBarActor->SetVisibility(this->ShowScalarBar);
    }
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void vtkViewImage::ChangeActorProperty(vtkProp3D* iActor,
                                       vtkProperty* iProperty)
{
  if (iActor && iProperty)
    {
    if (Prop3DCollection->IsItemPresent(iActor))
      {
      vtkActor* temp = dynamic_cast<vtkActor*>(iActor);
      if (temp)
        {
        temp->SetProperty(iProperty);
        //temp->Modified();
        Render();
        }
      }
    }
}
//----------------------------------------------------------------------------
// void vtkViewImage::HighlightContour( vtkProp3D* iProp, const bool& iToDo )
// {
//   if( !iProp )
//     {
//     return;
//     }
//   else
//     {
//     vtkActor* temp = dynamic_cast< vtkActor* >( iProp );
//
//     if( temp )
//       {
//       if( iToDo )
//         {
//         temp->GetProperty()->SetColor( 1., 1., 0. );
//         temp->GetProperty()->SetLineWidth( 3. );
//         }
//       else
//         {
//         temp->GetProperty()->SetLineWidth( 1. );
//         temp->GetProperty()->SetColor( 1., 1., 1. );
//         }
//       }
//     }
// }
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void vtkViewImage::Render()
{
  if (this->RenderWindow && !this->RenderWindow->GetNeverRendered())
    {
    if (this->FirstRender)
      {
      this->ResetCamera();
      this->FirstRender = 0;
      }
    }

  if (this->GetInput())
    {
    this->RenderWindow->Render();
    }
}

//----------------------------------------------------------------------------
vtkRenderWindowInteractor* vtkViewImage::GetInteractor()
{
  return this->Interactor;
}

//----------------------------------------------------------------------------
vtkRenderWindowInteractor* vtkViewImage::GetRenderWindowInteractor()
{
  if (this->GetRenderWindow())
    {
    return this->GetRenderWindow()->GetInteractor();
    }
  else
    {
    return static_cast<vtkRenderWindowInteractor*>(0x0);
    }
}

//----------------------------------------------------------------------------
void vtkViewImage::SetCameraPosition(double* arg)
{
  if (this->Renderer)
    {
    vtkCamera *cam = this->Renderer->GetActiveCamera();
    if (cam)
      {
      cam->SetPosition (arg);
      }
    }
}

double* vtkViewImage::GetCameraPosition(void)
{
  if (this->Renderer)
    {
    vtkCamera *cam = this->Renderer->GetActiveCamera();
    if (cam)
      {
      return cam->GetPosition ();
      }
    }

  return static_cast<double*>(0x0);
}

//----------------------------------------------------------------------------
void vtkViewImage::SetCameraFocalPoint(double* arg)
{
  if (this->Renderer)
    {
    vtkCamera *cam = this->Renderer->GetActiveCamera();
    if (cam)
      {
      cam->SetFocalPoint (arg);
      }
    }
}

//----------------------------------------------------------------------------
double* vtkViewImage::GetCameraFocalPoint(void)
{
  if (this->Renderer)
    {
    vtkCamera *cam = this->Renderer->GetActiveCamera();
    if (cam)
      {
      return cam->GetFocalPoint ();
      }
    }
  return static_cast<double*>(0x0);
}

//----------------------------------------------------------------------------
void vtkViewImage::SetCameraViewUp(double* arg)
{
  if (this->Renderer)
    {
    vtkCamera *cam = this->Renderer->GetActiveCamera();
    if (cam)
      {
      cam->SetViewUp (arg);
      }
    }
}

//----------------------------------------------------------------------------
double* vtkViewImage::GetCameraViewUp(void)
{
  if (this->Renderer)
    {
    vtkCamera *cam = this->Renderer->GetActiveCamera();

    if (cam)
      {
      return cam->GetViewUp();
      }
    }
  return static_cast<double*>(0x0);
}

//----------------------------------------------------------------------------
void vtkViewImage::SetCameraParallelScale(double arg)
{
  if (this->Renderer)
    {
    vtkCamera *cam = this->Renderer->GetActiveCamera();

    if (cam)
      {
      cam->SetParallelScale (arg);
      }
    }
}

//----------------------------------------------------------------------------
double vtkViewImage::GetCameraParallelScale(void)
{
  if (this->Renderer)
    {
    vtkCamera *cam = this->Renderer->GetActiveCamera();

    if (cam)
      {
      return cam->GetParallelScale();
      }
    }

  return 0.;
}

//----------------------------------------------------------------------------
void vtkViewImage::SetShowAnnotations(const int& val)
{
  this->ShowAnnotations = val;
  this->CornerAnnotation->SetVisibility (val);
}
