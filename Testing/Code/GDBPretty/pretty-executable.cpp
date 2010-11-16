/** An executable to test the gdb pretty printer on. */

#include "itkBoxMeanImageFilter.h"
#include "itkImageFileReader.h"

int main( int argc, char** argv )
{
  if( argc < 2 )
    {
    std::cerr << "Usage: " << argv[0] << " inputImage" << std::endl;
    return 1;
    }

  const unsigned int Dimension = 3;
  typedef float PixelType;
  typedef itk::Image< PixelType, Dimension > ImageType;

  typedef itk::ImageFileReader< ImageType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );

  try
    {
    reader->Update();
    }
  catch( itk::ExceptionObject& e )
    {
    std::cerr << "Error: " << e << std::endl;
    return 1;
    }

  ImageType::ConstPointer output = reader->GetOutput();

  typedef itk::BoxMeanImageFilter< ImageType, ImageType > FilterType;
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput( reader->GetOutput() );

  FilterType::RadiusType radius;
  radius.Fill( 3 );
  filter->SetRadius( radius );

  try
    {
    filter->Update();
    }
  catch( itk::ExceptionObject& e )
    {
    std::cerr << "Error: " << e << std::endl;
    return 1;
    }

  return 0;
}
