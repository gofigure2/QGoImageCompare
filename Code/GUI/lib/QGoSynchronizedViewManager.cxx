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
#include "QGoSynchronizedViewManager.h"

#include "QGoSynchronizedView2D.h"
#include "QGoSynchronizedView3D.h"
#include "QGoSynchronizedView2DCallbacks.h"
#include "QGoSynchronizedView3DCallbacks.h"
#include <algorithm>

//--------------------------------------------------------------------------
QGoSynchronizedViewManager::QGoSynchronizedViewManager(QObject *iParent):
  QObject           (iParent),
  m_Synchronizer    (NULL),
  m_Synchronizer3D  (NULL)
{}

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
QGoSynchronizedViewManager::
~QGoSynchronizedViewManager()
{
  std::vector< QGoSynchronizedView2D * >::iterator SynchronizedViewIt;
  std::vector< QGoSynchronizedView3D * >::iterator SynchronizedView3DIt;
  QGoSynchronizedView2D *                          tempComp;
  QGoSynchronizedView3D *                          tempComp3D;

  // first we delete the synchronizer if any
  if ( m_Synchronizer != NULL ) { delete ( m_Synchronizer ); }
  if ( m_Synchronizer3D != NULL ) { delete ( m_Synchronizer3D ); }

  // and the we delete the SynchronizedViews2D
  SynchronizedViewIt = m_openSynchronizedViews.begin();
  while ( !m_openSynchronizedViews.empty() )
    {
    // delete all pointers in the vector
    tempComp = *SynchronizedViewIt;
    m_openSynchronizedViews.erase(SynchronizedViewIt);
    delete ( tempComp );
    }

  // and the we delete the SynchronizedViews3D
  SynchronizedView3DIt = m_openSynchronizedViews3D.begin();
  while ( !m_openSynchronizedViews3D.empty() )
    {
    // delete all pointers in the vector
    tempComp3D = *SynchronizedView3DIt;
    m_openSynchronizedViews3D.erase(SynchronizedView3DIt);
    delete ( tempComp3D );
    }
}

//--------------------------------------------------------------------------
QGoSynchronizedView *
QGoSynchronizedViewManager::newSynchronizedView(
  QString iSynchronizedViewName,
  vtkImageData *iImage)
{
  if ( iImage == NULL )
    {
    return NULL;
    }

  int dim[3];
  iImage->GetDimensions(dim);

  if ( ( dim[0] != 1 ) && ( dim[1] != 1 ) && ( dim[2] != 1 ) )
    {
    return static_cast< QGoSynchronizedView * >(
             newSynchronizedView3D(iSynchronizedViewName, iImage) );
    }
  else
    {
    return static_cast< QGoSynchronizedView * >(
             newSynchronizedView2D(iSynchronizedViewName, iImage) );
    }
}

