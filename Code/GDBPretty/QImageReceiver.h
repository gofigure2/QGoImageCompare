#ifndef _QImageReceiver_h
#define _QImageReceiver_h

#include <QtCore/QObject>
#include <QLocalSocket>

#include "StreamingImageImporter.h"

/** @brief Receives an image from the python module with a local socket and
 * shared memory. */
class QImageReceiver: public QObject
{
  Q_OBJECT

public:
  enum DatagramContent {
    InitializationString,
    ImageSize,
    FinalizationString
    };

  QImageReceiver( QObject * parent );
  ~QImageReceiver();

private slots:
  void displayError( QLocalSocket::LocalSocketError socketError );
  void readPendingDatagram();

private:
  QLocalSocket * m_Socket;

  StreamingImageImporter * m_Importer;

  DatagramContent m_ExpectedContent;
};

#endif
