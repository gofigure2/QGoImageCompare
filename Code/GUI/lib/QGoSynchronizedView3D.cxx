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
#include "QGoSynchronizedView3D.h"

#include "QGoSynchronizedView3DCallbacks.h"
#include "vtkImageData.h"
#include "vtkViewImage3D.h"
#include "vtkViewImage2D.h"
#include "vtkEventQtSlotConnect.h"
#include "QGoImageView3D.h"
#include "QGoSynchronizedViewManager.h"
#include "SnapshotHelper.h"

//--------------------------------------------------------------------------
QGoSynchronizedView3D::QGoSynchronizedView3D(QString iViewName,
                                             QWidget *iParent):
  QGoSynchronizedView(iViewName, iParent),
  m_View (NULL)
{}

//--------------------------------------------------------------------------
QGoSynchronizedView3D::
~QGoSynchronizedView3D()
{
  // remove the SynchronizedView from the orchestra
  if ( m_ViewManager )
    {
    m_ViewManager->removeSynchronizedView3D(this);
    m_ViewManager = NULL;
    }

  // delete the view if any
  if ( m_View )
    {
    // we delete the viewer
    delete ( m_View );
    m_View = NULL;
    }
}

//--------------------------------------------------------------------------
/* Update the viewer contained in the widget */
void
QGoSynchronizedView3D::Update()
{
  if ( m_View )
    {
    this->m_View->Update();
    // in addition to standard parameters, we don't want to interpolate data
    this->m_View->SetInterpolate(0);
    }
}

//--------------------------------------------------------------------------
void
QGoSynchronizedView3D::PrintOs(ostream & os)
{
  // if we have an imageview, the we print its image information
  if ( m_Image != NULL )
    {
    os << "SynchronizedView 3D " << this << " contains :" << std::endl;
    m_Image->Print(os);
    }
  else
    {
    os << "SynchronizedView 3D " << this << " contains no Image :" << std::endl;
    }
}

//--------------------------------------------------------------------------
/*  returns the type of SynchronizedView (2 for 2D, 3 for 3D) */
int
QGoSynchronizedView3D::GetSynchronizedViewType()
{
  return 3;
}

//--------------------------------------------------------------------------
void
QGoSynchronizedView3D::SetImage(vtkImageData *iImage)
{
  if ( iImage )
    {
    // if there is no viewer, we create one
    if ( !m_View )
      {
      createViewer();
      }
    // set the image to the current view
    m_View->SetImage(iImage);
    // update current image
    m_Image = iImage;
    this->Update();
    }
}

//--------------------------------------------------------------------------
void
QGoSynchronizedView3D::Render()
{
  if ( HasViewer() )
    {
    m_View->GetImageViewer(0)->Render();
    m_View->GetImageViewer(1)->Render();
    m_View->GetImageViewer(2)->Render();
    m_View->GetImageViewer3D()->Render();
    }
}

//--------------------------------------------------------------------------
void
QGoSynchronizedView3D::Render(const int & iId)
{
  if ( HasViewer() )
    {
    if ( ( iId >= 0 ) && ( iId <= 2 ) ) // if we want to render one of the 2D
                                        // view
      {
      m_View->GetImageViewer(iId)->Render();
      }
    else // if we want to render the 3D view
      {
      m_View->GetImageViewer3D()->Render();
      }
    }
}

//--------------------------------------------------------------------------
/* get the camera of the current viewer */
vtkCamera *
QGoSynchronizedView3D::GetCamera()
{
  int currentView = GetFullScreenView();

  if ( HasViewer() )
    {
    if ( ( currentView >= 1 ) && ( currentView <= 3 ) )
      {
      return m_View->GetImageViewer(currentView)
             ->GetRenderer()
             ->GetActiveCamera();
      }
    else
      {
      return m_View->GetImageViewer3D()
             ->GetRenderer()
             ->GetActiveCamera();
      }
    }
  return NULL;
}

