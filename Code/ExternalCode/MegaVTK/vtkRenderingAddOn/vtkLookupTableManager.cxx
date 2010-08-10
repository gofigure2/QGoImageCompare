/*=========================================================================

Program:   vtkINRIA3D
Module:    $Id: vtkLookupTableManager.cxx 842 2008-05-20 16:57:09Z ntoussaint $
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

#include "vtkLookupTableManager.h"
#include <vtkObjectFactory.h>

vtkCxxRevisionMacro(vtkLookupTableManager, "$Revision: 490 $");
vtkStandardNewMacro(vtkLookupTableManager);

#include "lut/Spectrum.h"
#include "lut/HotMetal.h"
#include "lut/GEColor.h"
#include "lut/Flow.h"
#include "lut/LONI.h"
#include "lut/LONI2.h"
#include "lut/Asymmetry.h"

#include <time.h>

vtkLookupTableManager::vtkLookupTableManager()
  {
  }

vtkLookupTableManager::~vtkLookupTableManager()
  {
  }

std::vector<std::string> vtkLookupTableManager::GetAvailableLookupTables()
{
  std::string lutNames[] =
         { "B/W", "B/W Inverse", "Spectrum", "HotMetal", "GEColor", "Flow", "LONI", "LONI2", "Asymmetry", "P-Value",
         "ROI"};

  std::vector<std::string> v_lutNames;
  for (int i = 0; i < 10; i++)
    {
    v_lutNames.push_back(lutNames[i]);
    }

  return v_lutNames;
}

vtkLookupTable* vtkLookupTableManager::GetLookupTable(const int& n)
{
  vtkLookupTable* lut = 0;
  switch (n)
    {
    case LUT_BW:
      lut = vtkLookupTableManager::GetBWLookupTable();
      break;

    case LUT_BWINV:
      lut = vtkLookupTableManager::GetBWInverseLookupTable();
      break;

    case LUT_SPECTTUM:
      lut = vtkLookupTableManager::GetSpectrumLookupTable();
      break;

    case LUT_HOTMETAL:
      lut = vtkLookupTableManager::GetHotMetalLookupTable();
      break;

    case LUT_GECOLORS:
      lut = vtkLookupTableManager::GetGEColorLookupTable();
      break;

    case LUT_FLOW:
      lut = vtkLookupTableManager::GetFlowLookupTable();
      break;

    case LUT_LONI:
      lut = vtkLookupTableManager::GetLONILookupTable();
      break;

    case LUT_LONI2:
      lut = vtkLookupTableManager::GetLONI2LookupTable();
      break;

    case LUT_ASYMETRY:
      lut = vtkLookupTableManager::GetAsymmetryLookupTable();
      break;

    case LUT_PVALUE:
      lut = vtkLookupTableManager::GetPValueLookupTable();
      break;

    case LUT_ROI:
      lut = vtkLookupTableManager::GetROILookupTable();
      break;

    default:
      break;
    }

  return lut;
}

vtkLookupTable* vtkLookupTableManager::GetBWLookupTable()
{
  vtkLookupTable* bwLut = vtkLookupTable::New();
  bwLut->SetTableRange (0, 1);
  bwLut->SetSaturationRange (0, 0);
  bwLut->SetHueRange (0, 0);
  bwLut->SetValueRange (0, 1);
  bwLut->Build();

  return bwLut;
}

vtkLookupTable* vtkLookupTableManager::GetBWInverseLookupTable()
{
  vtkLookupTable* bwLut = vtkLookupTable::New();
  bwLut->SetTableRange (0, 1);
  bwLut->SetSaturationRange (0, 0);
  bwLut->SetHueRange (0, 0);
  bwLut->SetValueRange (1, 0);
  bwLut->Build();

  return bwLut;
}

vtkLookupTable* vtkLookupTableManager::GetSpectrumLookupTable()
{
  vtkLookupTable* lut = vtkLookupTable::New();
  lut->SetNumberOfTableValues(256);
  lut->Build();

  for (int i = 0; i < 256; i++)
    {
    lut->SetTableValue(i, (double) Spectrum[i] / 255.0, (double) Spectrum[256 + i] / 255.0,
                       (double) Spectrum[256 * 2 + i] / 255.0, 1.0);
    }

  return lut;
}

vtkLookupTable* vtkLookupTableManager::GetHotMetalLookupTable()
{
  vtkLookupTable* lut = vtkLookupTable::New();
  lut->SetNumberOfTableValues(256);
  lut->Build();

  for (int i = 0; i < 256; i++)
    {
    lut->SetTableValue(i, (double) HotMetal[i] / 255.0, (double) HotMetal[256 + i] / 255.0,
                       (double) HotMetal[256 * 2 + i] / 255.0, 1.0);
    }

  return lut;
}

vtkLookupTable* vtkLookupTableManager::GetGEColorLookupTable()
{
  vtkLookupTable* lut = vtkLookupTable::New();
  lut->SetNumberOfTableValues(256);
  lut->Build();

  for (int i = 0; i < 256; i++)
    {
    lut->SetTableValue(i,
                       (double) GEColor[i] / 255.0,
                       (double) GEColor[256 + i] / 255.0,
                       (double) GEColor[256 * 2 + i] / 255.0,
                       1.0);
    }

  return lut;
}

vtkLookupTable* vtkLookupTableManager::GetFlowLookupTable()
{
  vtkLookupTable* lut = vtkLookupTable::New();
  lut->SetNumberOfTableValues(256);
  lut->Build();

  for (int i = 0; i < 256; i++)
    {
    lut->SetTableValue(i,
                       (double) Flow[i] / 255.0,
                       (double) Flow[256 + i] / 255.0,
                       (double) Flow[256 * 2 + i] / 255.0,
                       1.0);
    }

  return lut;
}

vtkLookupTable* vtkLookupTableManager::GetLONILookupTable()
{
  vtkLookupTable* lut = vtkLookupTable::New();
  lut->SetNumberOfTableValues(203);
  lut->Build();

  for (int i = 0; i < 203; i++)
    {
    lut->SetTableValue(i, (double) LONI[i][0], (double) LONI[i][1], (double) LONI[i][2], 1.0);
    }

  return lut;
}

vtkLookupTable* vtkLookupTableManager::GetLONI2LookupTable()
{
  vtkLookupTable* lut = vtkLookupTable::New();
  lut->SetNumberOfTableValues(120);
  lut->Build();

  for (int i = 0; i < 120; i++)
    {
    lut->SetTableValue(i, (double) LONI2[i][0], (double) LONI2[i][1], (double) LONI2[i][2], 1.0);
    }

  return lut;
}

vtkLookupTable* vtkLookupTableManager::GetAsymmetryLookupTable()
{
  vtkLookupTable* lut = vtkLookupTable::New();
  lut->SetNumberOfTableValues(256);
  lut->Build();

  for (int i = 0; i < 256; i++)
    {
    lut->SetTableValue(i, (double) AsymmetryLUT[i][0], (double) AsymmetryLUT[i][1], (double) AsymmetryLUT[i][2], 1.0);
    }

  return lut;
}

vtkLookupTable* vtkLookupTableManager::GetPValueLookupTable()
{
  vtkLookupTable* lut = vtkLookupTable::New();
  lut->SetTableRange (0, 1);
  lut->SetSaturationRange (1.0, 1.5);
  lut->SetHueRange (0.666, 0.8333);
  //lut->SetHueRange (0.666, 0.0);
  lut->SetValueRange (0.5, 1.0);
  lut->Build();

  return lut;
}

vtkLookupTable* vtkLookupTableManager::GetROILookupTable()
{
  vtkLookupTable* lut = vtkLookupTable::New();
  lut->SetNumberOfTableValues (256);
  lut->SetTableValue (0, 0.0, 0.0, 0.0, 0.0); // erase color

  lut->SetTableValue (1, 1.0, 0.0, 0.0, 0.5);
  lut->SetTableValue (2, 0.0, 1.0, 0.0, 0.5);
  lut->SetTableValue (3, 0.0, 0.0, 1.0, 0.5);
  lut->SetTableValue (4, 1.0, 1.0, 0.0, 0.5);
  lut->SetTableValue (5, 0.0, 1.0, 1.0, 0.5);
  lut->SetTableValue (6, 1.0, 0.0, 1.0, 0.5);

  lut->SetTableValue (7, 1.0, 0.5, 0.0, 0.5);
  lut->SetTableValue (8, 0.0, 1.0, 0.5, 0.5);
  lut->SetTableValue (9, 0.5, 0.0, 1.0, 0.5);
  lut->SetTableValue (10, 1.0, 1.0, 0.5, 0.5);
  lut->SetTableValue (11, 0.5, 1.0, 1.0, 0.5);
  lut->SetTableValue (12, 1.0, 0.5, 1.0, 0.5);

  // Fill the rest of the labels with color ramps, code taken from SNAP
//   for (int i = 13; i < 256; i++)
//   {
//     if (i < 85)
//     {
//       lut->SetTableValue (i, ((84.0-i)/85.0 * 200.0 + 50.0)/255.0, (i/85.0 * 200.0 + 50.0)/255.0, 0, 0.5);
//     }
//     else if (i < 170)
//     {
//       lut->SetTableValue (i, 0, ((169.0-i)/85.0 * 200.0 + 50)/255.0, ((i-85)/85.0 * 200.0 + 50)/255.0, 0.5);
//     }
//     else
//     {
//       lut->SetTableValue (i, ((i-170)/85.0 * 200.0 + 50)/255.0, 0.0, ((255.0-i)/85.0 * 200.0 + 50)/255.0, 0.5);
//     }
//   }

  // Fill the rest with random colors

//   for (int i = 13; i < 256; i++)
//   {
//     srand (clock());

//     // put a random color
//     int i1 = 1 + (int) (100.0 * ((double)rand() / (RAND_MAX + 1.0)));
//     int i2 = 1 + (int) (100.0 * ((double)rand() / (RAND_MAX + 1.0)));
//     int i3 = 1 + (int) (100.0 * ((double)rand() / (RAND_MAX + 1.0)));
//     double rand_1 = (double)(i1)/(100.0);
//     double rand_2 = (double)(i2)/(100.0);
//     double rand_3 = (double)(i3)/(100.0);
//     double r = rand_1, g = rand_2, b = rand_3;
//     if (i1 < 33)
//       r = 1;
//     else if (i1 < 66)
//       g = 1;
//     else if (i1 < 100)
//       b = 1;

//     lut->SetTableValue (i, r, g, b, 0.5);
//   }

  for (int i = 12; i < 256; i++)
    {
    if (i % 12 == 0) lut->SetTableValue (i, 1.0, 0.0, 0.0, 0.5);
    else if (i % 12 == 1)
      lut->SetTableValue (i, 0.0, 1.0, 0.0, 0.5);
    else if (i % 12 == 2)
      lut->SetTableValue (i, 0.0, 0.0, 1.0, 0.5);
    else if (i % 12 == 3)
      lut->SetTableValue (i, 1.0, 1.0, 0.0, 0.5);
    else if (i % 12 == 4)
      lut->SetTableValue (i, 0.0, 1.0, 1.0, 0.5);
    else if (i % 12 == 5)
      lut->SetTableValue (i, 1.0, 0.0, 1.0, 0.5);
    else if (i % 12 == 6)
      lut->SetTableValue (i, 1.0, 0.5, 0.0, 0.5);
    else if (i % 12 == 7)
      lut->SetTableValue (i, 0.0, 1.0, 0.5, 0.5);
    else if (i % 12 == 8)
      lut->SetTableValue (i, 0.5, 0.0, 1.0, 0.5);
    else if (i % 12 == 9)
      lut->SetTableValue (i, 1.0, 1.0, 0.5, 0.5);
    else if (i % 12 == 10)
      lut->SetTableValue (i, 0.5, 1.0, 1.0, 0.5);
    else if (i % 12 == 11)
      lut->SetTableValue (i, 1.0, 0.5, 1.0, 0.5);
    }

  return lut;

}
