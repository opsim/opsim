[![Contributors](https://img.shields.io/github/contributors/daar/opsim.svg)](https://github.com/daar/opsim/graphs/contributors)
[![license](https://img.shields.io/badge/license-%20GPL--2-blue.svg)](../master/LICENSE)
[![Build Status](https://travis-ci.org/daar/opsim.svg?branch=master)](https://travis-ci.org/daar/opsim)

<img src="https://github.com/daar/opsim/blob/master/images/logo/logo.png" alt="OpSim logo"/>

OpSim is an open source Chemical Engineering Process Simulator with a user friendly drag-and-drop graphical user interface and an underlying high performance simulation engine. OpSim is cross-platform and runs equally well on Linux, Windows and Macintosh computers.

## Building and installation

The following is a basic outline for compiling and installing OpSim...

  - To build you'll first need to setup a build environment. The procedure for this can vary depending on your OS so I'll only cover the high points...
  - You'll need the latest version of the Free Pascal Compiler (FPC 3.0.2)
  - Once you have a working build environment you can setup the build by running `pmake ..` for an out of source build
  - You can now build OpSim by issuing `./make`
  - To install OpSim, including all platform dependent libraries, scripts and other supporting files issue a `./make install`
  - If all goes well OpSim will be compiled and installed on your machine
