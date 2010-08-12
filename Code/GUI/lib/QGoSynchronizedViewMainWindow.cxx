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
#include "QGoSynchronizedViewMainWindow.h"

#include <sstream>

#include <QtGui>
#include "qinputdialog.h"

#include "QGoSynchronizedView.h"
#include "QGoSynchronizedView3D.h"
#include "QGoSynchronizedViewManager.h"

#include "itkImageFileReader.h"

#include "vtkImageReader2Factory.h"
#include "vtkImageReader2.h"
#include "vtkImageData.h"

QGoSynchronizedViewMainWindow::
QGoSynchronizedViewMainWindow()
  {
  m_SynchronizedViewManager = new QGoSynchronizedViewManager(this);

  mdiArea = new QMdiArea;

  setCentralWidget(mdiArea);
  connect(mdiArea,
          SIGNAL(subWindowActivated(QMdiSubWindow*)),
          this,
          SLOT(updateMenus()));

  windowMapper = new QSignalMapper(this);
  connect(windowMapper,
          SIGNAL(mapped(QWidget*)),
          this,
          SLOT(setActiveSubWindow(QWidget*)));

  createActions();
  createMenus();
  createToolBars();
  createStatusBar();
  updateMenus();

  setWindowTitle(tr("QGoSynchronizedViewMainWindow"));
  setUnifiedTitleAndToolBarOnMac(true);

  //mdiArea->setViewMode(QMdiArea::TabbedView);
  }

QGoSynchronizedViewMainWindow::
~QGoSynchronizedViewMainWindow()
  {

  }

QGoSynchronizedViewManager*
QGoSynchronizedViewMainWindow::
GetSynchronizedViewManager()
{
  return m_SynchronizedViewManager;
}

void
QGoSynchronizedViewMainWindow::
Update()
{
  m_SynchronizedViewManager->Update();
  m_SynchronizedViewManager->show();
}

QGoSynchronizedView*
QGoSynchronizedViewMainWindow::
newSynchronizedView(QString iSynchronizedViewName, vtkImageData* iImage)
{
  QGoSynchronizedView* synchronizedView;

  synchronizedView = dynamic_cast<QGoSynchronizedView*> (
    m_SynchronizedViewManager->newSynchronizedView(iSynchronizedViewName,
                                                   iImage));

  mdiArea->addSubWindow(synchronizedView, Qt::SubWindow);
  synchronizedView->parentWidget()
  ->resize(300, 300);

  synchronizedView->show();
  tileAct->trigger();

  return synchronizedView;
}

///*
//QGoSynchronizedView3D* QGoSynchronizedViewMainWindow::newSynchronizedView3D(QString iSynchronizedViewName, QString iImagePath)
//{
//    QGoSynchronizedView3D *synchronizedView = m_SynchronizedViewManager->newSynchronizedView3D( iSynchronizedViewName, iImage);
//
//    mdiArea->addSubWindow(synchronizedView);
//
//    return synchronizedView;
//}
//*/

void
QGoSynchronizedViewMainWindow::
deleteSynchronizedView2D(const int& iId)
{
  m_SynchronizedViewManager->deleteSynchronizedView2D(iId);
}

void
QGoSynchronizedViewMainWindow::
deleteSynchronizedView3D(const int& iId)
{
  m_SynchronizedViewManager->deleteSynchronizedView3D(iId);
}

void
QGoSynchronizedViewMainWindow::
closeEvent(QCloseEvent *iEvent)
{
  if (m_SynchronizedViewManager != NULL)
    {
    delete m_SynchronizedViewManager;
    }
  mdiArea->closeAllSubWindows();
  QMainWindow::closeEvent(iEvent);
  iEvent->accept();
}

void
QGoSynchronizedViewMainWindow::
openfile()
{
  QString filename = QFileDialog::getOpenFileName(
    this,
    tr("Select Image"), "",
    tr("Images (*.png *.bmp *.jpg *.jpeg *.tiff *.mha *.mhd *.img *.lsm)"));
  if (!filename.isEmpty())
    {
    this->OpenSynchronizedViewForFile(filename);
    }
}

