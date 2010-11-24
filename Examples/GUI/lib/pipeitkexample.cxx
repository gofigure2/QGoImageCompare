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

#include "itkImage.h"
#include "itkImageFileReader.h"

#include "itkRescaleIntensityImageFilter.h"
#include "itkCurvatureAnisotropicDiffusionImageFilter.h"
#include "itkBinaryThresholdImageFilter.h"

// includes for the visualization :
#include <QApplication>
#include "QGoSynchronizedViewManager.h"

int main( int argc, char * argv[] )
{
  if( argc < 2 )
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << "  inputImageFile" << std::endl;
    return EXIT_FAILURE;
    }

  QApplication app(argc, argv);
  QCoreApplication::setOrganizationName("MegasonLab");
  QCoreApplication::setOrganizationDomain("http://gofigure2.sourceforge.net");


  QGoSynchronizedViewManager *ViewManager = new QGoSynchronizedViewManager ();
  QString ViewName;



  typedef    float    InputPixelType;
  typedef    float    OutputPixelType;

  typedef itk::Image< InputPixelType,  3 >   InputImageType;
  typedef itk::Image< OutputPixelType, 3 >   OutputImageType;


  // read input image
  typedef itk::ImageFileReader< InputImageType >  ReaderType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );


  // rescale input from 0  to 255
  typedef itk::RescaleIntensityImageFilter<
               InputImageType, InputImageType > RescaleFilterType;
  RescaleFilterType::Pointer inputRescaler = RescaleFilterType::New();

  inputRescaler->SetInput(reader->GetOutput());
  inputRescaler->SetOutputMinimum(   0 );
  inputRescaler->SetOutputMaximum( 255 );
  inputRescaler->Update();


  // Visualisation :
  // define a name for this viewer
  ViewName = "input Image";
  // create it and let the ViewManager deal with it
  ViewManager->newSynchronizedView<InputPixelType>( ViewName,
                 inputRescaler->GetOutput() );
  ViewManager->Update();


  // Anisotropic diffusion filter
  typedef itk::CurvatureAnisotropicDiffusionImageFilter<
               InputImageType, InputImageType >  FilterType;

  FilterType::Pointer filter = FilterType::New();

  const unsigned int numberOfIterations = 5;
  const double       timeStep = 0.01225;
  const double       conductance = 7.;
  const bool         useImageSpacing = true;

  filter->SetInput( inputRescaler->GetOutput() );
  filter->SetNumberOfIterations( numberOfIterations );
  filter->SetTimeStep( timeStep );
  filter->SetConductanceParameter( conductance );
  if (useImageSpacing)
    {
    filter->UseImageSpacingOn();
    }
  filter->Update();

  // Visualisation :
  // define a name for this viewer
  ViewName = "filtered image";
  // create it and let the ViewManager deal with it
  ViewManager->newSynchronizedView<InputPixelType>( ViewName,
                 filter->GetOutput() );
  ViewManager->Update();


  // thresholding
  typedef itk::BinaryThresholdImageFilter< InputImageType, InputImageType >
      ThresholdFilterType;
  ThresholdFilterType::Pointer threshold = ThresholdFilterType::New();

  threshold->SetInput( filter->GetOutput() );
  threshold->SetOutsideValue( 0 );
  threshold->SetInsideValue( 255 );
  threshold->SetLowerThreshold( 30);
  threshold->SetUpperThreshold( 255 );
  threshold->Update();


  // Visualisation :
  // define a name for this viewer
  ViewName = "threshold";
  // create it and let the ViewManager deal with it
  ViewManager->newSynchronizedView<InputPixelType>( ViewName,
                 threshold->GetOutput() );
  ViewManager->Update();


  // display the visualisation
  ViewManager->show();
  ViewManager->synchronizeOpenSynchronizedViews();

  // run the Qt event loop
  app.processEvents();
  int output = app.exec();

  // clean up
  delete ViewManager;
  return output;

}
