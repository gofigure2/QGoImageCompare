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
#ifndef __QGoSynchronizedViewMainWindow_h
#define __QGoSynchronizedViewMainWindow_h

#include <QMainWindow>
#include <QMdiArea>

#include "itkImage.h"
#include "itkSmartPointer.h"

#include "QGoSynchronizedViewManager.h"

class QGoSynchronizedView;
class QGoSynchronizedView3D;
class vtkImageData;

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QMdiArea;
class QMdiSubWindow;
class QSignalMapper;
QT_END_NAMESPACE

/**
 * \class QGoSynchronizedViewMainWindow
 * \brief This object is intended to demonstrate the use of the
 * comparer classes package :
 * QGoSynchronizedViewManager
 * QGoSynchronizedView
 * QGoSynchronizedView2D
 * QGoSynchronizedView3D
 * QGoSynchronizedView3DCallbacks
 * QGoSynchronizedView2DCallbacks
 * It is not documented.
 * It is used by the example program : qgosynchronizedviewguitest.cxx
 */

// \example GUI/lib/qgosynchronizedviewguitest.cxx

class QGoSynchronizedViewMainWindow:public QMainWindow
{
// QT macro
  Q_OBJECT
public:

  QGoSynchronizedViewMainWindow();
  ~QGoSynchronizedViewMainWindow();

  QGoSynchronizedViewManager * GetSynchronizedViewManager();

  void Update();

  QGoSynchronizedView * newSynchronizedView(
    QString iSynchronizedViewName,
    vtkImageData *iImage);

  template< typename TPixel >
  QGoSynchronizedView * newSynchronizedView(
    QString iSynchronizedViewName,
    typename itk::Image< TPixel, 3 >::Pointer iImage)
  {
    QGoSynchronizedView *synchronizedView;

    synchronizedView = m_SynchronizedViewManager->newSynchronizedView< TPixel >(
      iSynchronizedViewName, iImage);

    mdiArea->addSubWindow(synchronizedView, Qt::SubWindow);
    synchronizedView->parentWidget()->resize(300, 300);
    synchronizedView->show();
    tileAct->trigger();
    return synchronizedView;
  }

  template< typename TPixel >
  QGoSynchronizedView * newSynchronizedView(
    QString iSynchronizedViewName,
    typename itk::Image< TPixel, 2 >::Pointer iImage)
  {
    QGoSynchronizedView *synchronizedView;

    synchronizedView = m_SynchronizedViewManager->newSynchronizedView< TPixel >(
      iSynchronizedViewName, iImage);

    mdiArea->addSubWindow(synchronizedView, Qt::SubWindow);
    synchronizedView->parentWidget()->resize(300, 300);
    synchronizedView->show();
    tileAct->trigger();
    return synchronizedView;
  }

  void OpenSynchronizedViewForFile(QString & iFile);

  void deleteSynchronizedView2D(const int & iId);

  void deleteSynchronizedView3D(const int & iId);

protected:
  void closeEvent(QCloseEvent *event);

private slots:

  void openfile();

  void snapshotAs();

  void about();

  void aboutGF2();

  void updateMenus();

  void updateWindowMenu();

  void setActiveSubWindow(QWidget *window);

  void synchronize();

  void FullscreenXY();

  void FullscreenXZ();

  void FullscreenYZ();

  void FullscreenXYZ();

  void Quadscreen();

private:

  void SaveSnapshotInFile(QString & iFile, QGoSynchronizedView *SynchronizedView);

  void createActions();

  void createMenus();

  void createToolBars();

  void createStatusBar();

  void readSettings();

  void writeSettings();

  void imageinfo();

  QGoSynchronizedView * activeSynchronizedView();

  QMdiSubWindow * findSynchronizedView(const QString & iSynchronizedViewName);

  QMdiArea *     mdiArea;
  QSignalMapper *windowMapper;

  QMenu *fileMenu;
  QMenu *editMenu;
  QMenu *windowMenu;
  QMenu *helpMenu;

  QToolBar *ToolBar;
  QToolBar *View3DToolBar;
  QAction * syncAct;
  QAction * openfileAct;
//  QAction * openmemAct;
  QAction *snapshotAsAct;
  QAction *exitAct;
  QAction *closeAct;
  QAction *closeAllAct;
  QAction *tileAct;
  QAction *cascadeAct;
  QAction *aboutAct;
  QAction *aboutQtAct;
  QAction *aboutGF2Act;
  QAction *XYviewAct;
  QAction *XZviewAct;
  QAction *YZviewAct;
  QAction *XYZviewAct;
  QAction *QuadviewAct;

  QGoSynchronizedViewManager *m_SynchronizedViewManager;

  typedef void ( QGoSynchronizedView3D::*SynchronizedViewMethod ) ();

  void ApplyToActiveView( SynchronizedViewMethod );
};

#endif // QGOSYNCHRONIZEDVIEWMAINWINDOW_H
