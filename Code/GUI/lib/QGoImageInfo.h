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

#ifndef __QGoImageInfo_h
#define __QGoImageInfo_h

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QScrollArea>
#include <QtGui/QWidget>

#include "QGoGUILibConfigure.h"

class QGOGUILIB_EXPORT QGoImageInfo : public QWidget
  {
  Q_OBJECT
public:
  explicit QGoImageInfo(QWidget* parent = 0);
  virtual ~QGoImageInfo();

public slots:
  void setDimension(const unsigned int&);
  void setNumberOfChannels(const unsigned int&);
  void setMemory(const unsigned long&);
  void setSize(const std::vector<unsigned int>&);
  void setSpacing(const std::vector<float>&);
  void setPixelPosition(const std::vector<unsigned int>&);
  void setWorldPosition(const std::vector<float>&);
  void setTimePoint(const float&);
  void setValue(const std::vector<float>&);

protected:
  QVBoxLayout *verticalLayout;
  QScrollArea *scrollArea;
  QWidget *    scrollAreaWidgetContents;
  QGridLayout *gridLayout;

  QLabel *TImageStaticlabel;
  QLabel *TImageLabel;
  QLabel *DimensionStaticLabel;
  QLabel *DimensionLabel;
  QLabel *ChannelStaticLabel;
  QLabel *ChannelLabel;
  QLabel *SpacingStaticLabel;
  QLabel *SpacingLabel;
  QLabel *PPositionLabel;
  QLabel *WPositionStaticLabel;
  QLabel *WPositionLabel;
  QLabel *TTimeStaticLabel;
  QLabel *TTimeLabel;
  QLabel *TValueStaticLabel;
  QLabel *TValueLabel;
  QLabel *ValueStaticLabel;
  QLabel *ValueLabel;
  QLabel *SizeStaticLabel;
  QLabel *SizeLabel;
  QLabel *TimeStaticLabel;
  QLabel *TimeLabel;
  QLabel *TPositionLabel;
  QLabel *TPositionStaticLabel;
  QLabel *MemoryStaticLabel;
  QLabel *MemoryLabel;
  QLabel *PPositionStaticLabel;

  unsigned int  m_Dimension;
  unsigned int  m_Channel;
  unsigned long m_Memory;

  std::vector<unsigned int> m_Size;
  std::vector<float>        m_Spacing;
  std::vector<unsigned int> m_PPos;
  std::vector<float>        m_WPos;
  std::vector<float>        m_Value;
  float                     m_TimePoint;

  template<class TContainer>
  QString ConvertToQString(const TContainer& iVector)
  {
    if (iVector.empty())
      {
      return QString();
      }
    else
      {
      size_t tsize = iVector.size();

      if (tsize == 1)
        {
        return QString("%1").arg(*iVector.begin());
        }
      else
        {
        QString      v("[ ");
        unsigned int i = 0;
        for (typename TContainer::const_iterator it = iVector.begin();
             it != iVector.end();
             ++it, ++i)
          {
          if (i == (tsize - 1))
            {
            v.append(QString("%1 ]").arg(*it));
            }
          else
            {
            v.append(QString("%1 ; ").arg(*it));
            }
          }
        return v;
        }
      }
  }

  void setupUi(QWidget* Form);
  void retranslateUi(QWidget* Form);

private:
  QGoImageInfo(const QGoImageInfo &);
  void operator =(const QGoImageInfo&);
  };

#endif