void QGoSynchronizedViewMainWindow::
OpenSynchronizedViewForFile(QString& iFile)
{
  if (QFile::exists(iFile))
    {
    // parse extension
    QString ext = QFileInfo(iFile).suffix();
    //if( ext.compare( "lsm", Qt::CaseInsensitive ) == 0 )
    //  {
    //  this->OpenLSMImage( m_CurrentFile, 0 );
    //  }
    //else
      {
      vtkImageReader2Factory* r_factory = vtkImageReader2Factory::New();
      vtkImageReader2*        reader =
        r_factory->CreateImageReader2(iFile.toAscii().data());

      reader->SetFileName(iFile.toAscii().data());
      reader->Update();

      vtkImageData* image = reader->GetOutput();

      newSynchronizedView(iFile, image);

      reader->Delete();
      r_factory->Delete();
      }
    }

}

void
QGoSynchronizedViewMainWindow::
snapshotAs()
{
  QGoSynchronizedView* ScreenshotSynchronizedView = activeSynchronizedView();
  QString              filename = QFileDialog::getSaveFileName(
    this,
    tr("Select Image"), "",
    tr("Images (*.png *.bmp *.jpg *.jpeg *.tiff)")
    );

  if (!filename.isEmpty())
    {
    this->SaveSnapshotInFile(filename, ScreenshotSynchronizedView);
    }
}

void
QGoSynchronizedViewMainWindow::
SaveSnapshotInFile(QString& iFile, QGoSynchronizedView* SynchronizedView)
{
  QGoSynchronizedView3D* temp3DSynchronizedView = NULL;
  QGoSynchronizedView2D* temp2DSynchronizedView = NULL;
  GoFigure::FileType     iType;

  QString extension = iFile.section('.', -1);
  QString nameOfScreenshot = iFile.section('/', -1);

  if (extension.isEmpty() || nameOfScreenshot.isEmpty())
    {
    std::cerr << "QGoSynchronizedViewMainWindow::SaveSnapshotInFile incorrect name of file" << std::endl;
    return;
    }
  std::cout << iFile.toStdString() << std::endl;
  std::cout << extension.toStdString() << std::endl;
  std::cout << nameOfScreenshot.toStdString() << std::endl;
  // file extension  parsing
  if ((extension.contains("jpg", Qt::CaseInsensitive))
      || (extension.contains("jpeg", Qt::CaseInsensitive)))
    {
    iType = GoFigure::JPEG;
    }
  else
    {
    if (extension.contains("bmp", Qt::CaseInsensitive))
      {
      iType = GoFigure::BMP;
      }
    else
      {
      if (extension.contains("png", Qt::CaseInsensitive))
        {
        iType = GoFigure::PNG;
        }
      else
        {
        if (extension.contains("eps", Qt::CaseInsensitive))
          {
          iType = GoFigure::EPS;
          }
        else
          {
          if (extension.contains("tiff", Qt::CaseInsensitive))
            {
            iType = GoFigure::TIFF;
            }
          else
            {
            std::cerr << "QGoSynchronizedViewMainWindow::SaveSnapshotInFile couldn't find appropriate extension" <<
            std::endl;
            return;
            }
          }
        }
      }
    }

  if (SynchronizedView != 0)
    // if we take a snapshot of a 3D synchronizedView
    if (SynchronizedView->GetSynchronizedViewType() == 3)
      {
      temp3DSynchronizedView = static_cast<QGoSynchronizedView3D*>(SynchronizedView);

      temp3DSynchronizedView->GetFullScreenView();

      switch (temp3DSynchronizedView->GetFullScreenView())
        {
        case 0:
          temp3DSynchronizedView->SnapshotViewXYZ(iType, iFile);
          break;

        case 1:
          temp3DSynchronizedView->SnapshotViewXY(iType, iFile);
          break;

        case 2:
          temp3DSynchronizedView->SnapshotView2(iType, iFile);
          break;

        case 3:
          temp3DSynchronizedView->SnapshotView3(iType, iFile);
          break;

        case 4:
          temp3DSynchronizedView->SnapshotViewXYZ(iType, iFile);
          break;

        default:
          std::cerr << "QGoSynchronizedViewMainWindow::SaveSnapshotInFile can't access fullscreen view" << std::endl;
          return;
          break; // facultative
        }
      }
    else // if we take a snapshot of a 2D synchronizedView
      {
      temp2DSynchronizedView = static_cast<QGoSynchronizedView2D*>(SynchronizedView);
      temp2DSynchronizedView->SnapshotViewXY(iType, iFile);
      }
  else
    {
    std::cerr << "QGoSynchronizedViewMainWindow::SaveSnapshotInFile synchronizedView pointer error" << std::endl;
    return;
    }
}

