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
#include "QGoSynchronizedView3DCallbacks.h"

#include "QGoImageView3D.h"
#include "QGoSynchronizedView3D.h"
#include "vtkCommand.h"
#include "vtkCallbackCommand.h"
#include "vtkCamera.h"
#include "vtkRenderer.h"
#include "vtkSmartPointer.h"
#include <algorithm>

//--------------------------------------------------------------------------
QGoSynchronizedView3DCallbacks::
QGoSynchronizedView3DCallbacks(std::vector<QGoSynchronizedView3D*>
                               ioOpenSynchronizedViews,
                               QObject* iParent) :
  QObject                  (iParent),
  m_openSynchronizedView   (ioOpenSynchronizedViews)
  {
  // create the callback object
  SetupCallBack();

  // for every opened SynchronizedView :
  std::vector<QGoSynchronizedView3D*>::iterator SynchronizedViewIt =
         m_openSynchronizedView.begin();
  while ( SynchronizedViewIt != m_openSynchronizedView.end() )
    {
    if ((*SynchronizedViewIt)->HasViewer())
    // add the callback to the SynchronizedView's camera
      {
      (*SynchronizedViewIt)->GetCamera(0)
      ->AddObserver(vtkCommand::ModifiedEvent,
                    QGoSynchronizedView3DCallbacks::
                    m_vtkCallBackCamSync0);
      (*SynchronizedViewIt)->GetCamera(1)
      ->AddObserver(vtkCommand::ModifiedEvent,
                    QGoSynchronizedView3DCallbacks::
                    m_vtkCallBackCamSync1);
      (*SynchronizedViewIt)->GetCamera(2)
      ->AddObserver(vtkCommand::ModifiedEvent,
                    QGoSynchronizedView3DCallbacks::
                    m_vtkCallBackCamSync2);
      (*SynchronizedViewIt)->GetCamera(3)
      ->AddObserver(vtkCommand::ModifiedEvent,
                    QGoSynchronizedView3DCallbacks::
                    m_vtkCallBackCamSync3);

      // we connect the sliders to the synchronizer (monitor slider changes) :
      QObject::connect((*SynchronizedViewIt)->GetImageView(),
                       SIGNAL(SliceViewXYChanged(int)),
                       this,
                       SIGNAL(SliceViewXYChanged(int)));
      QObject::connect((*SynchronizedViewIt)->GetImageView(),
                       SIGNAL(SliceViewXZChanged(int)),
                       this,
                       SIGNAL(SliceViewXZChanged(int)));
      QObject::connect((*SynchronizedViewIt)->GetImageView(),
                       SIGNAL(SliceViewYZChanged(int)),
                       this,
                       SIGNAL(SliceViewYZChanged(int)));

      // we connect the synchronizer to the SynchronizedView
      QObject::connect(this,
                       SIGNAL(SliceViewXYChanged(int)),
                       (*SynchronizedViewIt)->GetImageView(),
                       SLOT(SetSliceViewXY(int)));
      QObject::connect(this,
                       SIGNAL(SliceViewXZChanged(int)),
                       (*SynchronizedViewIt)->GetImageView(),
                       SLOT(SetSliceViewXZ(int)));
      QObject::connect(this,
                       SIGNAL(SliceViewYZChanged(int)),
                       (*SynchronizedViewIt)->GetImageView(),
                       SLOT(SetSliceViewYZ(int)));
      }
      ++SynchronizedViewIt;
    }
  }

//--------------------------------------------------------------------------
// the destructor is very important here, we want to leave clean
// SynchronizedViews behind
QGoSynchronizedView3DCallbacks::
~QGoSynchronizedView3DCallbacks()
  {
  std::vector<QGoSynchronizedView3D*>::iterator SynchronizedViewIt;

  // we remove the open synchronized SynchronizedViews
  SynchronizedViewIt = m_openSynchronizedView.begin();
  while (!m_openSynchronizedView.empty())
    {
    // remove (AND NOT DELETE, this is the Orchestra's business)
    // all pointers in the vector

    // We remove the observer if any
    if ((*SynchronizedViewIt)->HasViewer())
      {
      // remove the callback object from each object's camera
      (*SynchronizedViewIt)->GetCamera(0)
      ->RemoveObserver(QGoSynchronizedView3DCallbacks::
                       m_vtkCallBackCamSync0);
      (*SynchronizedViewIt)->GetCamera(1)
      ->RemoveObserver(QGoSynchronizedView3DCallbacks::
                       m_vtkCallBackCamSync1);
      (*SynchronizedViewIt)->GetCamera(2)
      ->RemoveObserver(QGoSynchronizedView3DCallbacks::
                       m_vtkCallBackCamSync2);
      (*SynchronizedViewIt)->GetCamera(3)
      ->RemoveObserver(QGoSynchronizedView3DCallbacks::
                       m_vtkCallBackCamSync3);
      }
    m_openSynchronizedView.erase(SynchronizedViewIt);
    }

  // we can now delete the callbacks !
  m_vtkCallBackCamSync0->Delete();
  m_vtkCallBackCamSync1->Delete();
  m_vtkCallBackCamSync2->Delete();
  m_vtkCallBackCamSync3->Delete();
  }

