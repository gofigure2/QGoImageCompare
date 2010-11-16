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

#ifndef __vtkImageBlendWithMask_h
#define __vtkImageBlendWithMask_h

#include "vtkThreadedImageAlgorithm.h"
#include "vtkLookupTable.h"
#include "MegaVTK2Configure.h"

/**
 * \class vtkImageBlendWithMask
 * \brief TO BE FILLED
 * \ingroup MegaVTK
 */
class VTK_RENDERINGADDON2_EXPORT vtkImageBlendWithMask:
  public vtkThreadedImageAlgorithm
{
public:
    /*
     * \brief Convenient method to access the constructor
     */
  static vtkImageBlendWithMask * New();

  vtkTypeRevisionMacro (vtkImageBlendWithMask, vtkThreadedImageAlgorithm);
  void PrintSelf(ostream & os, vtkIndent indent);

  /*
   * \brief Set the LUT to map the mask
   */
  vtkSetObjectMacro (LookupTable, vtkLookupTable);

  /*
   * \brief Get the LUT to map the mask
   */
  vtkGetObjectMacro (LookupTable, vtkLookupTable);

  /*
   * \brief Set the input to be masked
   * \param[in] in vtkImageData pointer to the input image
   */
  void SetImageInput(vtkImageData *in);

  /*
   * \brief Set the mask to be used
   * \param[in] in vtkImageData pointer to the input mask
   */
  void SetMaskInput(vtkImageData *in);

   // Set the input volume 1 for this filter
  virtual void SetInput1(vtkDataObject *in) { this->SetInput(0, in); }
   // Set the input volume 2 for this filter
  virtual void SetInput2(vtkDataObject *in) { this->SetInput(1, in); }

protected:
  vtkImageBlendWithMask();
  ~vtkImageBlendWithMask();

  vtkLookupTable *LookupTable;

  virtual int RequestInformation (vtkInformation *vtkNotUsed(request),
                                  vtkInformationVector **inputVector,
                                  vtkInformationVector *outputVector);

  virtual void ThreadedRequestData(vtkInformation *vtkNotUsed(request),
                                   vtkInformationVector **vtkNotUsed(inputVector),
                                   vtkInformationVector *vtkNotUsed(outputVector),
                                   vtkImageData ***inData,
                                   vtkImageData **outData,
                                   int extent[6], int threadId);
private:
  vtkImageBlendWithMask (const vtkImageBlendWithMask &);
  void operator=(const vtkImageBlendWithMask &);
};

#endif
