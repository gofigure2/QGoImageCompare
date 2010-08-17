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

#ifndef _vtkViewImage2DCollection_h_
#define _vtkViewImage2DCollection_h_

#include "vtkCollection.h"
#include "vtkCommand.h"

#include "vtkViewImage2D.h"
#include "MegaVTK2Configure.h"

#include <vtkstd/vector>

/**
   This macro can be useful as we frequently set
   an instance this way, meaning unregistering previously set instance,
   registering given instance, and call a modified event.
   However this might be not the best place to define it...
*/
//BTX
#define vtkSyncSetMacro(name, type, base)                      \
  virtual void SyncSet ## name (type _arg)                \
    {                                                     \
    this->InitTraversal();                              \
    base* item = this->GetNextItem();   \
    while (item)                                         \
      {                                                   \
      item->Set ## name (_arg);                           \
      item = this->GetNextItem();                       \
      }                                                   \
    }
///ETX

///BTX
#define vtkSyncSetObjectMacro(name, type, base)         \
  virtual void SyncSet ## name (type * _arg)               \
    {                                                     \
    this->InitTraversal();                              \
    base* item = this->GetNextItem();   \
    while (item)                                         \
      {                                                   \
      item->Set ## name (_arg);                           \
      item = this->GetNextItem();                       \
      }                                                   \
    }

///ETX

class vtkScalarBarActor;
class vtkLookupTable;
class vtkTextProperty;
class vtkCornerAnnotation;
class vtkOrientationAnnotation;
class vtkPlane;
class vtkActor;
class vtkDataSet;
class vtkPolyData;
class vtkProperty;
class vtkProp3DCollection;
class vtkDataSetCollection;
class vtkMatrixToLinearTransform;
class vtkViewImage2DCollectionCommand;

/**
 * \class vtkViewImage2DCollection
 * \ingroup MegaVTK
 * \brief Manage a collection of 2D views
 */
