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

/****************************************************************************
**
** This file is part of a Qt Solutions component.
**
** Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
**
** Contact:  Qt Software Information (qt-info@nokia.com)
**
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Solutions Commercial License Agreement provided
** with the Software or, alternatively, in accordance with the terms
** contained in a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** Please note Third Party Software included with Qt Solutions may impose
** additional restrictions and it is the user's responsibility to ensure
** that they have met the licensing requirements of the GPL, LGPL, or Qt
** Solutions Commercial license and the relevant license of the Third
** Party Software they are using.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
**
****************************************************************************/

#ifndef __QtColorComboBox_h
#define __QtColorComboBox_h

#include <QtGui/QComboBox>
#include <QtGui/QColor>

#include "QGoGUILibConfigure.h"

//#if defined(Q_WS_WIN)
//#  if !defined(QT_QTCOLORCOMBOBOX_EXPORT) && !defined(QT_QTCOLORCOMBOBOX_IMPORT)
//#    define QT_QTCOLORCOMBOBOX_EXPORT
//#  elif defined(QT_QTCOLORCOMBOBOX_IMPORT)
//#    if defined(QT_QTCOLORCOMBOBOX_EXPORT)
//#      undef QT_QTCOLORCOMBOBOX_EXPORT
//#    endif
//#    define QT_QTCOLORCOMBOBOX_EXPORT __declspec(dllimport)
//#  elif defined(QT_QTCOLORCOMBOBOX_EXPORT)
//#    undef QT_QTCOLORCOMBOBOX_EXPORT
//#    define QT_QTCOLORCOMBOBOX_EXPORT __declspec(dllexport)
//#  endif
//#else
//#  define QT_QTCOLORCOMBOBOX_EXPORT
//#endif

/**
 * \brief Defines the QtColorCombobox
 */

class QGOGUILIB_EXPORT QtColorComboBox : public QComboBox
  {
  Q_OBJECT
public:
  explicit QtColorComboBox(QWidget *parent = 0, const char *name = 0);

  /** \brief insert a color at a given index*/
  void insertColor(int index, const QColor& color, const QString& name);

  int colorCount() const;

  void setCurrentColor(const QColor& color);
  QColor currentColor() const;

  QColor color(int index) const;

  void setColorDialogEnabled(bool enabled = true, std::string TextToAdd = "More...");
  bool isColorDialogEnabled() const;

  void setCreationCollection(bool enabled = true);
  bool isCreationCollectionEnabled() const;

  void setStandardColors();

  int FindItemText(std::string Text);

  //not used
  void IncrementTrackID();

  QSize sizeHint() const;

  /** \brief return the name and the QColor of the current selected color*/
  std::pair<std::string, QColor> GetCurrentColorData();

signals:
  void activated(const QColor& color);
  void highlighted(const QColor& color);
  /** \brief  signal emitted when a new color has been created by the user, in order
  to be saved in the database*/
  void NewColorToBeSaved(std::vector<std::string>);
  /** \brief  signal emitted when a new collection has been created by the user, in order
  to be saved in the database*/
  void NewCollectionToBeSaved();

public slots:
  /** \brief insert a color at the end, before the "more" if the ColorDialog
  is enabled*/
  void addColor(const QColor& iColor, const QString& iName)
  { insertColor(colorCount(), iColor, iName); }
  /** \brief insert the existing colors with the data taken from the database,
  containing the color names with the associated vector of rgba*/
  void setExistingColors(
    std::list<std::pair<std::string, std::vector<int> > > iDataColorsFromDB);
  /** \brief delete the corresponding collectionID in the list displayed
  by the colorcombobox*/
  void DeleteCollectionID(unsigned int);
  void DontAddTheColor();

private slots:
  void emitHighlightedColor(int index);
  void emitActivatedColor(int index);

private:
  QColor lastActivated;
  int    numUserColors;
  bool   NewColorToBeAdded;
  /** \brief list of pair containing all the datas for the existing colors from the database
  with, for each color: a color name and a vector if rgba*/
  std::list<std::pair<std::string, std::vector<int> > > m_DataFromDB;
  bool                                                  colorDialogEnabled;
  bool                                                  creationCollection;
  /** \brief convert the QColor and NameColor to string and emit the signal for the
  new color to be saved in the database*/
  void PassDataForNewColorToBeSaved(QColor Color, std::string NameColor);
  };

#endif
