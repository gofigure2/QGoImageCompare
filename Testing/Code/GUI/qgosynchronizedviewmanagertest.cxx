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
#include "vtkPNGReader.h"
#include "vtkImageGaussianSmooth.h"
#include "vtkImageGradient.h"
#include "vtkMetaImageReader.h"

#include "QGoSynchronizedView.h"
#include "QGoSynchronizedViewManager.h"
#include "QGoSynchronizedView3D.h"

#include "itkImage.h"
#include "itkSmartPointer.h"
#include "itkImageFileReader.h"

#include <QStringList>
#include <QString>

int main(int argc, char **argv)
{
  if ( argc != 4 )
    {
    std::cout << "Usage : QGoSynchronizedViewManagerTest(.exe) " << std::endl;
    std::cout << "1-file.png" << std::endl;
    std::cout << "2-file.mhd" << std::endl;
    std::cout << "3-test (boolean)" << std::endl;
    return EXIT_FAILURE;
    }

  QApplication app(argc, argv);
  QCoreApplication::setOrganizationName("MegasonLab");
  QCoreApplication::setOrganizationDomain("http://gofigure2.sourceforge.net");

  // ITK Typedefs
  // ITK Reader Typedef
  typedef double InputPixelType;
  const unsigned int Dimension = 3;
  typedef itk::Image< InputPixelType, Dimension > InputImage3DType;
  typedef InputImage3DType::Pointer               InputImage3DPointer;

  //itk reader
  typedef itk::ImageFileReader< InputImage3DType > itkReaderType;
  itkReaderType::Pointer itkReader = itkReaderType::New();
  itkReader->SetFileName(argv[2]);
  itkReader->Update();

  // create 3 2D images from 1

  vtkSmartPointer< vtkPNGReader > reader = vtkSmartPointer< vtkPNGReader >::New();
  reader->SetFileName(argv[1]);
  reader->Update();

  vtkSmartPointer< vtkImageGaussianSmooth > filter1 =
    vtkSmartPointer< vtkImageGaussianSmooth >::New();
  filter1->SetInputConnection( reader->GetOutputPort() );
  filter1->Update();

  vtkSmartPointer< vtkImageGradient > filter2 =
    vtkSmartPointer< vtkImageGradient >::New();
  filter2->SetInputConnection( reader->GetOutputPort() );
  filter2->Update();

  // create 3 3D images from 1

  vtkSmartPointer< vtkMetaImageReader > reader3D = vtkSmartPointer< vtkMetaImageReader >::New();
  reader3D->SetFileName(argv[2]);
  reader3D->Update();

  vtkSmartPointer< vtkImageGaussianSmooth > filter13D =
    vtkSmartPointer< vtkImageGaussianSmooth >::New();
  filter13D->SetInputConnection( reader3D->GetOutputPort() );
  filter13D->Update();

  vtkSmartPointer< vtkImageGradient > filter23D =
    vtkSmartPointer< vtkImageGradient >::New();
  filter23D->SetInputConnection( reader3D->GetOutputPort() );
  filter23D->Update();

  QString cp0 = "comp0";
  QString cp1 = "comp1";
  QString cp2 = "comp3";

  QString cp03D = "comp03D";
  QString cp13D = "comp13D";
  QString cp23D = "comp33D";
  QString cp33D = "compITK_3D";

  QGoSynchronizedViewManager *syncViewManage = new QGoSynchronizedViewManager();

  syncViewManage->newSynchronizedView( cp0, reader->GetOutput() );
  syncViewManage->newSynchronizedView( cp1, filter1->GetOutput() );
  syncViewManage->newSynchronizedView( cp03D, reader3D->GetOutput() );
  syncViewManage->newSynchronizedView( cp13D, filter13D->GetOutput() );

  syncViewManage->newSynchronizedView< InputPixelType >( cp33D, itkReader->GetOutput() );

  syncViewManage->Update();
  syncViewManage->show();
  syncViewManage->synchronizeOpenSynchronizedViews();

  QTimer *timer = new QTimer;
  timer->setSingleShot(true);

  QObject::connect( timer, SIGNAL( timeout() ), qApp, SLOT( closeAllWindows() ) );

  if ( atoi(argv[3]) == 1 )
    {
    timer->start(1000);
    }

  app.processEvents();
  int output = app.exec();

  delete timer;
  delete syncViewManage;

  return output;
}
