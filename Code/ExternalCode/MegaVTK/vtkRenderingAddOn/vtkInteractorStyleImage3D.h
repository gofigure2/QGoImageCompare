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

#ifndef _vtk_InteractorStyleImage3D_h_
#define _vtk_InteractorStyleImage3D_h_

#include <vtkInteractorStyleTrackballCamera.h>
//#include "vtkInteractorStyleInterface.h"
#include "MegaVTK2Configure.h"

class vtkProp;

/*
 * \defgroup visualization ‘‘Visualization’’
 */

/*
 * \class vtkInteractorStyleImage3D
 * \ingroup visualization
 * \brief Define the interactor behavior withing a vtk3DImage.
 */
//MOTION FLAG
#define VTKIS_PICK3D         1050

class VTK_RENDERINGADDON2_EXPORT vtkInteractorStyleImage3D :
  public vtkInteractorStyleTrackballCamera//, public vtkInteractorStyleInterface
  {
public:
  static vtkInteractorStyleImage3D *New();
  vtkTypeRevisionMacro (vtkInteractorStyleImage3D, vtkInteractorStyleTrackballCamera);

  //BTX
  enum InteractionTypeIds
    {
    InteractionTypeWindowLevel = 0,
    InteractionTypeZoom,
    InteractionTypePan,
    InteractionTypeMeshPicking,
    InteractionTypeDefault
    };

  virtual void OnMouseMove();
  virtual void OnLeftButtonDown();
  virtual void OnLeftButtonUp();
  virtual void OnRightButtonDown();
  virtual void OnRightButtonUp();
  virtual void OnMiddleButtonDown();
  virtual void OnMiddleButtonUp();

  void     SetCurrentProp();
  vtkProp* GetCurrentProp();

  /**
   *
   */
  void StartPick();

  /**
   *
   */
  void HighlightCurrentActor();

  /**
   *
   */
  void EnablePickMode();

  /**
   *
   */
  void EnableDefaultMode();
  /**
   *
   */
  void EnableZoomMode();
  /**
   *
   */
  void EnablePanMode();

protected:
  vtkInteractorStyleImage3D();
  ~vtkInteractorStyleImage3D();

private:

  vtkInteractorStyleImage3D(const vtkInteractorStyleImage3D &);  // Not implemented.
  void operator =(const vtkInteractorStyleImage3D&);  // Not implemented.

  vtkProp* m_CurrentProp;
  unsigned int m_Mode;
  };

#endif
