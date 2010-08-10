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

#include "vtkViewImage3DCommand.h"

#include "vtkViewImage3D.h"
#include "vtkPolyData.h"

#include "vtkOrientedBoxWidget.h"

//----------------------------------------------------------------------------
vtkViewImage3DCommand::
vtkViewImage3DCommand()
{
}

//----------------------------------------------------------------------------
vtkViewImage3DCommand::
~vtkViewImage3DCommand()
{
}

//----------------------------------------------------------------------------
vtkViewImage3DCommand*
vtkViewImage3DCommand::New()
{
  return new vtkViewImage3DCommand;
}

//----------------------------------------------------------------------------
void
vtkViewImage3DCommand::
Execute(vtkObject *caller, unsigned long event, void *callData)
{
  (void)callData;

  if (event == vtkViewImage3DCommand::MeshPickingEvent)
    {
    m_vtkViewImage3D->UpdateCurrentActor();
    return;
    }
  if (event == vtkCommand::InteractionEvent)
    {
    vtkOrientedBoxWidget* test = static_cast<vtkOrientedBoxWidget*>(caller);
    vtkPolyData* pd = vtkPolyData::New();
    test->GetPolyData(pd);

    double pt[3];
    pd->GetPoint(0, pt);

    double bextent[6];
    bextent[0] = pt[0];
    bextent[1] = pt[0];
    bextent[2] = pt[1];
    bextent[3] = pt[1];
    bextent[4] = pt[2];
    bextent[5] = pt[2];

    // Get Extent of the box
    for (int i = 1; i < 8; i += 2)
      {
      pd->GetPoint(i, pt);

      if (pt[0] < bextent[0]) bextent[0] = pt[0];
      if (pt[0] > bextent[1]) bextent[1] = pt[0];
      if (pt[1] < bextent[2]) bextent[2] = pt[1];
      if (pt[1] > bextent[3]) bextent[3] = pt[1];
      if (pt[2] < bextent[4]) bextent[4] = pt[2];
      if (pt[2] > bextent[5]) bextent[5] = pt[2];
      }
    pd->Delete();

    //Get Actors which are in the box (picked/unpicked)
    m_vtkViewImage3D->UpdateActorsStatus(bextent);
    return;
    }
}

//----------------------------------------------------------------------------
void
vtkViewImage3DCommand::
SetVtkImageView3D(vtkViewImage3D* iViewImage3D)
{
  if (iViewImage3D)
    {
    m_vtkViewImage3D = iViewImage3D;
    }
}
