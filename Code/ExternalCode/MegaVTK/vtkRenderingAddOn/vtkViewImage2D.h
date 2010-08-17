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
//  and/or other materials provided with the distribution.
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

#ifndef _vtkViewImage2D_h_
#define _vtkViewImage2D_h_

#include "vtkViewImage.h"
#include "MegaVTK2Configure.h"

#include <list>

#include "vtkTransform.h"
#include "vtkImageActor.h"
#include "vtkInteractorStyleImage2D.h"

class vtkPlane;
class vtkViewImage2DCommand;
class vtkTransform;
class vtkInteractorStyle;
class vtkInteractorStyleRubberBandZoom;
class vtkOrientationAnnotation;
class vtkPointHandleRepresentation2D;
class vtkCursor2D;
class vtkCellPicker;
class vtkActor;
// class vtkQuadricLODActor;

/**
 * \class vtkViewImage2D
 * \ingroup MegaVTK
 * \brief Basic class to handle 32d/3D items such as images and polydatas
 * visualization in 2D
 */
class VTK_RENDERINGADDON2_EXPORT vtkViewImage2D : public vtkViewImage
  {

public:

  static vtkViewImage2D* New();
  vtkTypeRevisionMacro(vtkViewImage2D, vtkViewImage);
  /**
     \brief The orientation of the view is a abstract representation of the object
     we are looking at. It results from the acquisition plane. Setting the View
     Orientation by calling SetViewOrientation() will imply the view to set its
     inner "slice" orientation. (slice orientation == 2 means plane of
     acquisition.)

     \note The view orientations defined here are orthogonal to the normal basis
     in the scanner. A very interesting improvement would be to define "oblique"
     view orientations for cardiac imaging, something like:

     VIEW_ORIENTATION_SHORT_AXIS, VIEW_ORIENTATION_LONG_AXIS, and
     VIEW_ORIENTATION_FOUR_CHAMBER could define the different views that are
     usually used in cardiac imaging.

     From this user-input information, the idea would be to evaluate which slice
     orientation does correspond to the requested view. This can be done by
     evaluating the dot product between the axis of acquisition and a
     pre-defined oblique axis that "should" correspond to the requested
     orientation...
  **/
  //BTX
  enum
    {
    VIEW_ORIENTATION_SAGITTAL = 0,
    VIEW_ORIENTATION_CORONAL = 1,
    VIEW_ORIENTATION_AXIAL = 2
    };
  //ETX
  /**
     Description:
  **/
  //BTX
  enum
    {
    VIEW_CONVENTION_RADIOLOGICAL = 0,
    VIEW_CONVENTION_NEUROLOGICAL = 1
    };
  //ETX
  /**
     Description:
     These types describe the behaviour
     of the interactor style.
  */
  //BTX
  enum
    {
    INTERACTOR_STYLE_NAVIGATION = 0,
    INTERACTOR_STYLE_RUBBER_ZOOM
    };
  //ETX

  /**
   * \brief The SliceImplicitPlane instance (GetImplicitSlicePlane()) is the
   * implicit function that cuts every dataset that is added with AddDataSet().
  */
  vtkGetObjectMacro(SliceImplicitPlane, vtkPlane);
  /**
     \brief The SlicePlane instance (GetSlicePlane()) is the polygonal
     square corresponding to the slice plane,
     it is updated each time the slice changes,
     and is color-coded according to conventions
  */
  vtkGetObjectMacro(SlicePlane, vtkPolyData);
  /**
   * \brief Get the orientation annotation. This annotation describes the
   * orientation of the slice plane, according to the rule
   * Right(R)-Left(L) Anterior(A)-Posterior(P) Inferior(I)-Superior(S)
 */
  vtkGetObjectMacro(OrientationAnnotation, vtkOrientationAnnotation);

  /**
   * \brief The world is not always what we think it is ...
   * Use this method to move the viewer slice such that the position
   * (in world coordinates) given by the arguments is contained by
   * the slice plane. If the given position is outside the bounds
   * of the image, then the slice will be as close as possible.
   */
  virtual void SetWorldCoordinates(double pos[3]);

  /**
   * \brief Set/Get the current slice to display (depending on the orientation
   * this can be in X, Y or Z).
   */
  virtual void SetSlice(int s);

  /**
   * \brief Instead of setting the slice orientation to an axis (YZ - XZ - XY),
   * you can force the view to be axial (foot-head), coronal (front-back),
   * or sagittal (left-right). It will just use the OrientationMatrix
   * (GetOrientationMatrix()) to check which slice orientation to pick.
  */
  vtkGetMacro(ViewOrientation, int);
  virtual void SetViewOrientation(int orientation);
  virtual void SetOrientationMatrix(vtkMatrix4x4* matrix);

  /**
   * \brief The ViewConvention instance explains where to place the camera around
   * the patient. Default behaviour is Radiological convention, meaning
   * we respectively look at the patient from his feet, his face and his left
   * ear.
   * For Neurological convention, we respectively look from the top of his head,
   * the the back of his head, and his left ear.
   * \todo Why not adding cardiologic conventions where we look at the patient in
   * oblique angles ?
   */
  vtkGetMacro(ViewConvention, int);
  virtual void SetViewConvention(int convention);

  /**
   * \brief Convert an indices coordinate point (image coordinates) into a world
   * coordinate point
  */
  virtual double* GetWorldCoordinatesForSlice(int slice);
  /**
   * \brief Convert a world coordinate point into an image indices coordinate point
  */
  virtual int GetSliceForWorldCoordinates(double pos[3]);
  /**
   * \brief Reset the 3D position to center
  */
  virtual void ResetPosition(void);
  /**
   * \brief Reset position - zoom - window/level to default
  */
  virtual void Reset(void);
  /**
   * \brief Reset the camera in a nice way for the 2D view
   */
  virtual void ResetCamera(void);
  /**
   * \brief Get/Set the zoom factor of the view
   */
  vtkSetMacro(Zoom, double);
  vtkGetMacro(Zoom, double);
  /**
   * \brief Useful method that transform a display position into a world corrdinate point
  */
  virtual double* GetWorldCoordinatesFromDisplayPosition(int xy[2]);
  virtual double* GetWorldCoordinatesFromDisplayPosition(const int& x,
                                                         const int& y);

  //BTX
  /**
     Access to the command of the viewer.
     This instance is in charge of observing the interactorstyle (GetInteractorStyle())
     and update things accordingly in the view (i.e. the slice number when moving slice).
  */
  vtkGetObjectMacro (Command, vtkViewImage2DCommand);
  //ETX

  /**
     Get/Set whether or not the interpolation between pixels should be activated.
     It is On by default
  */
  virtual void SetInterpolate(const int& val);
  virtual int GetInterpolate();
  vtkBooleanMacro (Interpolate, int);

  /**
   * \brief Add a dataset to the view (has to be subclass of vtkPointSet).
   * The dataset will be cut through the implicit slice plane
   * (GetImplicitSlicePlane()).
   * This results in a loss of dimensionality, i.e. tetrahedron will be displayed
   * as triangles, triangles as lines, lines as points.
   * A vtkProperty of the dataset can be specified.
   *
   * \param[in] dataset Data to be displayed (shape, position, etc.)
   * \param[in] property Property of the data to be displayed (color, opacity, etc.)
   * \param[in] intersection Display projection or intersection of the dataset with the current slice
   * \param[in] iDataVisibility Visibility of the current actor
  */
//   virtual vtkQuadricLODActor*
  virtual vtkActor* AddDataSet(vtkDataSet* dataset,
                               vtkProperty* property = NULL,
                               const bool& intersection = true,
                               const bool& iDataVisibility = true);

  //virtual vtkQuadricLODActor*
  virtual vtkActor* AddDataSet(vtkPolyData* polydata,
                               vtkProperty* property = NULL,
                               const bool& intersection = true,
                               const bool& iDataVisibility = true);

  vtkSetVector3Macro(CameraMotionVector, double);
  vtkGetVector3Macro(CameraMotionVector, double);

  /**
   * \brief Show/Hide the annotations.
  */
  vtkGetMacro(ShowAnnotations, int);
  /**
   * \brief Show/Hide the annotations.
  */
  vtkBooleanMacro(ShowAnnotations, int);
  /**
   * \brief Show/Hide the annotations.
  */
  virtual void SetShowAnnotations(const int&);

  void SetCameraFocalAndPosition(double focal[3], double pos[3]);
  void GetCameraFocalAndPosition(double focal[3], double pos[3]);

  /**
     Access to the position of the center of the view.
     CAUTION: for the moment this feature is de-activated because updating it
     slows down the visualization process.
  */
  vtkGetVector3Macro (ViewCenter, double);

  virtual void Update(void)
  { this->UpdateOrientation(); }

  /**
   * \brief Set interaction style to Default mode:
   * Left button: Window level
   * Right button: Zoom
   * Middle button: Pan
   * Wheel: Throught volume
  */
  void SetDefaultInteractionStyle(void)
  {
    vtkInteractorStyleImage2D* t = vtkInteractorStyleImage2D::SafeDownCast (this->InteractorStyle);
    if (t)
      {
      t->SetDefaultMode();
      }
  }

  /**
   * \brief Set interaction style to Zoom mode:
   * All buttons: Zoom
  */
  void SetZoomInteractionStyle(void)
  {
    vtkInteractorStyleImage2D* t = vtkInteractorStyleImage2D::SafeDownCast (this->InteractorStyle);
    if (t)
      {
      t->SetZoomMode();
      }
  }

  /**
   * \brief Set interaction style to Pan mode:
   * All buttons: Pan
  */
  void SetPanInteractionStyle(void)
  {
    vtkInteractorStyleImage2D* t = vtkInteractorStyleImage2D::SafeDownCast (this->InteractorStyle);
    if (t)
      {
      t->SetPanMode();
      }
  }

  /**
   * \brief Set interaction style to Zoom mode:
   * The actor the mouse is highlighted in addition to the Default Mode.
  */
  void SetPickInteractionStyle(void)
  {
    vtkInteractorStyleImage2D* t = vtkInteractorStyleImage2D::SafeDownCast (this->InteractorStyle);
    if (t)
      {
      t->SetPickMode();
      }
  }

  /**
     Access to the actor corresponding to the cursor. It follows the mouse cursor
     everywhere it goes, and can be activated by pressing 'c'
  */
  vtkGetObjectMacro (Cursor, vtkPointHandleRepresentation2D);
  vtkGetObjectMacro (CursorGenerator, vtkCursor2D);


  /**
   *
   */
  template<class TContourContainer,
           class TPropertyContainer>
  void AddContours(TContourContainer& iContours,
                   TPropertyContainer& iProperty,
                   const bool& iIntersection = true)
  {
    if (iContours.size() != iProperty.size())
      {
      vtkWarningMacro(<< "iContours.size() != iProperty.size()");
      return;
      }

    typedef typename TContourContainer::iterator  ContourContainerIterator;
    typedef typename TPropertyContainer::iterator PropertyContainerIterator;

    ContourContainerIterator contour_it = iContours.begin();
    ContourContainerIterator contour_end = iContours.end();

    PropertyContainerIterator prop_it = iProperty.begin();
    PropertyContainerIterator prop_end = iProperty.end();

    while (contour_it != contour_end)
      {
      this->AddDataSet(*contour_it, *prop_it, iIntersection);
      ++contour_it;
      ++prop_it;
      }
  }

  /**
   *
   */

  template<class TContourContainer>
  void RemoveContours(TContourContainer& iContours)
  {
    typedef typename TContourContainer::iterator ContourContainerIterator;
    ContourContainerIterator contour_it = iContours.begin();
    ContourContainerIterator contour_end = iContours.end();

    while (contour_it != contour_end)
      {
      this->RemoveDataSet(*contour_it);
      ++contour_it;
      }
  }

  /**
   *
   */
  void UpdateWindowLevelObservers();

protected:

  vtkViewImage2D();
  ~vtkViewImage2D();

  virtual void UpdateSlicePlane(void);

  virtual void UpdateCenter(void);

  virtual void UpdateOrientation();

  virtual void PostUpdateOrientation(void);

  virtual void SetSlicePlaneFromOrientation(void);

  virtual int  SetCameraFromOrientation(void);

  virtual void SetAnnotationsFromOrientation(void);

  /**
   * \brief After the orientation has changed, it is crucial to adapt
   * a couple of things according to new orientation.
   * Thus UpdateOrientation() is here overwritten and calls
   * PostUpdateOrientation(), where the SlicePlane, the Camera settings,
   * the CornerAnnotation, and the SliceImplicitPlane are modified.
  */
  virtual void SetImplicitPlaneFromOrientation(void);
  /**
   * \brief Update the cursor position and the CornerAnnotation (top-left) according
   * to current mouse position.
  */
  ///\todo This may has to be modified as with this configuration, the user has no possibility of changing the upper-left corner annotation because it is bypassed by this method at each mouse movement.
  virtual void UpdateCursor(void);
  /**
   * \brief This method is called each time the orientation changes (SetViewOrientation())
   * and sets the appropriate color to the slice plane.
   * Red: R-L direction --> sagittal orientation
   * Green: A-P direction --> coronal orientation
   * Blue: I-S direction --> axial orientation
  */
  virtual void InitializeSlicePlane(void);
  /**
   * \brief Overwrite of the Superclass InstallPipeline() method in order to set up the
   * home made InteractorStyle, and make it observe all events we need
  */
  virtual void InstallPipeline(void);

  virtual int SetCameraToConvention(void);
  virtual void SetAnnotationToConvention(void);
  virtual void SetSlicePlaneToConvention(unsigned int axis);

  vtkMatrix4x4* ConventionMatrix;

  vtkPlane* SliceImplicitPlane;

  vtkTransform* AdjustmentTransform;
  /**
   * \brief This polydata instance is a square colored (see InitializeSlicePlane()) according to the
   * orientation of the view. It follows the image actor and is used by other view to display
   * intersections between views.
  */
  vtkPolyData* SlicePlane;
  /**
   * \brief Get the orientation annotation. This annotation describes the orientation
   * of the slice plane, according to the rule
   * Right(R)-Left(L) Anterior(A)-Posterior(P) Inferior(I)-Superior(S)
  */
  vtkOrientationAnnotation* OrientationAnnotation;

  vtkInteractorStyle* InteractorStyleSwitcher;
  /**
   * \brief InteractorStyle used in this view. It is a vtkInteractorStyleImage2D by default
   * but can be set to vtkInteractorStyleRubberBandZoom with SetInteractorStyleType().
   * Rubber band zoom is in beta. Prefer using default behaviour.
  */
  vtkInteractorStyleRubberBandZoom* InteractorStyleRubberZoom;
  /**
   * \brief Access to the command of the viewer.
   * This instance is in charge of observing the interactorstyle (GetInteractorStyle())
   * and update things accordingly in the view (i.e. the slice number when moving slice).
  */
  vtkViewImage2DCommand* Command;
  /**
   * \brief Access to the actor corresponding to the cursor. It follows the mouse cursor
   * everywhere it goes, and can be activated by pressing 'c'
  */
  vtkPointHandleRepresentation2D* Cursor;
  /**
   * \brief Access to the actor corresponding to the cursor. It follows the mouse cursor
   * everywhere it goes, and can be activated by pressing 'c'
  */
  vtkCursor2D* CursorGenerator;

  int ViewOrientation;
  int ViewConvention;
  int InteractorStyleType;

  double ViewCenter[3];

  char SliceAndWindowInformation[64];
  char ImageInformation[64];

  double Zoom;
  double CameraMotionVector[3];
  };

#endif /* _vtkViewImage2D_h_ */
