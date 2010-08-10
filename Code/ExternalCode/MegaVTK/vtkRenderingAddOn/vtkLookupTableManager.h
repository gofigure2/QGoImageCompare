/*=========================================================================

Program:   vtkINRIA3D
Module:    $Id: vtkLookupTableManager.h 700 2008-02-22 16:19:25Z filus $
Language:  C++
Author:    $Author: arnaudgelas $
Date:      $Date: 2009-07-31 14:33:39 -0400 (Fri, 31 Jul 2009) $
Version:   $Revision: 490 $

Copyright (c) 2007 INRIA - Asclepios Project. All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

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

#ifndef _vtk_LookupTableManager_h_
#define _vtk_LookupTableManager_h_

#include <vector>

#include <vtkObject.h>
#include <vtkLookupTable.h>
#include "MegaVTK2Configure.h"

class VTK_RENDERINGADDON2_EXPORT vtkLookupTableManager : public vtkObject
  {

public:

  //BTX
  enum LookupTableIds
    {
    LUT_BW = 0,
    LUT_BWINV,
    LUT_SPECTTUM,
    LUT_HOTMETAL,
    LUT_GECOLORS,
    LUT_FLOW,
    LUT_LONI,
    LUT_LONI2,
    LUT_ASYMETRY,
    LUT_PVALUE,
    LUT_ROI
    };
  //ETX

  static vtkLookupTableManager* New();
  vtkTypeRevisionMacro (vtkLookupTableManager, vtkObject);

  static vtkLookupTable* GetBWLookupTable(void);

  static vtkLookupTable* GetBWInverseLookupTable(void);

  static vtkLookupTable* GetSpectrumLookupTable(void);

  static vtkLookupTable* GetHotMetalLookupTable(void);

  static vtkLookupTable* GetGEColorLookupTable(void);

  static vtkLookupTable* GetFlowLookupTable(void);

  static vtkLookupTable* GetLONILookupTable(void);

  static vtkLookupTable* GetLONI2LookupTable(void);

  static vtkLookupTable* GetAsymmetryLookupTable(void);

  static vtkLookupTable* GetPValueLookupTable(void);

  static std::vector<std::string> GetAvailableLookupTables(void);

  static vtkLookupTable* GetROILookupTable(void);

  static vtkLookupTable* GetLookupTable(const int&);

protected:
  vtkLookupTableManager();
  ~vtkLookupTableManager();

private:

  };

#endif
