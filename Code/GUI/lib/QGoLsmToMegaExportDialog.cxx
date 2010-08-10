/*=========================================================================
  Author: $Author$  // Author of last commit
  Version: $Rev$  // Revision of last commit
  Date: $Date$  // Date of last commit
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

#include "QGoLsmToMegaExportDialog.h"

#include <QFileDialog>
#include <QProgressDialog>

//-------------------------------------------------------------------------
QGoLsmToMegaExportDialog::
QGoLsmToMegaExportDialog(QWidget* iParent) : QDialog(iParent), m_LsmPath(""),
  m_LsmName(""), m_MegaPath(""), m_FileFormatIsPNG(true), m_Counter(0)
  {
  this->setupUi(this);

  m_ProgressDialog = new QProgressDialog("Conversion in progress.", "Cancel", 0, 100, this);

  ConversionLsmToMegaThreadSend = new ConversionLsmToMegaThread;

  QObject::connect(ConversionLsmToMegaThreadSend, SIGNAL(ConversionTerminatedSent()),
                   this, SLOT(ConversionTerminatedReceived()));

  QObject::connect(ConversionLsmToMegaThreadSend, SIGNAL(InitialisationProgressSent()), this,
                   SLOT(InitialisationProgressReceived()));

  QObject::connect(ConversionLsmToMegaThreadSend, SIGNAL(ProgressSent()), this, SLOT(ProgressReceived()));

  QObject::connect(m_ProgressDialog, SIGNAL(canceled()), this, SLOT(CanceledReceived()));
  }
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
QGoLsmToMegaExportDialog::
~QGoLsmToMegaExportDialog()
  {
  delete  ConversionLsmToMegaThreadSend;
  }
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void
QGoLsmToMegaExportDialog::
on_selectLsmFile_clicked()
{
  m_LsmPath = QFileDialog::getOpenFileName(this,
                                           tr("Select the LSM file to convert"), QDir::currentPath()
                                           , tr("Image Files (*.lsm)"));

  QFileInfo fileInfo(m_LsmPath);

  m_LsmName = fileInfo.fileName();
  m_LsmName.replace(QString(" "), QString("_"));

  // Write the lsm file name in the dialog window
  lsmFileName->setText(m_LsmName);
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void
QGoLsmToMegaExportDialog::
on_selectMegaPath_clicked()
{
  m_MegaPath = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                 QDir::homePath(), QFileDialog::ShowDirsOnly |
                                                 QFileDialog::DontResolveSymlinks);

  m_MegaPath.insert(m_MegaPath.size(), QString("/"));

  megaFilePath->setText(m_MegaPath);
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void
QGoLsmToMegaExportDialog::
on_outputFormat_activated(int index)
{
  if (index == 0)
    {
    m_FileFormatIsPNG = true;
    }
  else
    {
    m_FileFormatIsPNG = false;
    }
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void
QGoLsmToMegaExportDialog::
on_convert_clicked()
{
  if (m_LsmName.isNull() ||  m_LsmName.isEmpty() || m_LsmPath.isNull() ||
      m_LsmPath.isEmpty() || m_MegaPath.isNull() ||  m_MegaPath.isEmpty())
    {
    /// \todo use QMessageBox
    std::cerr << "Please select good path for lsm and megacapture" << std::endl;
    }
  else
    {
    // Disable everything
    this->lsmFileName->setEnabled(false);
    this->megaFilePath->setEnabled(false);
    this->selectLsmFile->setEnabled(false);
    this->selectMegaPath->setEnabled(false);
    this->outputFormat->setEnabled(false);
    this->convert->setEnabled(false);
    this->label->setEnabled(false);
    this->selectLSMLabel->setEnabled(false);
    this->megaFilePath_2->setEnabled(false);
    this->outputFormatLabel->setEnabled(false);
    this->label_2->setEnabled(false);

    this->convertLabel->setText(tr("READS LSM READERS"));

    // Set conversion parameters
    GoFigure::FileType filetype = GoFigure::PNG;
    if (!m_FileFormatIsPNG)
      {
      filetype = GoFigure::TIFF;
      }

    // Remove extension
    m_LsmName.replace(QString(".lsm"), QString("_lsm"));

    // conversion fonction called from there to enable progress bar
    ConversionLsmToMegaThreadSend->SetBaseName(m_LsmName.toStdString());
    ConversionLsmToMegaThreadSend->SetLsmPath(m_LsmPath.toStdString());
    ConversionLsmToMegaThreadSend->SetOutputFileType(filetype);
    ConversionLsmToMegaThreadSend->SetMegaPath(m_MegaPath.toStdString());

    ConversionLsmToMegaThreadSend->start();
    }
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void
QGoLsmToMegaExportDialog::
ConversionTerminatedReceived()
{
  ConversionLsmToMegaThreadSend->terminate();
  ConversionLsmToMegaThreadSend->exit();
  m_ProgressDialog->accept();
  this->accept();
}
//-------------------------------------------------------------------------
/**
 * \todo check values for progress bar...(ubuntu bug..?)
 */
//-------------------------------------------------------------------------
void
QGoLsmToMegaExportDialog::
InitialisationProgressReceived()
{
  this->convertLabel->setText(tr("CONVERSION in PROGRESS"));

  int sizeProgressBar = ConversionLsmToMegaThreadSend->GetNumberOfPoints();

  // if starts at 0 strange things happen...
  m_Counter = 1;

  // sizeProgressBar because
  // +1 because m_Counter starts at 1
  // +1 to prevent that the dialoProgressWindow automatically close
  //    then re-open when 100% reached
  m_ProgressDialog->setRange(0, sizeProgressBar + 5);
  m_ProgressDialog->setValue(m_Counter);
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void
QGoLsmToMegaExportDialog::
ProgressReceived()
{
  m_Counter++;
  m_ProgressDialog->setValue(m_Counter);
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void
QGoLsmToMegaExportDialog::
CanceledReceived()
{
  ConversionLsmToMegaThreadSend->terminate();
  ConversionLsmToMegaThreadSend->exit();
  this->ConversionTerminatedReceived();
}
//-------------------------------------------------------------------------
