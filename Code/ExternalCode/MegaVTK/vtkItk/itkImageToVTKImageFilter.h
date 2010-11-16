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
  Module:    $RCSfile: itkImageToVTKImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2007-11-20 12:46:10 -0500 (Tue, 20 Nov 2007) $
  Version:   $Revision: 477 $

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkImageToVTKImageFilter_h
#define __itkImageToVTKImageFilter_h

#include "itkVTKImageExport.h"
#include "vtkImageImport.h"
#include "vtkImageData.h"

namespace itk
{
/** \class ImageToVTKImageFilter
 * \brief Converts an ITK image into a VTK image and plugs a
 *  itk data pipeline to a VTK datapipeline.
 *
 *  This class puts together an itkVTKImageExporter and a vtkImageImporter.
 *  It takes care of the details related to the connection of ITK and VTK
 *  pipelines. The User will perceive this filter as an adaptor to which
 *  an itk::Image can be plugged as input and a vtkImage is produced as
 *  output.
 *
 * \ingroup   ImageFilters
 */
template< class TInputImage >
class ITK_EXPORT ImageToVTKImageFilter:public ProcessObject
{
public:
  /** Standard class typedefs. */
  typedef ImageToVTKImageFilter      Self;
  typedef ProcessObject              Superclass;
  typedef SmartPointer< Self >       Pointer;
  typedef SmartPointer< const Self > ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ImageToVTKImageFilter, ProcessObject);

  /** Some typedefs. */
  typedef TInputImage                              InputImageType;
  typedef typename    InputImageType::ConstPointer InputImagePointer;
  typedef VTKImageExport< InputImageType >         ExporterFilterType;
  typedef typename ExporterFilterType::Pointer     ExporterFilterPointer;

  /** Get the output in the form of a vtkImage.
      This call is delegated to the internal vtkImageImporter filter  */
  vtkImageData *  GetOutput() const;

  /** Set the input in the form of an itk::Image */
  void SetInput(const InputImageType *);

  /** Return the internal VTK image importer filter.
      This is intended to facilitate users the access
      to methods in the importer */
  vtkImageImport * GetImporter() const;

  /** Return the internal ITK image exporter filter.
      This is intended to facilitate users the access
      to methods in the exporter */
  ExporterFilterType * GetExporter() const;

  /** This call delegate the update to the importer */
  void Update();

protected:
  ImageToVTKImageFilter();
  virtual ~ImageToVTKImageFilter();
private:
  ImageToVTKImageFilter(const Self &); //purposely not implemented
  void operator=(const Self &);        //purposely not implemented

  ExporterFilterPointer m_Exporter;
  vtkImageImport *      m_Importer;
};
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkImageToVTKImageFilter.txx"
#endif

#endif
