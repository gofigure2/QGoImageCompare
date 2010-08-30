#ifndef _QImageReceiver_h
#define _QImageReceiver_h

#include "vtkSmartPointer.h"
#include "vtkImageData.h"

#include <QtCore/QObject>
#include <QLocalSocket>

#include <vector>

#include "QGoSynchronizedViewManager.h"

/** @brief Receives an image from the python module with a local socket and
 * shared memory. */
class QImageReceiver: public QObject
{
  Q_OBJECT

public:
  enum DatagramContent {
    InitializationString,
    ScalarType,
    ImageSize,
    ImageOrigin,
    ImageSpacing,
    ValueHistoryCount
    };

  QImageReceiver( QGoSynchronizedViewManager * manager );
  ~QImageReceiver();

private slots:
  void displayError( QLocalSocket::LocalSocketError socketError );
  void readPendingDatagram();

private:
  void applyContent( DatagramContent content, QByteArray & ba );
  QLocalSocket * m_Socket;

  std::vector< vtkSmartPointer< vtkImageData > > m_Images;
  size_t m_ImageIndex;

  QGoSynchronizedViewManager * m_ViewManager;
};

#endif
