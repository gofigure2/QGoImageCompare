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
#ifndef __QGoImageView_h
#define __QGoImageView_h

#include <QWidget>
#include "vtkSmartPointer.h"

class vtkLookupTable;
class vtkImageData;
class vtkViewImage2D;
class vtkViewImage2DCollection;
class vtkImageActor;
class vtkActor;
class vtkPolyData;
class vtkProperty;
class vtkProp3D;
class QVTKInteractor;

// For the seed widget
#include "vtkPoints.h"
#include "vtkSeedRepresentation.h"
class vtkSeedWidget;
class vtkConstrainedPointHandleRepresentation;
class vtkImageActorPointPlacer;

// For the distance widget...
class vtkDistanceWidget;

// For the angle widget...
class vtkAngleWidget;

// For the contour widget
class vtkContourWidget;
class vtkOrientedGlyphContourRepresentation;
class vtkDistanceRepresentation2D;

#include "QGoGUILibConfigure.h"

/**
\class QGoImageView
\brief Abstract class for the visualization of 3D Image represented by one vtkImageData*.
*/
class QGOGUILIB_EXPORT QGoImageView : public QWidget
  {
  Q_OBJECT

public:
  /** \brief Default Constructor. */
  explicit QGoImageView(QWidget* parent = 0);

  /** \brief Destructor. */
  virtual ~QGoImageView();

  /** \brief Set the image to displaid. */
  virtual void SetImage(vtkImageData* iImage) = 0;

  /** \brief Get the displaid image.*/
  vtkImageData* GetImage();

  /** \brief Get Image Coordinates from World Coordinates. */
  int* GetImageCoordinatesFromWorldCoordinates(double pos[3]);

  void Update();

  /** \brief Returns the interactor for one given view. */
  virtual QVTKInteractor* GetInteractor(const int&) = 0;

  virtual void setupUi(QWidget* parent) = 0;
  virtual void retranslateUi(QWidget *parent) = 0;

  /** \brief Returns used background color by viewers.
  \param[out] r red
  \param[out] g green
  \param[out] b blue    */
  void GetBackgroundColor(double& r, double& g, double& b);
  /** \overload */
  double* GetBackgroundColor();

  /**
   * \brief
   * \param[in] iId
   * \return
   */
  vtkViewImage2D* GetImageViewer(const int& iId);
  int GetNumberOfImageViewers();

  virtual void RemoveActor(const int& iId, vtkActor* iActor);
  virtual void AddActor(const int& iId, vtkActor* iActor);

//   virtual std::vector< vtkQuadricLODActor* >

  /**
   * \brief Add contour with given property into the visualization.
   * \param[in] iId direction
   * \param[in] iDataset contour
   * \param[in] iProperty
   * \return vector of vtkActor rendered in each 2D viewer.
   * \todo check the utility of iId.
   */

  virtual std::vector<vtkActor*> AddContour(const int& iId,
                                            vtkPolyData* dataset,
                                            vtkProperty* property = NULL);

  /** \brief Highlight a given contour (iProp) if iToDo is true. */
  virtual void ChangeActorProperty(vtkProp3D* iActor, vtkProperty* iProperty);

  /**
   * \brief Use the default interactor style
   */
  void DefaultMode();

  /**
   * \brief Use the zoom interactor style
   */
  void ZoomMode();

  /**
   * \brief Use the pan interactor style
   */
  void PanMode();

  /**
   * \brief Use the contour picking mode
   */
  void EnableContourPickingMode();

  /**
   * \brief Reset collection Window level
   */
  void ResetWindowLevel();

  /**
   * \brief Set the lookup table in the collection
   */
  void SetLookupTable(vtkLookupTable* iLut);

  /**
   * \brief Show/hide the scalar bar in the collection
   */
  void ShowScalarBar(const bool&);

  /**
   * \brief Update only the visualization
   */
  void UpdateRenderWindows();

  /**
   * \brief Show annotations in the collection
   */
  void ShowAnnotations();

  /**
   * \brief Get the image actor
   */
  vtkImageActor* GetImageActor(const int& iId);

  /**
   * \brief Change the select actor property in the selected view
   */
  void ChangeActorProperty(int iDir, vtkProp3D* iActor, vtkProperty* iProperty);

  /**
   * \brief Invert visibility of the Spline Plane
   */
  void ShowSplinePlane();

  /**
   * \brief Interpolate the data for visualization
   */
  void SetInterpolate(const int& val);

  /**
   * \brief Get list of the 2D picked actors
   */
  std::list<vtkProp3D*> GetListOfPickedContours();

  /**
   * \brief Get list of the 2D unpicked actors
   */
  std::list<vtkProp3D*> GetListOfUnPickedContours();

  virtual void ChangeCursorShape(QCursor iCursorShape) = 0;

  /**
  * \brief Initializae the distance widget
  */
  void InitializeDistanceWidget();
  void EnableDistanceWidget(bool iEnable);

  /**
  * \brief Initializae the angle widget
  */
  void InitializeAngleWidget();
  void EnableAngleWidget(bool iActive);

  /**
  * \brief Initializae the angle widget
  */
  void InitializeContourWidget();
  void EnableContourWidget(bool iActivate);
  void InitializeContourWidgetNodes( int iDir, vtkPolyData* iNodes );
  void ReinitializeContourWidget();
  void UpdateContourRepresentationProperties(float  linewidth, QColor linecolor,
                                        QColor nodecolor, QColor activenodecolor);
  vtkPolyData* GetContourRepresentationAsPolydata(int iDir);
  vtkPolyData* GetContourRepresentationNodePolydata(int iDir);

  /**
  * \brief Initializae the seed widget
  */
  void InitializeSeedWidget();
  void EnableSeedWidget(bool iEnable);
  /**
   * \brief Get all the seeds positions.
   * The seeds have been defined using the one click mode.
   */
  vtkPoints* GetAllSeeds();
  /**
   * \brief Clear all the seeds positions after using it.
   */
  void       ClearAllSeeds();

public slots:
  /** \brief Set background color for all views.
  \param[in] r red
  \param[in] g green
  \param[in] b blue    */
  void SetBackgroundColor(const double& r,
                          const double& g,
                          const double& b);
  /** \overload */
  void SetBackgroundColor(double rgb[3]);
  /** \overload */
  void SetBackgroundColor(const QColor& iColor);

protected:
  vtkViewImage2DCollection* m_Pool;
  vtkImageData*             m_Image;
  std::vector<vtkSeedWidget*>                           m_SeedWidget;
  std::vector<vtkConstrainedPointHandleRepresentation*> m_Handle;
  std::vector<vtkSeedRepresentation*>                   m_SeedRep;

  // Distance Widget specific members
  std::vector<vtkSmartPointer<vtkDistanceWidget> > m_DistanceWidget;
//   std::vector< vtkSmartPointer< vtkDistanceRepresentation2D > > m_DistanceRepresentation;

  // Angle widget specific members
  std::vector<vtkSmartPointer<vtkAngleWidget> > m_AngleWidget;

  // Contour Widget specific members
  std::vector<vtkSmartPointer<vtkContourWidget> >                      m_ContourWidget;
  std::vector<vtkSmartPointer<vtkOrientedGlyphContourRepresentation> > m_ContourRepresentation;
  float  m_LinesWidth;
  QColor m_LinesColor;
  QColor m_NodesColor;
  QColor m_ActiveNodesColor;

  unsigned int              m_SnapshotId;
  bool                      m_ShowAnnotations;
  bool                      m_ShowSplinePlane;
  };

#endif
