#include "QImageReceiver.h"

#include <iostream>
#include <sstream>
#include <stdexcept>

#include <QtCore/QFile>
#include <QCoreApplication>
#include <QtGui/QMessageBox>

QImageReceiver
::QImageReceiver( QGoSynchronizedViewManager * manager ):
  QObject( manager ),
  m_ImageIndex( 0 ),
  m_ViewManager( manager )
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
  QByteArray ba;

  ba = m_Socket->readLine();
  this->applyContent( InitializationString, ba );

  ba = m_Socket->readLine();
  // Remove the newline.
  ba.chop( 1 );
  this->applyContent( ScalarType, ba );

  ba = m_Socket->read( 3 * sizeof( long ));
  this->applyContent( ImageSize, ba );

  ba = m_Socket->read( 3 * sizeof( double ));
  this->applyContent( ImageOrigin, ba );

  ba = m_Socket->read( 3 * sizeof( double ));
  this->applyContent( ImageSpacing, ba );

  ba = m_Socket->readLine();
  ba.prepend( "gdb pretty print of $" );
  this->applyContent( ValueHistoryCount, ba );
}


void
QImageReceiver
::applyContent( DatagramContent content, QByteArray & ba )
{
  switch ( content )
    {
  case InitializationString:
    m_Images.push_back( vtkSmartPointer< vtkImageData >::New() );
    m_Images[m_ImageIndex]->SetNumberOfScalarComponents( 1 );
    break;

  case ScalarType:
    if( ba == "float" )
      m_Images[m_ImageIndex]->SetScalarTypeToFloat();
    else if( ba == "double" )
      m_Images[m_ImageIndex]->SetScalarTypeToDouble();
    else if( ba == "int" )
      m_Images[m_ImageIndex]->SetScalarTypeToInt();
    else if( ba == "unsigned int" )
      m_Images[m_ImageIndex]->SetScalarTypeToUnsignedInt();
    else if( ba == "long" )
      m_Images[m_ImageIndex]->SetScalarTypeToLong();
    else if( ba == "unsigned long" )
      m_Images[m_ImageIndex]->SetScalarTypeToUnsignedLong();
    else if( ba == "short" )
      m_Images[m_ImageIndex]->SetScalarTypeToShort();
    else if( ba == "unsigned short" )
      m_Images[m_ImageIndex]->SetScalarTypeToUnsignedShort();
    else if( ba == "char" )
      m_Images[m_ImageIndex]->SetScalarTypeToChar();
    else if( ba == "signed char" )
      m_Images[m_ImageIndex]->SetScalarTypeToSignedChar();
    else if( ba == "unsigned char" )
      m_Images[m_ImageIndex]->SetScalarTypeToUnsignedChar();
    else
      std::cerr << "Unknown scalar type: " << ba.data() << std::endl;
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
      }
    m_Images[m_ImageIndex]->AllocateScalars();
    break;

  case ImageOrigin:
      {
      double * origin = reinterpret_cast< double * >( ba.data() );
      m_Images[m_ImageIndex]->SetOrigin( origin );
      }
    break;

  case ImageSpacing:
      {
      double * spacing = reinterpret_cast< double * >( ba.data() );
      m_Images[m_ImageIndex]->SetSpacing( spacing );
      }
    break;

  case ValueHistoryCount:
    // @todo add the image to the visualization
    m_ViewManager->newSynchronizedView( ba.data(), m_Images[m_ImageIndex] );
    m_ViewManager->Update();
    m_ViewManager->show();
    ++m_ImageIndex;
    break;

  default:
    std::cerr << "Error: Unknown content " << content << std::endl;
    }
}
