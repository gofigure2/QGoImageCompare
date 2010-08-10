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

#include "vtkPolylineDecimation.h"

#include "vtkDoubleArray.h"
#include "vtkLine.h"
#include "vtkMath.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkObjectFactory.h"
#include "vtkLine.h"
#include "vtkPolyData.h"
#include "vtkPoints.h"
#include "vtkCellArray.h"
#include "vtkPointData.h"
#include "vtkCellData.h"
#include <vtkstd/list>
#include <vtkstd/vector>
#include <vtkstd/queue>

vtkCxxRevisionMacro(vtkPolylineDecimation, "$Revision: 1.1 $");
vtkStandardNewMacro(vtkPolylineDecimation);

//---------------------------------------------------------------------
// Create object with specified reduction of 90%.
vtkPolylineDecimation::vtkPolylineDecimation()
  {
  this->TargetReduction = 0.90;
  this->Closed = true;
  this->PriorityQueue = vtkPriorityQueue::New();
  }

//---------------------------------------------------------------------
vtkPolylineDecimation::~vtkPolylineDecimation()
  {
  this->PriorityQueue->Delete();
  }

double vtkPolylineDecimation::ComputeError(vtkPolyData* input,
                                           int prev, int id, int next)
{
  vtkPoints * inputPoints = input->GetPoints();

  double x1[3], x[3], x2[3];
  inputPoints->GetPoint(prev, x1);
  inputPoints->GetPoint(id, x);
  inputPoints->GetPoint(next, x2);

  if (vtkMath::Distance2BetweenPoints(x1, x2) == 0.0)
    {
    return 0.0;
    }
  else
    {
    return vtkLine::DistanceToLine(x, x1, x2);
    }
}

//---------------------------------------------------------------------
//  Reduce the number of points in a set of polylines
//
int vtkPolylineDecimation::RequestData(
  vtkInformation *vtkNotUsed(request),
  vtkInformationVector **inputVector,
  vtkInformationVector *outputVector)
{
  // get the info objects
  vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
  vtkInformation *outInfo = outputVector->GetInformationObject(0);

  // get the input and ouptut
  vtkPolyData *input = vtkPolyData::SafeDownCast(
    inInfo->Get(vtkDataObject::DATA_OBJECT()));
  vtkPolyData *output = vtkPolyData::SafeDownCast(
    outInfo->Get(vtkDataObject::DATA_OBJECT()));

  vtkCellArray *inputLines = input->GetLines();
  vtkPoints *   inputPoints = input->GetPoints();

  if (!inputLines || !inputPoints)
    {
    return 1;
    }
  vtkIdType numLines = inputLines->GetNumberOfCells();
  vtkIdType numPts = inputPoints->GetNumberOfPoints();
  if (numLines < 1 || numPts < 1)
    {
    return 1;
    }

  // Allocate memory and prepare for data processing
  vtkPoints *   newPts = vtkPoints::New();
  vtkCellArray *newLines = vtkCellArray::New();
  newLines->Allocate(numLines, 2);
  vtkPointData *inPD = input->GetPointData();
  vtkPointData *outPD = output->GetPointData();
  vtkCellData * inCD = input->GetCellData();
  vtkCellData * outCD = output->GetCellData();
  outPD->CopyAllocate(inPD);
  outCD->CopyAllocate(inCD);

  // Loop over all polylines, decimating each independently
  vtkIdType i, cellId = 0, newId;
  double    error;

  for (i = 0; i < numPts; i++)
    {
    this->VertexErrorMap[i] = 0.;
    }

  for (i = 0; i < numPts; i++)
    {
    error = ComputeError(input, GetPrev(i), i, GetNext(i));
    this->VertexErrorMap[i] = error;
    this->PriorityQueue->Insert(error, i);
    } //for all points in polyline

  // Now process structures,
  // deleting points until the decimation target is met.
  vtkIdType currentNumPts = this->PriorityQueue->GetNumberOfItems();
  while (1.0 - (static_cast<double>(currentNumPts) / static_cast<double>(
                  numPts))
         < this->TargetReduction &&
         currentNumPts > 2)
    {
    i = this->PriorityQueue->Pop();
    currentNumPts--;
    UpdateError(input, i);
    this->VertexErrorMap.erase(i);
    }

  // What's left over is now spit out as a new polyline
  newId = newLines->InsertNextCell(currentNumPts + 1);
  outCD->CopyData(inCD, cellId, newId);

  for (std::map<int, double>::iterator it = this->VertexErrorMap.begin();
       it != this->VertexErrorMap.end();
       ++it)
    {
    newId = newPts->InsertNextPoint(inputPoints->GetPoint(it->first));
    newLines->InsertCellPoint(newId);
    outPD->CopyData(inPD, it->first, newId);
    }
  if (this->Closed)
    {
    newId = newPts->InsertNextPoint(newPts->GetPoint(0));
    newLines->InsertCellPoint(0);
    outPD->CopyData(inPD, this->VertexErrorMap.begin()->first, newId);
    }

  // Clean up in preparation for the next line
  this->PriorityQueue->Reset();

// Create output and clean up
  output->SetPoints(newPts);
  output->SetLines(newLines);

  newLines->Delete();
  newPts->Delete();

  return 1;
}
//---------------------------------------------------------------------
int vtkPolylineDecimation::GetPrev(const int& iId)
{
  std::map<int, double>::iterator it = this->VertexErrorMap.find(iId);

  if (it == this->VertexErrorMap.begin())
    {
    if (this->Closed)
      {
      it = this->VertexErrorMap.end();
      it--;
      return it->first;
      }
    else
      return iId;
    }
  else
    {
    it--;
    return it->first;
    }
}
//---------------------------------------------------------------------
int vtkPolylineDecimation::GetNext(const int& iId)
{
  std::map<int, double>::iterator it = this->VertexErrorMap.find(iId);
  std::map<int, double>::iterator end_it = this->VertexErrorMap.end();
  --end_it;
  if (it == end_it)
    {
    if (this->Closed) return this->VertexErrorMap.begin()->first;
    else
      return iId;
    }
  else
    {
    it++;
    return it->first;
    }
}
//---------------------------------------------------------------------
void vtkPolylineDecimation::UpdateError(vtkPolyData* input, const int& iId)
{
  int prev = GetPrev(iId);
  int prev_prev = GetPrev(prev);
  int next = GetNext(iId);
  int next_next = GetNext(next);

  double prev_error = ComputeError(input, prev_prev, prev, next);
  this->VertexErrorMap[prev] = prev_error;
  this->PriorityQueue->DeleteId(prev);
  this->PriorityQueue->Insert(prev_error, prev);

  double next_error = ComputeError(input, prev, next, next_next);
  this->VertexErrorMap[next] = next_error;
  this->PriorityQueue->DeleteId(next);
  this->PriorityQueue->Insert(next_error, next);
}
//---------------------------------------------------------------------
void vtkPolylineDecimation::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);

  os << indent << "Target Reduction: " << this->TargetReduction << "\n";
}
