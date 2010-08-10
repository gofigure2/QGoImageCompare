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

#ifndef __vtkPolylineDecimation_h
#define __vtkPolylineDecimation_h

#include <map>
#include <vtkPriorityQueue.h>
#include <vtkPolyDataAlgorithm.h>

#include "MegaVTK2Configure.h"

class VTK_RENDERINGADDON2_EXPORT vtkPolylineDecimation : public vtkPolyDataAlgorithm
  {
public:
  // Description:
  // Standard methods for type information and printing.
  vtkTypeRevisionMacro(vtkPolylineDecimation, vtkPolyDataAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Instantiate this object with a target reduction of 0.90.
  static vtkPolylineDecimation *New();

  // Description:
  // Specify the desired reduction in the total number of polygons (e.g., if
  // TargetReduction is set to 0.9, this filter will try to reduce the data set
  // to 10% of its original size).
  vtkSetClampMacro(TargetReduction, double, 0.0, 1.0);
  vtkGetMacro(TargetReduction, double);

protected:
  vtkPolylineDecimation();
  ~vtkPolylineDecimation();

  int RequestData(vtkInformation *,
                  vtkInformationVector **,
                  vtkInformationVector *);

  double ComputeError(vtkPolyData* input, int prev, int id, int next);
  void UpdateError(vtkPolyData* input, const int& iId);

  int GetPrev(const int& iId);
  int GetNext(const int& iId);

  bool                  Closed;
  double                TargetReduction;
  std::map<int, double> VertexErrorMap;
  vtkPriorityQueue*     PriorityQueue;

private:
  vtkPolylineDecimation(const vtkPolylineDecimation&);      // Not implemented.
  void operator =(const vtkPolylineDecimation&);     // Not implemented.
  };
#endif
