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
#include "QGoImageView.h"

#include "QVTKWidget.h"
#include "vtkEventQtSlotConnect.h"
#include "vtkLookupTable.h"
#include "vtkRenderWindow.h"
#include "vtkRendererCollection.h"
#include "vtkImageData.h"
#include "vtkTextProperty.h"
#include "vtkViewImage2DCollection.h"
#include "vtkViewImage2DCollectionCommand.h"
#include "vtkViewImage2D.h"

// For the seed widget
#include "vtkConstrainedPointHandleRepresentation.h"
#include "vtkSeedWidget.h"
#include "vtkImageActorPointPlacer.h"
#include "vtkProperty.h"
#include "vtkWidgetEvent.h"
#include "vtkWidgetEventTranslator.h"

// For the distance widget...
#include "vtkDistanceWidget.h"

// For the angle widget...
#include "vtkAngleWidget.h"

// For the contour widget...
#include "vtkContourWidget.h"
#include "vtkOrientedGlyphContourRepresentation.h"
#include "vtkPolyData.h"

//--------------------------------------------------------------------------
/**
 * \brief Default Constructor.
 * \param iParent
 */
QGoImageView::
QGoImageView(QWidget* iParent) : QWidget(iParent),
  m_Pool(0),
  m_Image(0),
  m_SnapshotId(0),
  m_ShowAnnotations(true),
  m_ShowSplinePlane(true)
  {
  m_Pool = vtkViewImage2DCollection::New();
  }

//--------------------------------------------------------------------------
QGoImageView::
~QGoImageView()
  {
  std::vector<vtkSeedWidget*>::iterator seedWidgetIterator = m_SeedWidget.begin();
  while (seedWidgetIterator != m_SeedWidget.end())
    {
    (*seedWidgetIterator)->Delete();
    ++seedWidgetIterator;
    }

  std::vector<vtkConstrainedPointHandleRepresentation*>::iterator
  handleIterator = m_Handle.begin();
  while (handleIterator != m_Handle.end())
    {
    (*handleIterator)->Delete();
    ++handleIterator;
    }

  std::vector<vtkSeedRepresentation*>::iterator
  seedIterator = m_SeedRep.begin();
  while (seedIterator != m_SeedRep.end())
    {
    (*seedIterator)->Delete();
    ++seedIterator;
    }

  if (m_Pool)
    {
    m_Pool->Delete();
    m_Pool = 0;
    }
  }

//--------------------------------------------------------------------------
void
QGoImageView::
GetBackgroundColor(double& r,
                                      double& g, double& b)
{
  double* rgb = this->GetBackgroundColor();
  r = rgb[0];
  g = rgb[1];
  b = rgb[2];
}

//-------------------------------------------------------------------------
double*
QGoImageView::
GetBackgroundColor()
{
  return m_Pool->GetItem(0)->GetBackground();
}

//-------------------------------------------------------------------------
void
QGoImageView::
SetBackgroundColor(const double& r,
                                      const double& g,
                                      const double& b)
{
  double textcolor[3];

  if ((r != 0.5) && (g != 0.5) && (b != 0.5))
    {
    textcolor[0] = 1. - r;
    textcolor[1] = 1. - g;
    textcolor[2] = 1. - b;
    }
  else
    {
    textcolor[0] = 1.;
    textcolor[1] = 1.;
    textcolor[2] = 0.;
    }

  double rgb[3] = { r, g, b };

  m_Pool->SyncSetBackground(rgb);
  int n = m_Pool->GetNumberOfItems();

  for (int i = 0; i < n; i++)
    {
    vtkTextProperty* tproperty =
      m_Pool->GetItem(i)->GetTextProperty();
    tproperty->SetFontFamilyToArial();
    tproperty->SetFontSize(14);
    tproperty->SetColor(textcolor);
    }

  m_Pool->SyncRender();
}