void
QGoSynchronizedViewMainWindow::
imageinfo()
{
  std::stringstream timageinfo;
  if (activeSynchronizedView())
    {
    activeSynchronizedView()->PrintOs(timageinfo);
    QMessageBox::about(this, tr("Image Informations"),
                       QString::fromStdString(timageinfo.str()));
    }

}

void
QGoSynchronizedViewMainWindow::
synchronize()
{
  if (m_SynchronizedViewManager->isSynchronizing())
    {
    m_SynchronizedViewManager->unSynchronizeOpenSynchronizedViews();
    syncAct->setText(tr("&Synchronize images"));
    syncAct->setStatusTip(
      tr("Synchronize open images for point-to-point comparison"));
    }
  else
    {
    m_SynchronizedViewManager->synchronizeOpenSynchronizedViews();
    syncAct->setText(tr("De&Synchronize images"));
    syncAct->setStatusTip(
      tr("Synchronize open images for point-to-point comparison"));
    }
}

void
QGoSynchronizedViewMainWindow::
FullscreenXY()
{
  if (activeSynchronizedView() != 0)
    {
    if (activeSynchronizedView()->GetSynchronizedViewType() == 3)
      {
      static_cast<QGoSynchronizedView3D*>(activeSynchronizedView())->SetFullXYScreenView();
      }
    }
}

void
QGoSynchronizedViewMainWindow::
FullscreenXZ()
{
  if (activeSynchronizedView() != 0)
    {
    if (activeSynchronizedView()->GetSynchronizedViewType() == 3)
      {
      static_cast<QGoSynchronizedView3D*>(activeSynchronizedView())->SetFullXZScreenView();
      }
    }
}

void
QGoSynchronizedViewMainWindow::
FullscreenYZ()
{
  if (activeSynchronizedView() != 0)
    {
    if (activeSynchronizedView()->GetSynchronizedViewType() == 3)
      {
      static_cast<QGoSynchronizedView3D*>(activeSynchronizedView())->SetFullYZScreenView();
      }
    }
}

void
QGoSynchronizedViewMainWindow::
FullscreenXYZ()
{
  if (activeSynchronizedView() != 0)
    {
    if (activeSynchronizedView()->GetSynchronizedViewType() == 3)
      {
      static_cast<QGoSynchronizedView3D*>(activeSynchronizedView())->SetFullXYZScreenView();
      }
    }
}

void
QGoSynchronizedViewMainWindow::
Quadscreen()
{
  if (activeSynchronizedView() != 0)
    {
    if (activeSynchronizedView()->GetSynchronizedViewType() == 3)
      {
      static_cast<QGoSynchronizedView3D*>(activeSynchronizedView())->SetQuadView();
      }
    }
}

void
QGoSynchronizedViewMainWindow::
about()
{
  QMessageBox::about(this,
                     tr("About QGoCompare"),
                     tr("<b>QGoCompare</b> lets you open multiple"
                        " VTK/ITK images from a"
                        " VTK or ITK pipeline and compare them."
                        " This program uses"
                        " Qt, VTK, ITK and GoFigure2 libraries"));
}

void
QGoSynchronizedViewMainWindow::
aboutGF2()
{
  QMessageBox::about(this,
                     tr("About GoFigure2"),
                     tr("<b>GoFigure2</b> is a cross-platform,"
                        " free open source software (FOSS), for"
                        " visualizing, processing and analysing of bioimages"
                        " http://gofigure2.sourceforge.net/"));
}

