/*=========================================================================
  Author: $Author$  // Author of last commit
  Version: $Rev$  // Revision of last commit
  Date: $Date$  // Date of last commit
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
#include "QGoSynchronizedView2DCallbacks.h"

#include "QGoSynchronizedView.h"
#include "vtkCommand.h"
#include "vtkCallbackCommand.h"
#include "vtkCamera.h"
#include "vtkRenderer.h"
#include "vtkSmartPointer.h"
#include <algorithm>

//--------------------------------------------------------------------------
QGoSynchronizedView2DCallbacks::
QGoSynchronizedView2DCallbacks (
  std::vector<QGoSynchronizedView2D*> ioOpenSynchronizedViews) :
  m_openSynchronizedView  (ioOpenSynchronizedViews)
  {
  // create the callback object
  SetupCallBack();

  // for every opened SynchronizedView :
  for (std::vector<QGoSynchronizedView2D*>::iterator
       SynchronizedViewIt = m_openSynchronizedView.begin();
       SynchronizedViewIt != m_openSynchronizedView.end();
       ++SynchronizedViewIt)
    {
    // add the callback object as an observer of each SynchronizedView's camera
    (*SynchronizedViewIt)->GetCamera()->AddObserver(
      vtkCommand::ModifiedEvent,
      QGoSynchronizedView2DCallbacks::m_vtkCallBackCamSync);
    }
  }

//--------------------------------------------------------------------------
// the destructor is very important here, we want to leave clean
//  SynchronizedViews behind
QGoSynchronizedView2DCallbacks::
~QGoSynchronizedView2DCallbacks()
  {
  std::vector<QGoSynchronizedView2D*>::iterator SynchronizedViewIt;
  // we remove the open synchronized SynchronizedViews
  SynchronizedViewIt = m_openSynchronizedView.begin();

  // we pop out the element of the vector until it is empty
  while (!m_openSynchronizedView.empty())
    {
    // remove (AND NOT DELETE, this is the Manager's business)
    // all pointers in the vector

    // We remove the observer if any
    if (m_openSynchronizedView.back()->HasViewer())
      {
      // remove the callback object from each object's camera
      m_openSynchronizedView.back()->GetCamera()->RemoveObserver(
        QGoSynchronizedView2DCallbacks::
        m_vtkCallBackCamSync);
      }
    // we remove the SynchronizedView from the vector
    m_openSynchronizedView.pop_back();
    }

  // we can now delete the callback !
  m_vtkCallBackCamSync->Delete();
  }

//--------------------------------------------------------------------------
// this is the callback function : do deep copies to keep track of
//  master's camera position
void
QGoSynchronizedView2DCallbacks::
synchronizeCameras(vtkObject* caller,
                   long unsigned int eventId,
                   void* clientData,
                   void* callData)
{
  (void) eventId;
  (void) callData;

  // client data is a pointer to std::vector<QGoImageView2D*>
  // so client data is a std::vector<QGoSynchronizedView2D*>*
  // we get the p_m_QGoSynchronizedView2D array by the following cast :
  std::vector<QGoSynchronizedView2D*> p_m_QGoSynchronizedViews
    = *static_cast<std::vector<QGoSynchronizedView2D*>*>
      (clientData);
  // the observer are set on cameras, so that the caller is a vtk camera*
  vtkCamera* movedCamera
    = static_cast<vtkCamera*>(caller);
  // for every opened SynchronizedView :
  for (std::vector<QGoSynchronizedView2D*>::iterator SynchronizedViewIt =
         p_m_QGoSynchronizedViews.begin();
       SynchronizedViewIt != p_m_QGoSynchronizedViews.end();
       ++SynchronizedViewIt)
    {
    // we copy the position of the moved camera into
    // each SynchronizedView's camera
    if  (((*SynchronizedViewIt)->GetCamera() != NULL)
         &&  ((*SynchronizedViewIt)->GetCamera() != movedCamera))
      {
      (*SynchronizedViewIt)->GetCamera()->DeepCopy(movedCamera);
      // we render all SynchronizedViews
      (*SynchronizedViewIt)->Render();
      }
    }
}

//--------------------------------------------------------------------------
void
QGoSynchronizedView2DCallbacks::
SetupCallBack()
{
  // create the callback object (connection event -> function )
  m_vtkCallBackCamSync = vtkCallbackCommand::New();
  m_vtkCallBackCamSync->SetCallback(QGoSynchronizedView2DCallbacks::
                                    synchronizeCameras);
  m_vtkCallBackCamSync->SetClientData(&m_openSynchronizedView);
}

//--------------------------------------------------------------------------
void
QGoSynchronizedView2DCallbacks::
removeSynchronizedView(QGoSynchronizedView2D* ioSynchronizedView)
{
  std::vector<QGoSynchronizedView2D*>::iterator SynchronizedViewIt;

  if (ioSynchronizedView != NULL) // this should always be true
    {
    // We look for the SynchronizedView in the vector
    // of synchronized SynchronizedViews
    SynchronizedViewIt = std::find(m_openSynchronizedView.begin(),
                                   m_openSynchronizedView.end(),
                                   ioSynchronizedView);
    if (SynchronizedViewIt != m_openSynchronizedView.end()) // if we found it
      {
      if (ioSynchronizedView->HasViewer())
        // remove the callback object from each object's camera
        ioSynchronizedView->GetCamera()
        ->RemoveObserver(QGoSynchronizedView2DCallbacks::
                         m_vtkCallBackCamSync);
      (*SynchronizedViewIt) = NULL;
      // we remove the SynchronizedView
      m_openSynchronizedView.erase(SynchronizedViewIt);
      }
    }
}

//--------------------------------------------------------------------------
void
QGoSynchronizedView2DCallbacks::
addSynchronizedView(QGoSynchronizedView2D* ioSynchronizedView)
{

  if (ioSynchronizedView != NULL) // this should always be true
    {
    m_openSynchronizedView.push_back(ioSynchronizedView);
    // if this SynchronizedView has a viewer, we add an observer
    if (ioSynchronizedView->HasViewer())
      // add the callback to the SynchronizedView's camera
      ioSynchronizedView->GetCamera()->AddObserver(
        vtkCommand::ModifiedEvent,
        QGoSynchronizedView2DCallbacks::m_vtkCallBackCamSync);
    else
      {
      std::cerr << "trying to synchronize a visualization object missing a QGoImageView"
                << std::endl;
      }
    }
}
