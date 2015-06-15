# AgentSpeakTranslator

A prototype translator from AgentSpeak to C.

For more information, see [here](http://www.cprover.org/UAVs/TAROS2015/).

**Important:** This translator was created as a prototype/proof-of-concept. Many features are missing or experimental and specifications may change significantly in the course of development.

See also

* [modified `tum_ardrone` package](https://github.com/OxfordUAVAutonomy/tum_ardrone)


## Installation and Usage

In order to install the translator, please use the following steps:

1. Obtain the code
    
        mkdir translator
        cd translator
        git clone https://github.com/OxfordUAVAutonomy/AgentSpeakTranslator.git

2. As a first step, we recommend to generate the documentation:

        cd src
        make documentation
    
     This steps requires Doxygen (version 1.7.6.1 works). The generated documentation can be accessed via `doc\index.html`, where an example walk-through and a code overview can be found.
    
3. In order to compile the code, do

        cd src
        make
          

## Autopilot

* To generate C code from autopilot in `test/autopilot`, do

        make autopilot
          
    This should generate the following files

        actions.h
        autopilot.c
        autopilot.h
          
    The generated files can be found in `test/autopilot`. These files, together with `types.h` from the same directory, are used in

        catkin_ws/src/tum_ardrone/src/autopilot/autopilotAI/
          
## Troubleshooting

* If you work on Ubuntu 12.04 (e.g., because you use the modified `tum_ardrone` package), you need to install gcc 4.7. This is required because the translator works uses C++11, which requires gcc 4.7, but Ubuntu 12.04 only comes with gcc 4.6. In order to do so, use the following steps (see http://www.swiftsoftwaregroup.com/upgrade-gcc-4-7-ubuntu-12-04/):
        
        sudo add-apt-repository ppa:ubuntu-toolchain-r/test
        sudo apt-get update
        sudo apt-get install gcc-4.7 g++-4.7
        
        sudo update-alternatives --remove gcc /usr/bin/gcc-4.6
        sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.7 60 --slave /usr/bin/g++ g++ /usr/bin/g++-4.7
        sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.6 40 --slave /usr/bin/g++ g++ /usr/bin/g++-4.6
        
        sudo update-alternatives --config gcc
    
* This code also uses Flex (version 2.5.35 works) and GNU bison (version 2.5 works).
    

## License for AgentSpeak translator

Copyright (c) 2015, Samuel Bucheli, Department of Computer Science, University
of Oxford. All rights reserved.

Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Note that this license only applies to the actual translator code as provided in the `src` directory. Files in the `test` come with their own, potentially different license.