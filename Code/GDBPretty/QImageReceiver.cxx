/**
* Copyright (c) 2010, Matthew McCormick, Arnaud Gelas, and Antonin Perrot-Audet.
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* Redistributions of source code must retain the above copyright notice,
* this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright notice,
* this list of conditions and the following disclaimer in the documentation
* and/or other materials provided with the distribution.
* Neither the name of the  President and Fellows of Harvard College
* nor the names of its contributors may be used to endorse or promote
* products derived from this software without specific prior written
* permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
* THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
* PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
* BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
* OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
* OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
* OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
* ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "QImageReceiver.h"

#include <iostream>
#include <sstream>
#include <stdexcept>

#include <QtCore/QFile>
#include <QtGui/QMessageBox>

QImageReceiver
::QImageReceiver( QGoSynchronizedViewManager * manager,
  const char * pid ):
  QObject( manager ),
  m_ImageIndex( 0 ),
  m_ViewManager( manager )
{
  m_Socket = new QLocalSocket( this );
  m_ScalarMemory = new QSharedMemory( this );

  connect( m_Socket, SIGNAL(readyRead()),
    this, SLOT(readPendingDatagram()));
  connect( m_Socket, SIGNAL(error(QLocalSocket::LocalSocketError)),
    this, SLOT(displayError(QLocalSocket::LocalSocketError)));
  m_Socket->connectToServer( QString( "icpPythonModule" ) + pid );
  if( !m_Socket->waitForConnected( 5000 ) )
    this->displayError( m_Socket->error() );
  m_ScalarMemory->setKey( QString( "icpPythonModuleSharedMemory" ) + pid );
}

QImageReceiver
::~QImageReceiver()
{
  // Should never happen, but just in case.
  if( m_ScalarMemory->isAttached() )
    m_ScalarMemory->detach();
}


void
QImageReceiver
::displayError( QLocalSocket::LocalSocketError socketError )
{
  switch ( socketError )
  {
  case QLocalSocket::ServerNotFoundError:
    QMessageBox::critical(0, tr("icpGui"),
      tr("The host was not found." ));
    break;
  case QLocalSocket::ConnectionRefusedError:
    QMessageBox::critical(0, tr("icpGui"),
      tr("The connection was refused by the peer. " ));
    break;
  case QLocalSocket::PeerClosedError:
    break;
  default:
    QMessageBox::critical(0, tr("icpGui"),
      tr("The following error occurred: %1.").arg(m_Socket->errorString()));
  }
}


void
QImageReceiver
::readPendingDatagram()
{
  QByteArray ba;

  ba = m_Socket->readLine();
  if( ba == QByteArray( "syn\n" ))
    {
    if( m_Socket->write( "ack\n" ) == -1 )
      QMessageBox::critical(0, tr("icpGui"),
        tr("Could not send ack."));
    return;
    }
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
      QMessageBox::critical( 0, tr("icpGui"),
        tr( "Unknown scalar type: %1" ).arg( ba.data() ));
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
    // Copy the scalar data to the image.

    if( !m_ScalarMemory->attach() )
      QMessageBox::critical( 0, tr("icpGui"),
        tr( "Could not attach to scalar shared memory because of error %1." ).arg( m_ScalarMemory->error() ));
    m_ScalarMemory->lock();
    memcpy( m_Images[m_ImageIndex]->GetScalarPointer(), m_ScalarMemory->constData(), m_ScalarMemory->size() );
    m_ScalarMemory->unlock();
    m_ScalarMemory->detach();

    // Add the image to the view manager.
    m_ViewManager->newSynchronizedView( ba.data(), m_Images[m_ImageIndex] );
    m_ViewManager->Update();
    m_ViewManager->show();

    // Prepare for the next image.
    ++m_ImageIndex;
    break;

  default:
    QMessageBox::critical( 0, tr("icpGui"),
      tr( "Unknown content: %1" ).arg( content ));
    }
}