class VTK_RENDERINGADDON2_EXPORT vtkViewImage2DCollection : public vtkCollection
  {
public:

  static vtkViewImage2DCollection* New();
  vtkTypeRevisionMacro(vtkViewImage2DCollection, vtkCollection);

  /*
   * \bief Get the next vtkViewImage2D in the list. Return NULL when at the end of
   * the list
   */
  vtkViewImage2D *GetNextItem()
  {
    return static_cast<vtkViewImage2D *>(this->GetNextItemAsObject());
  }
  /*
   * \bief Get the next vtkViewImage2D2D in the list. Return NULL when at the end of
   * the list
   */
  vtkViewImage2D *GetItem(int i)
  {
    return static_cast<vtkViewImage2D *>(this->GetItemAsObject(i));
  }
  /*
   * \bief Add an object to the list. Does not prevent duplicate entries.
   */
  void AddItem(vtkViewImage2D*);
  /*
   * \bief Replace the i'th item in the collection with another 2D image
   */
  void ReplaceItem(int i, vtkViewImage2D *);

  /*
   * \brief Remove the i'th item in the list.
   * Be careful if using this function during traversal of the list using
   * GetNextItemAsObject (or GetNextItem in derived class).  The list WILL
   * be shortened if a valid index is given!  If this->Current is equal to the
   * element being removed, have it point to then next element in the list.
   */
  void RemoveItem(int i);

  /*
   * \brief Remove an object from the list. Removes the first object found, not
   * all occurrences. If no object found, list is unaffected.  See warning
   * in description of RemoveItem(int).
   */
  void RemoveItem(vtkViewImage2D *);

  /*
   * \brief Remove all objects from the collection.
   */
  void RemoveAllItems();

  /*
   * \brief Initialize the planes actors in all the views
   */
  void Initialize();
  /*
   * \brief Initialize the the observers.
   */
  void InitializeAllObservers();

  vtkGetObjectMacro (Command, vtkViewImage2DCollectionCommand);
  vtkGetObjectMacro (ExtraRenderWindow, vtkRenderWindow);

  /*
   * \brief Add an extra window of a different type to the collection
   * \param[in] wim Render Window associated to the extra window
   */
  void SetExtraRenderWindow(vtkRenderWindow* win)
  { this->ExtraRenderWindow = win; }

  vtkSyncSetMacro (Slice, int, vtkViewImage2D);
  vtkSyncSetMacro (SliceOrientation, int, vtkViewImage2D);
  vtkSyncSetMacro (ShowAnnotations, bool, vtkViewImage2D);
  vtkSyncSetMacro (ShowScalarBar, bool, vtkViewImage2D);

  vtkSyncSetMacro (ColorWindow, double, vtkViewImage2D);
  vtkSyncSetMacro (ColorLevel, double, vtkViewImage2D);

  vtkSyncSetObjectMacro (OrientationMatrix, vtkMatrix4x4, vtkViewImage2D);
  vtkSyncSetObjectMacro (LookupTable, vtkLookupTable, vtkViewImage2D);
  vtkSyncSetObjectMacro (TextProperty, vtkTextProperty, vtkViewImage2D);
  vtkSyncSetObjectMacro (Input, vtkImageData, vtkViewImage2D);
  vtkSyncSetObjectMacro (InputConnection, vtkAlgorithmOutput, vtkViewImage2D);

  vtkSyncSetObjectMacro (Size, int, vtkViewImage2D);
  vtkSyncSetObjectMacro (Position, int, vtkViewImage2D);
  vtkSyncSetObjectMacro (WorldCoordinates, double, vtkViewImage2D);

  /// Description: Synchronize interpolate between views
  vtkSyncSetMacro (Interpolate, int, vtkViewImage2D);

  /*/// Description: Synchronize background color between views
  vtkSyncSetObjectMacro (Background, double, vtkViewImage2D);
  /// Description: Synchronize camera position between views
  vtkSyncSetObjectMacro (CameraPosition, double,vtkViewImage2D);
  /// Description: Synchronize camera focal point between views
  vtkSyncSetObjectMacro (CameraFocalPoint, double, vtkViewImage2D);
  /// Description: Synchronize camera view up between views
  vtkSyncSetObjectMacro (CameraViewUp, double, vtkViewImage2D);
  /// Description: Synchronize camera parallel scale between views
  vtkSyncSetMacro (CameraParallelScale, double, vtkViewImage2D);

  /// Description: Synchronize view orientation between views (also see SyncSetSliceOrientation())
  vtkSyncSet2DMacro (ViewOrientation, int);
  /// Description: Synchronize view convention between views
  vtkSyncSet2DMacro (ViewConvention, int);

  /// Description: Synchronize interactor style type between views
  vtkSyncSet2DMacro (InteractorStyleType, int);
  /// Description: Synchronize mouse interaction between views
  vtkSyncSet2DMacro (LeftButtonInteractionStyle, int);
  /// Description: Synchronize mouse interaction between views
  vtkSyncSet2DMacro (RightButtonInteractionStyle, int);
  /// Description: Synchronize mouse interaction between views
  vtkSyncSet2DMacro (MiddleButtonInteractionStyle, int);
  /// Description: Synchronize mouse interaction between views
  vtkSyncSet2DMacro (WheelInteractionStyle, int);
  /// Description: Synchronize mouse interaction between views
  vtkSyncSet2DMacro (InteractionStyle, int);*/

  /// Description: Synchronize dataset addition between views
  virtual void SyncAddDataSet(vtkDataSet* dataset, vtkProperty* property = NULL);
  /// Description: Synchronize dataset removal between views
  virtual void SyncRemoveDataSet(vtkDataSet* dataset);

  /// Description: Synchronize reset window level between views
  virtual void SyncResetWindowLevel(void);
  /// Description: Synchronize reset camera between views
  virtual void SyncResetCamera(void);
  /// Description: Synchronize render between views
  virtual void SyncRender();
  /// Description: Synchronize reset between views
  virtual void SyncReset(void);
  /// Description: Synchronize interactor start between views
  virtual void SyncStart(void);

  // Decide weither or not the collection will link interactions

  /// Description: link slice flag
  vtkGetMacro (LinkSliceMove, unsigned int);
  /// Description: link slice flag
  virtual void SetLinkSliceMove(unsigned int v);
  /// Description: link slice flag
  vtkBooleanMacro (LinkSliceMove, unsigned int);

  /// Description: link color window flag
  vtkGetMacro (LinkColorWindowLevel, unsigned int);
  /// Description: link color window flag
  virtual void SetLinkColorWindowLevel(unsigned int v);
  /// Description: link color window flag
  vtkBooleanMacro (LinkColorWindowLevel, unsigned int);

  /// Description: link reset color window flag
  vtkGetMacro (LinkResetWindowLevel, unsigned int);
  /// Description: link reset color window flag
  virtual void SetLinkResetWindowLevel(unsigned int v);
  /// Description: link reset color window flag
  vtkBooleanMacro (LinkResetWindowLevel, unsigned int);

  /// Description: link reset viewer flag
  vtkGetMacro (LinkResetViewer, unsigned int);
  /// Description: link reset viewer flag
  virtual void SetLinkResetViewer(unsigned int v);
  /// Description: link reset viewer flag
  vtkBooleanMacro (LinkResetViewer, unsigned int);

  /// Description: link requested position (double click) flag
  vtkGetMacro (LinkRequestedPosition, unsigned int);
  /// Description: link requested position (double click) flag
  virtual void SetLinkRequestedPosition(unsigned int v);
  /// Description: link requested position (double click) flag
  vtkBooleanMacro (LinkRequestedPosition, unsigned int);

  /// Description: link camera flag
  vtkGetMacro (LinkCamera, unsigned int);
  /// Description: link camera flag
  virtual void SetLinkCamera(unsigned int v);
  /// Description: link camera flag
  vtkBooleanMacro (LinkCamera, unsigned int);

  /// Description: link position flag
  vtkGetMacro (LinkPosition, unsigned int);
  /// Description: link position flag
  virtual void SetLinkPosition(unsigned int v);
  /// Description: link position flag
  vtkBooleanMacro (LinkPosition, unsigned int);

  /// Description: show axes (view intersections) flag
  virtual void SetShowAxes(unsigned int v);
  /// Description: show axes (view intersections) flag
  vtkBooleanMacro (ShowAxes, unsigned int);
  /// Description: show axes (view intersections) flag
  vtkGetMacro (ShowAxes, unsigned int);

  void SyncSetBackground(double* rgb);
  void SyncPan();
  void SyncSetZoomAndParallelScale(double Zoom, double ParallelScale);

  /**
   * \brief Set the visibility of the plane actor.
   * \param[in] iVisibility
   */
  void SetSplinePlaneActorsVisibility(bool iVisibility);
  vtkstd::vector<vtkActor*> SlicePlaneActors;
  /**
   * \brief Update the Window Level Observers according to the color of the number
   *  of channels of the current image.
   */
  void UpdateWindowLevelObservers();

  /**
   * \brief Change Interaction mode of the collection to DefaultMode()
   */
  void EnableDefaultInteractionMode();

  /**
   * \brief Change Interaction mode of the collection to ZoomMode()
   */
  void EnableZoomInteractionMode();

  /**
   * \brief Change Interaction mode of the collection to ZoomMode()
   */
  void EnablePanInteractionMode();

  /**
   * \brief Change Interaction mode of the collection to ContourPickingMode()
   */
  void EnableContourPickingMode();

protected:

  vtkViewImage2DCollection();
  ~vtkViewImage2DCollection();

  vtkViewImage2DCollectionCommand* Command;
  vtkRenderWindow*                 ExtraRenderWindow;

  unsigned int LinkSliceMove;
  unsigned int LinkColorWindowLevel;
  unsigned int LinkResetWindowLevel;
  unsigned int LinkResetViewer;
  unsigned int LinkRequestedPosition;
  unsigned int LinkCamera;
  unsigned int LinkPosition;
  unsigned int ShowAxes;
  };

#endif /* _vtkViewImage2DCollection_h_ */
