GoFigure2- Image Compare subproject
============================================================================

1-Introduction

  The prime goal of GoFigure is the automatic segmentation of nuclei and 
cell membranes and in temporally tracking them amidst cell division to 
create lineages.

  Using ITK, VTK and Qt libraries, we developed an application for images visualization and comparison. 
A set of classes is provided for simple integration in a program, an executable is also provided for a command line use.
in case of a 3D dataset, the visualization is rendered with a variant of the vtkINRIA engine.



2-Installation
  2.1 From Sources

    Please refer to
      * http://sourceforge.net/apps/trac/gofigure2/wiki/DeveloperSetUp
      * http://sourceforge.net/apps/trac/gofigure2/wiki/Compilation
      * http://sourceforge.net/apps/trac/gofigure2/wiki/VideoSupport

    2.1.1 Prerequites

  User or developpers should first install the following libraries and
softwares:
      * CMake (>=2.6) visit http://www.cmake.org
      * Qt  (>=4.5)   visit http://www.qtsoftware.com
      * VTK with Qt and MYSQL (>=5.6)   visit http://www.vtk.org 
      * ITK (>=3.18)  visit http://www.itk.org


    2.1.2 Configuration

      * On Linux:

$ cd gofigure2/trunk/

Create a build directory where GoFigure will be compiled
$ mkdir BUILD

Launch cmake 
$ cd BUILD
$ ccmake path/to/source/directory

Build
$ make -j2


