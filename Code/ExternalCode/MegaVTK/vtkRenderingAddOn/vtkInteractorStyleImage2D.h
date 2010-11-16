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

#ifndef _vtk_InteractorStyleImage2D_h_
#define _vtk_InteractorStyleImage2D_h_

#include <vtkInteractorStyleImage.h>
#include "MegaVTK2Configure.h"

#define VTKIS_SLICE_MOVE  5051

/**
 * \class vtkInteractorStyleImage2D
 * \ingroup MegaVTK
 * \brief Define the interactor behavior withing a vtkImage2D.
 * 4 modes (Default, Zoom, Pan and Pick)
 */

class VTK_RENDERINGADDON2_EXPORT vtkInteractorStyleImage2D:
  public vtkInteractorStyleImage
{
public:
    /**
     * \brief Convenient method to access the constructor
     */
  static vtkInteractorStyleImage2D * New();

  vtkTypeRevisionMacro (vtkInteractorStyleImage2D, vtkInteractorStyleImage);

  //BTX
  enum InteractionTypeIds {
    InteractionTypeWindowLevel = 0,
    InteractionTypeZoom,
    InteractionTypePan,
    InteractionTypeContourPicking,
    InteractionTypeDefault
    };

  virtual void OnMouseMove();

  virtual void OnLeftButtonDown();

  virtual void OnLeftButtonUp();

  virtual void OnMiddleButtonDown();

  virtual void OnMiddleButtonUp();

  virtual void OnRightButtonDown();

  virtual void OnRightButtonUp();

  virtual void OnMouseWheelForward();

  virtual void OnMouseWheelBackward();

  virtual void OnChar();

  virtual void OnKeyDown();

  virtual void OnKeyUp();

  virtual void OnKeyPress();

  virtual void OnKeyRelease();

  virtual void StartSliceMove();

  virtual void SliceMove();

  virtual void EndSliceMove();

  virtual void DefaultMoveAction();

  // Different modes
  /**
    * \brief Start the Default Mode
    */
  void SetDefaultMode();

  /**
    * \brief Start the Zoom Mode
    */
  void SetZoomMode();

  /**
    * \brief Start the Pan Mode
    */
  void SetPanMode();

  /**
    * \brief Start the Pick Mode
    */
  void SetPickMode();

  /**
   * \brief Draw a bounding box around the actor which is pointed by the cursor
   */
  void HighlightCurrentActor();

  vtkGetMacro(SliceStep, int);
  int * GetRequestedPosition(void)
  { return this->RequestedPosition; }

  /**
   * \brief Return the actor which is pointed by the cursor
   */
  vtkProp * GetCurrentProp();

protected:
  vtkInteractorStyleImage2D();
  ~vtkInteractorStyleImage2D();

  int SliceStep;
  int *RequestedPosition;
private:

  vtkInteractorStyleImage2D(const vtkInteractorStyleImage2D &); // Not
                                                                // implemented.
  void operator=(const vtkInteractorStyleImage2D &);            // Not
                                                                // implemented.

  unsigned int m_Mode;
  bool         m_LeftButtonDown;
};

#endif
