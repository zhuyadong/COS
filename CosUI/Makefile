#
#  C Object System
#  COS Standard Library
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
cos := ../CosBase/include/cos

#
# standard COS makefile
#

include $(cos)/prologue

# project
library := CosUI
version := 0.1
release := 201002
package := $(library)-$(version)_$(release)

# targets
targets := debug

# files and modules
sources := src/*.c
headers := include/cos/*.h include/cos/*/*.h
defgens := include/cos/gen/*.h
defprps := include/cos/prp/*.h

# project dependencies (as with -lname)
libdeps := m wapcaplet
moddeps := CosBase CosStd

# project dependencies (as with -Ipath or -Lpath)
incdirs := src include ../CosBase/include ../CosStd/include ./libwapcaplet/include
bindirs :=
libdirs := ../CosBase/$(OSNAME)/lib ../CosStd/$(OSNAME)/lib ./libwapcaplet
moddirs :=

# documentation
auxdocs :=

# patterns (clean, install-excluded, tar-excluded)
cleanpat :=
tarexpat :=
insexpat :=

include $(cos)/epilogue

# end of makefile
