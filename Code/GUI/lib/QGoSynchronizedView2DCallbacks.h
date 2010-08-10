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
#ifndef __QGoSynchronizedView2DCallbacks_h
#define __QGoSynchronizedView2DCallbacks_h

#include "vtkCommand.h"
#include "QGoSynchronizedView2D.h"

class vtkObject;
class vtkCallbackCommand;

/**
 * \class QGoSynchronizedView2DCallbacks
 * \brief This object takes a list of QGoSynchronizedView and
 * synchronize their cameras by setting up callbacks.
 * It is recommended to let the
 * QGoSynchronizedViewManager deal
 * with SynchronizedView synchronization.
 */
class QGoSynchronizedView2DCallbacks
  {
public:
  /** \brief the constructor do most of the work :
   *  add observers & callbacks to QGoSynchronizedViews of the vector
   */
  QGoSynchronizedView2DCallbacks(std::vector<QGoSynchronizedView2D*> ioOpenSynchronizedViews);

  ~QGoSynchronizedView2DCallbacks();

  /** \brief callback function to synchornize cameras (has to be public)
   */
  static void synchronizeCameras(vtkObject* caller, long unsigned int eventId,
                                 void* clientData, void* callData);

  /** \brief remove a SynchronizedView from the vector
   * of synchronized SynchronizedViews
   * (this method takes care of removing the callback)
   */
  void removeSynchronizedView(QGoSynchronizedView2D* ioSynchronizedView);

  /** \brief add a SynchronizedView to the vector
   * of synchronized SynchronizedViews
   * (this method takes care of adding the callback)
   */
  void addSynchronizedView(QGoSynchronizedView2D* ioSynchronizedView);

private:
  /** \brief setup the callback command object
   * (client data, called function...etc)
   */
  void SetupCallBack();

  /** \brief callback object to link callback function to
   * SynchronizedView's event
   */
  vtkCallbackCommand* m_vtkCallBackCamSync;

  /** \brief all open SynchronizedViews are stored in this array,
   *  to transmit it to the callback function
   */
  std::vector<QGoSynchronizedView2D*> m_openSynchronizedView;
  };

#endif // QGoSynchronizedView2DCallbacks_H
