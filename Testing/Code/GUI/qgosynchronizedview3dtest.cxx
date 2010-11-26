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
#include "vtkSmartPointer.h"
#include "vtkMetaImageReader.h"
#include "QGoSynchronizedView3D.h"
#include <QStringList>
#include <QString>

#include "itkImage.h"
#include "itkSmartPointer.h"
#include "itkImageFileReader.h"

int main(int argc, char **argv)
{
  if ( argc != 3 )
    {
    std::cout << "Usage : QGoSynchronizedView3DTest(.exe) " << std::endl;
    std::cout << "1-file.mhd" << std::endl;
    std::cout << "2-test (boolean)" << std::endl;
    return EXIT_FAILURE;
    }

  QApplication app(argc, argv);
  QCoreApplication::setOrganizationName("MegasonLab");
  QCoreApplication::setOrganizationDomain("http://gofigure2.sourceforge.net");

// ITK Typedefs
// ITK Reader and filters Typedef
  typedef double                          InputPixelType;
  typedef itk::Image< InputPixelType, 3 > InputImage3DType;
  typedef InputImage3DType::Pointer       InputImage3DPointer;

  //itk reader
  typedef itk::ImageFileReader< InputImage3DType > itkReaderType;
  itkReaderType::Pointer itkReader = itkReaderType::New();
  itkReader->SetFileName(argv[1]);
  itkReader->Update();

  //vtk reader
  vtkSmartPointer< vtkMetaImageReader > reader =
    vtkSmartPointer< vtkMetaImageReader >::New();
  reader->SetFileName(argv[1]);
  reader->Update();

  QString                cp0 = "itk sync view";
  QGoSynchronizedView3D *SynchronizedView0 = new QGoSynchronizedView3D(cp0, 0);
  QString                cp1 = "vtk sync view";
  QGoSynchronizedView3D *SynchronizedView1 = new QGoSynchronizedView3D(cp1, 0);

  QTimer *timer = new QTimer;
  timer->setSingleShot(true);
  QObject::connect( timer, SIGNAL( timeout() ),
                    SynchronizedView0, SLOT( close() ) );
  QObject::connect( timer, SIGNAL( timeout() ),
                    SynchronizedView1, SLOT( close() ) );

  SynchronizedView0->SetImage< InputPixelType >( itkReader->GetOutput() );
  SynchronizedView0->Update();
  SynchronizedView0->show();

  SynchronizedView1->SetImage( reader->GetOutput() );
  SynchronizedView1->Update();
  SynchronizedView1->show();

  if ( atoi(argv[2]) == 1 )
    {
    timer->start(1500);

    SynchronizedView0->SetFullScreenView(1);
    if ( SynchronizedView0->GetFullScreenView() != 1 )
      {
      std::cerr << "SynchronizedView0->GetFullScreenView() = " << SynchronizedView0->GetFullScreenView();
      std::cerr << " != 1" << std::endl;
      app.closeAllWindows();

      reader->Delete();
      delete timer;
      delete SynchronizedView0;
      delete SynchronizedView1;
      return EXIT_FAILURE;
      }

    SynchronizedView0->SetFullScreenView(2);
    if ( SynchronizedView0->GetFullScreenView() != 2 )
      {
      std::cerr << "SynchronizedView0->GetFullScreenView() = " << SynchronizedView0->GetFullScreenView();
      std::cerr << " != 2" << std::endl;
      app.closeAllWindows();

      delete timer;
      delete SynchronizedView0;
      delete SynchronizedView1;
      return EXIT_FAILURE;
      }

    SynchronizedView0->SetFullScreenView(3);
    if ( SynchronizedView0->GetFullScreenView() != 3 )
      {
      std::cerr << "SynchronizedView0->GetFullScreenView() = " << SynchronizedView0->GetFullScreenView();
      std::cerr << " != 3" << std::endl;
      app.closeAllWindows();

      delete timer;
      delete SynchronizedView0;
      delete SynchronizedView1;
      return EXIT_FAILURE;
      }

    SynchronizedView0->SetFullScreenView(4);
    if ( SynchronizedView0->GetFullScreenView() != 4 )
      {
      std::cerr << "SynchronizedView0->GetFullScreenView() = " << SynchronizedView0->GetFullScreenView();
      std::cerr << " != 4" << std::endl;
      app.closeAllWindows();

      delete timer;
      delete SynchronizedView0;
      delete SynchronizedView1;
      return EXIT_FAILURE;
      }

    SynchronizedView0->SetFullScreenView(0);
    if ( SynchronizedView0->GetFullScreenView() != 0 )
      {
      std::cerr << "SynchronizedView0->GetFullScreenView() = " << SynchronizedView0->GetFullScreenView();
      std::cerr << " != 0" << std::endl;
      app.closeAllWindows();

      delete timer;
      delete SynchronizedView0;
      delete SynchronizedView1;
      return EXIT_FAILURE;
      }

    SynchronizedView1->SetFullScreenView(1);
    if ( SynchronizedView1->GetFullScreenView() != 1 )
      {
      std::cerr << "SynchronizedView1->GetFullScreenView() = " << SynchronizedView1->GetFullScreenView();
      std::cerr << " != 1" << std::endl;
      app.closeAllWindows();

      reader->Delete();
      delete timer;
      delete SynchronizedView0;
      delete SynchronizedView1;
      return EXIT_FAILURE;
      }

    SynchronizedView1->SetFullScreenView(2);
    if ( SynchronizedView1->GetFullScreenView() != 2 )
      {
      std::cerr << "SynchronizedView1->GetFullScreenView() = " << SynchronizedView1->GetFullScreenView();
      std::cerr << " != 2" << std::endl;
      app.closeAllWindows();

      delete timer;
      delete SynchronizedView0;
      delete SynchronizedView1;
      return EXIT_FAILURE;
      }

    SynchronizedView1->SetFullScreenView(3);
    if ( SynchronizedView1->GetFullScreenView() != 3 )
      {
      std::cerr << "SynchronizedView1->GetFullScreenView() = " << SynchronizedView1->GetFullScreenView();
      std::cerr << " != 3" << std::endl;
      app.closeAllWindows();

      delete timer;
      delete SynchronizedView0;
      delete SynchronizedView1;
      return EXIT_FAILURE;
      }

    SynchronizedView1->SetFullScreenView(4);
    if ( SynchronizedView1->GetFullScreenView() != 4 )
      {
      std::cerr << "SynchronizedView1->GetFullScreenView() = " << SynchronizedView1->GetFullScreenView();
      std::cerr << " != 4" << std::endl;
      app.closeAllWindows();

      delete timer;
      delete SynchronizedView0;
      delete SynchronizedView1;
      return EXIT_FAILURE;
      }

    SynchronizedView1->SetFullScreenView(0);
    if ( SynchronizedView1->GetFullScreenView() != 0 )
      {
      std::cerr << "SynchronizedView1->GetFullScreenView() = " << SynchronizedView1->GetFullScreenView();
      std::cerr << " != 0" << std::endl;
      app.closeAllWindows();

      delete timer;
      delete SynchronizedView0;
      delete SynchronizedView1;
      return EXIT_FAILURE;
      }
    }

  app.processEvents();
  int output = app.exec();

  delete timer;
  delete SynchronizedView0;
  delete SynchronizedView1;

  return output;
}
