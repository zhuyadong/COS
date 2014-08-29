#
#  C Object System
#  COS example
# 
#  Copyright 2007+ Laurent Deniau <laurent.deniau@gmail.com>
# 
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
# 
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.
#

# bootstrap, normally $(cos) = cos
cos := ../../CosBase/include/cos
base := ../../CosBase
std := ../../CosStd
ui := ../../CosUI

# standard COS makefile

include $(cos)/prologue

# project
program := main

# targets
targets := debug # debug.run

# files & modules
sources := *.c
defgens := 
defprps :=

# project dependencies (as with -lname)
libdeps := wapcaplet
moddeps := CosBase CosStd CosUI

# project dependencies (as with -Ipath or -Lpath)
incdirs := $(base)/include $(std)/include $(ui)/include ../../include ../libwapcaplet/include .
libdirs := $(base)/$(OSNAME)/lib $(std)/$(OSNAME)/lib $(ui)/$(OSNAME)/lib ../libwapcaplet

include $(cos)/epilogue

# end of COS example makefile
