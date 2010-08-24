#include "QRemoteControl.h"

#include <QtCore/QFile>

RemoteControl
::RemoteControl()
{
  QSocketNotifier * m_Notifier = new QSocketNotifier( fileno( stdin ),
    QSocketNotifier::Read, this );
  connect( m_Notifier, SIGNAL( activated(int) ), this, SLOT( receivedData() ));
  m_Notifier->setEnabled( true );
}

void
RemoteControl
::receivedData()
{
  m_Notifier->setEnabled( false );
  // do some stuff
  m_Notifier->setEnabled( true );
}
