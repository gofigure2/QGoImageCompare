/*=========================================================================
  Author: $Author$  // Author of last commit
  Version: $Revision$  // Revision of last commit
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

#include "QGoImageInfo.h"
#include <iostream>

QGoImageInfo::QGoImageInfo(QWidget* iParent) : QWidget(iParent)
  {
  setupUi(this);
  }

QGoImageInfo::~QGoImageInfo()
  {
  }

void QGoImageInfo::setupUi(QWidget *ImageInfo)
{
  if (ImageInfo->objectName().isEmpty()) ImageInfo->setObjectName(QString::fromUtf8("ImageInfo"));
  ImageInfo->resize(293, 315);
  QSizePolicy tsizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  tsizePolicy.setHorizontalStretch(0);
  tsizePolicy.setVerticalStretch(0);
  tsizePolicy.setHeightForWidth(
    ImageInfo->sizePolicy().hasHeightForWidth());
  ImageInfo->setSizePolicy(tsizePolicy);
  verticalLayout = new QVBoxLayout(ImageInfo);
  verticalLayout->setContentsMargins(3, -1, 3, -1);
  scrollArea = new QScrollArea(ImageInfo);
  scrollArea->setWidgetResizable(true);
  scrollAreaWidgetContents = new QWidget();
  scrollAreaWidgetContents->setGeometry(QRect(0, 0, 281, 301));
  gridLayout = new QGridLayout(scrollAreaWidgetContents);
  TImageStaticlabel = new QLabel(scrollAreaWidgetContents);
  QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
  sizePolicy1.setHorizontalStretch(0);
  sizePolicy1.setVerticalStretch(0);
  sizePolicy1.setHeightForWidth(TImageStaticlabel->sizePolicy().hasHeightForWidth());
  TImageStaticlabel->setSizePolicy(sizePolicy1);

  gridLayout->addWidget(TImageStaticlabel, 0, 0, 1, 1);

  TImageLabel = new QLabel(scrollAreaWidgetContents);

  gridLayout->addWidget(TImageLabel, 0, 1, 1, 1);

  DimensionStaticLabel = new QLabel(scrollAreaWidgetContents);
  sizePolicy1.setHeightForWidth(DimensionStaticLabel->sizePolicy().hasHeightForWidth());
  DimensionStaticLabel->setSizePolicy(sizePolicy1);

  gridLayout->addWidget(DimensionStaticLabel, 1, 0, 1, 1);

  DimensionLabel = new QLabel(scrollAreaWidgetContents);
  QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Preferred);
  sizePolicy2.setHorizontalStretch(0);
  sizePolicy2.setVerticalStretch(0);
  sizePolicy2.setHeightForWidth(DimensionLabel->sizePolicy().hasHeightForWidth());
  DimensionLabel->setSizePolicy(sizePolicy2);

  gridLayout->addWidget(DimensionLabel, 1, 1, 1, 1);

  ChannelStaticLabel = new QLabel(scrollAreaWidgetContents);
  sizePolicy1.setHeightForWidth(ChannelStaticLabel->sizePolicy().hasHeightForWidth());
  ChannelStaticLabel->setSizePolicy(sizePolicy1);

  gridLayout->addWidget(ChannelStaticLabel, 2, 0, 1, 1);

  ChannelLabel = new QLabel(scrollAreaWidgetContents);
  sizePolicy2.setHeightForWidth(ChannelLabel->sizePolicy().hasHeightForWidth());
  ChannelLabel->setSizePolicy(sizePolicy2);

  gridLayout->addWidget(ChannelLabel, 2, 1, 1, 1);

  SpacingStaticLabel = new QLabel(scrollAreaWidgetContents);
  sizePolicy1.setHeightForWidth(SpacingStaticLabel->sizePolicy().hasHeightForWidth());
  SpacingStaticLabel->setSizePolicy(sizePolicy1);

  gridLayout->addWidget(SpacingStaticLabel, 4, 0, 1, 1);

  SpacingLabel = new QLabel(scrollAreaWidgetContents);
  sizePolicy2.setHeightForWidth(SpacingLabel->sizePolicy().hasHeightForWidth());
  SpacingLabel->setSizePolicy(sizePolicy2);

  gridLayout->addWidget(SpacingLabel, 4, 1, 1, 1);

  PPositionLabel = new QLabel(scrollAreaWidgetContents);

  gridLayout->addWidget(PPositionLabel, 7, 1, 1, 1);

  WPositionStaticLabel = new QLabel(scrollAreaWidgetContents);
  QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Fixed);
  sizePolicy3.setHorizontalStretch(0);
  sizePolicy3.setVerticalStretch(0);
  sizePolicy3.setHeightForWidth(WPositionStaticLabel->sizePolicy().hasHeightForWidth());
  WPositionStaticLabel->setSizePolicy(sizePolicy3);

  gridLayout->addWidget(WPositionStaticLabel, 8, 0, 1, 1);

  WPositionLabel = new QLabel(scrollAreaWidgetContents);
  sizePolicy2.setHeightForWidth(WPositionLabel->sizePolicy().hasHeightForWidth());
  WPositionLabel->setSizePolicy(sizePolicy2);

  gridLayout->addWidget(WPositionLabel, 8, 1, 1, 1);

  TTimeStaticLabel = new QLabel(scrollAreaWidgetContents);
  sizePolicy3.setHeightForWidth(TTimeStaticLabel->sizePolicy().hasHeightForWidth());
  TTimeStaticLabel->setSizePolicy(sizePolicy3);

  gridLayout->addWidget(TTimeStaticLabel, 9, 0, 1, 1);

  TTimeLabel = new QLabel(scrollAreaWidgetContents);
  sizePolicy2.setHeightForWidth(TTimeLabel->sizePolicy().hasHeightForWidth());
  TTimeLabel->setSizePolicy(sizePolicy2);

  gridLayout->addWidget(TTimeLabel, 9, 1, 1, 1);

  TValueStaticLabel = new QLabel(scrollAreaWidgetContents);
  sizePolicy3.setHeightForWidth(TValueStaticLabel->sizePolicy().hasHeightForWidth());
  TValueStaticLabel->setSizePolicy(sizePolicy3);

  gridLayout->addWidget(TValueStaticLabel, 11, 0, 1, 1);

  TValueLabel = new QLabel(scrollAreaWidgetContents);

  gridLayout->addWidget(TValueLabel, 11, 1, 1, 1);

  ValueStaticLabel = new QLabel(scrollAreaWidgetContents);
  sizePolicy1.setHeightForWidth(ValueStaticLabel->sizePolicy().hasHeightForWidth());
  ValueStaticLabel->setSizePolicy(sizePolicy1);

  gridLayout->addWidget(ValueStaticLabel, 12, 0, 1, 1);

  ValueLabel = new QLabel(scrollAreaWidgetContents);
  sizePolicy2.setHeightForWidth(ValueLabel->sizePolicy().hasHeightForWidth());
  ValueLabel->setSizePolicy(sizePolicy2);

  gridLayout->addWidget(ValueLabel, 12, 1, 1, 1);

  SizeStaticLabel = new QLabel(scrollAreaWidgetContents);
  sizePolicy1.setHeightForWidth(SizeStaticLabel->sizePolicy().hasHeightForWidth());
  SizeStaticLabel->setSizePolicy(sizePolicy1);

  gridLayout->addWidget(SizeStaticLabel, 3, 0, 1, 1);

  SizeLabel = new QLabel(scrollAreaWidgetContents);
  sizePolicy2.setHeightForWidth(SizeLabel->sizePolicy().hasHeightForWidth());
  SizeLabel->setSizePolicy(sizePolicy2);

  gridLayout->addWidget(SizeLabel, 3, 1, 1, 1);

  TimeStaticLabel = new QLabel(scrollAreaWidgetContents);
  sizePolicy3.setHeightForWidth(TimeStaticLabel->sizePolicy().hasHeightForWidth());
  TimeStaticLabel->setSizePolicy(sizePolicy3);

  gridLayout->addWidget(TimeStaticLabel, 10, 0, 1, 1);

  TimeLabel = new QLabel(scrollAreaWidgetContents);
  sizePolicy2.setHeightForWidth(TimeLabel->sizePolicy().hasHeightForWidth());
  TimeLabel->setSizePolicy(sizePolicy2);

  gridLayout->addWidget(TimeLabel, 10, 1, 1, 1);

  TPositionLabel = new QLabel(scrollAreaWidgetContents);
  sizePolicy2.setHeightForWidth(TPositionLabel->sizePolicy().hasHeightForWidth());
  TPositionLabel->setSizePolicy(sizePolicy2);

  gridLayout->addWidget(TPositionLabel, 6, 1, 1, 1);

  TPositionStaticLabel = new QLabel(scrollAreaWidgetContents);
  sizePolicy1.setHeightForWidth(TPositionStaticLabel->sizePolicy().hasHeightForWidth());
  TPositionStaticLabel->setSizePolicy(sizePolicy1);

  gridLayout->addWidget(TPositionStaticLabel, 6, 0, 1, 1);

  MemoryStaticLabel = new QLabel(scrollAreaWidgetContents);
  sizePolicy1.setHeightForWidth(MemoryStaticLabel->sizePolicy().hasHeightForWidth());
  MemoryStaticLabel->setSizePolicy(sizePolicy1);

  gridLayout->addWidget(MemoryStaticLabel, 5, 0, 1, 1);

  MemoryLabel = new QLabel(scrollAreaWidgetContents);
  sizePolicy2.setHeightForWidth(MemoryLabel->sizePolicy().hasHeightForWidth());
  MemoryLabel->setSizePolicy(sizePolicy2);

  gridLayout->addWidget(MemoryLabel, 5, 1, 1, 1);

  PPositionStaticLabel = new QLabel(scrollAreaWidgetContents);
  sizePolicy3.setHeightForWidth(PPositionStaticLabel->sizePolicy().hasHeightForWidth());
  PPositionStaticLabel->setSizePolicy(sizePolicy3);

  gridLayout->addWidget(PPositionStaticLabel, 7, 0, 1, 1);

  scrollArea->setWidget(scrollAreaWidgetContents);

  verticalLayout->addWidget(scrollArea);

  retranslateUi(ImageInfo);

  QMetaObject::connectSlotsByName(ImageInfo);
} // setupUi

void QGoImageInfo::retranslateUi(QWidget *Form)
{
  Form->setWindowTitle(QApplication::translate("Form", "Form", 0, QApplication::UnicodeUTF8));
  TImageStaticlabel->setText(QApplication::translate("Form",
                                                     "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
                                                     "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
                                                     "p, li { white-space: pre-wrap; }\n"
                                                     "</style></head><body style=\" font-family:'Sans Serif'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
                                                     "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">"
                                                     "<span style=\" font-weight:600; color:#0000ff;\">Image :</span></p></body></html>",
                                                     0, QApplication::UnicodeUTF8));
  DimensionStaticLabel->setText(QApplication::translate("Form",
                                                        "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
                                                        "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
                                                        "p, li { white-space: pre-wrap; }\n"
                                                        "</style></head><body style=\" font-family:'Sans Serif'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
                                                        "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">"
                                                        "<span style=\" font-size:9pt; font-weight:600;\">  Dimension :</span></p></body></html>",
                                                        0, QApplication::UnicodeUTF8));
  DimensionLabel->setText(QString());
  SizeStaticLabel->setText(QApplication::translate("Form",
                                                   "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
                                                   "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
                                                   "p, li { white-space: pre-wrap; }\n"
                                                   "</style></head><body style=\" font-family:'Sans Serif'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
                                                   "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">"
                                                   "<span style=\" font-size:9pt; font-weight:600;\">  Size :</span></p></body></html>",
                                                   0, QApplication::UnicodeUTF8));
  SizeLabel->setText(QString());
  SpacingLabel->setText(QString());
  MemoryLabel->setText(QString());
  PPositionLabel->setText(QString());
  WPositionLabel->setText(QString());
  TimeLabel->setText(QString());
  SpacingStaticLabel->setText(QApplication::translate("Form",
                                                      "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
                                                      "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
                                                      "p, li { white-space: pre-wrap; }\n"
                                                      "</style></head><body style=\" font-family:'Sans Serif'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
                                                      "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">"
                                                      "<span style=\" font-size:9pt; font-weight:600;\">  Spacing :</span></p></body></html>",
                                                      0, QApplication::UnicodeUTF8));
  MemoryStaticLabel->setText(QApplication::translate("Form",
                                                     "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
                                                     "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
                                                     "p, li { white-space: pre-wrap; }\n"
                                                     "</style></head><body style=\" font-family:'Sans Serif'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
                                                     "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">"
                                                     "<span style=\" font-size:9pt; font-weight:600;\">  Memory :</span></p></body></html>",
                                                     0, QApplication::UnicodeUTF8));
  TPositionStaticLabel->setText(QApplication::translate("Form",
                                                        "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
                                                        "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
                                                        "p, li { white-space: pre-wrap; }\n"
                                                        "</style></head><body style=\" font-family:'Sans Serif'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
                                                        "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">"
                                                        "<span style=\" font-weight:600; color:#0000ff;\">Position :</span></p></body></html>",
                                                        0, QApplication::UnicodeUTF8));
  PPositionStaticLabel->setText(QApplication::translate("Form",
                                                        "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
                                                        "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
                                                        "p, li { white-space: pre-wrap; }\n"
                                                        "</style></head><body style=\" font-family:'Sans Serif'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
                                                        "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">"
                                                        "<span style=\" font-size:9pt; font-weight:600;\">  Position (pixel) :</span></p></body></html>",
                                                        0, QApplication::UnicodeUTF8));
  WPositionStaticLabel->setText(QApplication::translate("Form",
                                                        "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
                                                        "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
                                                        "p, li { white-space: pre-wrap; }\n"
                                                        "</style></head><body style=\" font-family:'Sans Serif'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
                                                        "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">"
                                                        "<span style=\" font-size:9pt; font-weight:600;\">  Position (um) :</span></p></body></html>",
                                                        0, QApplication::UnicodeUTF8));
  ChannelStaticLabel->setText(QApplication::translate("Form",
                                                      "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
                                                      "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
                                                      "p, li { white-space: pre-wrap; }\n"
                                                      "</style></head><body style=\" font-family:'Sans Serif'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
                                                      "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">"
                                                      "<span style=\" font-size:9pt; font-weight:600;\">  # Channels : </span></p></body></html>",
                                                      0, QApplication::UnicodeUTF8));
  TTimeStaticLabel->setText(QApplication::translate("Form",
                                                    "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
                                                    "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
                                                    "p, li { white-space: pre-wrap; }\n"
                                                    "</style></head><body style=\" font-family:'Sans Serif'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
                                                    "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">"
                                                    "<span style=\" font-weight:600; color:#0000ff;\">Time :</span></p></body></html>",
                                                    0, QApplication::UnicodeUTF8));
  TValueStaticLabel->setText(QApplication::translate("Form",
                                                     "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
                                                     "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
                                                     "p, li { white-space: pre-wrap; }\n"
                                                     "</style></head><body style=\" font-family:'Sans Serif'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
                                                     "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">"
                                                     "<span style=\" font-weight:600; color:#0000ff;\">Value :</span></p></body></html>",
                                                     0, QApplication::UnicodeUTF8));
  ValueStaticLabel->setText(QApplication::translate("Form",
                                                    "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
                                                    "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
                                                    "p, li { white-space: pre-wrap; }\n"
                                                    "</style></head><body style=\" font-family:'Sans Serif'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
                                                    "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">"
                                                    "<span style=\" font-size:9pt; font-weight:600;\">  Pixel value :</span></p></body></html>",
                                                    0, QApplication::UnicodeUTF8));
  ValueLabel->setText(QString());
  ChannelLabel->setText(QString());
  TimeStaticLabel->setText(QApplication::translate("Form",
                                                   "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
                                                   "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
                                                   "p, li { white-space: pre-wrap; }\n"
                                                   "</style></head><body style=\" font-family:'Sans Serif'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
                                                   "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">"
                                                   "<span style=\" font-size:9pt; font-weight:600;\">  Time Point :</span></p></body></html>",
                                                   0, QApplication::UnicodeUTF8));
  Q_UNUSED(Form);
} // retranslateUi

void QGoImageInfo::setDimension(const unsigned int& dim)
{
  m_Dimension = dim;
  this->DimensionLabel->setText(QString("%1").arg(m_Dimension));
  m_Size.resize(dim);
  m_Spacing.resize(dim);

  bool space_time = (dim > 3);
  this->TTimeStaticLabel->setVisible(space_time);
  this->TTimeLabel->setVisible(space_time);
  this->TimeStaticLabel->setVisible(space_time);
  this->TimeLabel->setVisible(space_time);

  if (space_time)
    {
    m_PPos.resize(3);
    m_WPos.resize(3);
    }
  else
    {
    m_PPos.resize(dim);
    m_WPos.resize(dim);
    }
}

void QGoImageInfo::setNumberOfChannels(const unsigned int& channel)
{
  m_Channel = channel;
  this->ChannelLabel->setText(QString("%1").arg(m_Channel));

  m_Value.resize(m_Channel);
}

void QGoImageInfo::setMemory(const unsigned long& iMem)
{
  m_Memory = iMem;

  if (static_cast<double>(m_Memory) > 1e12)
    {
    this->MemoryLabel->setText(QString("%1 TB").arg(1e-12 * static_cast<double>(m_Memory), 0, 'g', 3));
    }
  else
    {
    if (static_cast<double>(m_Memory) > 1e9)
      {
      this->MemoryLabel->setText(QString("%1 GB").arg(1e-9 * static_cast<double>(m_Memory), 0, 'g', 3));
      }
    else
      {
      if (static_cast<double>(m_Memory) > 1e6)
        {
        this->MemoryLabel->setText(QString("%1 MB").arg(1e-6 * static_cast<double>(m_Memory), 0, 'g', 3));
        }
      else
        {
        if (static_cast<double>(m_Memory) > 1e3)
          {
          this->MemoryLabel->setText(QString("%1 KB").arg(1e-3 * static_cast<double>(m_Memory), 0, 'g', 3));
          }
        else
          {
          this->MemoryLabel->setText(QString("%1").arg(m_Memory));
          }
        }
      }
    }
}

void QGoImageInfo::setSize(const std::vector<unsigned int>& iSize)
{
  if (!m_Size.empty())
    {
    if (iSize.size() == m_Dimension)
      {
      m_Size = iSize;
      this->SizeLabel->setText(ConvertToQString(m_Size));
      }
    else
      {
      std::cout << "iSize.size() != " << m_Dimension << std::endl;
      }
    }
  else
    {
    std::cout << "Please setDimension first!" << std::endl;
    }
}

void QGoImageInfo::setSpacing(const std::vector<float>& iSpacing)
{
  if (!m_Spacing.empty())
    {
    if (iSpacing.size() == m_Dimension)
      {
      m_Spacing = iSpacing;
      this->SpacingLabel->setText(ConvertToQString(m_Spacing));
      }
    else
      {
      std::cout << "iSpacing.size() != " << m_Dimension << std::endl;
      }
    }
  else
    {
    std::cout << "Please setDimension first!" << std::endl;
    }
}

void QGoImageInfo::setPixelPosition(const std::vector<unsigned int>& iPos)
{
  if (!m_PPos.empty())
    {
    unsigned int len = (m_Dimension > 3) ? 3 : m_Dimension;
    if (iPos.size() == len)
      {
      m_PPos = iPos;
      this->PPositionLabel->setText(ConvertToQString(m_PPos));
      }
    else
      {
      std::cout << "iPos.size() != " << len << std::endl;
      }
    }
  else
    {
    std::cout << "Please setDimension first!" << std::endl;
    }
}

void QGoImageInfo::setWorldPosition(const std::vector<float>& iPos)
{
  if (!m_WPos.empty())
    {
    unsigned int len = (m_Dimension > 3) ? 3 : m_Dimension;
    if (iPos.size() == len)
      {
      m_WPos = iPos;
      this->WPositionLabel->setText(ConvertToQString(m_WPos));
      }
    else
      {
      std::cout << "iPos.size() != " << len << std::endl;
      }
    }
  else
    {
    std::cout << "Please setDimension first!" << std::endl;
    }
}

void QGoImageInfo::setTimePoint(const float& iTime)
{
  m_TimePoint = iTime;
  this->TimeLabel->setText(QString("%1").arg(m_TimePoint));
}

void QGoImageInfo::setValue(const std::vector<float>& iVal)
{
  QString val;

  if (!m_Value.empty())
    {
    if (iVal.size() == m_Channel)
      {
      m_Value = iVal;
      this->ValueLabel->setText(ConvertToQString(m_Value));
      }
    else
      {
      std::cout << "iVal.size() != " << m_Channel << std::endl;
      }
    }
  else
    {
    std::cout << "Please setDimension first!" << std::endl;
    }
}
