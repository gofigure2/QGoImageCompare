/** An executable to test the gdb pretty printer on. */

#include "itkImageFileReader.h"

int main( int argc, char** argv )
{
  if( argc < 2 )
    {
    std::cerr << "Usage: " << argv[0] << " inputImage" << std::endl;
    return 1;
    }

  return 0;
}