//--------------------------------------------------------------------------
// this is the callback function : do deep copies to keep track of
// the moving camera's position
void
QGoSynchronizedView3DCallbacks::
synchronizeCameras0(vtkObject* caller,
                    long unsigned int eventId,
                    void* clientData,
                    void* callData)
{
  (void) eventId;
  (void) callData;

  // client data is a pointer to std::vector<QGoImageView2D*>
  // so client data is a std::vector<QGoSynchronizedView3D*>*
  // we get the p_m_QGoSynchronizedView3D array by the following cast :
  std::vector<QGoSynchronizedView3D*> p_m_QGoSynchronizedView3Ds
    = *static_cast<std::vector<QGoSynchronizedView3D*>*>(clientData);
  // the observer are set on cameras, so that the caller is a vtk camera*
  vtkCamera* movedCamera
    = static_cast<vtkCamera*>(caller);
  // for every opened SynchronizedView :
  std::vector<QGoSynchronizedView3D*>::iterator SynchronizedViewIt
         = p_m_QGoSynchronizedView3Ds.begin();
  while ( SynchronizedViewIt != p_m_QGoSynchronizedView3Ds.end() )
    {
    // if the SynchronizedView is visible and the modified camera is rendrered
    if      ((*SynchronizedViewIt)->isVisible()
             &&  (((*SynchronizedViewIt)->GetFullScreenView() == 0)
                  ||  ((*SynchronizedViewIt)->GetFullScreenView() == 1)))
      {
      // we copy the position of the moved camera
      // into each SynchronizedView's camera
      if  (((*SynchronizedViewIt)->GetCamera(0) != NULL)
           &&  ((*SynchronizedViewIt)->GetCamera(0) != movedCamera))
        {
        (*SynchronizedViewIt)->GetCamera(0)->DeepCopy(movedCamera);
        // we render all SynchronizedViews
        (*SynchronizedViewIt)->Render(0);
        }
      }
    ++SynchronizedViewIt;
    }
}

//--------------------------------------------------------------------------
// this is the callback function : do deep copies to keep track of
//  master's camera position
//
void
QGoSynchronizedView3DCallbacks::
synchronizeCameras1(vtkObject* caller,
                    long unsigned int eventId,
                    void* clientData,
                    void* callData)
{
  (void) eventId;
  (void) callData;

  // client data is a pointer to std::vector<QGoImageView2D*>
  // so client data is a std::vector<QGoSynchronizedView3D*>*
  // we get the p_m_QGoSynchronizedView3D array by the following cast :
  std::vector<QGoSynchronizedView3D*> p_m_QGoSynchronizedView3Ds
    = *static_cast<std::vector<QGoSynchronizedView3D*>*>
      (clientData);
  // the observer are set on cameras, so that the caller is a vtk camera*
  vtkCamera* movedCamera
    = static_cast<vtkCamera*> (caller);
  // for every opened SynchronizedView :
  for (std::vector<QGoSynchronizedView3D*>::iterator SynchronizedViewIt =
         p_m_QGoSynchronizedView3Ds.begin();
       SynchronizedViewIt != p_m_QGoSynchronizedView3Ds.end();
       ++SynchronizedViewIt)
    {
    // if the SynchronizedView is visible and the modified camera is rendrered
    if      ((*SynchronizedViewIt)->isVisible()
             &&  (((*SynchronizedViewIt)->GetFullScreenView() == 0)
                  ||  ((*SynchronizedViewIt)->GetFullScreenView() == 2)))
      {
      // we copy the position of the moved camera into each
      // SynchronizedView's camera
      if  (((*SynchronizedViewIt)->GetCamera(1) != NULL)
           &&  ((*SynchronizedViewIt)->GetCamera(1) != movedCamera))
        {
        (*SynchronizedViewIt)->GetCamera(1)->DeepCopy(movedCamera);
        // we render all SynchronizedViews
        (*SynchronizedViewIt)->Render(1);
        }
      }
    }
}

