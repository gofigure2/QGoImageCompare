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

#include "vtkFillImageWithPolyData.h"

#include "vtkImageData.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkObjectFactory.h"
#include "vtkStreamingDemandDrivenPipeline.h"
#include "vtkPoints.h"

#include "vtkMath.h"
// extern int vtkrint(double a);

vtkCxxRevisionMacro (vtkFillImageWithPolyData, "$Revision: 490 $");
vtkStandardNewMacro (vtkFillImageWithPolyData);

vtkFillImageWithPolyData::vtkFillImageWithPolyData()
  {
  this->PolyData = 0;
  this->InsidePixelValue = 1.0;
  this->ExtractionDirection = 0;
  }

vtkFillImageWithPolyData::~vtkFillImageWithPolyData()
  {
  if (this->PolyData) this->PolyData->Delete();
  }

//----------------------------------------------------------------------------
// The output extent is the intersection.
int vtkFillImageWithPolyData::RequestInformation(
  vtkInformation * vtkNotUsed(request),
  vtkInformationVector **inputVector,
  vtkInformationVector *outputVector)
{
  (void) inputVector;

  // get the info objects
  vtkInformation* outInfo = outputVector->GetInformationObject(0);

  // copy the polydata bounding box into bbox
  if (this->PolyData) this->PolyData->GetBounds (BBox);

  vtkDataObject::SetPointDataActiveScalarInfo(outInfo, VTK_UNSIGNED_CHAR, 1);

  return 1;
}

void vtkFillImageWithPolyData::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
  if (this->PolyData)
    {
    os << indent << "PolyData: \n";
    os << indent << *PolyData << endl;
    }
}

//----------------------------------------------------------------------------
// This templated function executes the filter for any type of data.
template <class T>
void vtkFillImageWithPolyDataExecuteZ(vtkFillImageWithPolyData *self,
                                      int ext[6],
                                      vtkImageData *inData,
                                      T *in1Ptr,
                                      vtkImageData *outData,
                                      unsigned char *outPtr,
                                      int id, int slice, double bbox[6])
{
  int           num0, num1, num2, numC, pixSize;
  int           idx0, idx1, idx2;
  vtkIdType     in1Inc0, in1Inc1, in1Inc2;
  vtkIdType     outInc0, outInc1, outInc2;
  unsigned long count = 0;
  unsigned long target;

  numC = outData->GetNumberOfScalarComponents();
  pixSize = numC * static_cast<int>(sizeof(T));

  // Get information to march through data
  inData->GetContinuousIncrements(ext, in1Inc0, in1Inc1, in1Inc2);
  outData->GetContinuousIncrements(ext, outInc0, outInc1, outInc2);
  num0 = ext[1] - ext[0] + 1;
  num1 = ext[3] - ext[2] + 1;
  num2 = ext[5] - ext[4] + 1;

  double outVal = self->GetInsidePixelValue();

  vtkPoints* points = self->GetPolyData()->GetPoints();
  vtkIdType  numP = self->GetPolyData()->GetNumberOfPoints();

  double* spacing = inData->GetSpacing();
  double* origin  = inData->GetOrigin();

  target = (unsigned long)(num2 * num1 / 50.0);
  target++;

  // Loop through ouput pixels
  for (idx2 = ext[4]; idx2 < ext[4] + num2; ++idx2)
    {

    for (idx1 = ext[2]; !self->AbortExecute && idx1 < ext[2] + num1; ++idx1)
      {
      if (!id)
        {
        if (!(count % target)) self->UpdateProgress(static_cast<double>(count) / (50.0 * static_cast<double>(target)));
        count++;
        }

      for (idx0 = ext[0]; idx0 < ext[0] + num0; ++idx0)
        {

        double val = 0.0;
        if (idx2 == slice)
          {
          double x = idx0 * spacing[0] + origin[0];
          double y = idx1 * spacing[1] + origin[1];

          if (x >= bbox[0] && x <= bbox[1] && y >= bbox[2] && y <= bbox[3])
            {
            double angle = 0;

            for (int i = 0; i < numP; i++)
              {

              double p1[3], p2[3], dp1[2], dp2[2];
              points->GetPoint (i, p1);
              points->GetPoint ((i + 1) % numP, p2);

              dp1[0] = p1[0] - x;
              dp1[1] = p1[1] - y;

              dp2[0] = p2[0] - x;
              dp2[1] = p2[1] - y;

              angle += self->Angle2D (dp1, dp2);

              }
            if (fabs (angle) >= vtkMath::Pi()) val = outVal;
            }
          }

        *outPtr = (unsigned char)(val);
        ++outPtr;
        in1Ptr += in1Inc0;
        }
      in1Ptr += in1Inc1;
      outPtr += outInc1;
      }
    in1Ptr += in1Inc2;
    outPtr += outInc2;
    }

}