void
QGoSynchronizedViewMainWindow::
updateMenus()
{
  bool hasSynchronizedView = (activeSynchronizedView() != 0);
  syncAct->setEnabled(hasSynchronizedView);
  closeAct->setEnabled(hasSynchronizedView);
  closeAllAct->setEnabled(hasSynchronizedView);
  tileAct->setEnabled(hasSynchronizedView);
  cascadeAct->setEnabled(hasSynchronizedView);

  // if it is a 3D view, we activate the change view actions
  bool has3DSynchronizedView = ((hasSynchronizedView)
                                && (activeSynchronizedView()->GetSynchronizedViewType() == 3));
  XYviewAct->setEnabled(has3DSynchronizedView);
  XZviewAct->setEnabled(has3DSynchronizedView);
  YZviewAct->setEnabled(has3DSynchronizedView);
  XYZviewAct->setEnabled(has3DSynchronizedView);
  QuadviewAct->setEnabled(has3DSynchronizedView);
  View3DToolBar->setVisible(has3DSynchronizedView);
}

void
QGoSynchronizedViewMainWindow::
updateWindowMenu()
{
  windowMenu->clear();
  windowMenu->addAction(closeAct);
  windowMenu->addAction(closeAllAct);
  windowMenu->addSeparator();
  windowMenu->addAction(tileAct);
  windowMenu->addAction(cascadeAct);
}

void
QGoSynchronizedViewMainWindow::
createActions()
{
  openfileAct = new QAction( /*QIcon( ":/images/open.png" ),*/
    tr("&Open an image file"), this);
  openfileAct->setShortcuts(QKeySequence::Open);
  openfileAct->setStatusTip(tr("Open an image from file"));
  connect(openfileAct,
          SIGNAL(triggered()),
          this,
          SLOT(openfile()));
/*
  openmemAct = new QAction(
    tr("Open from &memory"), this);
  //    openmemAct->setShortcuts(QKeySequence::Open);
  openmemAct->setStatusTip(tr("Open an image from memory"));
  connect(openmemAct,
          SIGNAL(triggered()),
          this,
          SLOT(openfile()));
*/
  syncAct = new QAction(tr("&Synchronize images"), this);
  //    syncAct->setShortcuts(QKeySequence::SaveAs);
  syncAct->setStatusTip(
    tr("Synchronize open images for point-to-point comparison"));
  connect(syncAct,
          SIGNAL(triggered()),
          this,
          SLOT(synchronize()));

  snapshotAsAct = new QAction(tr("Sna&pshot..."), this);
  snapshotAsAct->setShortcuts(QKeySequence::SaveAs);
  snapshotAsAct->setStatusTip(tr("Save a Snapshot as.."));
  connect(snapshotAsAct,
          SIGNAL(triggered()),
          this,
          SLOT(snapshotAs()));

  //! [0]
  exitAct = new QAction(tr("E&xit"), this);
  #if ((QT_MAJOR_VERSION == 4) && (QT_MINOR_VERSION >= 6))
  exitAct->setShortcuts(QKeySequence::Quit);
  #endif
  exitAct->setStatusTip(tr("Exit the application"));
  connect(exitAct,
          SIGNAL(triggered()),
          qApp,
          SLOT(closeAllWindows()));
  //! [0]

  closeAct = new QAction(tr("Cl&ose"), this);
  closeAct->setStatusTip(tr("Close the active image"));
  connect(closeAct,
          SIGNAL(triggered()),
          mdiArea,
          SLOT(closeActiveSubWindow()));

  closeAllAct = new QAction(tr("Close &All"), this);
  closeAllAct->setStatusTip(tr("Close all images"));
  connect(closeAllAct,
          SIGNAL(triggered()),
          mdiArea,
          SLOT(closeAllSubWindows()));

  tileAct = new QAction(tr("&Tile"), this);
  tileAct->setStatusTip(tr("Tile the images"));
  connect(tileAct,
          SIGNAL(triggered()),
          mdiArea,
          SLOT(tileSubWindows()));

  cascadeAct = new QAction(tr("&Cascade"), this);
  cascadeAct->setStatusTip(tr("Cascade the images"));
  connect(cascadeAct,
          SIGNAL(triggered()),
          mdiArea,
          SLOT(cascadeSubWindows()));

  aboutAct = new QAction(tr("&About"), this);
  aboutAct->setStatusTip(tr("Show the application's About box"));
  connect(aboutAct,
          SIGNAL(triggered()),
          this,
          SLOT(about()));

  aboutGF2Act = new QAction(tr("About &GoFigure2"), this);
  aboutGF2Act->setStatusTip(tr("Show the Gofigure2 About box"));
  connect(aboutGF2Act,
          SIGNAL(triggered()),
          this,
          SLOT(aboutGF2()));

  aboutQtAct = new QAction(tr("About &Qt"), this);
  aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
  connect(aboutQtAct,
          SIGNAL(triggered()),
          qApp,
          SLOT(aboutQt()));

  XYviewAct = new QAction(tr("XY view"), this);
  aboutQtAct->setStatusTip(tr("Shows XY view"));
  connect(XYviewAct,
          SIGNAL(triggered()),
          this,
          SLOT(FullscreenXY()));

  XZviewAct = new QAction(tr("XZ view"), this);
  aboutQtAct->setStatusTip(tr("Shows XZ view"));
  connect(XZviewAct,
          SIGNAL(triggered()),
          this,
          SLOT(FullscreenXZ()));

  YZviewAct = new QAction(tr("YZ view"), this);
  aboutQtAct->setStatusTip(tr("Shows YZ view"));
  connect(YZviewAct,
          SIGNAL(triggered()),
          this,
          SLOT(FullscreenYZ()));

  XYZviewAct = new QAction(tr("3D view"), this);
  aboutQtAct->setStatusTip(tr("Show 3D view"));
  connect(XYZviewAct,
          SIGNAL(triggered()),
          this,
          SLOT(FullscreenXYZ()));

  QuadviewAct = new QAction(tr("Quad-view"), this);
  aboutQtAct->setStatusTip(tr("Show 3D view and XY,XZ,YZ projections"));
  connect(QuadviewAct,
          SIGNAL(triggered()),
          this,
          SLOT(Quadscreen()));
}

