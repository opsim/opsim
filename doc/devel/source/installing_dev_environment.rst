Installing the development environment
======================================

OpSim is designed to have a minimal set of dependencies and therefore not a lot of third party libraries are needed to compile and run OpSim. This page will describe how to setup your environment.

Repository access
-----------------
OpSim source code is hosted at GitHub_, together with additional scripts, translations and prebuilt Windows and Mac OS X libraries.

.. _GitHub: https://github.com/daar/opsim

The main URL for the repository is:

.. code-block:: bash

   https://github.com/daar/opsim.git

Additional information
~~~~~~~~~~~~~~~~~~~~~~
* `Overview of git clients <https://git-scm.com/downloads/guis/>`_
* `Version Control with git <https://git-scm.com/doc/>`_
* `Git website <https://git-scm.com/>`_

Installing the FreePascal compiler
----------------------------------
The FreePascal_ compiler is a 32, 64 and 16 bit professional Pascal compiler. It can target many processor architectures and platforms, under which windows 32/64bit, Linux and MacOS. OpSim requires FPC 3.0.4 to work properly.

.. _FreePascal: http://www.freepascal.org


MS-Windows
^^^^^^^^^^

For MS-Windows one can download and install an installer from the website. This is a normal installation program, which offers the usual options of selecting a directory, and which parts of the distribution you want to install. It will, optionally, associate the .pp or .pas extensions with the text mode IDE. It is not recommended to install the compiler in a directory which has spaces in it’s path name. Some of the external tools do not support filenames with spaces in them, and you will have problems creating programs.

Linux
^^^^^

The Linux distribution of Free Pascal comes in three forms:

* a ``.tar.gz`` version, also available as separate files.
* a ``.rpm`` (Red Hat Package Manager) version, and
* a ``.deb`` (Debian) version.

Additionally operating systems like Ubuntu provide FPC in their apps directory, however before installing, please check if the correct FPC version will be installed.

For Ubuntu use: ``sudo apt-get install fpc``


MacOS
^^^^^

FreePascal also provides binary installers for MacOS. Download and install as per instructions on the FreePascal website.

```
ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)" < /dev/null 2> /dev/null

brew install fpc
```

PMake
-----
The build system that is used by OpSim is called PMake_.

.. _PMake: http://daar.github.io/pmake/quickstart.html

Building OpSim
--------------
To build OpSim with PMake open the terminal and enter the following commands from the ``./opsim-code/trunk`` folder.

.. code-block:: bash

   >./pmake
   >./make
   >./make install

Once finished, the OpSim executable and all the corresponding libraries, scripts and database files will be copied to a location on your machine. For MS-Windows this will be ``C:\\Program Files\\OpSim<ver>``. On Linux this will be ``/usr/bin/opsim<ver>``. You will need elevated privileges to install opsim on your computer. 
