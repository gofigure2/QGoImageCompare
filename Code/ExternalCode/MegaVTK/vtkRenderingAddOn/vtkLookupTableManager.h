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

/**
 \class vtkLookupTableManager
 \ingroup MegaVTK
 \brief Define and manage all the lookup tables available in GoFigure2
 */
class VTK_RENDERINGADDON2_EXPORT vtkLookupTableManager:public vtkObject
{
public:


  /*
   * \brief Name of all the lookup tables
   */
  //BTX
  enum LookupTableIds {
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

  /**
   * \brief Convenient method to access the constructor
   */
  static vtkLookupTableManager * New();

  vtkTypeRevisionMacro (vtkLookupTableManager, vtkObject);

  /**
   * \brief Get the BW lookup table
   * \return vtkLookupTable pointer to the BW lookup table
   */
  static vtkLookupTable * GetBWLookupTable(void);

  /**
   * \brief Get the BWInverse lookup table
   * \return vtkLookupTable pointer to the BWInverse lookup table
   */
  static vtkLookupTable * GetBWInverseLookupTable(void);

  /**
   * \brief Get the Spectrum lookup table
   * \return vtkLookupTable pointer to the Spectrum lookup table
   */
  static vtkLookupTable * GetSpectrumLookupTable(void);

  /**
   * \brief Get the Hot Metal lookup table
   * \return vtkLookupTable pointer to the Hot Metal lookup table
   */
  static vtkLookupTable * GetHotMetalLookupTable(void);

  /**
   * \brief Get the GE Color lookup table
   * \return vtkLookupTable pointer to the GE Color lookup table
   */
  static vtkLookupTable * GetGEColorLookupTable(void);

  /**
   * \brief Get the Spectrum lookup table
   * \return vtkLookupTable pointer to the Spectrum lookup table
   */
  static vtkLookupTable * GetFlowLookupTable(void);

  /**
   * \brief Get the LONI lookup table
   * \return vtkLookupTable pointer to the LONI lookup table
   */
  static vtkLookupTable * GetLONILookupTable(void);

  /**
   * \brief Get the LONI2 lookup table
   * \return vtkLookupTable pointer to the LONI2 lookup table
   */
  static vtkLookupTable * GetLONI2LookupTable(void);

  /**
   * \brief Get the Asymmetry lookup table
   * \return vtkLookupTable pointer to the Asymmetry lookup table
   */
  static vtkLookupTable * GetAsymmetryLookupTable(void);

  /**
   * \brief Get the PValue lookup table
   * \return vtkLookupTable pointer to the PValue lookup table
   */
  static vtkLookupTable * GetPValueLookupTable(void);

  /**
   * \brief Get the available lookup table
   * \return Vector of strings where the strings are the name of the available
   * lookup tables
   */
  static std::vector< std::string > GetAvailableLookupTables(void);

  /**
   * \brief Get the ROI lookup table
   * \return vtkLookupTable pointer to the ROI lookup table
   */
  static vtkLookupTable * GetROILookupTable(void);

  /**
   * \brief Get the indexed lookup table
   * \param[in] iIndex of the lookup table
   * \return vtkLookupTable pointer to the indexed lookup table
   */
  static vtkLookupTable * GetLookupTable(const int & iIndex);

protected:
  vtkLookupTableManager();
  ~vtkLookupTableManager();
private:
};

#endif
