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

#ifndef _vtkViewImage_h_
#define _vtkViewImage_h_

#include "MegaVTK2Configure.h"
#include "vtkImageViewer2.h"

#include <vtkRenderer.h>
#include <vtkTextProperty.h>
#include <vtkCornerAnnotation.h>
#include <vtkActor.h>
#include <vtkDataSet.h>
#include <vtkProp3DCollection.h>

#include <vector>

/**
   This macro can be useful as we frequently set
   an instance this way, meaning unregistering previously set instance,
   registering given instance, and call a modified event.
   However this might be not the best place to define it...
*/
//BTX
#define vtkSetObjectMacro2Body(object, type, _arg) \
  if ( this->object == _arg ) { return; }          \
  if ( this->object )                              \
    {                                              \
    this->object->UnRegister(this);                \
    }                                              \
  this->object = _arg;                             \
                                                   \
  if ( this->object != NULL )                      \
    {                                              \
    this->object->Register(this);                  \
    }                                              \
                                                   \
  this->Modified();                                \
//ETX

/**
   This macro can be useful as we frequently set
   an instance this way, meaning unregistering previously set instance,
   registering given instance, and call a modified event.
   However this might be not the best place to define it...
*/
//BTX
#define vtkSetObjectMacro2(object, type)         \
  virtual void Set ## object (type * _arg)       \
    {                                            \
    vtkSetObjectMacro2Body (object, type, _arg); \
    }                                            \

//ETX

class vtkMatrix4x4;
class vtkScalarBarActor;
class vtkLookupTable;
class vtkTextProperty;
class vtkCornerAnnotation;
class vtkOrientationAnnotation;
// class vtkQuadricLODActor;
class vtkActor;
class vtkDataSet;
class vtkPolyData;
class vtkProperty;
class vtkProp3DCollection;
class vtkMatrixToLinearTransform;
class vtkRenderWindowInteractor;

class vtkInteractorStyle;
class vtkColorTransferFunction;
class vtkProp3D;
class vtkProp;
class vtkTransform;
class vtkScalarsToColors;

/**
 * \defgroup MegaVTK MegaVTK
 */

/**
  \class vtkViewImage
  \ingroup MegaVTK
  \brief This class is a top-level convenience class for displaying a
  scalar - or RGB image in a 2D or 3D scene.

  It inherits from the vtkImageViewer2 class, which is initially designed
  for 2D scene visualization. However, vtkViewImage overrides some of its
  parents' methods (SetSlice()) in order to generalize its purpose to 2D
  AND 3D scene visualization.

  As a high-level class, it provides the user with convinient
  functionalities such as a colormap (SetLookupTable()), a scalar bar
  (ScalarBarActor), some corner annotations (CornerAnnotation), access to
  the background color (SetBackground()), the annotation text properties
  (SetTextProperty()), or a call for reseting to default values (Reset() or
  ResetCamera()).


  The principle add-on of this class is to tacke the common issue of placing
  different objects in a same consistent reference frame. In a world
  coordinates system, an volume image can be localized by its origin and
  its spacing, and an orientation vector defining how to rotate the volume
  to be consistent with reality.

  The vtkImageData class has among its attributes the origin and the
  spacing information. However, the orientation information is missing.

  The vtkViewImage class tackle this lack by providing the user the
  possibility to set an orientation matrix with SetOrientationMatrix().
  This matrix will directly be applied to the actor describing the image
  in the 2D - or 3D - scene. The rotation 3x3 component of this matrix has
  to be orthogonal (no scaling). The offset component may contain the
  origin information.
  In this case the user will have to make sure that this information is
  absent from the vtkImageData instance given in SetInput(). For that you
  can call : view->GetInput()->SetOrigin(0,0,0).
*/
class VTK_RENDERINGADDON2_EXPORT vtkViewImage:public vtkImageViewer2
{
public:
  vtkTypeRevisionMacro(vtkViewImage, vtkImageViewer2);

