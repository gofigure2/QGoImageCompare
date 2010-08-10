/*========================================================================
 Copyright (c) INRIA - ASCLEPIOS Project (http://www-sop.inria.fr/asclepios).
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice,
 this list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.

 * Neither the name of INRIA or ASCLEPIOS, nor the names of any contributors
 may be used to endorse or promote products derived from this software
 without specific prior written permission.

 * Modified source versions must be plainly marked as such, and must not be
 misrepresented as being the original software.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS ``AS IS''
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
 ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 =========================================================================*/

/*=========================================================================
 Modifications were made by the GoFigure Dev. Team.
 while at Megason Lab, Systems biology, Harvard Medical school, 2009-10

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

#include "vtkImage3DCroppingBoxCallback.h"

#include "vtkSmartPointer.h"
#include "vtkBoxWidget.h"
#include "vtkPolyData.h"

void vtkImage3DCroppingBoxCallback::Execute(vtkObject *caller,
                                            unsigned long, void*)
{

  if (!this->VolumeMapper)
    {
    return;
    }

  // get the box widget
  vtkBoxWidget *widget = reinterpret_cast<vtkBoxWidget*>(caller);

  if (!widget)
    {
    return;
    }

  // Get the poly data defined by the vtkBoxWidget
  vtkSmartPointer<vtkPolyData> myBox = vtkSmartPointer<vtkPolyData>::New();
  widget->GetPolyData(myBox);

  double bounds[6] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
  this->VolumeMapper->GetBounds(bounds);

  // myBox contains 15 points and points 8 to 13
  // define the bounding box
  double  xmin, xmax, ymin, ymax, zmin, zmax;
  double* pt = myBox->GetPoint(8);
  xmin = pt[0];
  pt = myBox->GetPoint(9);
  xmax = pt[0];
  pt = myBox->GetPoint(10);
  ymin = pt[1];
  pt = myBox->GetPoint(11);
  ymax = pt[1];
  pt = myBox->GetPoint(12);
  zmin = pt[2];
  pt = myBox->GetPoint(13);
  zmax = pt[2];

  if (xmin < bounds[0]) xmin = bounds[0];
  if (ymin < bounds[2]) ymin = bounds[2];
  if (zmin < bounds[4]) zmin = bounds[4];
  if (xmax > bounds[1]) xmax = bounds[1];
  if (ymax > bounds[3]) ymax = bounds[3];
  if (zmax > bounds[5]) zmax = bounds[5];

  this->VolumeMapper->SetCroppingRegionFlagsToSubVolume();
  this->VolumeMapper->SetCroppingRegionPlanes(xmin, xmax,
                                              ymin, ymax,
                                              zmin, zmax);
}