//--------------------------------------------------------------------------
// add a SynchronizedView to QGoSynchronizedViewManager's parent Object/Widget
QGoSynchronizedView2D *
QGoSynchronizedViewManager::newSynchronizedView2D(QString iSynchronizedViewName, vtkImageData *iImage)
{
  if ( ( iSynchronizedViewName.isEmpty() ) || ( iImage == NULL ) )
    {
    return NULL;
    }
  // we create a new SynchronizedView with the same parent windows as this
  QGoSynchronizedView2D *nSynchronizedView =
    new QGoSynchronizedView2D( iSynchronizedViewName,
                               static_cast< QWidget * >( this->parent() ) );

  // we add the vtkimage to it
  nSynchronizedView->SetImage(iImage);

  // we add the SynchronizedView to the list of open SynchronizedViews
  m_openSynchronizedViews.push_back(nSynchronizedView);

  // if we are synchronizing different SynchronizedViews
  if ( m_Synchronizer != NULL )
    {
    // we tell the synchronizer to also synchronize the new SynchronizedView
    m_Synchronizer->addSynchronizedView(nSynchronizedView);
    }

  // tell the SynchronizedView who is his Manager
  nSynchronizedView->SetViewManager(this);

  return nSynchronizedView;
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// add a SynchronizedView to QGoSynchronizedViewManager's parent Object/Widget
QGoSynchronizedView3D *
QGoSynchronizedViewManager::newSynchronizedView3D(QString iSynchronizedViewName, vtkImageData *iImage)
{
  if ( ( iSynchronizedViewName.isEmpty() ) || ( iImage == NULL ) )
    {
    return NULL;
    }
  // we create a new SynchronizedView with the same parent windows as this
  QGoSynchronizedView3D *nSynchronizedView =
    new QGoSynchronizedView3D( iSynchronizedViewName,
                               static_cast< QWidget * >( this->parent() ) );

  // we add the vtkimage to it
  nSynchronizedView->SetImage(iImage);

  // we add the SynchronizedView to the list of open SynchronizedViews
  m_openSynchronizedViews3D.push_back(nSynchronizedView);

  // if we are synchronizing different SynchronizedViews
  if ( m_Synchronizer3D != NULL )
    {
    // we tell the synchronizer to also synchronize the new SynchronizedView
    m_Synchronizer3D->addSynchronizedView(nSynchronizedView);
    }

  // tell the SynchronizedView who is his Manager
  nSynchronizedView->SetViewManager(this);

  return nSynchronizedView;
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// delete a SynchronizedView from QGoSynchronizedViewManager's parent
// Object/Widget
void
QGoSynchronizedViewManager::deleteSynchronizedView2D(QGoSynchronizedView2D *ioSynchronizedView)
{
  std::vector< QGoSynchronizedView2D * >::iterator SynchronizedViewIt;

  if ( ioSynchronizedView == NULL ) { return; }

  // We look for the SynchronizedView in the vector of synchronized
  // SynchronizedViews
  SynchronizedViewIt = std::find(m_openSynchronizedViews.begin(), m_openSynchronizedViews.end(),
                                 ioSynchronizedView);

  if ( SynchronizedViewIt == m_openSynchronizedViews.end() ) { return; }

  // if we are synchronizing different SynchronizedViews
  if ( m_Synchronizer != NULL )
    {
    // we tell the synchronizer to remove the SynchronizedView
    m_Synchronizer->removeSynchronizedView(ioSynchronizedView);
    }

  ioSynchronizedView->close();

  delete ioSynchronizedView;

  // we remove the SynchronizedView to the list of open SynchronizedViews
  m_openSynchronizedViews.erase(SynchronizedViewIt);
}

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// delete a SynchronizedView from QGoSynchronizedViewManager's parent
// Object/Widget
void
QGoSynchronizedViewManager::deleteSynchronizedView2D(const int & iIndex)
{
  QGoSynchronizedView2D *tempComp;

  // if there is a problem acessing the SynchronizedView
  if ( m_openSynchronizedViews.at(iIndex) == NULL ) { return; }

  tempComp = m_openSynchronizedViews.at(iIndex);
  // if we are synchronizing different SynchronizedViews
  if ( m_Synchronizer != NULL )
    {
    // we tell the synchronizer to remove the SynchronizedView
    m_Synchronizer->removeSynchronizedView(tempComp);
    }

  tempComp->close();

  delete tempComp;

  // we remove the SynchronizedView to the list of open SynchronizedViews
  m_openSynchronizedViews.erase(m_openSynchronizedViews.begin() + iIndex);
}

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// take a SynchronizedView out of the Manager lists, without deleting it.
void
QGoSynchronizedViewManager::removeSynchronizedView2D(QGoSynchronizedView2D *ioSynchronizedView)
{
  if ( ioSynchronizedView == NULL ) { return; }

  std::vector< QGoSynchronizedView2D * >::iterator SynchronizedViewIt;

  // We look for the SynchronizedView in the vector of synchronized
  // SynchronizedViews
  SynchronizedViewIt = std::find(m_openSynchronizedViews.begin(), m_openSynchronizedViews.end(),
                                 ioSynchronizedView);
  // if we don't find it
  if ( SynchronizedViewIt == m_openSynchronizedViews.end() ) { return; }

  // if we are synchronizing SynchronizedViews
  if ( m_Synchronizer != NULL )  // we tell the synchronizer to remove the
                                 // SynchronizedView
    {
    m_Synchronizer->removeSynchronizedView(ioSynchronizedView);
    }

  ioSynchronizedView->close();

  // we remove the SynchronizedView to the list of open SynchronizedViews
  m_openSynchronizedViews.erase(SynchronizedViewIt);
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// delete a SynchronizedView from QGoSynchronizedViewManager's parent
// Object/Widget
void
QGoSynchronizedViewManager::deleteSynchronizedView3D(QGoSynchronizedView3D *ioSynchronizedView)
{
  std::vector< QGoSynchronizedView3D * >::iterator SynchronizedView3DIt;

  if ( ioSynchronizedView == NULL ) { return; }

  // We look for the SynchronizedView in the vector of synchronized
  // SynchronizedViews
  SynchronizedView3DIt = std::find(m_openSynchronizedViews3D.begin(), m_openSynchronizedViews3D.end(),
                                   ioSynchronizedView);
  // if we don't find it
  if ( SynchronizedView3DIt == m_openSynchronizedViews3D.end() ) { return; }

  // if we are synchronizing SynchronizedViews
  if ( m_Synchronizer3D != NULL )
    {
    // we tell the synchronizer to remove the SynchronizedView
    m_Synchronizer3D->removeSynchronizedView(ioSynchronizedView);
    }

  ioSynchronizedView->close();

  delete ioSynchronizedView;

  // we remove the SynchronizedView fromo the list of open SynchronizedViews
  m_openSynchronizedViews3D.erase(SynchronizedView3DIt);
}

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// delete a SynchronizedView from QGoSynchronizedViewManager's parent
// Object/Widget
void
QGoSynchronizedViewManager::deleteSynchronizedView3D(const int & iIndex)
{
  QGoSynchronizedView3D *tempComp3D;

  // if there is a problem acessing the SynchronizedView
  if ( m_openSynchronizedViews3D.at(iIndex) == NULL ) { return; }

  tempComp3D = m_openSynchronizedViews3D.at(iIndex);

  // if we are synchronizing different SynchronizedViews
  if ( m_Synchronizer3D != NULL )
    {
    // we tell the synchronizer to remove the SynchronizedView
    m_Synchronizer3D->removeSynchronizedView(tempComp3D);
    }

  tempComp3D->close();

  delete tempComp3D;

  // we remove the SynchronizedView to the list of open SynchronizedViews
  m_openSynchronizedViews3D.erase(m_openSynchronizedViews3D.begin() + iIndex);
}

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// take a SynchronizedView out of the Manager lists, without deleting it.
void
QGoSynchronizedViewManager::removeSynchronizedView3D(QGoSynchronizedView3D *ioSynchronizedView)
{
  if ( ioSynchronizedView == NULL ) { return; }

  std::vector< QGoSynchronizedView3D * >::iterator SynchronizedView3DIt;

  // We look for the SynchronizedView in the vector of synchronized
  // SynchronizedViews
  SynchronizedView3DIt = std::find(m_openSynchronizedViews3D.begin(), m_openSynchronizedViews3D.end(),
                                   ioSynchronizedView);
  // if we don't find it
  if ( SynchronizedView3DIt == m_openSynchronizedViews3D.end() ) { return; }

  // if we are synchronizing different SynchronizedViews
  if ( m_Synchronizer3D != NULL )
    {
    // we tell the synchronizer to remove the SynchronizedView
    m_Synchronizer3D->removeSynchronizedView(ioSynchronizedView);
    }

  ioSynchronizedView->close();

  // we remove the SynchronizedView to the list of open SynchronizedViews
  m_openSynchronizedViews3D.erase(SynchronizedView3DIt);
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// remove the synchronizer leaving the SynchronizedViews independent
void
QGoSynchronizedViewManager::unSynchronizeOpenSynchronizedViews()
{
  if ( m_Synchronizer != NULL ) { delete ( m_Synchronizer ); }

  if ( m_Synchronizer3D != NULL ) { delete ( m_Synchronizer3D ); }

  m_Synchronizer = NULL;
  m_Synchronizer3D = NULL;
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// synchronize all open SynchronizedViews
void
QGoSynchronizedViewManager::synchronizeOpenSynchronizedViews()
{
  if ( m_Synchronizer == NULL )
    {
    m_Synchronizer =
      new QGoSynchronizedView2DCallbacks(m_openSynchronizedViews);
    }

  if ( m_Synchronizer3D == NULL ) { m_Synchronizer3D = new QGoSynchronizedView3DCallbacks(m_openSynchronizedViews3D); }
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// true if the orchestra synchronizes copmarers
bool
QGoSynchronizedViewManager::isSynchronizing()
{
  if ( ( m_Synchronizer != NULL ) ||  ( m_Synchronizer3D != NULL ) ) { return true; }
  else
    {
    return false;
    }
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// synchronize all open SynchronizedViews
void
QGoSynchronizedViewManager::Update()
{
  std::vector< QGoSynchronizedView2D * >::iterator SynchronizedViewIt;

  // we update all SynchronizedViews
  for ( SynchronizedViewIt = m_openSynchronizedViews.begin();
        SynchronizedViewIt != m_openSynchronizedViews.end();
        SynchronizedViewIt++ )
    {
    ( *SynchronizedViewIt )->Update();
    }

  std::vector< QGoSynchronizedView3D * >::iterator SynchronizedView3DIt;

  // we update all SynchronizedViews
  for ( SynchronizedView3DIt = m_openSynchronizedViews3D.begin();
        SynchronizedView3DIt != m_openSynchronizedViews3D.end();
        SynchronizedView3DIt++ )
    {
    ( *SynchronizedView3DIt )->Update();
    }
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
//show all the SynchronizedViews
void
QGoSynchronizedViewManager::show()
{
  std::vector< QGoSynchronizedView2D * >::iterator SynchronizedViewIt;

  // we update all SynchronizedViews
  for ( SynchronizedViewIt = m_openSynchronizedViews.begin();
        SynchronizedViewIt != m_openSynchronizedViews.end();
        SynchronizedViewIt++ )
    {
    ( *SynchronizedViewIt )->show();
    }

  std::vector< QGoSynchronizedView3D * >::iterator SynchronizedView3DIt;

  // we update all SynchronizedViews
  for ( SynchronizedView3DIt = m_openSynchronizedViews3D.begin();
        SynchronizedView3DIt != m_openSynchronizedViews3D.end();
        SynchronizedView3DIt++ )
    {
    ( *SynchronizedView3DIt )->show();
    }
}

//--------------------------------------------------------------------------
