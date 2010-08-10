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
 
#ifndef __vtkViewImage3DCommand_h
#define __vtkViewImage3DCommand_h

#include "vtkCommand.h"
#include "vtkObject.h"
#include "MegaVTK2Configure.h"

class vtkViewImage3D;

class VTK_RENDERINGADDON2_EXPORT vtkViewImage3DCommand :
  public vtkCommand
  {
public:

  static vtkViewImage3DCommand* New();

  //BTX
  enum EventIds
    {
    SliceMoveEvent = (vtkCommand::UserEvent + 1),
    StartSliceMoveEvent,
    EndSliceMoveEvent,
    ZoomEvent,
    PanEvent,
    RequestedPositionEvent,
    ResetViewerEvent,
    MeshPickingEvent,
    BoxPickingEvent,
    CameraMoveEvent,
    DefaultMoveEvent,
    BoxWidgetModifiedEvent,
    ReadyEvent,
    BoxWidgetReadyEvent
    };
  //ETX

  // Description:
  // Satisfy the superclass API for callbacks. Recall that the caller is
  // the instance invoking the event; eid is the event id (see
  // vtkCommand.h); and calldata is information sent when the callback
  // was invoked (e.g., progress value in the vtkCommand::ProgressEvent).
  virtual void Execute(vtkObject *caller, unsigned long, void *);

  void SetVtkImageView3D(vtkViewImage3D* vtkViewImage3D);

protected:

  vtkViewImage3DCommand();
  ~vtkViewImage3DCommand();

private:
  vtkViewImage3D*       m_vtkViewImage3D;
  };

#endif
