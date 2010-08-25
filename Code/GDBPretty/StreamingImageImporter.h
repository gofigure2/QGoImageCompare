#ifndef __StreamingImageImporter_h
#define __StreamingImageImporter_h

#include <QtCore/QByteArray>

#include "itkImage.h"

/** Construct an itk::Image piecewise as it is streamed over the socket. */
class StreamingImageImporter
{
public:
  typedef itk::Image< float, 3 > ImageType;

  ImageType* GetImage()
    {
    return m_Image.GetPointer();
    }

  void ApplyContent( int content, QByteArray & ba );

protected:
  ImageType::Pointer m_Image;
};

#endif
