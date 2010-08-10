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
#ifndef __QGoSynchronizedView3D_h
#define __QGoSynchronizedView3D_h

#include "itkImageToVTKImageFilter.h"
#include "itkCastImageFilter.h"
#include "itkSmartPointer.h"
#include "itkImage.h"
#include "QGoSynchronizedView.h"
#include "vtkSmartPointer.h"

class vtkCamera;
class vtkImageData;
class vtkEventQtSlotConnect;
class QGoImageView3D;

/**
 * \class QGoSynchronizedView3D
 * \brief class used to display a QWidget containing a two dimensional
 * a vtkimagedata* or an itkimage*.
 * QGoSynchronizedView3D provide the interface to synchronize cameras among
 * several GoSynchronizedView3D.
 * \example GUI/lib/qgosynchronizedview3dtest.cxx
 */
class QGoSynchronizedView3D : public QGoSynchronizedView
  {
  Q_OBJECT

public:
  // itk typedef :
  // type of itk image for visualization
  typedef itk::Image<unsigned char, 3> VisuImageType;
  // itk to vtk connector
  typedef itk::ImageToVTKImageFilter<VisuImageType> itkvtkConnectorType;

  explicit QGoSynchronizedView3D(QString iViewName, QWidget *iParent = 0);

  ~QGoSynchronizedView3D();

  /** print the SynchronizedView information :
   *  it consists in the image information if any.
   */
  void PrintOs(ostream& os);

  /** Set image displayed by the SynchronizedView
   */
  void SetImage(vtkImageData* iImage);

  /** \brief Set ITK image displayed by the SynchronizedView
   */
  template<typename TPixel>
  void SetImage(typename itk::Image<TPixel, 3>::Pointer iImage)
  {
    typedef itk::Image<TPixel, 3> InputImageType;
    // we cast the input to have a known image to display
    typedef itk::CastImageFilter<InputImageType, VisuImageType>
    CastFilterType;
    typedef typename CastFilterType::Pointer CastFilterTypePointer;

    CastFilterTypePointer castITKFilter = CastFilterType::New();
    m_itkvtkConnector = itkvtkConnectorType::New();

    castITKFilter->SetInput(iImage);
    castITKFilter->Update();
    m_itkvtkConnector->SetInput(castITKFilter->GetOutput());
    m_itkvtkConnector->Update();

    SetImage(m_itkvtkConnector->GetOutput());
    Update();
  }

  /** \brief returns the type of SynchronizedView (2 for 2D, 3 for 3D)
   */
  int GetSynchronizedViewType(void);

  /** render the iId'th imageview:
   *  3D visualization usually contains 4 imageviewers :
   *  three 2D projection and a 3D view : iId=[0-3]
   */
  void Render(const int& iId);

  /** get the camera of the current viewer;
   *  iId=[0-3]
   */
  vtkCamera* GetCamera(const int& iId);

  /** Returns the QGoImageView3D* displayed.
   */
  QGoImageView3D* GetImageView();

  /** Get the fullscreen view :
   *  0 : Quadview (all 4 views)
   *  1 : XY
   *  2 : XZ
   *  3 : YZ
   *  4 : 3D view
   */
  int GetFullScreenView();

public slots:
  QString SnapshotViewXY(const GoFigure::FileType& iType,
                         const QString& iBaseName = tr("Snapshot"));

  QString SnapshotView2(const GoFigure::FileType& iType,
                        const QString& iBaseName = QString("snapshot"));

  QString SnapshotView3(const GoFigure::FileType& iType,
                        const QString& iBaseName = QString("snapshot"));

  QString SnapshotViewXYZ(const GoFigure::FileType& iType,
                          const QString& iBaseName = QString("snapshot"));

  /** Set the fullscreen view : iId = [0-4]
   *  0 : Quadview (all 4 views)
   *  1 : XY
   *  2 : XZ
   *  3 : YZ
   *  4 : 3D view
   */
  void SetFullScreenView(const int& iId);

  /** Set the fullscreen view : XY
   */
  void SetFullXYScreenView();

  /** Set the fullscreen view : XZ
   */
  void SetFullXZScreenView();

  /** Set the fullscreen view : YZ
   */
  void SetFullYZScreenView();

  /** Set the fullscreen view : XYZ
   */
  void SetFullXYZScreenView();

  /** Set the fullscreen view : 3D view
   */
  void SetQuadView();

private:
  /** Create the viewer in the widget
   */
  void createViewer();

  itkvtkConnectorType::Pointer m_itkvtkConnector;

  Q_DISABLE_COPY(QGoSynchronizedView3D);
  };

#endif // QGoSynchronizedView3D_H