  virtual void SetInput(vtkImageData *input);

  // Description:
  // Render the resulting image.
  virtual void Render(void);

  /**
     Access to the RenderWindow interactor
  */
  virtual vtkRenderWindowInteractor * GetInteractor();

  virtual vtkRenderWindowInteractor * GetRenderWindowInteractor();

  /**
   \brief Get the corner annotation.
   \todo make the corner annotation such that it follows the slice number,
   the image scalar value at cursor, the spacing, etc
  */
  vtkGetObjectMacro(CornerAnnotation, vtkCornerAnnotation);
  /**
   \brief Get the scalar bar actor. This instance follows the color
   window/level of the viewer.
  */
  vtkGetObjectMacro(ScalarBarActor, vtkScalarBarActor);
  /**
   \brief The OrientationMatrix instance (GetOrientationMatrix()) is a
    very important added feature of this viewer. It describes the rotation
    and translation to apply to the image bouding box (axis aligned) to the
    world coordinate system.

    Rotation part is usually given by the GetDirection() method on an
    itk::Image for instance. Translation usually correspond to the origin
    of the image given by GetOrigin() on an itk::Image.

    CAUTION: if you provide non-zero origin to the viewer vtkImageData input
     (SetInput()), then don't provide translation to the OrientationMatrix
     instance, otherwise the information is redundant.

     The best behaviour is to force the origin of the vtkImageData input
     to zero and provide this origin information in the OrientationMatrix.
  */
  vtkGetObjectMacro(OrientationMatrix, vtkMatrix4x4);
  virtual void SetOrientationMatrix(vtkMatrix4x4 *matrix);

  /**
     The LookupTable instance (GetLookupTable()) can be used to set a
     user-defined color-table to the viewer. Default is a linear black to
     white table.
  */
  vtkGetObjectMacro(LookupTable, vtkLookupTable);
  virtual void SetLookupTable(vtkLookupTable *lookuptable);

  /**
   * \brief Get a pointer to the current vtkTextProperty
   *
   * The TextProperty instance (GetTextProperty()) describes the font
   * and other settings of the CornerAnnotation instance
   * (GetCornerAnnotation())
  */
  vtkGetObjectMacro(TextProperty, vtkTextProperty);
  virtual void SetTextProperty(vtkTextProperty *textproperty);

  /**
   * \brief Get a pointer to the current vtkProp3DCollection
   *
   * All displayed dataset generates an actor which is added to the renderer.
   * These actors are gathered in this vtkProp3DCollection for easier access.
  */
  vtkGetObjectMacro (Prop3DCollection, vtkProp3DCollection);

  /**
   * \brief Set the world coordinates
   * \param[in] x x value
   * \param[in] y y value
   * \param[in] z z value
   *
   * The world is not always what we think it is ...
   * Use this method to move the viewer slice such that the position
   * (in world coordinates) given by the arguments is contained by
   * the slice plane. If the given position is outside the bounds
   * of the image, then the slice will be as close as possible.
  */
  void SetWorldCoordinates(const double & x,
                           const double & y, const double & z);

  virtual void SetWorldCoordinates(double pos[3]) = 0;

  /**
    \brief Add a dataset to the view (has to be subclass of vtkPointSet).
    The dataset will be cut through the implicit slice plane
    (GetImplicitSlicePlane()).

    This results in a loss of dimensionality, i.e. tetrahedron will be displayed
    as triangles, triangles as lines, lines as points.
    A vtkProperty of the dataset can be specified.
  */
  virtual vtkActor * AddDataSet(vtkDataSet *dataset,
                                vtkProperty *property = NULL,
                                const bool & intersection = true,
                                const bool & iDataVisibility = true) = 0;

  virtual void RemoveProp(vtkProp *iProp);

  /**
     \brief Set/Get the current slice to display (depending on the orientation
     this can be in X, Y or Z).

     This method has been overriden in order to generalize the use of this class
     to 2D AND 3D scene visualization. Thus in this top-level class SetSlice() does
     not do anything.
  */
  virtual void SetSlice(int s)
  {
    this->Superclass::SetSlice (s);
  }

