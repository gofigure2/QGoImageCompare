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

#ifndef _vtk_Image3DImagePlaneCallback_h_
#define _vtk_Image3DImagePlaneCallback_h_

#include <vtkCommand.h>
#include <vtkImageReslice.h>
#include <vtkMatrix4x4.h>
#include "MegaVTK2Configure.h"

class vtkViewImage2D;

/**
 * \class vtkImage3DImagePlaneCallback
 * \brief TO BE FILLED
 * \ingroup MegaVTK
 */
class VTK_RENDERINGADDON2_EXPORT vtkImage3DImagePlaneCallback:public vtkCommand
{
public:

  /*
   * \brief Convenient method to access the constructor
   */
  static vtkImage3DImagePlaneCallback * New()
  { return new vtkImage3DImagePlaneCallback; }

  virtual void Execute(vtkObject *caller, unsigned long, void *);

  /*
   * \brief TO BE FILLED
   * \return A pointer to the resliced vtkImageData
   */
  vtkImageData * GetOutput(void) const
  {
    return this->Reslice->GetOutput();
  }

  // Reset the input of the reslice filter
  virtual void Reset(void)
  {
    this->Reslice->SetInput (NULL);
  }

  /*
   * \brief TO BE FILLED
   * \return A pointer to the reslice filter
   */
  vtkImageReslice * GetReslice()
  {
    return this->Reslice;
  }

  /*
   * \brief Get the orientation matrix to extract the data
   * \return A pointer to a vtkMatrix4x4
   */
  vtkMatrix4x4 * GetMatrix()
  {
    return this->ResliceAxes;
  }

protected:
  vtkImage3DImagePlaneCallback()
  {
    this->Reslice     = vtkImageReslice::New();
    this->ResliceAxes = vtkMatrix4x4::New();
  }

  ~vtkImage3DImagePlaneCallback()
  {
    this->Reslice->Delete();
    this->ResliceAxes->Delete();
  }

private:
  vtkImageReslice *Reslice;
  vtkMatrix4x4 *   ResliceAxes;
};

#endif
