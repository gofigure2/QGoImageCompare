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
#include "QGoSynchronizedView.h"
#include "vtkImageData.h"
#include "vtkViewImage2D.h"
#include "vtkEventQtSlotConnect.h"
#include "SnapshotHelper.h"
#include "QGoImageView2D.h"
#include "QGoSynchronizedViewManager.h"

//--------------------------------------------------------------------------
/*
*  Default Constructor.
* \param iViewName
* \param iParent
*/
QGoSynchronizedView::QGoSynchronizedView(QString iViewName, QWidget *iParent):
  QWidget                  (iParent),
  m_ViewName       (iViewName),
  m_Image          (NULL),
  m_ViewManager    (NULL)
{
  setupUi(this);

  // the widget View is just for representing the place of the viewer
  // it is useless
  delete ( View );

  gridLayout->setContentsMargins(1, 1, 1, 1);
  gridLayout->setSpacing(1);

  this->setWindowTitle(iViewName);
  this->resize(300, 300);
}

//--------------------------------------------------------------------------
QGoSynchronizedView::
~QGoSynchronizedView()
{}

//--------------------------------------------------------------------------
void QGoSynchronizedView::changeEvent(QEvent *e)
{
  QWidget::changeEvent(e);

  switch ( e->type() )
    {
    case QEvent::LanguageChange:
      {
      retranslateUi(this);
      break;
      }
    default:
      break;
    }
}

////--------------------------------------------------------------------------
///*  Set image displayed by the comparer
//*/
///*
//void
//QGoSynchronizedView::
//SetImage(vtkImageData* iImage)
//{
//  if (iImage == NULL)
//    {
//    return;
//    }
//
//  // if there is no viewer, we create one
//  if (m_View == NULL)
//    {
//    createViewer();
//    }
//
//  // set the image to the view
//  m_View->SetImage(iImage);
//
//  // update image
//  m_Image = iImage;
//
//  this->Update();
//}
//*/

//--------------------------------------------------------------------------
/* get comparer's name */
QString *
QGoSynchronizedView::GetName()
{
  return &m_ViewName;
}

//--------------------------------------------------------------------------
/* Set the address of the manager */
void
QGoSynchronizedView::SetViewManager(QGoSynchronizedViewManager *iViewManager)
{
  m_ViewManager = iViewManager;
}
