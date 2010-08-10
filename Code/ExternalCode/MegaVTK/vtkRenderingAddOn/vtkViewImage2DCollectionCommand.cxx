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
#include "vtkViewImage2DCollectionCommand.h"
#include "vtkInteractorStyleImage2D.h"
#include "vtkViewImage2DCommand.h"
#include "vtkRenderer.h"
#include "vtkCamera.h"
#include "vtkMath.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkCellPicker.h"
#include "vtkProp3DCollection.h"
#include "vtkViewImage2DCollection.h"

//--------------------------------------------------------------------------
vtkViewImage2DCollectionCommand::
vtkViewImage2DCollectionCommand()
  {
  }

//--------------------------------------------------------------------------
vtkViewImage2DCollectionCommand::
~vtkViewImage2DCollectionCommand()
  {
  }
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void
vtkViewImage2DCollectionCommand::
SetCollection(vtkViewImage2DCollection* p)
{
  this->Collection = p;
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
vtkViewImage2DCollectionCommand*
vtkViewImage2DCollectionCommand::New()
{
  return new vtkViewImage2DCollectionCommand;
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
vtkViewImage2DCollection*
vtkViewImage2DCollectionCommand::
GetCollection()
{
  return this->Collection;
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
std::list<vtkProp3D*>
vtkViewImage2DCollectionCommand::
GetListOfPickedActors()
{
  return ListOfPickedActors;
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
std::list<vtkProp3D*>
vtkViewImage2DCollectionCommand::
GetListOfUnPickedActors()
{
  return ListOfUnPickedActors;
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void vtkViewImage2DCollectionCommand::Execute(vtkObject *caller,
                                              unsigned long event,
                                              void *vtkNotUsed(callData))
{
  if (!this->Collection)
    {
    return;
    }

  vtkInteractorStyleImage2D *isi =
    vtkInteractorStyleImage2D::SafeDownCast(caller);
  this->GetCollection()->InitTraversal();
  vtkViewImage2D* v = this->GetCollection()->GetNextItem();
  vtkViewImage2D* viewer = NULL;
  while (v)
    {
    if (isi == v->GetInteractorStyle())
      {
      viewer = v;
      break;
      }
    v = this->GetCollection()->GetNextItem();
    }

  if (!isi || !viewer || !viewer->GetInput())
    {
    return;
    }

  // Reset
  if (event == vtkCommand::ResetWindowLevelEvent)
    {
    this->Collection->SyncResetWindowLevel();
    this->Collection->SyncRender();
    return;
    }
  // Reset
  if (event == vtkViewImage2DCommand::ResetViewerEvent)
    {
    this->Collection->SyncReset();
    this->Collection->SyncRender();
    return;
    }

  // Adjust the window level here
  if (event == vtkCommand::WindowLevelEvent)
    {
    this->Collection->SyncSetColorWindow(viewer->GetColorWindow());
    this->Collection->SyncSetColorLevel(viewer->GetColorLevel());
    this->Collection->SyncRender();
    }

  // Move
  if (event == vtkViewImage2DCommand::SyncViewsEvent)
    {
    this->Collection->SyncRender();
    }

  if (event == vtkViewImage2DCommand::ZoomEvent)
    {
    double z = viewer->GetZoom();
    double parallel_scale =
      viewer->GetRenderer()->GetActiveCamera()->GetParallelScale();

    this->Collection->SyncSetZoomAndParallelScale(z, parallel_scale);
    }

  if (event == vtkViewImage2DCommand::PanEvent)
    {
    double motion[3];
    viewer->GetCameraMotionVector(motion);

    this->GetCollection()->InitTraversal();
    v = this->GetCollection()->GetNextItem();
    double focal[3], pos[3], n[3];
    double dot = 0., u;

    while (v)
      {
      if (v != viewer)
        {
        v->GetCameraFocalAndPosition(focal, pos);
        v->GetRenderer()->GetActiveCamera()->GetViewPlaneNormal(n);
        dot = vtkMath::Dot(n, motion);

        for (int dim = 0; dim < 3; dim++)
          {
          u = motion[dim] - dot * n[dim];
          focal[dim] += u;
          pos[dim] += u;
          }
        v->SetCameraFocalAndPosition(focal, pos);

        if (v->GetInteractorStyle()->GetInteractor()->GetLightFollowCamera())
          {
          v->GetRenderer()->UpdateLightsGeometryToFollowCamera();
          }
        }
      v->Render();
      v = this->GetCollection()->GetNextItem();
      }
    }

  // Position requested
  if (event == vtkViewImage2DCommand::RequestedPositionEvent)
    {
    double* position = viewer->GetWorldCoordinatesFromDisplayPosition (
      isi->GetRequestedPosition ());
    this->Collection->SyncSetWorldCoordinates(position);
    this->Collection->SyncRender();
    }

  if (event == vtkViewImage2DCommand::ContourPickingEvent)
    {
    vtkProp* prop = isi->GetCurrentProp();
    ListOfPickedActors.clear();
    ListOfUnPickedActors.clear();
    if (prop)
      {
      viewer->GetProp3DCollection()->InitTraversal();
      vtkProp3D* prop_temp = viewer->GetProp3DCollection()->GetNextProp3D();  // image
      prop_temp = viewer->GetProp3DCollection()->GetNextProp3D();  // 1st plane
      prop_temp = viewer->GetProp3DCollection()->GetNextProp3D();  // 2nd plane
      prop_temp = viewer->GetProp3DCollection()->GetNextProp3D();  // 3rd plane

      while (prop_temp)
        {
        if (prop_temp == prop)
          {
          ListOfPickedActors.push_back(prop_temp);
          }
        else
          {
          ListOfUnPickedActors.push_back(prop_temp);
          }
        prop_temp = viewer->GetProp3DCollection()->GetNextProp3D();
        }
      }
    }

  if (event == vtkViewImage2DCommand::SeedEvent)
    {
    }
}