//--------------------------------------------------------------------------
// this is the callback function : do deep copies to keep track of
// master's camera position
void
QGoSynchronizedView3DCallbacks::
synchronizeCameras2(vtkObject* caller,
                    long unsigned int eventId,
                    void* clientData,
                    void* callData)
{
  (void) eventId;
  (void) callData;

  // client data is a pointer to std::vector<QGoImageView2D*>
  // so client data is a std::vector<QGoSynchronizedView3D*>*
  // we get the p_m_QGoSynchronizedView3D array by the following cast :
  std::vector<QGoSynchronizedView3D*> p_m_QGoSynchronizedView3Ds
    = *static_cast<std::vector<QGoSynchronizedView3D*>*>
      (clientData);
  // the observer are aet on cameras, so that the caller is a vtk camera*
  vtkCamera* movedCamera
    = static_cast<vtkCamera*>(caller);
  // for every opened SynchronizedView :
  for (std::vector<QGoSynchronizedView3D*>::iterator SynchronizedViewIt =
         p_m_QGoSynchronizedView3Ds.begin();
       SynchronizedViewIt != p_m_QGoSynchronizedView3Ds.end();
       ++SynchronizedViewIt)
    {
    // if the SynchronizedView is visible and the modified camera is rendrered
    if      ((*SynchronizedViewIt)->isVisible()
             &&  (((*SynchronizedViewIt)->GetFullScreenView() == 0)
                  ||  ((*SynchronizedViewIt)->GetFullScreenView() == 3)))
      {
      // we copy the position of the moved camera into each
      // SynchronizedView's camera
      if  (((*SynchronizedViewIt)->GetCamera(2) != NULL)
           &&  ((*SynchronizedViewIt)->GetCamera(2) != movedCamera))
        {
        (*SynchronizedViewIt)->GetCamera(2)->DeepCopy(movedCamera);
        // we render all SynchronizedViews
        (*SynchronizedViewIt)->Render(2);
        }
      }
    }
}

//--------------------------------------------------------------------------
// this is the callback function : do deep copies to keep track of
// master's camera position
void
QGoSynchronizedView3DCallbacks::
synchronizeCameras3(vtkObject* caller,
                    long unsigned int eventId,
                    void* clientData,
                    void* callData)
{
  (void) eventId;
  (void) callData;

  // client data is a pointer to std::vector<QGoImageView2D*>
  // so client data is a std::vector<QGoSynchronizedView3D*>*
  // we get the p_m_QGoSynchronizedView3D array by the following cast :
  std::vector<QGoSynchronizedView3D*> p_m_QGoSynchronizedView3Ds =
    *static_cast<std::vector<QGoSynchronizedView3D*>*>(clientData);

  // the observer are aet on cameras, so that the caller is a vtk camera*
  vtkCamera* movedCamera =
    static_cast<vtkCamera*>(caller);

  // for every opened SynchronizedView :
  for (std::vector<QGoSynchronizedView3D*>::iterator SynchronizedViewIt =
         p_m_QGoSynchronizedView3Ds.begin();
       SynchronizedViewIt != p_m_QGoSynchronizedView3Ds.end();
       ++SynchronizedViewIt)
    {
    // if the SynchronizedView is visible and the modified camera is rendrered
    if      ((*SynchronizedViewIt)->isVisible()
             &&  (((*SynchronizedViewIt)->GetFullScreenView() == 0)
                  ||  ((*SynchronizedViewIt)->GetFullScreenView() == 4)))
      {
      // we copy the position of the moved camera
      // into each SynchronizedView's camera
      if  (((*SynchronizedViewIt)->GetCamera(3) != NULL)
           &&  ((*SynchronizedViewIt)->GetCamera(3) != movedCamera))
        {
        (*SynchronizedViewIt)->GetCamera(3)->DeepCopy(movedCamera);
        // we render all SynchronizedViews
        (*SynchronizedViewIt)->Render(3);
        }
      }
    }

}

//--------------------------------------------------------------------------
void
QGoSynchronizedView3DCallbacks::
SetupCallBack()
{
  // create the callback object (connection event -> function )
  // for the 1st 2D cut
  m_vtkCallBackCamSync0 = vtkCallbackCommand::New();
  m_vtkCallBackCamSync0->SetCallback(QGoSynchronizedView3DCallbacks::
                                     synchronizeCameras0);
  m_vtkCallBackCamSync0->SetClientData(&m_openSynchronizedView);
  // for the 2nd 2D cut
  m_vtkCallBackCamSync1 = vtkCallbackCommand::New();
  m_vtkCallBackCamSync1->SetCallback(QGoSynchronizedView3DCallbacks::
                                     synchronizeCameras1);
  m_vtkCallBackCamSync1->SetClientData(&m_openSynchronizedView);
  // for the 3rd 2D cut
  m_vtkCallBackCamSync2 = vtkCallbackCommand::New();
  m_vtkCallBackCamSync2->SetCallback(QGoSynchronizedView3DCallbacks::
                                     synchronizeCameras2);
  m_vtkCallBackCamSync2->SetClientData(&m_openSynchronizedView);
  // for the 3D view
  m_vtkCallBackCamSync3 = vtkCallbackCommand::New();
  m_vtkCallBackCamSync3->SetCallback(QGoSynchronizedView3DCallbacks::
                                     synchronizeCameras3);
  m_vtkCallBackCamSync3->SetClientData(&m_openSynchronizedView);
}

