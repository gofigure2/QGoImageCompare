/*=========================================================================
  Author: $Author:$  // Author of last commit
  Version: $Rev:$  // Revision of last commit
  Date: $Date:$  // Date of last commit
=========================================================================*/

/*=========================================================================
 Authors: The GoFigure Dev. Team.
 at Megason Lab, Systems biology, Harvard Medical school, 2009-10

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
#ifndef __vtkViewImage2DCollectionCommand_h
#define __vtkViewImage2DCollectionCommand_h

#include "vtkCommand.h"
#include "vtkObject.h"
#include "MegaVTK2Configure.h"

#include <list>

class vtkViewImage2DCollection;
class vtkProp3D;

class VTK_RENDERINGADDON2_EXPORT vtkViewImage2DCollectionCommand :
  public vtkCommand
  {
public:

  static vtkViewImage2DCollectionCommand* New();

  vtkViewImage2DCollection* GetCollection();
  void SetCollection(vtkViewImage2DCollection* p);

  // Description:
  // Satisfy the superclass API for callbacks. Recall that the caller is
  // the instance invoking the event; eid is the event id (see
  // vtkCommand.h); and calldata is information sent when the callback
  // was invoked (e.g., progress value in the vtkCommand::ProgressEvent).
  virtual void Execute(vtkObject * caller, unsigned long event,
                       void *vtkNotUsed(callData));

  std::list<vtkProp3D*> GetListOfPickedActors();
  std::list<vtkProp3D*> GetListOfUnPickedActors();

protected:

  vtkViewImage2DCollectionCommand();
  ~vtkViewImage2DCollectionCommand();

private:
  vtkViewImage2DCollection* Collection;

  double InitialWindow;
  double InitialLevel;

  std::list<vtkProp3D*> ListOfPickedActors;
  std::list<vtkProp3D*> ListOfUnPickedActors;
  };

#endif
