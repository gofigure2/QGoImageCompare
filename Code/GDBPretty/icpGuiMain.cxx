#include <QApplication>
#include <QtGui/QMessageBox>

#include "QGoSynchronizedViewManager.h"

#include "QImageReceiver.h"

/** This is the gui for viewing ITK images when using the gdb pretty printer.
 * This executable is intended to be launched as a process by the ICPPrinter
 * python printer, then images are sent to it via stdin.
 */

int main( int argc, char** argv )
{
  QApplication app( argc, argv );

  if( argc < 2 )
    {
    QMessageBox::critical(0, QObject::tr("icpGui"),
      QObject::tr("The first argument should be a unique identifier." ));
    QApplication::exit( 1 );
    }

  QGoSynchronizedViewManager * viewManager = new QGoSynchronizedViewManager ( );
  QImageReceiver * imageReceiver = new QImageReceiver( viewManager, argv[1] );
  (void) imageReceiver;

  viewManager->show();
  viewManager->synchronizeOpenSynchronizedViews();

  app.processEvents();
  int output = app.exec();

  delete viewManager;

  return output;
}
