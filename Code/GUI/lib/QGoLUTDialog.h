/*=========================================================================
  Author: $Author$  // Author of last commit
  Version: $Revision$  // Revision of last commit
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

#ifndef __QGoLUTDialog_h
#define __QGoLUTDialog_h

#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QWidget>
#include <QtGui/QSpacerItem>

class vtkLookupTable;
class vtkScalarBarActor;

#include "vtkSmartPointer.h"

#include "QVTKWidget.h"
#include "QGoGUILibConfigure.h"

/**
 * \class QGoLUTDialog
 * \brief Look-up Table Dialog
 * \image html QGoLUTDialog.png
 * \example GUI/lib/qgolutdialog.cxx
*/
class QGOGUILIB_EXPORT QGoLUTDialog : public QDialog
  {
  Q_OBJECT
public:
  /** \brief Constructor */
  explicit QGoLUTDialog(QWidget* parent = 0);

  /** \brief Destructor */
  virtual ~QGoLUTDialog();

  /** \brief get the selected look up table*/
  vtkLookupTable* GetLookupTable();

  /** \brief get the selected look up table*/
  static vtkLookupTable* GetLookupTable(QWidget* iParent,
                                        const QString& iTitle,
                                        const int& iIdx = 0 );

public slots:
  /** \brief Change look up table*/
  void ChangeLookupTable(const int&);

protected:
  vtkLookupTable*                    LUT;
  vtkSmartPointer<vtkScalarBarActor> LUTActor;
  vtkSmartPointer<vtkRenderer>       Renderer;
  QDialogButtonBox*                  ButtonBox;
  QVTKWidget*                        QvtkWidget;
  QHBoxLayout*                       HorizontalLayout;
  QHBoxLayout*                       HorizontalLayout_2;
  QVBoxLayout*                       VerticalLayout;
  QSpacerItem*                       HorizontalSpacer;
  QLabel*                            Label;
  QComboBox*                         LUTComboBox;

  void setupUi(QDialog *LUTDialog);

private:
  Q_DISABLE_COPY(QGoLUTDialog);
  };
#endif