  virtual void Update(void){}

  /**
     \brief Convert an indices coordinate point (image coordinates) into a world
     coordinate point
  */
  virtual double *GetWorldCoordinatesFromImageCoordinates(int indices[3]);
  /**
     \brief Convert a world coordinate point into an image indices coordinate point
  */
  virtual int *GetImageCoordinatesFromWorldCoordinates(double position[3]);
  /**
     \brief Get the pixel value at a given world coordinate point in space, return
     zero if out of bounds.
  */
  virtual double GetValueAtPosition(double worldcoordinates[3],
                                    int component = 0);
  /**
     \brief Set the background color. Format is RGB, 0 <= R,G,B <=1
     Example: SetBackground(0.9,0.9,0.9) for grey-white.
  */
  virtual void SetBackground(double rgb[3]);
  virtual void SetBackground(const double & r,
                             const double & g,
                             const double & b);

  virtual double * GetBackground(void);

  /**
     \brief Reset the camera
  */
  virtual void ResetCamera(void);

  /**
   * \brief Set the camera position
   * \param[in] arg pointer to a double[3] containing the new position of the camera
  */
  void SetCameraPosition(double *arg);

  /**
   * \brief Get the camera position
   * \return pointer to a double[3] containing the position of the camera
  */
  double * GetCameraPosition(void);

  /**
   * \brief Set the camera focal point
   * \param[in] arg pointer to a double[3] containing the new focal point of the
   *  camera
  */
  void SetCameraFocalPoint(double *arg);

  /**
   * \brief Get the camera focal point
   * \return pointer to a double[3] containing the new focal point of the
   *  camera
  */
  double * GetCameraFocalPoint(void);

  /**
   * \brief Set the camera view up
   * \param[in] arg pointer to a double[3] containing the new view up of the
   *  camera
  */
  void SetCameraViewUp(double *arg);

  /**
   * \brief Get the camera view up
   * \return pointer to a double[3] containing the new view up of the
   *  camera
  */
  double * GetCameraViewUp(void);

  /**
   * \brief Set the camera parallel scale
   * \param[in] arg double containing the new parallel scale
  */
  void SetCameraParallelScale(double arg);

  /**
   * \brief Get the camera parallel scale
   * \return double containing the new parallel scale
  */
  double GetCameraParallelScale(void);

  /**
     \brief Reset position - zoom - window/level to default
  */
  virtual void Reset(void);

  /**
   * \brief Set the annotation status.
   * 0: annotations are not visible
   * 1: annotations are visible
  */
  vtkBooleanMacro (ShowAnnotations, int);

  /**
   * \brief Get the annotation status.
   * 0: annotations are not visible
   * 1: annotations are visible
  */
  vtkGetMacro (ShowAnnotations, int);
  /**
   * \brief Change the visibility of the annotations
   * \param[in] iShowAnnotations int
   * 0: annotations are not visible
   * 1: annotations are visible
  */
  virtual void SetShowAnnotations(const int & iShowAnnotations);

  /**
     \brief Enable or Disable interaction on the view.
  */
  virtual void Enable(void);

  /**
     \brief Enable or Disable interaction on the view.
  */
  virtual void Disable(void);

  /**
     \brief Enable or Disable interaction on the view.
  */
  virtual bool GetEnabled(void);

  /**
   * \brief Set the visibility of the scalar bar.
   * 0: scalar bar is not visible
   * 1: scalar bar is visible
  */
  vtkBooleanMacro(ShowScalarBar, int);

  /**
   * \brief Get the visibility of the scalar bar.
   * 0: scalar bar is not visible
   * 1: scalar bar is visible
  */
  vtkGetMacro(ShowScalarBar, int);
  /**
     \brief Show/Hide scalar bar.
  */
  virtual void SetShowScalarBar(const bool &);