//--------------------------------------------------------------------------
void
QGoSynchronizedView3DCallbacks::
removeSynchronizedView(QGoSynchronizedView3D* ioSynchronizedView)
{
  if (ioSynchronizedView != NULL) // this should always be true
    {
    // We look for the SynchronizedView
    // in the vector of synchronized SynchronizedViews
    std::vector<QGoSynchronizedView3D*>::iterator
    SynchronizedViewIt = std::find(m_openSynchronizedView.begin(),
                                   m_openSynchronizedView.end(),
                                   ioSynchronizedView);
    if (SynchronizedViewIt != m_openSynchronizedView.end()) // if we found it
      {
      // remove the callback object from each object's camera
      ioSynchronizedView->GetCamera(0)
      ->RemoveObserver(QGoSynchronizedView3DCallbacks::
                       m_vtkCallBackCamSync0);
      ioSynchronizedView->GetCamera(1)
      ->RemoveObserver(QGoSynchronizedView3DCallbacks::
                       m_vtkCallBackCamSync1);
      ioSynchronizedView->GetCamera(2)
      ->RemoveObserver(QGoSynchronizedView3DCallbacks::
                       m_vtkCallBackCamSync2);
      ioSynchronizedView->GetCamera(3)
      ->RemoveObserver(QGoSynchronizedView3DCallbacks::
                       m_vtkCallBackCamSync3);
      // we remove the SynchronizedView
      m_openSynchronizedView.erase(SynchronizedViewIt);
      }
    }
}

//--------------------------------------------------------------------------
void
QGoSynchronizedView3DCallbacks::
addSynchronizedView(QGoSynchronizedView3D* ioSynchronizedView)
{
  if (ioSynchronizedView != NULL) // this should always be true
    {
    if (ioSynchronizedView->HasViewer())
    // add the callback to the SynchronizedView's camera
      {
      m_openSynchronizedView.push_back(ioSynchronizedView);

      ioSynchronizedView->GetCamera(0)
      ->AddObserver(vtkCommand::ModifiedEvent,
                    QGoSynchronizedView3DCallbacks::
                    m_vtkCallBackCamSync0);
      ioSynchronizedView->GetCamera(1)
      ->AddObserver(vtkCommand::ModifiedEvent,
                    QGoSynchronizedView3DCallbacks::
                    m_vtkCallBackCamSync1);
      ioSynchronizedView->GetCamera(2)
      ->AddObserver(vtkCommand::ModifiedEvent,
                    QGoSynchronizedView3DCallbacks::
                    m_vtkCallBackCamSync2);
      ioSynchronizedView->GetCamera(3)
      ->AddObserver(vtkCommand::ModifiedEvent,
                    QGoSynchronizedView3DCallbacks::
                    m_vtkCallBackCamSync3);

      // we connect the sliders to the synchronizer (monitor slider changes) :
      QObject::connect(ioSynchronizedView->GetImageView(),
                       SIGNAL(SliceViewXYChanged(int)),
                       this,
                       SIGNAL(SliceViewXYChanged(int)));
      QObject::connect(ioSynchronizedView->GetImageView(),
                       SIGNAL(SliceViewXZChanged(int)),
                       this,
                       SIGNAL(SliceViewXZChanged(int)));
      QObject::connect(ioSynchronizedView->GetImageView(),
                       SIGNAL(SliceViewYZChanged(int)),
                       this,
                       SIGNAL(SliceViewYZChanged(int)));

      // we connect the synchronizer to the SynchronizedView
      QObject::connect(this, SIGNAL(SliceViewXYChanged(int)),
                       ioSynchronizedView->GetImageView(), SLOT(SetSliceViewXY(int)));
      QObject::connect(this, SIGNAL(SliceViewXZChanged(int)),
                       ioSynchronizedView->GetImageView(), SLOT(SetSliceViewXZ(int)));
      QObject::connect(this, SIGNAL(SliceViewYZChanged(int)),
                       ioSynchronizedView->GetImageView(), SLOT(SetSliceViewYZ(int)));
      }
    else
      {
      std::cerr << "trying to synchronize a visualization object missing a QGoImageView"
                << std::endl;
      }
    }
}
