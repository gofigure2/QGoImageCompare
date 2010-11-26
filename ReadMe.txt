QGoImageCompare : Image Compare subproject
============================================================================

1-Introduction

  This application is part of the GoFigure project (see
http://gofigure2.sf.net), whose primary goal is the automatic segmentation
of nuclei and cell membranes and in temporally tracking them amidst cell
division to create lineages.

  Using ITK, VTK and Qt libraries, we developed an application for 2D and 3D
image visualization and comparison. We provide a set of classes for simple
integration in a program, an executable for a command line use, and a
pretty-printer for visual debugging in GDB.  In case of a 3D dataset, the
visualization is rendered with a variant of the vtkINRIA engine.

  The GDB pretty-printer requires the setup described at

  http://itk.org/Wiki/ITK/GDBPretty


2-Documentation

  2.1 Articles

  An article introducing the features of this project is provided via its
LateX sources.

  You can also find the Kitware Source Article (.doc and .odt).

    2.1.1 Prerequisites

  To be able to compile this article, reader should first have the
following software:
      * LateX distribution (TexLive 2009 was used with picins.sty)


    2.1.2 Compiling the pdf

$ cd Documentation/Article/Code
$ make

  2.2 Doxygen

    2.2.1 Prerequisites

  To be able to compile the documentation, User or Developer should first
install the following software:
      * Doxygen (>=1.7.1)

    2.2.2 Compiling the documentation:

  Turn on BUILD_DOCUMENTATION during cmake configuration (See 3-Installation).
ITK, VTK, Qt documentations can be linked by turning ON the option
LINK_EXTERNAL_DOC.

3-Installation

  3.1 From Sources

    3.1.1 Prerequites

  User or developpers should first install the following libraries and
softwares:
      * CMake (>=2.8.2) visit http://www.cmake.org
      * Qt  (>=4.5)   visit http://www.qtsoftware.com
      * VTK with Qt (>=5.6.0)   visit http://www.vtk.org
      * ITK (>=3.18)  visit http://www.itk.org

    3.1.2 Configuration

      * On Linux/Mac Os X:

$ git clone git://github.com/gofigure2/QGoImageCompare.git

$ cd QGoImageCompare

Create a build directory where GoFigure will be compiled
$ mkdir BUILD

Launch cmake
$ cd BUILD
$ ccmake ../

Build
$ make -j2

Test
$ ctest -j2


