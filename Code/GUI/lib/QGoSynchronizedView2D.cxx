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
#include "QGoSynchronizedView2D.h"

#include "QGoSynchronizedView2DCallbacks.h"
#include "vtkImageData.h"
#include "vtkViewImage2D.h"
#include "vtkEventQtSlotConnect.h"

#include "SnapshotHelper.h"
#include "QGoImageView2D.h"
#include "QGoSynchronizedViewManager.h"

#include "itkImageToVTKImageFilter.h"
#include "itkSmartPointer.h"
#include "itkImage.h"
//--------------------------------------------------------------------------
/*
*  Default Constructor.
* \param iSynchronizedViewName
* \param iParent
*/
QGoSynchronizedView2D::QGoSynchronizedView2D(QString iViewName, QWidget *iParent):
  QGoSynchronizedView(iViewName, iParent),
  m_View (NULL)
{}

//--------------------------------------------------------------------------
QGoSynchronizedView2D::
~QGoSynchronizedView2D()
{
  // remove the comparer from the manager
  if ( m_ViewManager != NULL )
    {
    m_ViewManager->removeSynchronizedView2D(this);
    m_ViewManager = NULL;
    }

  // delete the view if any
  if ( HasViewer() )
    {
    deleteViewer();
    }
}

//--------------------------------------------------------------------------
/*  Print self informations */
void
QGoSynchronizedView2D::PrintOs(ostream & os)
{
  // if we have an imageview, the we print its image information
  if ( m_Image )
    {
    os << "SynchronizedView 2D " << this << " contains :" << std::endl;
    m_Image->Print(os);
    }
  else
    {
    os << "SynchronizedView 2D " << this << " contains no Image :" << std::endl;
    }
}

//--------------------------------------------------------------------------
/* Update the viewer contained in the widget */
void
QGoSynchronizedView2D::Update()
{
  if ( m_View )
    {
    this->m_View->Update();
    // in addition to standard parameters, we don't want to interpolate data
    this->m_View->SetInterpolate(0);
    }
}

//--------------------------------------------------------------------------
/*  render the viewer contained in the widget if any */
void
QGoSynchronizedView2D::Render()
{
  if ( m_View )
    {
    m_View->GetImageViewer(0)->Render();
    }
}

//--------------------------------------------------------------------------
/*  get the camera of the viewer */
vtkCamera *
QGoSynchronizedView2D::GetCamera()
{
  if ( m_View )
    {
    return m_View->GetImageViewer(0)
           ->GetRenderer()
           ->GetActiveCamera();
    }
  else
    {
    return NULL;
    }
}
//--------------------------------------------------------------------------
void QGoSynchronizedView2D::SetLookupTable(vtkLookupTable *iLut)
{
  if( HasViewer() )
    {
    m_View->SetLookupTable( iLut );
    }
}

//--------------------------------------------------------------------------
/*  true if the widget has a viewer */
bool
QGoSynchronizedView2D::HasViewer()
{
  return ( m_View != NULL );
}

//--------------------------------------------------------------------------
/*  returns the type of comparer (2 for 2D, 3 for 3D) */
int
QGoSynchronizedView2D::GetSynchronizedViewType()
{
  return 2;
}

//--------------------------------------------------------------------------
/* set the image to be displaid */
void
QGoSynchronizedView2D::SetImage(vtkImageData *iImage)
{
  if ( iImage )
    {
    // if there is no viewer, we create one
    if ( !m_View )
      {
      createViewer();
      }
    // set the image to the view
    dynamic_cast< QGoImageView2D * >( m_View )->SetImage(iImage);
    // update image
    m_Image = iImage;

    this->Update();
    }
}

// ########################################################################
// Private

//--------------------------------------------------------------------------
/* delete the viewer contained in the widget */
void
QGoSynchronizedView2D::deleteViewer()
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
//  create the viewer contained in the widget
void
QGoSynchronizedView2D::createViewer()
{
  // if there is already a viewer
  if ( !m_View )
    {
    // else we create one
    QGoImageView2D *v = new QGoImageView2D(this);
    v->setContentsMargins(1, 1, 1, 1);

    // setup position of the widget
    this->gridLayout->addWidget(v);

    m_View = v;
    }
}

//--------------------------------------------------------------------------
QGoImageView2D *
QGoSynchronizedView2D::GetImageView()
{
  if ( HasViewer() )
    {
    return dynamic_cast< QGoImageView2D * >( m_View );
    }
  else
    {
    return NULL;
    }
}

//--------------------------------------------------------------------------
QString
QGoSynchronizedView2D::SnapshotViewXY(const GoFigure::FileType & iType,
                                      const QString & iBaseName)
{
  QGoImageView2D *viewer = GetImageView();

  if ( viewer )
    {
    return viewer->SnapshotViewXY(iType, iBaseName);
    }
  else
    {
    return QString();
    }
}
