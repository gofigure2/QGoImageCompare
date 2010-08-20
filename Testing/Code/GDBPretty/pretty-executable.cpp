/** An executable to test the gdb pretty printer on. */

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
  return 0;
}