void QGoSynchronizedViewMainWindow::createMenus()
{
  fileMenu = menuBar()->addMenu(tr("&File"));
  fileMenu->addAction(openfileAct);
//  fileMenu->addAction(openmemAct);
  fileMenu->addAction(snapshotAsAct);
  fileMenu->addSeparator();

  windowMenu = menuBar()->addMenu(tr("&Window"));
  updateWindowMenu();
  connect(windowMenu, SIGNAL(aboutToShow()), this, SLOT(updateWindowMenu()));

  menuBar()->addSeparator();

  helpMenu = menuBar()->addMenu(tr("&Help"));
  helpMenu->addAction(aboutAct);
  helpMenu->addAction(aboutQtAct);
  helpMenu->addAction(aboutGF2Act);
}

void QGoSynchronizedViewMainWindow::createToolBars()
{
  ToolBar = addToolBar(tr("ImageActions"));
//  ToolBar->addAction(openmemAct);
  ToolBar->addAction(openfileAct);
  ToolBar->addAction(snapshotAsAct);
  ToolBar->addAction(syncAct);

  View3DToolBar = addToolBar(tr("Image 3D View selection"));
  View3DToolBar->addAction(XYviewAct);
  View3DToolBar->addAction(XZviewAct);
  View3DToolBar->addAction(YZviewAct);
  View3DToolBar->addAction(XYZviewAct);
  View3DToolBar->addAction(QuadviewAct);
}

void QGoSynchronizedViewMainWindow::createStatusBar()
{
  statusBar()->showMessage(tr("Ready"));
}

QGoSynchronizedView *QGoSynchronizedViewMainWindow::activeSynchronizedView()
{
  if (QMdiSubWindow * activeSubWindow = mdiArea->activeSubWindow())
    {
    return static_cast<QGoSynchronizedView*>(activeSubWindow->widget());
    }
  else
    {
    return 0;
    }
}

QMdiSubWindow *QGoSynchronizedViewMainWindow::findSynchronizedView(const QString& iSynchronizedViewName)
{
  foreach (QMdiSubWindow * twindow, mdiArea->subWindowList())
    {
    QGoSynchronizedView *SynchronizedView =
      qobject_cast<QGoSynchronizedView *>(twindow->widget());
    if (SynchronizedView->GetName() == iSynchronizedViewName)
      {
      return twindow;
      }
    }
  return NULL;
}

void QGoSynchronizedViewMainWindow::setActiveSubWindow(QWidget *twindow)
{
  if (!twindow)
    {
    return;
    }
  else
    {
    mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow *>(twindow));
    }
}