//----------------------------------------------------------------------------
// This templated function executes the filter for any type of data.
template <class T>
void vtkFillImageWithPolyDataExecuteY(vtkFillImageWithPolyData *self, int ext[6],
                                      vtkImageData *inData, T *in1Ptr,
                                      vtkImageData *outData, unsigned char *outPtr,
                                      int id, int slice, double bbox[6])
{
  int           num0, num1, num2, numC;
  int           idx0, idx1, idx2;
  vtkIdType     in1Inc0, in1Inc1, in1Inc2;
  vtkIdType     outInc0, outInc1, outInc2;
  unsigned long count = 0;
  unsigned long target;

  numC = outData->GetNumberOfScalarComponents();
//   int pixSize = numC * static_cast<int>(sizeof(T));

  // Get information to march through data
  inData->GetContinuousIncrements(ext, in1Inc0, in1Inc1, in1Inc2);
  outData->GetContinuousIncrements(ext, outInc0, outInc1, outInc2);
  num0 = ext[1] - ext[0] + 1;
  num1 = ext[3] - ext[2] + 1;
  num2 = ext[5] - ext[4] + 1;

  vtkPoints* points = self->GetPolyData()->GetPoints();
  vtkIdType  numP = self->GetPolyData()->GetNumberOfPoints();

  double* spacing = inData->GetSpacing();
  double* origin  = inData->GetOrigin();

  target = (unsigned long)(num2 * num1 / 50.0);
  target++;

  // Loop through ouput pixels
  for (idx2 = ext[4]; idx2 < ext[4] + num2; ++idx2)
    {
    for (idx1 = ext[2]; !self->AbortExecute && idx1 < ext[2] + num1; ++idx1)
      {
      if (!id)
        {
        if (!(count % target)) self->UpdateProgress(static_cast<double>(count) / (50.0 * static_cast<double>(target)));
        count++;
        }

      for (idx0 = ext[0]; idx0 < ext[0] + num0; ++idx0)
        {

        double val = 0.0;

        if (idx1 == slice)
          {
          double angle = 0;

          double x = idx0 * spacing[0] + origin[0];
          double y = idx2 * spacing[2] + origin[2];

          if (x >= bbox[0] && x <= bbox[1] && y >= bbox[4] && y <= bbox[5])
            {

            for (int i = 0; i < numP; i++)
              {

              double p1[3], p2[3], dp1[2], dp2[2];
              points->GetPoint (i, p1);
              points->GetPoint ((i + 1) % numP, p2);

              dp1[0] = p1[0] - x;
              dp1[1] = p1[2] - y;

              dp2[0] = p2[0] - x;
              dp2[1] = p2[2] - y;

              angle += self->Angle2D (dp1, dp2);
              }

            if (fabs (angle) >= vtkMath::Pi()) val = self->GetInsidePixelValue();
            }
          }

        *outPtr = (unsigned char)(val);
        ++outPtr;
        in1Ptr += in1Inc0;
        }
      in1Ptr += in1Inc1;
      outPtr += outInc1;
      }
    in1Ptr += in1Inc2;
    outPtr += outInc2;
    }

}

//----------------------------------------------------------------------------
// This templated function executes the filter for any type of data.
template <class T>
void vtkFillImageWithPolyDataExecuteX(vtkFillImageWithPolyData *self, int ext[6],
                                      vtkImageData *inData, T *in1Ptr,
                                      vtkImageData *outData, unsigned char *outPtr,
                                      int id, int slice, double bbox[6])
{
  int           num0, num1, num2, numC, pixSize;
  int           idx0, idx1, idx2;
  vtkIdType     in1Inc0, in1Inc1, in1Inc2;
  vtkIdType     outInc0, outInc1, outInc2;
  unsigned long count = 0;
  unsigned long target;

  numC = outData->GetNumberOfScalarComponents();
  pixSize = numC * static_cast<int>(sizeof(T));

  // Get information to march through data
  inData->GetContinuousIncrements(ext, in1Inc0, in1Inc1, in1Inc2);
  outData->GetContinuousIncrements(ext, outInc0, outInc1, outInc2);
  num0 = ext[1] - ext[0] + 1;
  num1 = ext[3] - ext[2] + 1;
  num2 = ext[5] - ext[4] + 1;

  //get the polydata bounding box
  //double bbox[6];
  //self->GetPolyData()->GetBounds (bbox);

  vtkPoints* points = self->GetPolyData()->GetPoints();
  vtkIdType  numP = self->GetPolyData()->GetNumberOfPoints();

  double* spacing = inData->GetSpacing();
  double* origin  = inData->GetOrigin();

  target = (unsigned long)(num2 * num1 / 50.0);
  target++;

  // Loop through ouput pixels
  for (idx2 = ext[4]; idx2 < ext[4] + num2; ++idx2)
    {
    for (idx1 = ext[2]; !self->AbortExecute && idx1 < ext[2] + num1; ++idx1)
      {
      if (!id)
        {
        if (!(count % target)) self->UpdateProgress(static_cast<double>(count) / (50.0 * static_cast<double>(target)));
        count++;
        }

      for (idx0 = ext[0]; idx0 < ext[0] + num0; ++idx0)
        {

        double val = 0.0;

        if (idx0 == slice)
          {
          double angle = 0;

          double x = idx1 * spacing[1] + origin[1];
          double y = idx2 * spacing[2] + origin[2];

          if (x >= bbox[2] && x <= bbox[3] && y >= bbox[4] && y <= bbox[5])
            {

            for (int i = 0; i < numP; i++)
              {

              double p1[3], p2[3], dp1[2], dp2[2];
              points->GetPoint (i, p1);
              points->GetPoint ((i + 1) % numP, p2);

              dp1[0] = p1[1] - x;
              dp1[1] = p1[2] - y;

              dp2[0] = p2[1] - x;
              dp2[1] = p2[2] - y;

              angle += self->Angle2D (dp1, dp2);
              }

            if (fabs (angle) >= vtkMath::Pi()) val = self->GetInsidePixelValue();
            }
          }

        *outPtr = (unsigned char)(val);
        ++outPtr;
        in1Ptr += in1Inc0;
        }
      in1Ptr += in1Inc1;
      outPtr += outInc1;
      }
    in1Ptr += in1Inc2;
    outPtr += outInc2;
    }

}

