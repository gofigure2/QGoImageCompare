#include <QApplication>

#include "QGoSynchronizedViewManager.h"

/** This is the gui for viewing ITK images when using the gdb pretty printer.
 * This executable is intended to be launched as a process by the ICPPrinter
 * python printer, then images are sent to it via stdin.
 */

int main( int argc, char** argv )
{
  QApplication app( argc, argv );

  app.processEvents();
  int output = app.exec();

  return output;
}
