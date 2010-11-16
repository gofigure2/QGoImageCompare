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

#ifndef _vtkViewImage2DCommand_h_
#define _vtkViewImage2DCommand_h_

#include <vtkCommand.h>
#include "MegaVTK2Configure.h"

class vtkInteractorStyleImage2D;
class vtkInteractorStyleImage;
class vtkViewImage2D;

/**
 * \class vtkViewImage2DCommand
 * \ingroup MegaVTK
 * \brief Manage events occuring in 2D view
 */
class VTK_RENDERINGADDON2_EXPORT vtkViewImage2DCommand:public vtkCommand
{
public:
  /**
   * \brief Convenient method to access the constructor.
   */
  static vtkViewImage2DCommand * New()
  { return new vtkViewImage2DCommand; }

  //BTX
  enum EventIds {
    SliceMoveEvent = ( vtkCommand::UserEvent + 1 ),
    StartSliceMoveEvent,
    EndSliceMoveEvent,
    ZoomEvent,
    PanEvent,
    RequestedPositionEvent,
    ResetViewerEvent,
    ContourPickingEvent,
    MeshPickingEvent,
    CameraMoveEvent,
    DefaultMoveEvent,
    WindowLevelEvent,
    SyncViewsEvent
    };
  //ETX

  // Description:
  // Satisfy the superclass API for callbacks. Recall that the caller is
  // the instance invoking the event; eid is the event id (see
  // vtkCommand.h); and calldata is information sent when the callback
  // was invoked (e.g., progress value in the vtkCommand::ProgressEvent).
  virtual void Execute( vtkObject * caller, unsigned long event,
                        void *vtkNotUsed(callData) );

  /**
   * \brief Set the 2d image related to this 2d event manager
   * \param[in] viewer vtkViewImage2D Target 2D image
   */
  void SetViewer(vtkViewImage2D *viewer)
  { this->Viewer = viewer; }
protected:
  vtkViewImage2DCommand();
  ~vtkViewImage2DCommand(){}

  /**
   * \brief Method to be called if an event is caught in "Windowing Mode"
   * \param[in] isi Related interactor
   */
  void Windowing(vtkInteractorStyleImage2D *isi);

  /**
   * \brief Method to be called if an event is caught in "Zooming Mode"
   */
  void Zooming();

  /**
   * \brief Method to be called if an event is caught in "Panning Mode"
   */
  void Panning();

  /**
   * \brief Method to be called when an event is caught to update information in 2D view
   * such as position, pixel intensity, etc.
   */
  void PrintInformation();

private:
  vtkViewImage2D *Viewer;
  double          InitialWindow;
  double          InitialLevel;
  int             InitialSlice;
};

#endif
