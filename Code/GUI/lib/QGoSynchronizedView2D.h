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
#ifndef __QGoSynchronizedView2D_h
#define __QGoSynchronizedView2D_h

#include "itkImageToVTKImageFilter.h"
#include "itkCastImageFilter.h"
#include "itkSmartPointer.h"
#include "itkImage.h"
#include "QGoSynchronizedView.h"
#include "SnapshotHelper.h"
#include "vtkSmartPointer.h"

class vtkCamera;
class vtkImageData;
class vtkEventQtSlotConnect;
class QGoImageView2D;

/**
 * \class QGoSynchronizedView2D
 * \brief class used to display a QWidget containing a two dimensional
 * a vtkimagedata* or an itkimage*.
 * QGoSynchronizedView2D provide the interface to synchronize cameras among
 * several GoSynchronizedView2D.
 * \example GUI/lib/qgosynchronizedview2dtest.cxx
 */
class QGoSynchronizedView2D : public QGoSynchronizedView
  {
  // QT macro
  Q_OBJECT
  // itk typedef :
  // type of itk image for visualization
  typedef itk::Image<unsigned char, 2> VisuImageType;
  // itk to vtk connector typdef
  typedef itk::ImageToVTKImageFilter<VisuImageType> itkvtkConnectorType;

public:
  explicit QGoSynchronizedView2D(QString iViewName, QWidget *iParent = 0);

  ~QGoSynchronizedView2D();

  /** \brief Set image displayed by the SynchronizedView
   */
  void SetImage(vtkImageData* iImage);

  /** \brief Update the viewer contained in the widget
   */
  void Update( void );

  /** \brief render the viewer contained in the widget if any
   */
  void Render( void );

  /** \brief get the camera of the current viewer
   */
  vtkCamera* GetCamera( void );

  /** \brief true if the widget has a viewer
   */
  bool HasViewer( void );

  /** \brief Print self informations
   */
  void PrintOs(ostream& os);

  /** \brief returns the type of SynchronizedView (2 for 2D, 3 for 3D)
   */
  int GetSynchronizedViewType( void );



  /** \brief Set ITK image displayed by the SynchronizedView
   */
  template<typename TPixel>
  void SetImage(typename itk::Image<TPixel, 2>::Pointer iImage)
  {
    typedef itk::Image<TPixel, 2> InputImageType;
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

  /** \brief Returns the imageview managed by this SynchronizedView
   */
  QGoImageView2D* GetImageView( void );

public slots:
  /** \brief Save a screenshot of the viewer's content
   */
  QString SnapshotViewXY(const GoFigure::FileType& iType,
                         const QString& iBaseName = tr("Snapshot"));

private:
  /** delete the viewer contained in the widget
   */
  void deleteViewer( void );

  /** \brief create the viewer contained in the widget
   */
  void createViewer( void );


  QGoImageView2D*               m_currentView;
  itkvtkConnectorType::Pointer m_itkvtkConnector;


  Q_DISABLE_COPY(QGoSynchronizedView2D);
  };

#endif // __QGoSynchronizedView2D_h
