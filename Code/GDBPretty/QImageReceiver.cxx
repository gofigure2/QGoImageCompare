#include "QImageReceiver.h"

#include <iostream>
#include <sstream>
#include <stdexcept>

#include <QtCore/QFile>
#include <QCoreApplication>
#include <QtGui/QMessageBox>

QImageReceiver
::QImageReceiver( QObject * parent ):
  QObject( parent ),
  m_ImageIndex( 0 ),
  m_ExpectedContent( InitializationString )
{
  m_Socket = new QLocalSocket( this );

  connect( m_Socket, SIGNAL(readyRead()),
    this, SLOT(readPendingDatagram()));
  connect( m_Socket, SIGNAL(error(QLocalSocket::LocalSocketError)),
    this, SLOT(displayError(QLocalSocket::LocalSocketError)));
  QString pid;
  pid.setNum( QCoreApplication::applicationPid() );
  m_Socket->connectToServer( "icpPythonModule" + pid );
  if( !m_Socket->waitForConnected( 3000 ) )
    this->displayError( m_Socket->error() );
}

QImageReceiver
::~QImageReceiver()
{
}


void
QImageReceiver
::displayError( QLocalSocket::LocalSocketError socketError )
{
  switch ( socketError )
  {
  case QLocalSocket::ServerNotFoundError:
    QMessageBox::information(0, tr("icpGui"),
                               tr("The host was not found." ));
    break;
  case QLocalSocket::ConnectionRefusedError:
    QMessageBox::information(0, tr("icpGui"),
                               tr("The connection was refused by the peer. " ));
    break;
  case QLocalSocket::PeerClosedError:
    break;
  default:
    QMessageBox::information(0, tr("icpGui"),
                               tr("The following error occurred: %1.")
                               .arg(m_Socket->errorString()));
  }
}


void
QImageReceiver
::readPendingDatagram()
{
  int content;
  QByteArray ba;
  while( !m_Socket->atEnd() )
    {
    m_Socket->read( reinterpret_cast< char * >( &content ), sizeof( int ) );

    /** Check to make sure the content is as expected.  The pieces of data
     * describing an image should come in order. */
    if( content != m_ExpectedContent )
      throw( std::runtime_error( "Image datagram did not have its expected content." ));
    if( content == FinalizationString )
      m_ExpectedContent = InitializationString;
    else
      m_ExpectedContent = static_cast< DatagramContent >( content + 1 );


    switch( content )
      {
    case InitializationString:
      ba = m_Socket->readLine();
      std::cout << "****" << ba.data() << "****" << std::endl;
      break;
    case ImageSize:
      ba = m_Socket->read( 3 * sizeof( long ));
      break;
    default:
    // @todo throw an exception here?
      std::cerr << "Error: Unknown content " << content << std::endl;
      }
    }

  this->applyContent( content, ba );
}


void
QImageReceiver
::applyContent( int content, QByteArray & ba )
{
  switch ( content )
    {
  case InitializationString:
    m_Images.push_back( vtkSmartPointer< vtkImageData >::New() );
    m_Images[m_ImageIndex]->SetNumberOfScalarComponents( 1 );
    break;
  case ImageSize:
      {
      long * size_l = reinterpret_cast< long * >( ba.data() );

      int dims[3];
      for( unsigned int i = 0; i < 3; ++i )
        {
        dims[i] = static_cast< int >( size_l[i] );
        }
      m_Images[m_ImageIndex]->SetDimensions( dims );
      break;
      }
  case FinalizationString:
    ++m_ImageIndex;
    // @todo add the image to the visualization
    break;
  default:
    // @todo throw an exception here?
    std::cerr << "Error: Unknown content " << content << std::endl;
    }
}