  /** \brief Set window and level for mapping pixels to colors. */
  virtual void SetColorWindow(double s);

  virtual void SetColorLevel(double s);

  /**
     \brief Reset the window level
  */
  virtual void ResetWindowLevel(void);

  /**
   * \brief Get the current position in world coordinate.
   * \return double pointer to the current position in world coordinates
   *
   * This framework is only used in vtkViewImage2D to
   * update corner annotations and cursor position.
  */
  double * GetCurrentPoint(void)
  { return this->CurrentPoint; }

  /**
   * \brief Get the current position in world coordinate.
   * \param[in,out] point double pointer to the current position in world coordinates (double[3])
   *
   * This framework is only used in vtkViewImage2D to
   * update corner annotations and cursor position.
  */
  void GetCurrentPoint(double point[3])
  {
    point[0] = this->CurrentPoint[0];
    point[1] = this->CurrentPoint[1];
    point[2] = this->CurrentPoint[2];
  }

  /**
   * \brief Get information about the color of the image.
   * true: multi-channels image (i.e. color)
   * false: single-channel image (i.e. black and white)
  */
  vtkGetMacro(IsColor, bool);

  /**
   * \brief Change the property of an actor
   * \param[in] iActor vtkProp3D pointer to the actor to be modified
   * \param[in] iProperty vtkProperty pointer containing the new property to
   * be applied
  */
  virtual void ChangeActorProperty(vtkProp3D *iActor,
                                   vtkProperty *iProperty);

protected:

  vtkViewImage();
  ~vtkViewImage();

  /**
     The OrientationMatrix instance (GetOrientationMatrix()) is a very important
     added feature of this viewer. It describes the rotation and translation to
     apply to the image bouding box (axis aligned) to the world coordinate system.

     Rotation part is usually given by the GetDirection() method on an itk::Image
     for instance. Translation usually correspond to the origin of the image given
     by GetOrigin() on an itk::Image.

     CAUTION: if you provide non-zero origin to the viewer vtkImageData input
     (SetInput()), then don't provide translation to the OrientationMatrix instance,
     otherwise the information is redundant.

     The best behaviour is to force the origin of the vtkImageData input to zero and
     provide this origin information in the OrientationMatrix.

  */
  vtkMatrix4x4 *OrientationMatrix;
  /**
     The corner annotation gather information related to the image.
     In vtkViewImage2D, it displays slice number, spacing, window-level, position, etc
     Access and change the values with GetCornerAnnotation()->SetText(n, const char*).
     n begins down-right and increases anti-clockwise.
  */
  vtkCornerAnnotation *CornerAnnotation;
  /**
     The TextProperty instance (GetTextProperty()) describes the font and
     other settings of the CornerAnnotation instance (GetCornerAnnotation())
  */
  vtkTextProperty *TextProperty;
  /**
     The LookupTable instance (GetLookupTable()) can be used to set a user-defined
     color-table to the viewer. Default is a linear black to white table.
  */
  vtkLookupTable *LookupTable;
  /**
     Get the scalar bar actor. This instance follows the color window/level
     of the viewer.
  */
  vtkScalarBarActor *ScalarBarActor;
  /**
     All displayed dataset generates an actor which is added to the renderer. (See AddDataSet()).
     These actors are gathered in this vtkProp3DCollection for easier access.
  */
  vtkProp3DCollection *Prop3DCollection;
  /**
     This vtkTransform instance carries the OrientationMatrix (see GetOrientationMatrix())
     and is used to quickly transform the slice plane in vtkViewImage2D.
  */
  vtkMatrixToLinearTransform *OrientationTransform;

  std::string DirectionAnnotationMatrix[3][2];

  /**
     local instances.
  */
  int ShowAnnotations;
  int ShowScalarBar;
  /**
     Get the current position in world coordinate.
     This framework is only used in vtkViewImage2D to
     update corner annotations and cursor position.
  */
  double CurrentPoint[3];

  bool IsColor;
};

#endif /* _vtkViewImage_h_ */
