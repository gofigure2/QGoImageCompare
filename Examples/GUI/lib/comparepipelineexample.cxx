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

#include <QApplication>
#include "vtkImageReader2Factory.h"
#include "vtkImageReader2.h"
#include "vtkSmartPointer.h"
#include "vtkImageReader2Collection.h"
#include "vtkImageGaussianSmooth.h"

#include "QGoSynchronizedViewManager.h"


/* This simple examples shows how to use the high level interface
 * of the QGoSynchronize classes.
 * It takes a list of images as an input and displays the readable ones
 * with the standard VTK reader.
 * It synchronize the visualizations.
 */

int main(int argc, char** argv)
{
  if (argc != 2)
    {
    std::cout << "Usage : QGoSynchronizedViewManagerTest(.exe) " << std::endl;
    std::cout << "1-imagefile" << std::endl;
    return EXIT_FAILURE;
    }

  QApplication app(argc, argv);
  QCoreApplication::setOrganizationName("MegasonLab");
  QCoreApplication::setOrganizationDomain("http://gofigure2.sourceforge.net");

  vtkSmartPointer< vtkImageReader2Factory > imageFactory =
    vtkSmartPointer< vtkImageReader2Factory >::New();

  vtkSmartPointer<vtkImageGaussianSmooth> filter =
    vtkSmartPointer<vtkImageGaussianSmooth>::New();


  /* we simply create a new manager that will take care of
   ∗ creation/deletion of visualization and callbacks for us.
   */
  QGoSynchronizedViewManager* ViewManager	=	new	QGoSynchronizedViewManager ();

  QString ViewName;


  vtkImageReader2 * imageReader =
    imageFactory->CreateImageReader2( argv[1] );

  if (imageReader != NULL)
    {

    // Image before filtering :
    imageReader->SetFileName( argv[1] );
    imageReader->Update();

    ViewName.clear();
    ViewName.append(argv[1]);

    /* the synchronization manager can create visualization windows given
     * a valid pointer to a VTK image and
     * a string encoding the name of the visualization.
     */
    ViewManager->newSynchronizedView(ViewName, imageReader->GetOutput() );

    // Filtered Image :
    filter->SetInputConnection(imageReader->GetOutputPort());
    filter->Update();

    ViewName.append(" - filtered");

    ViewManager->newSynchronizedView(ViewName, filter->GetOutput() );
    ViewManager->Update();
    ViewManager->show();

    }
  else
    {
    std::cerr << argv[1]
              << "is not a readable image by standard vtk reader"
              << std::endl;
    }


/* the synchronization manager can synchronize the opened images
∗ with a simple function call */
ViewManager->synchronizeOpenSynchronizedViews();

app.processEvents();

int output = app.exec();

delete ViewManager;

return output;
}
