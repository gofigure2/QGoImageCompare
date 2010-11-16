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
#ifndef __QGoSynchronizedViewManager_h
#define __QGoSynchronizedViewManager_h

#include <QObject>
#include "QGoSynchronizedView2D.h"
#include "QGoSynchronizedView3D.h"
#include "itkImage.h"
#include "QGoSynchronizedView3DCallbacks.h"
#include "QGoSynchronizedView2DCallbacks.h"
#include "QGoGUILibConfigure.h"

class QGoSynchronizedView2DCallbacks;
class QGoSynchronizedView3DCallbacks;
class vtkImageData;

/** \class QGoSynchronizedViewManager
 * \brief High level class for
 * QGoSynchronizedView2D, QGoSynchronizedView2DCallbacks
 * QGoSynchronizedView3D, QGoSynchronizedView3DCallbacks.
 *
 * This class is dealing with QGoSynchronizedViews for correct
 * synchronization and handling provides a simple interface
 * to create/delete/synchronize QGoSynchronizedView s.
 * This class should be used with any class using QGoSynchronizedView
 * and QGoSynchronize.
 * \example GUI/lib/qgosynchronizedviewmanagertest.cxx
 */
class QGOGUILIB_EXPORT QGoSynchronizedViewManager:public QObject
{
  Q_OBJECT
public:

  explicit QGoSynchronizedViewManager(QObject *parent = 0);
  ~QGoSynchronizedViewManager();

  /** \brief create and add a QGoSynchronizedView to
   * QGoSynchronizedViewManager's parent Object/Widget
   */
  template< typename TPixel >
  QGoSynchronizedView3D * newSynchronizedView(
    QString iSynchronizedViewName,
    typename itk::Image< TPixel, 3 >::Pointer iImage)
  {
    // verify input
    if ( ( iSynchronizedViewName.isEmpty() ) || ( iImage.IsNull() ) )
      {
      return NULL;
      }

    // we create a new QGoSynchronizedView with the same parent windows as this
    QGoSynchronizedView3D *tempNewSynchronizedView =
      new QGoSynchronizedView3D( iSynchronizedViewName,
                                 static_cast< QWidget * >( this->parent() ) );
    // we add the image to it
    tempNewSynchronizedView->SetImage< TPixel >(iImage);
    // if we are synchronizing different QGoSynchronizedView s
    if ( m_Synchronizer3D != NULL )
      {
      // we tell the synchronizer to also synchronize
      // the new QGoSynchronizedView
      m_Synchronizer3D->addSynchronizedView(tempNewSynchronizedView);
      }
    // tell the QGoSynchronizedView who is his Manager
    tempNewSynchronizedView->SetViewManager(this);
    // we add the QGoSynchronizedView to the list of open
    // QGoSynchronizedViews
    m_openSynchronizedViews3D.push_back(tempNewSynchronizedView);

    return tempNewSynchronizedView;
  }

  template< typename TPixel >
  QGoSynchronizedView2D * newSynchronizedView(
    QString iSynchronizedViewName,
    typename itk::Image< TPixel, 2 >::Pointer iImage)
  {
    // verify input
    if ( ( iSynchronizedViewName.isEmpty() ) || ( iImage.IsNull() ) )
      {
      return NULL;
      }

    // we create a new QGoSynchronizedView
    // with the same parent windows as this
    QGoSynchronizedView2D *tempNewSynchronizedView =
      new QGoSynchronizedView2D( iSynchronizedViewName,
                                 static_cast< QWidget * >( this->parent() ) );
    // we add the image to it
    tempNewSynchronizedView->SetImage< TPixel >(iImage);
    // if we are synchronizing different QGoSynchronizedViews
    if ( m_Synchronizer != NULL )
      {
      // we tell the synchronizer to also synchronize the new
      // QGoSynchronizedView
      m_Synchronizer->addSynchronizedView(tempNewSynchronizedView);
      }
    // tell the QGoSynchronizedView who is his Manager
    tempNewSynchronizedView->SetViewManager(this);
    // we add the QGoSynchronizedView to the list of
    //open QGoSynchronizedViews
    m_openSynchronizedViews.push_back(tempNewSynchronizedView);

    return tempNewSynchronizedView;
  }

public slots:

  /** \brief create and add a QGoSynchronizedView to
   * QGoSynchronizedViewManager's parent Object/Widget
   * depending on the vtk image given as an input, the
   * returned QGoSynchronizedView can be downcasted to
   * QGoSynchronizedView2D or QGoSynchronizedView3D
   * safe downcasting is user's responsibility.
   */
  QGoSynchronizedView * newSynchronizedView(
    QString iSynchronizedViewName,
    vtkImageData *iImage);

  /** \brief create and add a QGoSynchronizedView2D to
   * QGoSynchronizedViewManager's parent Object/Widget
   */
  QGoSynchronizedView2D * newSynchronizedView2D(
    QString iSynchronizedViewName,
    vtkImageData *iImage);

  /** \brief create and add a QGoSynchronizedView3D to
   * QGoSynchronizedViewManager's parent Object/Widget
   */
  QGoSynchronizedView3D * newSynchronizedView3D(
    QString iSynchronizedViewName,
    vtkImageData *iImage);

  /** \brief delete a QGoSynchronizedView from QGoSynchronizedViewManager's parent
   * Object/Widget
   * delete the QGoSynchronizedView
   */
  void deleteSynchronizedView2D(QGoSynchronizedView2D *ioSynchronizedView);

  void deleteSynchronizedView2D(const int & iIndex);

  void deleteSynchronizedView3D(QGoSynchronizedView3D *ioSynchronizedView);

  void deleteSynchronizedView3D(const int & iIndex);

  /** \brief take a QGoSynchronizedView out of the Manager lists, without deleting it.
   */
  void removeSynchronizedView2D(QGoSynchronizedView2D *ioSynchronizedView);

  void removeSynchronizedView3D(QGoSynchronizedView3D *ioSynchronizedView);

  /** \brief synchronize all open QGoSynchronizedViews
   */
  void synchronizeOpenSynchronizedViews();

  /** \brief remove the synchronization leaving the all QGoSynchronizedView
   *  independent
   */
  void unSynchronizeOpenSynchronizedViews();

  /** \brief true if the manager synchronizes some QGoSynchronizedView
   */
  bool isSynchronizing();

  /** \brief update the QGoSynchronizedView s
   */
  void Update();

  /** \brief show all the QGoSynchronizedView s
   */
  void show();

protected:

  // vector to store the open QGoSynchronizedView s
  std::vector< QGoSynchronizedView2D * > m_openSynchronizedViews;
  std::vector< QGoSynchronizedView3D * > m_openSynchronizedViews3D;

  QGoSynchronizedView2DCallbacks *m_Synchronizer;
  QGoSynchronizedView3DCallbacks *m_Synchronizer3D;
private:
  Q_DISABLE_COPY(QGoSynchronizedViewManager);
};

#endif // __QGoSynchronizedViewManager_h