//----------------------------------------------------------------------------
// This method is passed a input and output Datas, and executes the filter
// algorithm to fill the output from the inputs.
// It just executes a switch statement to call the correct function for
// the Datas data types.
void vtkFillImageWithPolyData::ThreadedRequestData(
  vtkInformation * vtkNotUsed(request),
  vtkInformationVector ** vtkNotUsed(inputVector),
  vtkInformationVector * vtkNotUsed(outputVector),
  vtkImageData ***inData,
  vtkImageData **outData,
  int outExt[6], int id)
{
  void *inPtr1;
  void *outPtr;

  if (!PolyData)
    {
    vtkErrorMacro("PolyData not set");
    return;
    }

  if (PolyData->GetNumberOfPoints() < 3)
    {
    vtkErrorMacro("PolyData must have more than 2 points.");
    return;
    }

  // check for the orientation and slice to draw the ROI
  int     slice = 0;
  double* spacing = inData[0][0]->GetSpacing();
  double* origin  = inData[0][0]->GetOrigin();

  vtkPoints* points = PolyData->GetPoints();
  double     pt[3];
  points->GetPoint (0, pt);

  if (ExtractionDirection > 2 || ExtractionDirection < 0)
    {
    vtkErrorMacro("Dont know extraction direction.");
    }
  else
    {
//     slice = int( vtkrint ( (pt[0]-origin[0])/spacing[0] ));
    slice = static_cast<int>(
      (pt[ExtractionDirection] - origin[ExtractionDirection])
      / spacing[ExtractionDirection]);
    }

  inPtr1 = inData[0][0]->GetScalarPointerForExtent(outExt);
  outPtr = outData[0]->GetScalarPointerForExtent(outExt);

  if (ExtractionDirection == 2)
    {
    switch (inData[0][0]->GetScalarType())
      {
      vtkTemplateMacro(vtkFillImageWithPolyDataExecuteZ(this, outExt,
                                                        inData[0][0], (VTK_TT *)(inPtr1),
                                                        outData[0], (unsigned char *)(outPtr), id, slice, BBox));
      default:
        vtkErrorMacro(<< "Execute: Unknown ScalarType");
        return;
      }
    }
  else
    {
    if (ExtractionDirection == 1)
      {
      switch (inData[0][0]->GetScalarType())
        {

        vtkTemplateMacro(vtkFillImageWithPolyDataExecuteY(this, outExt,
                                                          inData[0][0], (VTK_TT *)(inPtr1),
                                                          outData[0], (unsigned char *)(outPtr), id, slice, BBox));
        default:
          vtkErrorMacro(<< "Execute: Unknown ScalarType");
          return;
        }

      }
    else
      {
      switch (inData[0][0]->GetScalarType())
        {
        vtkTemplateMacro(vtkFillImageWithPolyDataExecuteX(this, outExt,
                                                          inData[0][0], (VTK_TT *)(inPtr1),
                                                          outData[0], (unsigned char *)(outPtr), id, slice, BBox));
        default:
          vtkErrorMacro(<< "Execute: Unknown ScalarType");
          return;
        }

      }

    }
}

double vtkFillImageWithPolyData::Angle2D(const double dp1[2], const double dp2[2])
{
  double dt, t1, t2;
  t1 = atan2 (dp1[0], dp1[1]);
  t2 = atan2 (dp2[0], dp2[1]);

  dt = t2 - t1;

  while (dt > vtkMath::Pi())
    dt -= 2. * static_cast<double>(vtkMath::Pi());
  while (dt < -vtkMath::Pi())
    dt += 2. * static_cast<double>(vtkMath::Pi());

  return dt;
}
