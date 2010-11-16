/*=========================================================================
  Author: $Author:$  // Author of last commit
  Version: $Rev:$  // Revision of last commit
  Date: $Date:$  // Date of last commit
=========================================================================*/

/*=========================================================================
 Authors: The GoFigure Dev. Team.
 at Megason Lab, Systems biology, Harvard Medical school, 2009-10

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

/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkImageToVTKImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2007-11-20 12:46:10 -0500 (Tue, 20 Nov 2007) $
  Version:   $Revision: 477 $

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkImageToVTKImageFilter_txx
#define _itkImageToVTKImageFilter_txx

#include "itkImageToVTKImageFilter.h"

namespace itk
{
/**
 * Constructor
 */
template< class TInputImage >
ImageToVTKImageFilter< TInputImage >
::ImageToVTKImageFilter()
{
  m_Importer = vtkImageImport::New();

  m_Exporter = ExporterFilterType::New();

  m_Importer->SetUpdateInformationCallback( m_Exporter->GetUpdateInformationCallback() );
  m_Importer->SetPipelineModifiedCallback( m_Exporter->GetPipelineModifiedCallback() );
  m_Importer->SetWholeExtentCallback( m_Exporter->GetWholeExtentCallback() );
  m_Importer->SetSpacingCallback( m_Exporter->GetSpacingCallback() );
  m_Importer->SetOriginCallback( m_Exporter->GetOriginCallback() );
  m_Importer->SetScalarTypeCallback( m_Exporter->GetScalarTypeCallback() );
  m_Importer->SetNumberOfComponentsCallback( m_Exporter->GetNumberOfComponentsCallback() );
  m_Importer->SetPropagateUpdateExtentCallback( m_Exporter->GetPropagateUpdateExtentCallback() );
  m_Importer->SetUpdateDataCallback( m_Exporter->GetUpdateDataCallback() );
  m_Importer->SetDataExtentCallback( m_Exporter->GetDataExtentCallback() );
  m_Importer->SetBufferPointerCallback( m_Exporter->GetBufferPointerCallback() );
  m_Importer->SetCallbackUserData( m_Exporter->GetCallbackUserData() );
}

/**
 * Destructor
 */
template< class TInputImage >
ImageToVTKImageFilter< TInputImage >
::~ImageToVTKImageFilter()
{
  m_Importer->Delete();
}

/**
 * Set an itk::Image as input
 */
template< class TInputImage >
void
ImageToVTKImageFilter< TInputImage >
::SetInput(const InputImageType *inputImage)
{
  m_Exporter->SetInput(inputImage);
}

/**
 * Get a vtkImage as output
 */
template< class TInputImage >
vtkImageData *
ImageToVTKImageFilter< TInputImage >
::GetOutput() const
{
  return m_Importer->GetOutput();
}

/**
 * Get the importer filter
 */
template< class TInputImage >
vtkImageImport *
ImageToVTKImageFilter< TInputImage >
::GetImporter() const
{
  return m_Importer;
}

/**
 * Get the exporter filter
 */
template< class TInputImage >
typename ImageToVTKImageFilter< TInputImage >::ExporterFilterType *
ImageToVTKImageFilter< TInputImage >
::GetExporter() const
{
  return m_Exporter.GetPointer();
}

/**
 * Delegate the Update to the importer
 */
template< class TInputImage >
void
ImageToVTKImageFilter< TInputImage >
::Update()
{
  m_Importer->Update();
}
} // end namespace itk

#endif
