[![Contributors](https://img.shields.io/github/contributors/opsim/opsim.svg)](https://github.com/opsim/opsim/graphs/contributors)
[![license](https://img.shields.io/badge/license-%20GPL--2-blue.svg)](../master/LICENSE)
[![Build Status](https://travis-ci.org/opsim/opsim.svg?branch=master)](https://travis-ci.org/opsim/opsim)

<img src="./release/images/logo/logo.svg" alt="OpSim logo"/>

OpSim is an open source Chemical Engineering Process Simulator with a user friendly drag-and-drop graphical user interface and an underlying high performance simulation engine. OpSim is cross-platform and runs equally well on Linux, Windows and Macintosh computers.

## Binaries
If you would like to start using OpSim immediately please check out our [release](https://github.com/opsim/opsim/releases) page. There you will find a fully containing zipfile suited for your platform. Please [report](https://github.com/opsim/opsim/issues) any issues or ideas that you have to improve OpSim.

## Building and installation
The following is a basic outline for compiling and installing OpSim...

  - To build you'll first need to setup a build environment. The procedure for this can vary depending on your OS so I'll only cover the high points...
  - You'll need the [Free Pascal Compiler](https://freepascal.org/), version 3.0.4
  - OpSim uses [pmake](https://github.com/daar/pmake) to build the sources
  - Once you have a working build environment you can setup the build by creating a subfolder in the trunk (e.g. `build`) and running `pmake ..` for an out of source build
  - OpSim is designed to require a minimum set of dependencies. As of now only the readily available [OpenGL](http://www.opengl.org) is required.
  - You can now build OpSim by issuing `./make`
  - To install OpSim, including all platform dependent libraries, scripts and other supporting files issue a `./make install`
  - If all goes well OpSim will be compiled and installed on your machine
