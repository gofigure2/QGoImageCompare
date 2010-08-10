/*=========================================================================
  Author: $Author$  // Author of last commit
  Version: $Rev$  // Revision of last commit
  Date: $Date$  // Date of last commit
=========================================================================*/

/*=========================================================================
 Authors: The GoFigure Dev. Team.
 at Megason Lab, Systems biology, Harvard Medical school, 2009

 Copyright (c) 2009, President and Fellows of Harvard College.
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice,
 this list of conditions and the following disclaimer.
 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.
 Neither the name of the  President and Fellows of Harvard College
 nor the names of its contributors may be used to endorse or promote
 products derived from this software without specific prior written
 permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
 OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/
#include <qapplication.h>
#include <iostream>
#include <QTimer>

#include "QGoImageInfo.h"

int main(int argc, char** argv)
{
  if (argc != 2)
    {
    return EXIT_FAILURE;
    }

  QApplication app(argc, argv);
  QCoreApplication::setOrganizationName("MegasonLab");
  QCoreApplication::setOrganizationDomain("http://gofigure2.sourceforge.net");

  QGoImageInfo* info = new QGoImageInfo;

  const unsigned int Dim3 = 3;
  info->setDimension(Dim3);
  info->setNumberOfChannels(Dim3);
  std::vector<unsigned int> size3(Dim3, 512);
  std::vector<float>        spacing3(Dim3, static_cast<const float>(0.1));

  info->setSize(size3);
  info->setSpacing(spacing3);
  info->setMemory(1234567890);

  std::vector<unsigned int> ppos3(Dim3, 12);
  std::vector<float>        wpos3(Dim3, static_cast<const float>(1.2));
  info->setPixelPosition(ppos3);
  info->setWorldPosition(wpos3);

  std::vector<float> value(Dim3, 128.);
  info->setValue(value);

  info->show();

  QTimer* timer = new QTimer;
  timer->setSingleShot(true);
  QObject::connect(timer, SIGNAL(timeout()), info, SLOT(close()));

  if (atoi(argv[1]) == 1)
    {
    timer->start(500);
    }

  const unsigned int Dim4 = 4;

  info->setDimension(Dim4);
  info->setNumberOfChannels(Dim3);
  std::vector<unsigned int> size4(Dim4, 512);
  std::vector<float>        spacing4(Dim4, static_cast<const float>(0.1));

  info->setSize(size4);
  info->setSpacing(spacing4);
  info->setMemory(1234567890);

  info->setPixelPosition(ppos3);
  info->setWorldPosition(wpos3);
  info->setTimePoint(0.);
  info->setValue(value);

  info->show();

  if (atoi(argv[1]) == 1)
    {
    timer->start(500);
    }
  int output = app.exec();
  app.closeAllWindows();

  delete info;
  delete timer;

  return output;
}
