#ifndef _QRemoteControl_h
#define _QRemoteControl_h

#include "QtCore/QObject"
#include "QtCore/QSocketNotifier"

/** @brief Watches stdin and displays an image when it comes in. */
class RemoteControl: public QObject
{
  Q_OBJECT

public:
  RemoteControl();

private slots:
  void receivedData();

private:
  QSocketNotifier * m_Notifier;
};

#endif
