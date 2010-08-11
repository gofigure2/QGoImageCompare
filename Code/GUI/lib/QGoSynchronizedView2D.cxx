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
QGoSynchronizedView2D::
QGoSynchronizedView2D(QString iViewName, QWidget *iParent) :
  QGoSynchronizedView(iViewName, iParent),
  m_currentView (NULL)
  {
  }


//--------------------------------------------------------------------------
QGoSynchronizedView2D::
~QGoSynchronizedView2D()
  {
  // remove the comparer from the manager
  if (m_currentViewManager != NULL)
    {
    m_currentViewManager->removeSynchronizedView2D(this);
    m_currentViewManager = NULL;
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
QGoSynchronizedView2D::
PrintOs(ostream& os)
{
  // if we have an imageview, the we print its image information
  if (m_currentImage)
    {
    os << "SynchronizedView 2D " << this << " contains :" << std::endl;
    m_currentImage->Print(os);
    }
  else
    {
    os << "SynchronizedView 2D " << this << " contains no Image :" << std::endl;
    }
}

//--------------------------------------------------------------------------
/* Update the viewer contained in the widget */
void
QGoSynchronizedView2D::
Update()
{
  if (m_currentView)
    {
    this->m_currentView->Update();
    }
}

//--------------------------------------------------------------------------
/*  render the viewer contained in the widget if any */
void
QGoSynchronizedView2D::
Render()
{
  if (m_currentView)
    {
    m_currentView->GetImageViewer(0)->Render();
    }
}

//--------------------------------------------------------------------------
/*  get the camera of the current viewer */
vtkCamera*
QGoSynchronizedView2D::
GetCamera()
{
  if ( m_currentView )
    {
    return m_currentView->GetImageViewer(0)
           ->GetRenderer()
           ->GetActiveCamera();
    }
  else
    {
    return NULL;
    }
}

//--------------------------------------------------------------------------
/*  true if the widget has a viewer */
bool
QGoSynchronizedView2D::
HasViewer()
{
  return (m_currentView != NULL);
}

//--------------------------------------------------------------------------
/*  returns the type of comparer (2 for 2D, 3 for 3D) */
int
QGoSynchronizedView2D::
GetSynchronizedViewType()
{
  return 2;
}

//--------------------------------------------------------------------------
/* set the image to be displaid */
void
QGoSynchronizedView2D::
SetImage(vtkImageData* iImage)
{
  if (iImage)
    {
    // if there is no viewer, we create one
    if (!m_currentView)
      {
      createViewer();
      }
    // set the image to the current view
    dynamic_cast<QGoImageView2D*>(m_currentView)->SetImage(iImage);
    // update current image
    m_currentImage = iImage;

    this->Update();
    }
}

// ########################################################################
// Private

//--------------------------------------------------------------------------
/* delete the viewer contained in the widget */
void
QGoSynchronizedView2D::
deleteViewer()
{
  // if there is no viewer
  if (m_currentView)
    {
    // delete object
    delete (m_currentView);
    // set pointer to NULL
    m_currentView = NULL;
    }
}

//--------------------------------------------------------------------------
//  create the viewer contained in the widget
void
QGoSynchronizedView2D::
createViewer()
{
  // if there is already a viewer
  if (!m_currentView)
    {
    // else we create one
    QGoImageView2D* v = new QGoImageView2D(this);
    v->setContentsMargins(1, 1, 1, 1);

    // setup position of the widget
    this->gridLayout->addWidget( v );

    m_currentView = v;
    }
}

//--------------------------------------------------------------------------
QGoImageView2D*
QGoSynchronizedView2D::
GetImageView()
{
  if (HasViewer())
    {
    return dynamic_cast<QGoImageView2D*>(m_currentView);
    }
  else
    {
    return NULL;
    }
}

//--------------------------------------------------------------------------
QString
QGoSynchronizedView2D::
SnapshotViewXY(const GoFigure::FileType& iType,
               const QString& iBaseName)
{
  QGoImageView2D* viewer = GetImageView();
  if (viewer)
    {
    return viewer->SnapshotViewXY(iType, iBaseName);
    }
  else
    {
    return QString();
    }
}
