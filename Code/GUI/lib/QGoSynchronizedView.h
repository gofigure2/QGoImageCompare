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
#ifndef __QGoSynchronizedView_h
#define __QGoSynchronizedView_h

#include "SnapshotHelper.h"
#include "vtkSmartPointer.h"
#include "QGoGUILibConfigure.h"
#include "ui_QGoSynchronizedView.h"

#include "itkImage.h"

class vtkCamera;
class vtkImageData;
class vtkEventQtSlotConnect;
class vtkLookupTable;
class QGoImageView;
class QGoSynchronizedViewManager;

/**
 * \class QGoSynchronizedView
 * \brief abstract class for QGoSynchronizedView2D and QGoSynchronizedView3D.
 * Those classes are used to display a QWidget containing a
 * a vtkImageData* or an itk::Image<>::Pointer.
 * They provide the interface to synchronize cameras.
 * \example GUI/lib/qgosynchronizedview2dtest.cxx
 */

class QGOGUILIB_EXPORT QGoSynchronizedView:public QWidget,
  protected Ui::QGoSynchronizedView
{
  Q_OBJECT
public:
  explicit QGoSynchronizedView(QString iViewName, QWidget *iParent = 0);

  /** \brief Destructor.
   */
  virtual ~QGoSynchronizedView();

  /** \brief Set image displayed by the SynchronizedView
   */
  virtual void SetImage(vtkImageData *iImage) = 0;

  /** \brief Update the viewer contained in the widget
   */
  virtual void Update(void) = 0;

  /**
   * \brief Set the lookup table in the collection
   */
  virtual void SetLookupTable(vtkLookupTable *iLut) = 0;

  /** \brief Set the address of the QGoSynchronizedViewManager
   */
  void SetViewManager(QGoSynchronizedViewManager *iViewManager);

  /** \brief get SynchronizedView's name
   */
  QString * GetName(void);

  /** \brief render the viewer contained in the widget if any
   */
  virtual void Render(void) = 0;

  /** \brief get the camera of the current viewer
   */
  virtual vtkCamera * GetCamera(void) = 0;

  /** \brief true if the widget has a viewer
   */
  virtual bool HasViewer(void) = 0;

  /** \brief print the SynchronizedView information :
   *  it consists in the image information if any.
   */
  virtual void PrintOs(ostream & os) = 0;

  /** \brief returns the type of SynchronizedView (2 for 2D, 3 for 3D)
   */
  virtual int GetSynchronizedViewType(void) = 0;

public slots:

  /** \brief Save a snapshot of the displaid view, in a iType file
   */
  virtual QString SnapshotViewXY( const GoFigure::FileType & iType,
                                  const QString & iBaseName = tr("Snapshot") ) = 0;

protected:

  QString                     m_ViewName;
  vtkImageData *              m_Image;
  QString                     m_ImageName;
  QGoSynchronizedViewManager *m_ViewManager;
  vtkEventQtSlotConnect *     m_VTKEventQtConnector;

  /** \brief Qt change event function
   */
  void changeEvent(QEvent *e);

  /** delete the viewer contained in the widget
   */
  virtual void deleteViewer(void) = 0;

  /** create the viewer contained in the widget
   */
  virtual void createViewer(void) = 0;

private:

  Q_DISABLE_COPY(QGoSynchronizedView);
};

#endif //__QGoSynchronizedView_h
