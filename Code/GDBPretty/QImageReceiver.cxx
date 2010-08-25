#include "QImageReceiver.h"

#include <iostream>
#include <sstream>
#include <stdexcept>

#include <QtCore/QFile>
#include <QtGui/QMessageBox>

QImageReceiver
::QImageReceiver( QObject * parent ):
  QObject( parent ),
  m_ExpectedContent( InitializationString )
{
  m_Socket = new QLocalSocket( this );

  m_Importer = new StreamingImageImporter();

  connect( m_Socket, SIGNAL(readyRead()),
    this, SLOT(readPendingDatagram()));
  connect( m_Socket, SIGNAL(error(QLocalSocket::LocalSocketError)),
    this, SLOT(displayError(QLocalSocket::LocalSocketError)));
  m_Socket->connectToServer( "icpPythonModule" );
}

QImageReceiver
::~QImageReceiver()
{
  delete m_Importer;
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
      std::cerr << "Error: Unknown content " << content << std::endl;
      }
    }

  m_Importer->ApplyContent( content, ba );
}
