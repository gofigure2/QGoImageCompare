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

#include "QGoImageView2D.h"

#include "QVTKWidget.h"
#include "vtkEventQtSlotConnect.h"
#include "vtkLookupTable.h"
#include "vtkRenderWindow.h"
#include "vtkRendererCollection.h"
#include "vtkImageData.h"
#include "vtkTextProperty.h"
#include "vtkViewImage2DCollection.h"
#include "vtkViewImage2D.h"

//--------------------------------------------------------------------------
QGoImageView2D::QGoImageView2D(QWidget *iiParent):QGoImageView(iiParent)
{
  m_VTKEventQtConnector = vtkEventQtSlotConnect::New();

  this->setupUi(this);

  vtkViewImage2D *View = vtkViewImage2D::New();

  SetupViewGivenQVTKWidget(View, m_QVTKWidgetXY);

  m_Pool->AddItem(View);
  View->Delete();

  QGoImageView::InitializeSeedWidget();
  QGoImageView::InitializeDistanceWidget();
  QGoImageView::InitializeAngleWidget();
  QGoImageView::InitializeContourWidget();
}

//--------------------------------------------------------------------------
QGoImageView2D::
~QGoImageView2D()
{
  m_VTKEventQtConnector->Delete();
}

//--------------------------------------------------------------------------
void
QGoImageView2D::setupUi(QWidget *iiParent)
{
  if ( iiParent->objectName().isEmpty() )
    {
    iiParent->resize(800, 800);
    }

  m_QVTKWidgetXY = new QVTKWidget(this);
  m_QVTKWidgetXY->resize(800, 800);

  m_LayOut = new QHBoxLayout(iiParent);
  m_LayOut->addWidget(m_QVTKWidgetXY);

  retranslateUi(iiParent);

  QMetaObject::connectSlotsByName(iiParent);
}

//--------------------------------------------------------------------------
void
QGoImageView2D::retranslateUi(QWidget *iParent)
{
  iParent->setWindowTitle( tr("QGoImageView2D") );
  Q_UNUSED(iParent);
}

//--------------------------------------------------------------------------
void
QGoImageView2D::SetImage(vtkImageData *iImage)
{
  int dim[3];

  iImage->GetDimensions(dim);

  if ( ( dim[0] > 1 ) && ( dim[1] > 1 ) && ( dim[2] > 1 ) )
    {
    std::cout << "void QGoTabImageView2D::SetImage( vtkImageData* iImage )"
              << std::endl;
    std::cout << "This widget only display 2D images." << std::endl;
    return;
    }
  else
    {
    m_Image = iImage;
    }
}

//--------------------------------------------------------------------------
QVTKInteractor *
QGoImageView2D::GetInteractor(const int & i)
{
  if ( i == 0 )
    {
    return m_QVTKWidgetXY->GetInteractor();
    }
  else
    {
    return 0;
    }
}

//--------------------------------------------------------------------------
void
QGoImageView2D::Update()
{
  vtkViewImage2D *View = m_Pool->GetItem(0);

  View->SetInput(this->m_Image);
  View->GetTextProperty()->SetFontFamilyToArial();
  View->GetTextProperty()->SetFontSize(20);
  View->SetViewOrientation(vtkViewImage2D::VIEW_ORIENTATION_AXIAL);
  View->SetViewConvention(vtkViewImage2D::VIEW_CONVENTION_NEUROLOGICAL);

  m_Pool->Initialize();
  m_Pool->InitializeAllObservers();
  m_Pool->SyncSetShowAnnotations(true);
  m_Pool->SyncSetShowScalarBar(false);
  m_Pool->SyncRender();
  m_Pool->SyncReset();
}

//--------------------------------------------------------------------------
QString
QGoImageView2D::SnapshotViewXY(const GoFigure::FileType & iType, const QString & iBaseName)
{
  QString filename = SnapshotView(this->m_QVTKWidgetXY, iType,
                                  iBaseName, m_SnapshotId);

  m_SnapshotId++;
  return filename;
}

//-------------------------------------------------------------------------
void
QGoImageView2D::ChangeCursorShape(QCursor iCursorShape)
{
  //Change cursor
  m_QVTKWidgetXY->setCursor(iCursorShape);
}