//--------------------------------------------------------------------------
/* get the camera of the current viewer */
vtkCamera *
QGoSynchronizedView3D::GetCamera(const int & iId)
{
  if ( HasViewer() )
    {
    if ( ( iId >= 0 ) && ( iId <= 2 ) )
      {
      return m_View->GetImageViewer(iId)
             ->GetRenderer()
             ->GetActiveCamera();
      }
    else
      {
      return m_View->GetImageViewer3D()
             ->GetRenderer()
             ->GetActiveCamera();
      }
    }
  return NULL;
}

//--------------------------------------------------------------------------
/*  true if the widget has a viewer */
bool
QGoSynchronizedView3D::HasViewer()
{
  return ( m_View != NULL );
}

//--------------------------------------------------------------------------
int
QGoSynchronizedView3D::GetFullScreenView()
{
  if ( HasViewer() )
    {
    return m_View->GetFullScreenView();
    }
  else
    {
    return -1;
    }
}

//--------------------------------------------------------------------------
QGoImageView3D *
QGoSynchronizedView3D::GetImageView()
{
  if ( HasViewer() )
    {
    return m_View;
    }
  else
    {
    return NULL;
    }
}

//--------------------------------------------------------------------------
QString
QGoSynchronizedView3D::SnapshotViewXY(const GoFigure::FileType & iType, const QString & iBaseName)
{
  QGoImageView3D *viewer = this->GetImageView();

  if ( HasViewer() )
    {
    return viewer->SnapshotViewXY(iType, iBaseName);
    }
  else
    {
    return QString();
    }
}

//--------------------------------------------------------------------------
QString
QGoSynchronizedView3D::SnapshotViewXZ(const GoFigure::FileType & iType, const QString & iBaseName)
{
  QGoImageView3D *viewer = this->GetImageView();

  if ( viewer )
    {
    return viewer->SnapshotViewXZ(iType, iBaseName);
    }
  else
    {
    return QString();
    }
}

//--------------------------------------------------------------------------
QString
QGoSynchronizedView3D::SnapshotViewYZ(const GoFigure::FileType & iType, const QString & iBaseName)
{
  QGoImageView3D *viewer = this->GetImageView();

  if ( viewer )
    {
    return viewer->SnapshotViewYZ(iType, iBaseName);
    }
  else
    {
    return QString();
    }
}

//--------------------------------------------------------------------------
QString
QGoSynchronizedView3D::SnapshotViewXYZ(const GoFigure::FileType & iType, const QString & iBaseName)
{
  QGoImageView3D *viewer = this->GetImageView();

  if ( viewer )
    {
    return viewer->SnapshotViewXYZ(iType, iBaseName);
    }
  else
    {
    return QString();
    }
}

//--------------------------------------------------------------------------
void
QGoSynchronizedView3D::SetFullScreenView(const int & iId)
{
  if ( HasViewer() )
    {
    m_View->SetFullScreenView(iId);
    }
}

//--------------------------------------------------------------------------
void
QGoSynchronizedView3D::SetFullXYScreenView()
{
  SetFullScreenView(1);
}

//--------------------------------------------------------------------------
void
QGoSynchronizedView3D::SetFullXZScreenView()
{
  SetFullScreenView(2);
}

//--------------------------------------------------------------------------
void
QGoSynchronizedView3D::SetFullYZScreenView()
{
  SetFullScreenView(3);
}

//--------------------------------------------------------------------------
void
QGoSynchronizedView3D::SetFullXYZScreenView()
{
  SetFullScreenView(4);
}

//--------------------------------------------------------------------------
void
QGoSynchronizedView3D::SetQuadView()
{
  SetFullScreenView(0);
}

// ########################################################################
// Private

//--------------------------------------------------------------------------
/* delete the viewer contained in the widget */
void
QGoSynchronizedView3D::deleteViewer()
{
  // if there is no viewer
  if ( m_View )
    {
    // delete object
    delete ( m_View );
    // set pointer to NULL
    m_View = NULL;
    }
}

//--------------------------------------------------------------------------
/* Create the viewer in the widget */
void
QGoSynchronizedView3D::createViewer()
{
  // if there is no viewer
  if ( !HasViewer() )
    {
    // we create one
    m_View = new QGoImageView3D(this);
    dynamic_cast< QGoImageView3D * >
    ( m_View )->setContentsMargins(1, 1, 1, 1);
    // setup position of the widget
    gridLayout->addWidget(m_View);
    }
}
