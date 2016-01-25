# AMD CodeXL Static Analyzer Command Line Tool #

This package includes the command line tool for processing:
* OpenCL kernels
* HLSL shaders
* OpenGL shaders

The tool compiles and analyzes a kernel/shader and can output:
* Any build errors
* Textual IL and ISA representation
* Static Analysis results
* Binary object

This tool is part of the AMD CodeXL tools suite. See the CodeXL User Guide [here](http://developer.amd.com/tools-and-sdks/opencl-zone/codexl/) for documentation.

## System Requirements ##

* Windows: 7, 8.1 or 10, 64-bit. 
* Linux: Ubuntu 14.04, Red Hat 6.4 or later. Build with gcc 4.7.2 or later.

The Radeon Software Crimson Edition or AMD Catalyst release must be installed to run this tool.

## Building the tool ##

### Building on Windows ###
AMD CodeXL Static Analyzer builds with Visual Studio 2015.
Open the solution file at \CodeXL\AllProjectsBuild\CodeXLAnalyzerStandAlone.sln and build all of the projects.

### Building on Linux ###
One time setup:
* sudo apt-get install gcc-multilib g++-multilib
* sudo apt-get install libglu1-mesa-dev mesa-common-dev libgtk2.0-dev
* sudo apt-get install zlib1g-dev libx11-dev:i386
* sudo apt-get install scons

#### Building on CENTOS 6.X ####
Install compiler 4.7.2
* sudo wget http://people.centos.org/tru/devtools-1.1/devtools-1.1.repo -P /etc/yum.repos.d
* sudo sh -c 'echo "enabled=1" >> /etc/yum.repos.d/devtools-1.1.repo'
* sudo yum install devtoolset-1.1
* wget http://people.centos.org/tru/devtools-1.1/6/i386/RPMS/devtoolset-1.1-libstdc++-devel-4.7.2-5.el6.i686.rpm
* sudo yum install devtoolset-1.1-libstdc++-devel-4.7.2-5.el6.i686.rpm
* sudo ln -s /opt/centos/devtoolset-1.1/root/usr/bin/* /usr/local/bin/
* hash -r
* gcc --version (verify that version 4.7.2 is displayed)
Install zlib
* yum install zlib-devel

Install glibc
* yum -y install glibc-devel.i686 glibc-devel
 
Building CodeXL:
* CD to local copy of /CodeXL/Util/linux/
* Run ./buildCodeXLFullLinuxProjects CXL_build=debug
* You can add –j and state the number of concurrent jobs (-j6).

## Running ##

* Navigate to /CodeXL/Output/Release/bin or /CodeXL/Output/Debug/bin
* Run CodeXLAnalyzer or CodeXLAnalyzer-x64
* use the -h command line argument to display help information

## Support ##
For support, please visit the AMD CodeXL forum: http://devgurus.amd.com/community/codexl

## License ##
CodeXL Static Analyzer is licensed under the MIT license. See LICENSE file for full license information.

## Copyright information ##

**Boost**

Copyright Beman Dawes, 2003.
    
**TinyXML**

TinyXML is released under the zlib license
Files: *
Copyright: 2000-2007, Lee Thomason, 2002-2004, Yves Berquin 
Files: tinystr.*
Copyright: 2000-2007, Lee Thomason, 2002-2004, Yves Berquin, 2005, Tyge Lovset
    
**glew**

The OpenGL Extension Wrangler Library
Copyright (C) 2002-2007, Milan Ikits <milan ikits@ieee org>
Copyright (C) 2002-2007, Marcelo E. Magallon <mmagallo@debian org>
Copyright (C) 2002, Lev Povalahev
All rights reserved.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
THE POSSIBILITY OF SUCH DAMAGE.
    
**OpenCL**

Copyright (c) 2008-2015 The Khronos Group Inc.

**RSA Data Security, Inc.**

Copyright (C) 1990, RSA Data Security, Inc. All rights reserved.
License to copy and use this software is granted provided that
it is identified as the "RSA Data Security, Inc. MD5 Message
Digest Algorithm" in all material mentioning or referencing this 
software or this function.
License is also granted to make and use derivative works
provided that such works are identified as "derived from the RSA
Data Security, Inc. MD5 Message Digest Algorithm" in all
material mentioning or referencing the derived work.

RSA Data Security, Inc. makes no representations concerning
either the merchantability of this software or the suitability
of this software for any particular purpose.  It is provided "as
is" without express or implied warranty of any kind.

These notices must be retained in any copies of any part of this
documentation and/or software.
 
