/**
 * C Object System
 * COS DblVector
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

#define DBLVECTOR_ONLY

#define T     DblVector
#define TM   mDblVector
#define TP  pmDblVector
#define TN    DblVectorN
#define TF    DblVectorFix
#define TD    DblVectorDyn
// #define TL    DblVectorLzy
#define TV    DblVectorView
#define TW    DblVectorSubView
#define TS    "double vector"
#define TE    Double

#define TF1   DblFunction1
#define TF2   DblFunction2
#define TF3   DblFunction3
#define TF4   DblFunction4
#define TF5   DblFunction5

#define PF1   F64FCT1
#define PF2   F64FCT2
#define PF3   F64FCT3
#define PF4   F64FCT4
#define PF5   F64FCT5

#define VS    Vector.ValueSequence.Sequence

#define VAL       F64
#define VALOBJ(v) aDouble(v)

#define aTView(v,s)    aDblVectorView(v,s)
#define aTRef(v,s)     aDblVectorRef(v,s)

#define T_alloc(s)        DblVector_alloc(s)
#define TV_init(vw,v,s,b) DblVectorView_init(vw,v,s,b)

#define TOVALPTR     gdblPtr
#define TOVAL(v)     gdbl(v)
#define VALUE(v)     v->value
#define EQUAL(v1,v2) double_equal(v1,v2)

#include <cos/DblVector.h>
#include <cos/Function.h>

makclass(DblVector, FloatingVector);

// vector templates
#include "./tmpl/Vector_utl.h"

#include "./tmpl/Vector.c"

#include "./tmpl/Vector_dyn.c"
// #include "./tmpl/Vector_lzy.c"
#include "./tmpl/Vector_vw.c"

#include "./tmpl/Vector_acc.c"
#include "./tmpl/Vector_alg.c"
#include "./tmpl/Vector_fun.c"
// #include "./tmpl/Vector_fct.c"
// #include "./tmpl/Vector_vfn.c"

