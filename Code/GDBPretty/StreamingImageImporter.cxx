#include "StreamingImageImporter.h"

#include "QImageReceiver.h"

void
StreamingImageImporter
::ApplyContent( int content, QByteArray & ba )
{
  switch ( content )
    {
  case QImageReceiver::InitializationString:
    m_Image = ImageType::New();
    break;
  case QImageReceiver::ImageSize:
      {
      long * size_l = reinterpret_cast< long * >( ba.data() );
      ImageType::SizeType size;
      for( unsigned int i = 0; i < 3; ++i )
        {
        size[i] = size_l[i];
        }
      ImageType::IndexType index;
      index.Fill( 0 );
      ImageType::RegionType region;
      region.SetIndex( index );
      region.SetSize( size );
      m_Image->SetRegions( region );
      break;
      }
  default:
    std::cerr << "Error: Unknown content " << content << std::endl;
    }
}
