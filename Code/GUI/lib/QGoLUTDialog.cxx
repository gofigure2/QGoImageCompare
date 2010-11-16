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

#include "QGoLUTDialog.h"

#include "vtkLookupTable.h"
#include "vtkScalarBarActor.h"
#include "vtkLookupTableManager.h"
#include "vtkTextProperty.h"
#include "vtkRenderer.h"
#include "vtkRendererCollection.h"
#include "vtkRenderWindow.h"

QGoLUTDialog::QGoLUTDialog(QWidget *iParent):
  QDialog(iParent),
  LUT(0)
{
  setupUi(this);

  this->LUT = vtkLookupTableManager::GetBWLookupTable();
  this->Renderer = vtkSmartPointer< vtkRenderer >::New();

  vtkRenderWindow *renwin = this->QvtkWidget->GetRenderWindow();
  renwin->AddRenderer(this->Renderer);

  this->LUTActor = vtkSmartPointer< vtkScalarBarActor >::New();
  this->LUTActor->SetLookupTable(this->LUT);
  this->LUTActor->SetOrientationToHorizontal();
  this->LUTActor->SetWidth(0.8);
  this->LUTActor->SetHeight(0.9);
  this->LUTActor->SetPosition(0.1, 0.1);
  this->LUTActor->SetTitle("LUT");

  this->Renderer->AddActor2D(this->LUTActor);

  renwin->Render();
}

QGoLUTDialog::~QGoLUTDialog()
{
  // Is deleted outside
  //this->LUT->Delete();
  delete this->QvtkWidget;
}

vtkLookupTable * QGoLUTDialog::GetLookupTable()
{
  return this->LUT;
}

vtkLookupTable * QGoLUTDialog::GetLookupTable(QWidget *iiParent,
                                              const QString & iTitle,
                                              const int & iIdx)
{
  QGoLUTDialog dlg(iiParent);

  if ( !iTitle.isEmpty() )
    {
    dlg.setWindowTitle(iTitle);
    }
  dlg.ChangeLookupTable(iIdx);
  dlg.exec();
  return dlg.GetLookupTable();
}

void QGoLUTDialog::setupUi(QDialog *LUTDialog)
{
  if ( LUTDialog->objectName().isEmpty() ) { LUTDialog->setObjectName( QString::fromUtf8("LUTDialog") ); }

  LUTDialog->resize(321, 183);
  LUTDialog->setMinimumSize(200, 150);
  LUTDialog->setModal(true);

  this->VerticalLayout = new QVBoxLayout(LUTDialog);
  this->VerticalLayout->setObjectName( QString::fromUtf8("VerticalLayout") );
  this->VerticalLayout->setContentsMargins(10, 10, 10, 10);
  this->VerticalLayout->setSpacing(20);

  this->HorizontalLayout = new QHBoxLayout;
  this->HorizontalLayout->setObjectName( QString::fromUtf8("HorizontalLayout") );
  this->HorizontalLayout->setContentsMargins(0, 0, 0, 0);

  this->Label = new QLabel;
  this->Label->setObjectName( QString::fromUtf8("Label") );
  this->Label->setText( tr("Lookup Table") );

  this->HorizontalLayout->addWidget(Label);

  this->LUTComboBox = new QComboBox;
  this->LUTComboBox->setObjectName( QString::fromUtf8("LUTComboBox") );
  this->LUTComboBox->setLayoutDirection(Qt::RightToLeft);
  this->LUTComboBox->setAutoFillBackground(false);
  this->LUTComboBox->setEditable(false);
  this->LUTComboBox->setFrame(true);

  int k = 0;
  this->LUTComboBox->insertItem( k++, tr("B/W") );
  this->LUTComboBox->insertItem( k++, tr("B/W Inverse") );
  this->LUTComboBox->insertItem( k++, tr("Spectrum") );
  this->LUTComboBox->insertItem( k++, tr("Hot Metal") );
  this->LUTComboBox->insertItem( k++, tr("GE Color") );
  this->LUTComboBox->insertItem( k++, tr("Flow") );
  this->LUTComboBox->insertItem( k++, tr("LONI") );
  this->LUTComboBox->insertItem( k++, tr("LONI2") );
  this->LUTComboBox->insertItem( k++, tr("Asymmetry") );
  this->LUTComboBox->insertItem( k++, tr("P-Value") );

  this->HorizontalLayout->addWidget(this->LUTComboBox);

  this->VerticalLayout->addLayout(this->HorizontalLayout);

  this->QvtkWidget = new QVTKWidget;
  this->QvtkWidget->setObjectName( QString::fromUtf8("qvtkWidget") );
  this->QvtkWidget->setGeometry( QRect(10, 50, 301, 100) );

  this->VerticalLayout->addWidget(this->QvtkWidget);

  this->HorizontalLayout_2 = new QHBoxLayout;
  this->HorizontalLayout_2->setObjectName( QString::fromUtf8("horizontalLayout_2") );
  this->HorizontalLayout_2->setContentsMargins(0, 0, 0, 0);

  this->HorizontalSpacer = new QSpacerItem(166, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
  this->HorizontalLayout_2->addItem(HorizontalSpacer);

  this->ButtonBox = new QDialogButtonBox;
  this->ButtonBox->setObjectName( QString::fromUtf8("buttonBox") );
  this->ButtonBox->setGeometry( QRect(60, 160, 252, 32) );
  this->ButtonBox->setOrientation(Qt::Horizontal);
  this->ButtonBox->setStandardButtons(QDialogButtonBox::Cancel
                                      | QDialogButtonBox::Ok);
  this->ButtonBox->setCenterButtons(false);
  this->HorizontalLayout_2->addWidget(ButtonBox);

  this->VerticalLayout->addLayout(this->HorizontalLayout_2);

  LUTDialog->setLayout(this->VerticalLayout);

  QObject::connect( this->ButtonBox, SIGNAL( accepted() ),
                    LUTDialog, SLOT( accept() ) );
  QObject::connect( this->ButtonBox, SIGNAL( rejected() ),
                    LUTDialog, SLOT( reject() ) );
  QObject::connect( this->LUTComboBox, SIGNAL( currentIndexChanged(int) ),
                    this, SLOT( ChangeLookupTable(int) ) );

  LUTComboBox->setCurrentIndex(0);

  QMetaObject::connectSlotsByName(LUTDialog);
}

void QGoLUTDialog::ChangeLookupTable(const int & idx)
{
  this->LUT->Delete();
  this->LUT = vtkLookupTableManager::GetLookupTable(idx);
  this->LUTActor->SetLookupTable(this->LUT);

  this->QvtkWidget->GetRenderWindow()->Render();
}
