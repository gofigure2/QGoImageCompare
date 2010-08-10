/*=========================================================================
  Author: $Author$  // Author of last commit
  Version: $Rev$  // Revision of last commit
  Date: $Date$  // Date of last commit
=========================================================================*/

/*=========================================================================
 Authors: The GoFigure Dev. Team.
 at Megason Lab, Systems biology, Harvard Medical school, 2009-10

 Copyright (c) 2009-10, President and Fellows of Harvard College.
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

#include "SnapshotHelper.h"

#include "vtkRenderWindow.h"
#include "vtkRendererCollection.h"
#include "vtkImageData.h"

#include "vtkImageClip.h"
#include "vtkImagePermute.h"
#include "vtkImageResample.h"
#include "vtkWindowToImageFilter.h"
#include "vtkSmartPointer.h"

#include "vtkBMPWriter.h"
#include "vtkPostScriptWriter.h"
#include "vtkJPEGWriter.h"
#include "vtkPNGWriter.h"
#include "vtkTIFFWriter.h"

#include "QVTKWidget.h"

#include "vtkImageWriterHelper.h"
#include "vtkViewImage2D.h"

//-------------------------------------------------------------------------
bool BuildScreenshotFromImage(vtkImageData* image,
                              vtkImageData* screenshot,
                              int tsize)
{
  if (!image || !screenshot)
    {
    return false;
    }

  // Empty image, remove thumbnail/screenshot

  int image_dims[3];
  image->GetDimensions(image_dims);
  if (image_dims[0] == 0 ||
      image_dims[1] == 0 ||
      image_dims[2] == 0)
    {
    return false;
    }

  double        factor;
  vtkImageData *resample_input, *resample_output;

  // First, let's make sure we are processing the image as it
  // is by clipping its UpdateExtent. By doing so, we prevent our resample
  // and permute filter the process the image's *whole* extent.

  vtkSmartPointer<vtkImageClip> clip
    = vtkSmartPointer<vtkImageClip>::New();
  clip->SetInput(image);
  clip->SetOutputWholeExtent(image->GetUpdateExtent());
  clip->Update();

  // Permute, as a convenience

  int clip_dims[3];
  clip->GetOutput()->GetDimensions(clip_dims);

  vtkSmartPointer<vtkImagePermute> permute
    = vtkSmartPointer<vtkImagePermute>::New();
  if (clip_dims[2] != 1)
    {
    permute->SetInput(clip->GetOutput());
    if (clip_dims[0] == 1)
      {
      permute->SetFilteredAxes(1, 2, 0);
      }
    else
      {
      permute->SetFilteredAxes(0, 2, 1);
      }
    resample_input = permute->GetOutput();
    }
  else
    {
    resample_input = clip->GetOutput();
    }

  resample_input->Update();
  int resample_input_dims[3]; //, resample_output_dims[3];

  resample_input->GetDimensions(resample_input_dims);
  double *resample_input_spacing = resample_input->GetSpacing();

  int large_dim = 0, small_dim = 1;
  if (resample_input_dims[0] < resample_input_dims[1])
    {
    large_dim = 1;
    small_dim = 0;
    }

  if (tsize != 0)
    {
    vtkSmartPointer<vtkImageResample> resample =
      vtkSmartPointer<vtkImageResample>::New();
    resample->SetInput(resample_input);
    resample->SetInterpolationModeToCubic();
    resample->SetDimensionality(2);

    // Create the screenshot

    factor = static_cast<double>(tsize) /
             static_cast<double>(resample_input_dims[large_dim]);

    resample->SetAxisMagnificationFactor(large_dim, factor);
    resample->SetAxisMagnificationFactor(
      small_dim, factor * (resample_input_spacing[small_dim] /
                           resample_input_spacing[large_dim]));
    resample->Update();
    resample_output = resample->GetOutput();
    //   resample_output->GetDimensions(resample_output_dims);

    screenshot->ShallowCopy(resample_output);
    }
  else
    {
    screenshot->ShallowCopy(resample_input);
    }

  return true;
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
bool BuildScreenshotFromRenderWindow(
  vtkRenderWindow *win,
  vtkImageData* screenshot,
  int tsize)
{
  if (win && screenshot)
    {
    vtkSmartPointer<vtkWindowToImageFilter> filter =
      vtkSmartPointer<vtkWindowToImageFilter>::New();
    filter->ShouldRerenderOff();
    filter->SetInput(win);
    filter->Update();
    bool res = BuildScreenshotFromImage(filter->GetOutput(),
                                        screenshot, tsize);
    return res;
    }
  return false;
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
QString SnapshotView(QVTKWidget* iWidget,
                     const GoFigure::FileType& iType,
                     const QString& iBaseName,
                     const unsigned int& iSnapshotId)
{
  vtkSmartPointer<vtkImageData> image =
    vtkSmartPointer<vtkImageData>::New();
  BuildScreenshotFromRenderWindow(iWidget->GetRenderWindow(),
                                  image);
  QString filename = iBaseName;
  filename.append(QString("%1").arg(iSnapshotId));

  switch (iType)
    {
    case GoFigure::BMP:
      {
      filename.append(".bmp");
      vtkWriteImage<vtkBMPWriter>(image, filename);
      break;
      }
    case GoFigure::EPS:
      {
      filename.append(".eps");
      vtkWriteImage<vtkPostScriptWriter>(image, filename);
      break;
      }
    case GoFigure::JPEG:
      {
      filename.append(".jpeg");
      vtkWriteImage<vtkJPEGWriter>(image, filename);
      break;
      }
    case GoFigure::PNG:
      {
      filename.append(".png");
      vtkWriteImage<vtkPNGWriter>(image, filename);
      break;
      }
    case GoFigure::TIFF:
      {
      filename.append(".tiff");
      vtkWriteImage<vtkTIFFWriter>(image, filename);
      break;
      }
    default:
      {
      std::cerr << "FileType is not supported for Snapshot" << std::endl;
      return QString();
      }
    }
  return filename;
}
//-------------------------------------------------------------------------

void SetupViewGivenQVTKWidget(vtkViewImage2D* iView, QVTKWidget* iWidget)
{
  vtkRenderWindow* renwin = iWidget->GetRenderWindow();
  iView->SetRenderWindow(renwin);
  iView->SetRenderer(renwin->GetRenderers()->GetFirstRenderer());
  iView->SetupInteractor(iWidget->GetInteractor());
}
