Installing the development environment
======================================

OpSim is designed to have a minimal set of dependencies and therefore not a lot of third party libraries are needed to compile and run OpSim. This page will describe how to setup your environment.

Repository access
-----------------
OpSim source code is hosted at SourceForge_, together with additional scripts, translations and prebuilt Windows and Mac OS X libraries.

.. _SourceForge: https://sourceforge.net/p/opsim/code/HEAD/tree/

Access to the repository is provided both for anonymous (read access) and commit access (write access). The main urls for the repositories are:

Read only access:

.. code-block:: bash
   
   svn checkout svn://svn.code.sf.net/p/opsim/code/trunk opsim-code

Read write access:

.. code-block:: bash
   
   svn checkout --username=<username> svn+ssh://<username>@svn.code.sf.net/p/opsim/code/trunk opsim-code

HTTP access:

.. code-block:: bash
   
   svn checkout --username=<username> https://svn.code.sf.net/p/opsim/code/trunk opsim-code

Additional information
~~~~~~~~~~~~~~~~~~~~~~
* Overview of Subversion clients
* Version Control with Subversion (online book)
* Subversions website 

Installing the FreePascal compiler
----------------------------------
The FreePascal_ compiler is a 32, 64 and 16 bit professional Pascal compiler. It can target many processor architectures and platforms, under which windows 32/64bit, Linux and MacOS. OpSim requires FPC 3.0.0 to work properly.

.. _FreePascal: http://www.freepascal.org

For *Windows* one can download and install an installer. This is a normal installation program, which offers the usual options of selecting a directory, and which parts of the distribution you want to install. It will, optionally, associate the .pp or .pas extensions with the text mode IDE. It is not recommended to install the compiler in a directory which has spaces in it’s path name. Some of the external tools do not support filenames with spaces in them, and you will have problems creating programs.

The *Linux* distribution of Free Pascal comes in three forms:

* a tar.gz version, also available as separate files.
* a .rpm (Red Hat Package Manager) version, and
* a .deb (Debian) version.

Additionally operating systems like Ubuntu provide FPC in their apps directory, however before installing, please check if the correct FPC version will be installed.

FreePascal also provides binary installers for *MacOS*. Download and install as per instructions on the FreePascal website.

FMake
-----
The build system that is used by OpSim is called FMake_.

.. _FMake: http://daar.github.io/fmake/quickstart.html

Building OpSim
--------------
To build OpSim with FMake open the terminal and enter the following commands from the ./opsim-code/trunk folder.

.. code-block:: bash

   >./fmake
   >./make
   >./make install

Once finished, the OpSim executable and all the corresponding libraries, scripts and database files will be copied to a location on your machine. For *Windows* this will be "C:\\Program Files\\OpSim<ver>". On *Linux* this will be "/usr/bin/opsim<ver>".