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

#ifndef __QGoImageView2D_h
#define __QGoImageView2D_h

#include <QtGui/QWidget>
#include <QHBoxLayout>

#include "SnapshotHelper.h"

#include "QGoImageView.h"

class vtkImageData;
class vtkViewImage2DCollection;
class QVTKWidget;
class vtkEventQtSlotConnect;

/**
\class QGoImageView2D
\brief Widget to visualize a 2D image (represented as vtkImageData).
\example GUI/lib/qgoimageview2d.cxx
*/
class QGOGUILIB_EXPORT QGoImageView2D:public QGoImageView
{
  Q_OBJECT
public:
  explicit QGoImageView2D(QWidget *parent = 0);
  ~QGoImageView2D();

  void SetImage(vtkImageData *iImage);

  QVTKInteractor * GetInteractor(const int & i = 0);

  void Update();

  void setupUi(QWidget *parent);

  void retranslateUi(QWidget *parent);

  virtual void ChangeCursorShape(QCursor iCursorShape);

public slots:
  QString SnapshotViewXY( const GoFigure::FileType & iType,
                          const QString & iBaseName = tr("Snapshot") );

protected:
  QHBoxLayout *          m_LayOut;
  QVTKWidget *           m_QVTKWidgetXY;
  vtkEventQtSlotConnect *m_VTKEventQtConnector;
};
#endif