//--------------------------------------------------------------------------
void
QGoImageView::
SetBackgroundColor(double rgb[3])
{
  this->SetBackgroundColor(rgb[0], rgb[1], rgb[2]);
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void
QGoImageView::
SetBackgroundColor(const QColor& iColor)
{
  double r, g, b;
  iColor.getRgbF(&r, &g, &b);

  this->SetBackgroundColor(r, g, b);
}

//--------------------------------------------------------------------------
int*
QGoImageView::
GetImageCoordinatesFromWorldCoordinates(double iPos[3])
{
  vtkViewImage2D* View = m_Pool->GetItem(0);
  return View->GetImageCoordinatesFromWorldCoordinates(iPos);
}

//--------------------------------------------------------------------------

vtkViewImage2D*
QGoImageView::
GetImageViewer(const int& iId)
{
  return m_Pool->GetItem(iId);
}

//--------------------------------------------------------------------------
int
QGoImageView::
GetNumberOfImageViewers()
{
  return m_Pool->GetNumberOfItems();
}

//--------------------------------------------------------------------------
std::vector<vtkActor*>
QGoImageView::
AddContour(const int& iId, vtkPolyData* iDataset, vtkProperty* iProperty)
{
  (void) iId;

  int n = m_Pool->GetNumberOfItems();

  std::vector<vtkActor*> oActorVector(n);

  for (int i = 0; i < n; i++)
    {
    vtkViewImage2D* viewer = m_Pool->GetItem(i);
    vtkActor* temp = viewer->AddDataSet(iDataset, iProperty);
    viewer->Render();
    oActorVector[i] = temp;
    }

  return oActorVector;
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
/**
 * \brief Highlight contour (or not).
 * \param[in] iProp contour
 * \param[in] iToDo to be highlighted
 */
void
QGoImageView::
ChangeActorProperty(vtkProp3D* iActor,
                    vtkProperty* iProperty)
{
  int n = m_Pool->GetNumberOfItems();

  for (int i = 0; i < n; i++)
    {
    vtkViewImage2D* viewer = m_Pool->GetItem(i);
    viewer->ChangeActorProperty(iActor, iProperty);
    }
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void
QGoImageView::
RemoveActor(const int& iId, vtkActor* iActor)
{
  if ((iId >= 0) && (iId < m_Pool->GetNumberOfItems()))
    {
    vtkViewImage2D* viewer = m_Pool->GetItem(iId);
    viewer->GetRenderer()->RemoveActor(iActor);
    }
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void
QGoImageView::
AddActor(const int& iId, vtkActor* iActor)
{
  if ((iId >= 0) && (iId < m_Pool->GetNumberOfItems()))
    {
    vtkViewImage2D* viewer = m_Pool->GetItem(iId);
    viewer->GetRenderer()->AddActor(iActor);
    }
}

//--------------------------------------------------------------------------
void
QGoImageView::
DefaultMode()
{
  std::cout << "Default Mode" <<std::endl;
  //Change cursor
  ChangeCursorShape(Qt::ArrowCursor);

  // Change mode in the collection
  m_Pool->EnableDefaultInteractionMode();
}

//--------------------------------------------------------------------------
void
QGoImageView::
ZoomMode()
{
  std::cout << "Zoom Mode" <<std::endl;
  //Change cursors
  QCursor zoomCursor(QPixmap(QString::fromUtf8(":/fig/zoom.png")), -1, -1);
  ChangeCursorShape(zoomCursor);

  // Change mode in the collection
  m_Pool->EnableZoomInteractionMode();
}

//--------------------------------------------------------------------------
void
QGoImageView::
PanMode()
{
  std::cout << "Pan Mode" <<std::endl;
  //Change cursor
  ChangeCursorShape(Qt::OpenHandCursor);

  // Change mode in the collection
  m_Pool->EnablePanInteractionMode();
}

//-------------------------------------------------------------------------
void
QGoImageView::
ResetWindowLevel()
{
  m_Pool->SyncResetWindowLevel();
}

//-------------------------------------------------------------------------
void
QGoImageView::
SetLookupTable(vtkLookupTable* iLut)
{
  if (this->m_Image->GetNumberOfScalarComponents() == 1)
    {
    m_Pool->SyncSetLookupTable(iLut);
    m_Pool->SyncResetWindowLevel();
    m_Pool->SyncRender();
    }
}

//-------------------------------------------------------------------------
void
QGoImageView::
ShowScalarBar(const bool& iShow)
{
  if (this->m_Image->GetNumberOfScalarComponents() == 1)
    {
    m_Pool->SyncSetShowScalarBar(iShow);
    m_Pool->SyncRender();
    }
}

//-------------------------------------------------------------------------
void
QGoImageView::
UpdateRenderWindows()
{
  this->m_Pool->SyncRender();
}

//-------------------------------------------------------------------------
void
QGoImageView::
ShowAnnotations()
{
  m_ShowAnnotations = !m_ShowAnnotations;
  this->m_Pool->SyncSetShowAnnotations(m_ShowAnnotations);
  UpdateRenderWindows();
}

//-------------------------------------------------------------------------
vtkImageActor*
QGoImageView::
GetImageActor(const int& iId)
{
  int N = this->m_Pool->GetNumberOfItems();

  if ((iId < 0) || (iId >= N))
    {
    return NULL;
    }
  else
    {
    return m_Pool->GetItem(iId)->GetImageActor();
    }
}

//-------------------------------------------------------------------------
void
QGoImageView::
ChangeActorProperty(int iDir, vtkProp3D* iActor, vtkProperty* iProperty)
{
  m_Pool->GetItem(iDir)->ChangeActorProperty(iActor, iProperty);
}

//--------------------------------------------------------------------------
void
QGoImageView::
ShowSplinePlane()
{
  // Invert state of m_ShowPlane
  m_ShowSplinePlane = !m_ShowSplinePlane;
  m_Pool->SetSplinePlaneActorsVisibility(m_ShowSplinePlane);
}

//--------------------------------------------------------------------------
void
QGoImageView::
SetInterpolate(const int& val)
{
  m_Pool->SyncSetInterpolate(val);
}

//-------------------------------------------------------------------------
void
QGoImageView::
EnableContourPickingMode()
{
  std::cout << "Contour Picking Mode" <<std::endl;
  //Change cursor
  ChangeCursorShape(Qt::ArrowCursor);
  // Change mode in the collection
  m_Pool->EnableContourPickingMode();
}

//-------------------------------------------------------------------------
std::list<vtkProp3D*>
QGoImageView::
GetListOfPickedContours()
{
  // Get picked contours from all views
  return m_Pool->GetCommand()->GetListOfPickedActors();
}

//-------------------------------------------------------------------------
std::list<vtkProp3D*>
QGoImageView::
GetListOfUnPickedContours()
{
  return m_Pool->GetCommand()->GetListOfUnPickedActors();
}

//-------------------------------------------------------------------------
vtkImageData*
QGoImageView::
GetImage()
{
  return m_Image;
}

//-------------------------------------------------------------------------
void
QGoImageView::
InitializeSeedWidget()
{
  int N = this->m_Pool->GetNumberOfItems();

  // Enable seed interaction
  this->m_Handle.resize(N);
  this->m_SeedRep.resize(N);
  this->m_SeedWidget.resize(N);

  for (int i = 0; i < N; ++i)
    {
    this->m_Handle[i] = vtkConstrainedPointHandleRepresentation::New();
    this->m_Handle[i]->GetProperty()->SetColor(1, 0, 0);

    this->m_SeedRep[i] = vtkSeedRepresentation::New();
    this->m_SeedRep[i]->SetHandleRepresentation(this->m_Handle[i]);

    this->m_SeedWidget[i] = vtkSeedWidget::New();
    this->m_SeedWidget[i]->SetRepresentation(this->m_SeedRep[i]);
    this->m_SeedWidget[i]->SetRepresentation(this->m_SeedRep[i]);

    this->m_SeedWidget[i]->SetInteractor(
      this->m_Pool->GetItem(i)->GetInteractor());

    // to remove right click interaction in the one click widget
    this->m_SeedWidget[i]->GetEventTranslator()->RemoveTranslation(
      vtkCommand::RightButtonPressEvent);
    }
}

//-------------------------------------------------------------------------
void
QGoImageView::
EnableSeedWidget(bool iEnable)
{
  std::cout << "Seed ---Widget---" <<std::endl;

  if(iEnable)
    {
    DefaultMode();
    }

  std::vector<vtkSeedWidget*>::iterator
  it = m_SeedWidget.begin();
  while (it != m_SeedWidget.end())
    {
    (*it)->SetEnabled(iEnable);
    ++it;
    }
}

//-------------------------------------------------------------------------
vtkPoints*
QGoImageView::
GetAllSeeds()
{
  double worldPosition[3];

  /// TODO MEMORY LEAK HERE
  vtkPoints* oPoints = vtkPoints::New();

  for (unsigned int i = 0; i < this->m_SeedWidget.size(); i++)
    {
    int N = this->m_SeedRep[i]->GetNumberOfSeeds();
    for (int j = 0; j < N; j++)
      {
      // Get World position (may be not accurate if we are between 8 pixels (3D))
      this->m_SeedRep[i]->GetSeedWorldPosition(j, worldPosition);
      // Get indexes of the closest point
      int* index = this->m_Pool->GetItem(i)->GetImageCoordinatesFromWorldCoordinates(worldPosition);
      // Convert it back into world position
      double spacing[3];
      this->m_Pool->GetItem(i)->GetInput()->GetSpacing(spacing);
      double correctedPosition[3];
      correctedPosition[0] = static_cast<double>(index[0]) * spacing[0];
      correctedPosition[1] = static_cast<double>(index[1]) * spacing[1];
      correctedPosition[2] = static_cast<double>(index[2]) * spacing[2];
      oPoints->InsertNextPoint(correctedPosition);
      }
    }

  return oPoints;
}

//-------------------------------------------------------------------------
void
QGoImageView::
ClearAllSeeds()
{
  for (unsigned int i = 0; i < this->m_SeedWidget.size(); i++)
    {
    for (int k = this->m_SeedRep[i]->GetNumberOfSeeds() - 1; k >= 0; --k)
      {
      this->m_SeedWidget[i]->DeleteSeed(k);
      this->m_SeedRep[i]->RemoveLastHandle();
      }
    }
}

//-------------------------------------------------------------------------
void
QGoImageView::
InitializeDistanceWidget()
{
  int N = this->m_Pool->GetNumberOfItems();
  m_DistanceWidget.resize(N);
  for (int i = 0; i < N; ++i)
    {
    this->m_DistanceWidget[i] = vtkSmartPointer<vtkDistanceWidget>::New();
    this->m_DistanceWidget[i]->SetInteractor(this->m_Pool->GetItem(i)->GetInteractor());
    this->m_DistanceWidget[i]->CreateDefaultRepresentation();

    this->m_DistanceWidget[i]->Off();
    }
}

//-------------------------------------------------------------------------
void
QGoImageView::
EnableDistanceWidget(bool iActive)
{
  std::cout << "Distance ---Widget---" <<std::endl;
  if(iActive)
    {
    DefaultMode();
    }

  int N = this->m_Pool->GetNumberOfItems();
  for (int i = 0; i < N; i++)
  {
  this->m_DistanceWidget[i]->SetEnabled(iActive);
  }
}

//-------------------------------------------------------------------------
void
QGoImageView::
InitializeAngleWidget()
{
  int N = this->m_Pool->GetNumberOfItems();
  m_AngleWidget.resize(N);
  for (int i = 0; i < N; ++i)
    {
    this->m_AngleWidget[i] = vtkSmartPointer<vtkAngleWidget>::New();
    this->m_AngleWidget[i]->SetInteractor(this->m_Pool->GetItem(i)->GetInteractor());
    this->m_AngleWidget[i]->CreateDefaultRepresentation();

    this->m_AngleWidget[i]->Off();
    }
}

//-------------------------------------------------------------------------
void
QGoImageView::
EnableAngleWidget(bool iActive)
{
  std::cout << "Angle ---Widget---" <<std::endl;

  if(iActive)
    {
    DefaultMode();
    }

  int N = this->m_Pool->GetNumberOfItems();
  for (int i = 0; i < N; i++)
  {
  if (iActive)
    {
    this->m_AngleWidget[i]->On();
    }
  else
    {
    this->m_AngleWidget[i]->Off();
    }
  }
}

//-------------------------------------------------------------------------
void
QGoImageView::
InitializeContourWidget()
{
  int N = this->m_Pool->GetNumberOfItems();
  m_ContourWidget.resize(N);
  m_ContourRepresentation.resize(N);

  // use while iterator
  for (int i = 0; i < N; ++i)
    {
    // Contour widget
    m_ContourRepresentation[i] =
      vtkSmartPointer<vtkOrientedGlyphContourRepresentation>::New();
    m_ContourRepresentation[i]->GetProperty()->SetColor(0., 1., 1.);
    m_ContourRepresentation[i]->GetLinesProperty()->SetColor(1., 0., 1.);
    m_ContourRepresentation[i]->GetActiveProperty()->SetColor(1., 1., 0.);

    m_ContourWidget[i] = vtkSmartPointer<vtkContourWidget>::New();
    m_ContourWidget[i]->SetPriority(10.0);
    m_ContourWidget[i]->SetInteractor(this->m_Pool->GetItem(i)->GetInteractor());
    m_ContourWidget[i]->Off();

    m_ContourWidget[i]->SetRepresentation(this->m_ContourRepresentation[i]);
    }
}

//-------------------------------------------------------------------------
void
QGoImageView::
EnableContourWidget(bool iActivate)
{
  std::cout << "Contour ---Widget---" <<std::endl;

  if(iActivate)
    {
    DefaultMode();
    }

  std::vector<vtkSmartPointer<vtkContourWidget> >::iterator
  it = m_ContourWidget.begin();
  while (it != m_ContourWidget.end())
    {
    (*it)->SetEnabled(iActivate);
    ++it;
    }
}

//-------------------------------------------------------------------------
void
QGoImageView::
InitializeContourWidgetNodes( int iDir, vtkPolyData* iNodes )
{
  if( (iDir >= 0) && (iDir < m_Pool->GetNumberOfItems()) )
    {
    m_ContourWidget[iDir]->SetEnabled(1);
    m_ContourWidget[iDir]->Initialize(iNodes);
    }
}

//-------------------------------------------------------------------------
void
QGoImageView::
ReinitializeContourWidget()
{
  for (unsigned int i = 0; i < m_ContourWidget.size(); i++)
    {
    InitializeContourWidgetNodes( i, NULL );
    }
}

//-------------------------------------------------------------------------
void
QGoImageView::
UpdateContourRepresentationProperties(float  linewidth, QColor linecolor,
                                      QColor nodecolor, QColor activenodecolor)
{
  m_LinesWidth = linewidth;
  m_LinesColor = linecolor;
  m_NodesColor = nodecolor;
  m_ActiveNodesColor = activenodecolor;

  double rl, gl, bl;
  linecolor.getRgbF(&rl, &gl, &bl);

  double rn, gn, bn;
  nodecolor.getRgbF(&rn, &gn, &bn);

  double ra, ga, ba;
  activenodecolor.getRgbF(&ra, &ga, &ba);

  std::vector<vtkSmartPointer<vtkOrientedGlyphContourRepresentation> >::iterator
  it = m_ContourRepresentation.begin();
  while (it != m_ContourRepresentation.end())
    {
    (*it)->GetLinesProperty()->SetLineWidth(linewidth);
    (*it)->GetLinesProperty()->SetColor(rl, gl, bl);
    (*it)->GetProperty()->SetColor(rn, gn, bn);
    (*it)->GetActiveProperty()->SetColor(ra, ga, ba);
    ++it;
    }
}

//-------------------------------------------------------------------------
vtkPolyData*
QGoImageView::
GetContourRepresentationAsPolydata(int iDir)
{
  return m_ContourRepresentation[iDir]->GetContourRepresentationAsPolyData();
}

//-------------------------------------------------------------------------
vtkPolyData*
QGoImageView::
GetContourRepresentationNodePolydata(int iDir)
{
  vtkPolyData* contour_nodes = vtkPolyData::New();
  m_ContourRepresentation[iDir]->GetNodePolyData(contour_nodes);
  return contour_nodes;
}

//-------------------------------------------------------------------------
void
QGoImageView::
Update()
{
  std::vector<vtkSmartPointer<vtkOrientedGlyphContourRepresentation> >::iterator
  it = m_ContourRepresentation.begin();
  int i=0;

  while (it != m_ContourRepresentation.end())
    {
    vtkSmartPointer<vtkImageActorPointPlacer> point_placer =
      vtkSmartPointer<vtkImageActorPointPlacer>::New();
    point_placer->SetImageActor(GetImageActor(i));

    (*it)->SetPointPlacer(point_placer);
    ++it;
    ++i;
    }
}
