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
