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

#ifndef _vtkViewImage3D_h_
#define _vtkViewImage3D_h_

#include "vtkViewImage.h"
#include "MegaVTK2Configure.h"

#include "vtkInteractorStyleImage3D.h"

#include <vtkImageActor.h>
#include <vtkRenderer.h>
#include <vtkImageMapToWindowLevelColors.h>
#include <vtkImageMapToColors.h>
#include "vtkOrientedBoxWidget.h"
#include <vtkPlaneWidget.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkVolumeRayCastMapper.h>
#include <vtkVolumeRayCastCompositeFunction.h>
#include <vtkVolumeRayCastMIPFunction.h>
#include <vtkVolumeRayCastIsosurfaceFunction.h>
#include <vtkVolumeProperty.h>
#include "vtkImage3DCroppingBoxCallback.h"
#include <vtkObjectFactory.h>

#include <vector>

// For the picked/unpicked actors
#include <list>
#include "vtkProp3D.h"

class vtkViewImage3DCommand;
class vtkVolumeTextureMapper3D;
class vtkVolumeMapper;
class vtkVolumeRayCastFunction;
class vtkPiecewiseFunction;
class vtkColorTransferFunction;
class vtkVolume;
class vtkImageActor;
class vtkAxes;
class vtkDataSet3DCroppingPlaneCallback;
class vtkTubeFilter;
class vtkAnnotatedCubeActor;
class vtkOrientationMarkerWidget;
class vtkAxesActor;
class vtkImageBlend;
class vtkViewImage2D;
class vtkScalarsToColors;
class vtkColorTransferFunction;

class ImageActorCallback;

class vtkPlanes;

/**
 * \class vtkViewImage3D
 * \ingroup MegaVTK
 * \brief Basic class to handle items such as images and polydatas
 * visualization in 3D
 */
class VTK_RENDERINGADDON2_EXPORT vtkViewImage3D:public vtkViewImage
{
public:
  /**
   * \brief Convenient method to access the constructor.
   */
  static vtkViewImage3D * New();

  vtkTypeRevisionMacro(vtkViewImage3D, vtkViewImage);

  // Description:
  // Render the resulting image.
  virtual void Render(void);

  /**
    Add a dataset to the view (has to be subclass of vtkPointSet).
    The dataset will be cut through the implicit slice plane
    (GetImplicitSlicePlane()).

    This results in a loss of dimensionality, i.e. tetrahedron will be displayed
    as triangles, triangles as lines, lines as points.
    A vtkProperty of the dataset can be specified.
  */
//   virtual vtkQuadricLODActor*
  virtual vtkActor * AddDataSet(vtkDataSet *dataset,
                                vtkProperty *property = NULL,
                                const bool & intersection = true,
                                const bool & iDataVisibility = true);

  /**
     Set/Get the current slice to display (depending on the orientation
     this can be in X, Y or Z).
  */
  virtual void SetSlice(int s){ (void)s; }
  virtual void SetSliceOrientation(int orientation){ (void)orientation; }
  // Description:
  // Update the display extent manually so that the proper slice for the
  // given orientation is displayed. It will also try to set a
  // reasonable camera clipping range.
  // This method is called automatically when the Input is changed, but
  // most of the time the input of this class is likely to remain the same,
  // i.e. connected to the output of a filter, or an image reader. When the
  // input of this filter or reader itself is changed, an error message might
  // be displayed since the current display extent is probably outside
  // the new whole extent. Calling this method will ensure that the display
  // extent is reset properly.
  virtual void UpdateDisplayExtent(){}

  virtual void Add2DPhantom(
    const unsigned int & i,
    vtkImageActor *input,
    vtkPolyData *in_bounds = NULL);

  virtual void SetOrientationMatrix(vtkMatrix4x4 *matrix);

  /*
   * \brief Set volume rendering on
   */
  void SetVolumeRenderingOn();

  /*
   * \brief Set volume rendering off
   */
  void SetVolumeRenderingOff();

  /*
   * \brief Set TriPlanar rendering on
   */
  void SetTriPlanarRenderingOn();

  /*
   * \brief Set TriPlanar rendering off
   */
  void SetTriPlanarRenderingOff();

  /*
   * \brief Set the volume mapper to texture (for volume rendering purpose)
   */
  void SetVolumeMapperToTexture(void)
  {
    this->VolumeActor->SetMapper (this->VolumeMapper3D);
    this->Callback->SetVolumeMapper (this->VolumeMapper3D);
  }

  /*
   * \brief Set the volume mapper to ray cast (for volume rendering purpose)
   */
  void SetVolumeMapperToRayCast(void)
  {
    this->VolumeActor->SetMapper (this->VolumeRayCastMapper);
    this->Callback->SetVolumeMapper (this->VolumeRayCastMapper);
  }

  /*
   * \brief Set the volume ray cast function to composite
   * (for volume rendering purpose)
   */
  void SetVolumeRayCastFunctionToComposite(void)
  {
    this->VolumeRayCastMapper->SetVolumeRayCastFunction
      (this->VolumeRayCastCompositeFunction);
  }

  /*
   * \brief Set the volume ray cast function to MIP
   * (for volume rendering purpose)
   */
  void SetVolumeRayCastFunctionToMIP(void)
  {
    this->VolumeRayCastMapper->SetVolumeRayCastFunction
      (this->VolumeRayCastMIPFunction);
  }

