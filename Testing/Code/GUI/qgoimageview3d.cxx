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

#include <QApplication>
#include <QTimer>

#include "vtkMetaImageReader.h"
#include "vtkImageData.h"
#include "vtkSmartPointer.h"
#include "vtkPoints.h"

#include "QGoImageView3D.h"

int main(int argc, char **argv)
{
  if ( argc != 3 )
    {
    std::cout << "Usage : qgoimageview3d(.exe) " << std::endl;
    std::cout << "1-file.mhd" << std::endl;
    std::cout << "2-test (boolean)" << std::endl;
    return EXIT_FAILURE;
    }
  QApplication app(argc, argv);
  QCoreApplication::setOrganizationName("MegasonLab");
  QCoreApplication::setOrganizationDomain("http://gofigure2.sourceforge.net");

  QGoImageView3D *viewer = new QGoImageView3D;

  vtkSmartPointer< vtkMetaImageReader > reader =
    vtkSmartPointer< vtkMetaImageReader >::New();
  reader->SetFileName(argv[1]);
  reader->Update();

  vtkImageData *image = reader->GetOutput();

  QTimer *timer = new QTimer;
  timer->setSingleShot(true);
  QObject::connect( timer, SIGNAL( timeout() ), viewer, SLOT( close() ) );

  viewer->SetImage(image);
  std::cout << viewer->GetImage() << std::endl;

  viewer->Update();
  viewer->show();

  // Stupid tests to increase coverage and track leaks
  viewer->ShowAnnotations();
  viewer->ShowSplinePlane();
  viewer->ShowCube3D();
  viewer->UpdateRenderWindows();
  /// \todo Fix it
  //viewer->SetCamera(1);
  //viewer->SetCamera(2);
  //viewer->SetCamera(3);

  // Modes
  viewer->DefaultMode();
  viewer->ZoomMode();
  viewer->PanMode();
  viewer->EnableContourPickingMode();
  // Protected should be public
  //viewer->EnableMeshPickingMode();

  // Widgets
  viewer->EnableDistanceWidget(true);
  viewer->EnableDistanceWidget(false);
  viewer->EnableAngleWidget(true);
  viewer->EnableAngleWidget(false);
  viewer->EnableContourWidget(true);
  viewer->EnableContourWidget(false);
  viewer->EnableSeedWidget(true);
  viewer->EnableSeedWidget(false);
  viewer->EnableBoxWidget(true);
  viewer->EnableBoxWidget(false);

  vtkPoints *points = viewer->GetAllSeeds();
  points->Delete();
  viewer->ClearAllSeeds();

  if ( atoi(argv[2]) == 1 )
    {
    timer->start(1000);

    viewer->SetFullScreenView(1);
    if ( viewer->GetFullScreenView() != 1 )
      {
      std::cerr << "viewer->GetFullScreenView() = " << viewer->GetFullScreenView();
      std::cerr << " != 1" << std::endl;
      app.closeAllWindows();

      reader->Delete();
      delete timer;
      delete viewer;
      return EXIT_FAILURE;
      }

    viewer->SetFullScreenView(2);
    if ( viewer->GetFullScreenView() != 2 )
      {
      std::cerr << "viewer->GetFullScreenView() = " << viewer->GetFullScreenView();
      std::cerr << " != 2" << std::endl;
      app.closeAllWindows();

      delete timer;
      delete viewer;
      return EXIT_FAILURE;
      }

    viewer->SetFullScreenView(3);
    if ( viewer->GetFullScreenView() != 3 )
      {
      std::cerr << "viewer->GetFullScreenView() = " << viewer->GetFullScreenView();
      std::cerr << " != 3" << std::endl;
      app.closeAllWindows();

      delete timer;
      delete viewer;
      return EXIT_FAILURE;
      }

    viewer->SetFullScreenView(4);
    if ( viewer->GetFullScreenView() != 4 )
      {
      std::cerr << "viewer->GetFullScreenView() = " << viewer->GetFullScreenView();
      std::cerr << " != 4" << std::endl;
      app.closeAllWindows();

      delete timer;
      delete viewer;
      return EXIT_FAILURE;
      }

    viewer->SetFullScreenView(0);
    if ( viewer->GetFullScreenView() != 0 )
      {
      std::cerr << "viewer->GetFullScreenView() = " << viewer->GetFullScreenView();
      std::cerr << " != 0" << std::endl;
      app.closeAllWindows();

      delete timer;
      delete viewer;
      return EXIT_FAILURE;
      }

    int slice = viewer->GetSliceViewXY();
    viewer->SetSliceViewXY(slice + 1);

    if ( viewer->GetSliceViewXY() != slice + 1 )
      {
      std::cerr << "viewer->GetSliceViewXY() = " << viewer->GetSliceViewXY();
      std::cerr << " != slice + 1" << std::endl;
      app.closeAllWindows();

      delete timer;
      delete viewer;
      return EXIT_FAILURE;
      }

    slice = viewer->GetSliceViewXZ();
    viewer->SetSliceViewXZ(slice - 1);

    if ( viewer->GetSliceViewXZ() != slice - 1 )
      {
      std::cerr << "viewer->GetSliceViewXZ() = " << viewer->GetSliceViewXZ();
      std::cerr << " != slice - 1" << std::endl;
      app.closeAllWindows();

      delete timer;
      delete viewer;
      return EXIT_FAILURE;
      }

    slice = viewer->GetSliceViewYZ();
    viewer->SetSliceViewYZ(slice + 1);

    if ( viewer->GetSliceViewYZ() != slice + 1 )
      {
      std::cerr << "viewer->GetSliceViewYZ() = " << viewer->GetSliceViewYZ();
      std::cerr << " != slice + 1" << std::endl;
      app.closeAllWindows();

      delete timer;
      delete viewer;
      return EXIT_FAILURE;
      }

    double r, g, b;
    viewer->SetBackgroundColor(0.5, 0.5, 0.5);
    viewer->GetBackgroundColor(r, g, b);
    if ( r != 0.5 || g != 0.5 || b != 0.5 )
      {
      std::cerr << r << " " << g << " " << b << " != {0.5, 0.5, 0.5}" << std::endl;
      app.closeAllWindows();

      delete timer;
      delete viewer;
      return EXIT_FAILURE;
      }

    viewer->SetBackgroundColor(0.1, 0.6, 0.7);
    viewer->GetBackgroundColor(r, g, b);
    if ( r != 0.1 || g != 0.6 || b != 0.7 )
      {
      std::cerr << r << " " << g << " " << b << " != {0.1, 0.6, 0.7}" << std::endl;
      app.closeAllWindows();

      delete timer;
      delete viewer;
      return EXIT_FAILURE;
      }
    viewer->SetBackgroundColor(0., 0., 0.);
    viewer->GetBackgroundColor(r, g, b);
    if ( r != 0. || g != 0. || b != 0. )
      {
      std::cerr << r << " " << g << " " << b << " != {0., 0., 0.}" << std::endl;
      app.closeAllWindows();

      delete timer;
      delete viewer;
      return EXIT_FAILURE;
      }
    }

  app.processEvents();

  int output = app.exec();

  app.closeAllWindows();

  delete timer;
  delete viewer;

  return output;
}
