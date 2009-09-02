/*
 o---------------------------------------------------------------------o
 |
 | COS Range
 |
 o---------------------------------------------------------------------o
 |
 | C Object System
 |
 | Copyright (c) 2006+ Laurent Deniau, laurent.deniau@cern.ch
 |
 | For more information, see:
 | http://cern.ch/laurent.deniau/cos.html
 |
 o---------------------------------------------------------------------o
 |
 | This file is part of the C Object System framework.
 |
 | The C Object System is free software; you can redistribute it and/or
 | modify it under the terms of the GNU Lesser General Public License
 | as published by the Free Software Foundation; either version 3 of
 | the License, or (at your option) any later version.
 |
 | The C Object System is distributed in the hope that it will be
 | useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 | of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 |
 | See <http://www.gnu.org/licenses> for more details.
 |
 o---------------------------------------------------------------------o
 |
 | $Id: Range.c,v 1.10 2009/09/02 19:34:57 ldeniau Exp $
 |
*/

#include <cos/Range.h>
#include <cos/Functor.h>
#include <cos/Function.h>
#include <cos/Number.h>
#include <cos/IntVector.h>

#include <cos/gen/algorithm.h>
#include <cos/gen/compare.h>
#include <cos/gen/functor.h>
#include <cos/gen/object.h>
#include <cos/gen/sequence.h>
#include <cos/gen/value.h>

makclass(Range, ValueSequence);

// ----- new

defmethod(OBJ, gnewWithRng1, pmRange, (I32)end)
  retmethod( ginitWithRng1(galloc(_1), end) );
endmethod

defmethod(OBJ, gnewWithRng2, pmRange, (I32)start, (I32)end)
  retmethod( ginitWithRng2(galloc(_1), start, end) );
endmethod

defmethod(OBJ, gnewWithRng3, pmRange, (I32)start, (I32)end, (I32)stride)
  retmethod( ginitWithRng3(galloc(_1), start, end, stride) );
endmethod

// ----- constructors

defmethod(OBJ, ginitWithRng1, Range, (I32)end)
  retmethod( (OBJ)Range_init(self, 0, end, 1) );
endmethod

defmethod(OBJ, ginitWithRng2, Range, (I32)start, (I32)end)
  retmethod( (OBJ)Range_init(self, start, end, 1) );
endmethod

defmethod(OBJ, ginitWithRng3, Range, (I32)start, (I32)end, (I32)stride)
  retmethod( (OBJ)Range_init(self, start, end, stride) );
endmethod

// ----- copy

defmethod(OBJ, ginitWith, Range, Range)
  retmethod( (OBJ)Range_copy(self, self2) );
endmethod

// ----- equality

defmethod(OBJ, gisEqual, Range, Range)
  retmethod( Range_isEqual(self, self2) ? True : False );
endmethod

// ----- sequence

defmethod(OBJ, gmap, Functor, Range)
  struct IntVector* vec = IntVector_alloc(Range_size(self2));
  OBJ _vec = (OBJ)vec; PRT(_vec);

  I32 *dst   = vec->value;
  U32  dst_n = vec->size;
  U32  i;

  for (i = 0; i < dst_n; i++)
    dst[i] = gint( geval1(_1, aInt(Range_eval(self2,i))) );

  UNPRT(_vec);
  retmethod(gautoDelete(_vec));
endmethod

defmethod(OBJ, gmap, IntFunction1, Range)
  struct IntVector* vec = IntVector_alloc(Range_size(self2));
  OBJ _vec = (OBJ)vec; PRT(_vec);

  I32    *dst   = vec->value;
  U32     dst_n = vec->size;
  I32FCT1 fct   = self1->fct;
  U32     i;

  for (i = 0; i < dst_n; i++)
    dst[i] = fct(Range_eval(self2,i));

  UNPRT(_vec);
  retmethod(gautoDelete(_vec));
endmethod