  void SetVolumeRayCastFunctionToIsosurface(void)
  {
    this->VolumeRayCastMapper->SetVolumeRayCastFunction
      (this->VolumeRayCastIsosurfaceFunction);
  }

  /** Set the box widget visibility */
  /*void SetBoxWidgetVisibility(const bool& a)
  {
    if (this->Interactor) this->BoxWidget->SetEnabled (a);
  }
  bool GetBoxWidgetVisibility(void)
  {
    return (this->BoxWidget->GetEnabled() == 1);
  }
  vtkBooleanMacro (BoxWidgetVisibility, int);
*/
  /** Set the plane widget on */
  /*void SetPlaneWidgetVisibility (const bool& a)
  {
    if (this->Interactor)
      this->PlaneWidget->SetEnabled (a);
  }
  bool GetPlaneWidgetVisibility (void)
  {
    return ( this->PlaneWidget->GetEnabled() == 1 );
  }
  vtkBooleanMacro (PlaneWidgetVisibility, int);*/

  /**
   * \brief Set the cube visibility
   * \param[in] a true: visible, false: not visible
   * */
  void SetCubeVisibility(const bool & a)
  {
    if ( this->Interactor ) { this->Marker->SetEnabled (a); }
  }

  /**
   * \brief Get the cube visibility
   * \return true: visible, false: not visible
   * */
  bool GetCubeVisibility(void)
  {
    return ( this->Marker->GetEnabled() == 1 );
  }

  vtkBooleanMacro (CubeVisibility, int);

  /**
   * \brief Set the shade
   * \param[in] a true: enable, false: disable
   * */
  void SetShade(const bool & a)
  {
    this->VolumeProperty->SetShade (a);
  }

  /**
   * \brief Get the shade
   * \return a true: enable, false: disable
   * */
  bool GetShade(void)
  {
    return ( this->VolumeProperty->GetShade() == 1 );
  }

  vtkBooleanMacro (Shade, int);

  /** Get volume actor */
  vtkGetObjectMacro (VolumeActor, vtkVolume);
  //vtkGetObjectMacro (PlaneWidget, vtkPlaneWidget);
  vtkGetObjectMacro (VolumeProperty, vtkVolumeProperty);
  //vtkGetObjectMacro (BoxWidget, vtkOrientedBoxWidget);

  virtual void SetWorldCoordinates(double pos[3]) { (void)pos; }

  /**
   * \brief Set the bounds actors visibility
   * \param iVisibility true: visible, false: not visible
   * */
  void SetBoundsActorsVisibility(bool iVisibility);

  /**
   * \brief Get the interactor style for the vtkViewImage3D
   * \return pointer to the current vtkInteractorStyleImage3D
   * */
  vtkInteractorStyleImage3D * GetInteractorStyle3D();

  vtkGetObjectMacro (Command, vtkViewImage3DCommand);

  /**
   * \brief Compute the distance between 2 points
   * \param[in] n double pointer to the first point (double[3])
   * \param[in] origin double pointer to the second point (double[3])
   * */
  void ComputeDistances(double *n, double *origin);

  /**
   * \brief Compute the disctance from the actors to the surface
   * \param[in] planes vtkPlanes pointer to the surface of interest
   * */
  void ComputeDistancesToSquare(vtkPlanes *planes);

protected:

  vtkViewImage3D();
  ~vtkViewImage3D();

  // Description:
  virtual void InstallPipeline();

  virtual void UpdateOrientation(){}

  virtual void SetupVolumeRendering();

  virtual void SetupWidgets();

  virtual void SetupTextureMapper();

  // texture mapper in 3D
  vtkVolumeMapper *VolumeMapper3D;
  // volume ray cast mapper
  vtkVolumeRayCastMapper *VolumeRayCastMapper;
  // ray cast function
  vtkVolumeRayCastMIPFunction *VolumeRayCastMIPFunction;
  // ray cast function
  vtkVolumeRayCastCompositeFunction *VolumeRayCastCompositeFunction;
  // ray cast function
  vtkVolumeRayCastIsosurfaceFunction *VolumeRayCastIsosurfaceFunction;
  // volume property
  vtkVolumeProperty *VolumeProperty;
  // volume actor
  vtkVolume *VolumeActor;
  // opacity transfer function
  vtkPiecewiseFunction *OpacityFunction;
  // blender
  vtkImageBlend *Blender;
  // image 3D cropping box callback
  vtkImage3DCroppingBoxCallback *Callback;

  std::vector< vtkImageActor * >      Phantom;
  std::vector< ImageActorCallback * > PhantomCallback;
  std::vector< vtkActor * >           BoundsActor;

  // box widget
  //vtkOrientedBoxWidget* BoxWidget;
  // vtkPlane widget
  // vtkPlaneWidget*                 PlaneWidget;
  // annotated cube actor
  vtkAnnotatedCubeActor *     Cube;
  vtkOrientationMarkerWidget *Marker;

  /**
     Access to the command of the viewer.
     This instance is in charge of observing the interactorstyle (GetInteractorStyle())
     and update things accordingly in the view (i.e. the slice number when moving slice).
  */
  vtkViewImage3DCommand *    Command;
  vtkInteractorStyleImage3D *InteractorStyle3D;
};

#endif /* _vtkViewImage3D_h_ */
