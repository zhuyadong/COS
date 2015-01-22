#ifndef COS_DBLVECTOR_H
#define COS_DBLVECTOR_H

/**
 * C Object System
 * COS Vector, Dynamic Vector and Vector View
 *
 * Copyright 2006+ Laurent Deniau <laurent.deniau@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <cos/Vector.h>

/* NOTE-USER: DblVector class cluster constructors

   see cos/Vector.h for description
*/

defclass(DblVector, FloatingVector)
  F64 *value;
  U32  size;
  I32  stride;
endclass

// ----- automatic constructors

#define aDblVector(...)        ( (OBJ)atDblVector       (__VA_ARGS__) )
#define aDblVectorRef(...)     ( (OBJ)atDblVectorRef    (__VA_ARGS__) )
#define aDblVectorView(...)    ( (OBJ)atDblVectorView   (__VA_ARGS__) )
#define aDblVectorSubView(...) ( (OBJ)atDblVectorSubView(__VA_ARGS__) )

// --- shortcuts

#ifndef COS_NOSHORTCUT

#define aDVec(...)  aDblVector(__VA_ARGS__)

#endif

/***********************************************************
 * Implementation (private)
 */

// ----- Block vector

defclass(DblVector0, DblVector) F64 _value[]; endclass
defclass(DblVector1, DblVector) F64 _value[]; endclass
defclass(DblVector2, DblVector) F64 _value[]; endclass
defclass(DblVector3, DblVector) F64 _value[]; endclass
defclass(DblVector4, DblVector) F64 _value[]; endclass
defclass(DblVector5, DblVector) F64 _value[]; endclass
defclass(DblVector6, DblVector) F64 _value[]; endclass
defclass(DblVector7, DblVector) F64 _value[]; endclass
defclass(DblVector8, DblVector) F64 _value[]; endclass
defclass(DblVector9, DblVector) F64 _value[]; endclass
defclass(DblVectorN, DblVector) F64 _value[]; endclass

// ----- Fixed vector, Dynamic vector and Lazy vector

defclass(DblVectorFix, DblVector)
  F64 *_value;
  U32  capacity;
endclass

defclass(DblVectorDyn, DblVectorFix)
endclass

// ----- DblVector view and subview

defclass(DblVectorView, DblVector)
  struct DblVector *ref;
endclass

defclass(DblVectorSubView, DblVectorView)
endclass

// ----- allocators and initializers

struct Slice;
struct DblVector* DblVector_alloc(U32);
struct DblVector* DblVectorView_init(struct DblVectorView*, struct DblVector*,
                                     struct Slice*, BOOL);

// ----- automatic constructors

#define atDblVector(...)                 atVector    (Dbl,F64,__VA_ARGS__)
#define atDblVectorRef(...)              atVectorRef (Dbl,    __VA_ARGS__)
#define atDblVectorView(vector,slice)    atVectorView(Dbl,vector,slice,COS_NO)
#define atDblVectorSubView(vector,slice) atVectorView(Dbl,vector,slice,COS_YES)

#endif // COS_DBLVECTOR_H
