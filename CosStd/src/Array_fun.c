/*
 o---------------------------------------------------------------------o
 |
 | COS Array - algorithms using functors
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
 | $Id: Array_fun.c,v 1.16 2009/10/02 21:56:20 ldeniau Exp $
 |
*/

#include <cos/Array.h>
#include <cos/Functor.h>
#include <cos/IntVector.h>
#include <cos/Number.h>

#include <cos/gen/algorithm.h>
#include <cos/gen/container.h>
#include <cos/gen/functor.h>
#include <cos/gen/object.h>

// ----- 

useclass(Array);
useclass(Lesser,Equal,Greater);

// ----- applyWhile, apply (in-place map with returned value discarded)

defmethod(void, gapplyWhile, Functor, Array)
  U32 size   = self2->size;
  I32  val_s = self2->stride;
  OBJ *val   = self2->object;
  OBJ *end   = val + val_s*size;

  while (val != end && geval(_1, *val) != Nil)
    val += val_s;
endmethod

defmethod(void, gapply, Functor, Array)
  U32  size  = self2->size;
  I32  val_s = self2->stride;
  OBJ *val   = self2->object;
  OBJ *end   = val + val_s*size;

  while (val != end) {
    geval(_1, *val);
    val += val_s;
  }
endmethod

defmethod(void, gapply2, Functor, Array, Array)
  U32  size   = self2->size < self3->size ? self2->size : self3->size;
  I32  val_s  = self2->stride;
  OBJ *val    = self2->object;
  I32  val2_s = self3->stride;
  OBJ *val2   = self3->object;
  OBJ *end    = val + val_s*size;

  while (val != end) {
    geval(_1, *val, *val2);
    val  += val_s;
    val2 += val2_s;
  }
endmethod

defmethod(void, gapply3, Functor, Array, Array, Array)
  U32  size   = self2->size < self3->size ? self2->size : self3->size;
       size   = self4->size < size ? self4->size : size;
  I32  val_s  = self2->stride;
  OBJ *val    = self2->object;
  I32  val2_s = self3->stride;
  OBJ *val2   = self3->object;
  I32  val3_s = self4->stride;
  OBJ *val3   = self4->object;
  OBJ *end    = val + val_s*size;

  while (val != end) {
    geval(_1, *val, *val2, *val3);
    val  += val_s;
    val2 += val2_s;
    val3 += val3_s;
  }
endmethod

defmethod(void, gapply4, Functor, Array, Array, Array, Array)
  U32  size   = self2->size < self3->size ? self2->size : self3->size;
       size   = self4->size < size ? self4->size : size;
       size   = self5->size < size ? self5->size : size;
  I32  val_s  = self2->stride;
  OBJ *val    = self2->object;
  I32  val2_s = self3->stride;
  OBJ *val2   = self3->object;
  I32  val3_s = self4->stride;
  OBJ *val3   = self4->object;
  I32  val4_s = self5->stride;
  OBJ *val4   = self5->object;
  OBJ *end    = val + val_s*size;

  while (val != end) {
    geval(_1, *val, *val2, *val3, *val4);
    val  += val_s;
    val2 += val2_s;
    val3 += val3_s;
    val4 += val4_s;
  }
endmethod

// ----- mapIf, mapWhile, map, map2, map3, map4

defmethod(OBJ, gmapIf, Functor, Array)
  U32 size = self2->size;
  OBJ _arr = gautoDelete(gnewWith(Array,aInt(size)));
  struct Array* arr = STATIC_CAST(struct Array*, _arr);

  I32  val_s = self2->stride;
  OBJ *val   = self2->object;
  U32 *dst_n = &arr->size;
  OBJ *dst   = arr->object;
  OBJ *end   = val + val_s*size;
  OBJ  res;

  while (val != end) {
    if ((res = geval(_1, *val)) != Nil)
      *dst++ = gretain(res), ++*dst_n;
    val += val_s;
  }

  retmethod(gadjust(_arr));
endmethod

defmethod(OBJ, gmapWhile, Functor, Array)
  U32 size = self2->size;
  OBJ _arr = gautoDelete(gnewWith(Array,aInt(size)));
  struct Array* arr = STATIC_CAST(struct Array*, _arr);

  I32  val_s = self2->stride;
  OBJ *val   = self2->object;
  U32 *dst_n = &arr->size;
  OBJ *dst   = arr->object;
  OBJ *end   = val + val_s*size;
  OBJ  res;

  while (val != end && (res = geval(_1, *val)) != Nil) {
    *dst++ = gretain(res), ++*dst_n;
    val += val_s;
  }

  retmethod(gadjust(_arr));
endmethod

defmethod(OBJ, gmap, Functor, Array)
  U32 size = self2->size;
  struct Array* arr = Array_alloc(size);
  OBJ _arr = gautoDelete( (OBJ)arr );

  I32  val_s = self2->stride;
  OBJ *val   = self2->object;
  U32 *dst_n = &arr->size;
  OBJ *dst   = arr->object;
  OBJ *end   = dst + size;
  OBJ  res;

  while (dst != end) {
    res = geval(_1, *val);
    *dst++ = gretain(res), ++*dst_n;
    val += val_s;
  }

  retmethod(_arr);
endmethod

defmethod(OBJ, gmap2, Functor, Array, Array)
  U32 size = self2->size < self3->size ? self2->size : self3->size;
  struct Array* arr = Array_alloc(size);
  OBJ _arr = gautoDelete( (OBJ)arr );

  I32  val_s  = self2->stride;
  OBJ *val    = self2->object;
  I32  val2_s = self3->stride;
  OBJ *val2   = self3->object;
  U32 *dst_n  = &arr->size;
  OBJ *dst    = arr->object;
  OBJ *end    = dst + size;
  OBJ  res;

  while (dst != end) {
    res = geval(_1, *val, *val2);
    *dst++ = gretain(res), ++*dst_n;
    val  += val_s;
    val2 += val2_s;
  }

  retmethod(_arr);
endmethod

defmethod(OBJ, gmap3, Functor, Array, Array, Array)
  U32 size = self2->size < self3->size ? self2->size : self3->size;
      size = self4->size < size ? self4->size : size;
  struct Array* arr = Array_alloc(size);
  OBJ _arr = gautoDelete( (OBJ)arr );

  I32  val_s  = self2->stride;
  OBJ *val    = self2->object;
  I32  val2_s = self3->stride;
  OBJ *val2   = self3->object;
  I32  val3_s = self4->stride;
  OBJ *val3   = self4->object;
  U32 *dst_n  = &arr->size;
  OBJ *dst    = arr->object;
  OBJ *end    = dst + size;
  OBJ  res;

  while (dst != end) {
    res = geval(_1, *val, *val2, *val3);
    *dst++ = gretain(res), ++*dst_n;
    val  += val_s;
    val2 += val2_s;
    val3 += val3_s;
  }

  retmethod(_arr);
endmethod

defmethod(OBJ, gmap4, Functor, Array, Array, Array, Array)
  U32 size = self2->size < self3->size ? self2->size : self3->size;
      size = self4->size < size ? self4->size : size;
      size = self5->size < size ? self5->size : size;
  struct Array* arr = Array_alloc(size);
  OBJ _arr = gautoDelete( (OBJ)arr );

  I32  val_s  = self2->stride;
  OBJ *val    = self2->object;
  I32  val2_s = self3->stride;
  OBJ *val2   = self3->object;
  I32  val3_s = self4->stride;
  OBJ *val3   = self4->object;
  I32  val4_s = self5->stride;
  OBJ *val4   = self5->object;
  U32 *dst_n  = &arr->size;
  OBJ *dst    = arr->object;
  OBJ *end    = dst + size;
  OBJ  res;

  while (dst != end) {
    res = geval(_1, *val, *val2, *val3, *val4);
    *dst++ = gretain(res), ++*dst_n;
    val  += val_s;
    val2 += val2_s;
    val3 += val3_s;
    val4 += val4_s;
  }

  retmethod(_arr);
endmethod

// ----- all, any, count

defmethod(OBJ, gall, Array, Functor)
  U32  size  = self->size;
  I32  val_s = self->stride;
  OBJ *val   = self->object;
  OBJ *end   = val + val_s*size;

  while (val != end) {
    if (geval(_2, *val) != True)
      retmethod(False);
    val += val_s;
  }
      
  retmethod(True);
endmethod

defmethod(OBJ, gany, Array, Functor)
  U32  size  = self->size;
  I32  val_s = self->stride;
  OBJ *val   = self->object;
  OBJ *end   = val + val_s*size;

  while (val != end) {
    if (geval(_2, *val) == True)
      retmethod(True);
    val += val_s;
  }
      
  retmethod(False);
endmethod

defmethod(U32, gcount, Array, Functor)
  U32  size  = self->size;
  I32  val_s = self->stride;
  OBJ *val   = self->object;
  OBJ *end   = val + val_s*size;
  U32  cnt   = 0;

  while (val != end) {
    if (geval(_2, *val) == True)
      ++cnt;
    val += val_s;
  }
      
  retmethod(cnt);
endmethod

// ----- select, reject

defmethod(OBJ, gselect, Array, Functor)
  U32 size = self->size;
  OBJ _arr = gautoDelete(gnewWith(Array,aInt(size)));
  struct Array* arr = STATIC_CAST(struct Array*, _arr);

  I32  val_s = self->stride;
  OBJ *val   = self->object;
  U32 *dst_n = &arr->size;
  OBJ *dst   = arr ->object;
  OBJ *end   = val + val_s*size;

  while (val != end) {
    if (geval(_2, *val) == True)
      *dst++ = gretain(*val), ++*dst_n;
    val += val_s;
  }

  retmethod(gadjust(_arr));
endmethod

defmethod(OBJ, gselectIf, Array, Functor)
  U32 size = self->size;
  OBJ _arr = gautoDelete(gnewWith(Array,aInt(size)));
  struct Array* arr = STATIC_CAST(struct Array*, _arr);

  I32  val_s = self->stride;
  OBJ *val   = self->object;
  U32 *dst_n = &arr->size;
  OBJ *dst   = arr->object;
  OBJ *end   = val + val_s*size;

  while (val != end) {
    if (geval(_2, *val) != Nil)
      *dst++ = gretain(*val), ++*dst_n;
    val += val_s;
  }

  retmethod(gadjust(_arr));
endmethod

defmethod(OBJ, gselectWhile, Array, Functor)
  U32 size = self->size;
  OBJ _arr = gautoDelete(gnewWith(Array,aInt(size)));
  struct Array* arr = STATIC_CAST(struct Array*, _arr);

  I32  val_s = self->stride;
  OBJ *val   = self->object;
  U32 *dst_n = &arr->size;
  OBJ *dst   = arr->object;
  OBJ *end   = val + val_s*size;
  OBJ  res;

  while (val != end && (res = geval(_2, *val)) != Nil) {
    if (res == True)
      *dst++ = gretain(*val), ++*dst_n;
    val += val_s;
  }

  retmethod(gadjust(_arr));
endmethod

defmethod(OBJ, greject, Array, Functor)
  U32 size = self->size;
  OBJ _arr = gautoDelete(gnewWith(Array,aInt(size)));
  struct Array* arr = STATIC_CAST(struct Array*, _arr);

  I32  val_s = self->stride;
  OBJ *val   = self->object;
  U32 *dst_n = &arr->size;
  OBJ *dst   = arr ->object;
  OBJ *end   = val + val_s*size;

  while (val != end) {
    if (geval(_2, *val) == False)
      *dst++ = gretain(*val), ++*dst_n;
    val += val_s;
  }

  retmethod(gadjust(_arr));
endmethod

defmethod(OBJ, grejectIf, Array, Functor)
  U32 size = self->size;
  OBJ _arr = gautoDelete(gnewWith(Array,aInt(size)));
  struct Array* arr = STATIC_CAST(struct Array*, _arr);

  I32  val_s = self->stride;
  OBJ *val   = self->object;
  U32 *dst_n = &arr->size;
  OBJ *dst   = arr->object;
  OBJ *end   = val + val_s*size;

  while (val != end) {
    if (geval(_2, *val) == Nil)
      *dst++ = gretain(*val), ++*dst_n;
    val += val_s;
  }

  retmethod(gadjust(_arr));
endmethod

defmethod(OBJ, grejectWhile, Array, Functor)
  U32 size = self->size;
  OBJ _arr = gautoDelete(gnewWith(Array,aInt(size)));
  struct Array* arr = STATIC_CAST(struct Array*, _arr);

  I32  val_s = self->stride;
  OBJ *val   = self->object;
  U32 *dst_n = &arr->size;
  OBJ *dst   = arr->object;
  OBJ *end   = val + val_s*size;
  OBJ  res;

  while (val != end && (res = geval(_2, *val)) != Nil) {
    if (res == False)
      *dst++ = gretain(*val), ++*dst_n;
    val += val_s;
  }

  retmethod(gadjust(_arr));
endmethod

// ----- reduce

defmethod(OBJ, greduce, Array, Functor)
  U32  size  = self->size;
  I32  val_s = self->stride;
  OBJ *val   = self->object;
  OBJ *end   = val + val_s*size;
  OBJ  res   = Nil;

  if (val != end) {
    res  = gautoDelete(gclone(*val));
    val += val_s;
    
    while (val != end) {
      res = geval(_2, res, *val);
      val += val_s;
    }
  }

  retmethod(res);
endmethod

defmethod(OBJ, grreduce, Array, Functor)
  U32  size  = self->size;
  I32  val_s = self->stride;
  OBJ *val   = self->object;
  OBJ *end   = val + val_s*size;
  OBJ  res   = Nil;

  if (val != end) {
    end -= val_s;
    res = gautoDelete(gclone(*end));

    while (val != end) {
      end -= val_s;
      res = geval(_2, *end, res);
    }
  }

  retmethod(res);
endmethod

defmethod(OBJ, greduce1, Array, Functor, Object)
  U32  size  = self->size;
  I32  val_s = self->stride;
  OBJ *val   = self->object;
  OBJ *end   = val + val_s*size;
  OBJ  res   = _3;
  
  while (val != end) {
    res = geval(_2, res, *val);
    val += val_s;
  }

  retmethod(res);
endmethod

defmethod(OBJ, grreduce1, Array, Functor, Object)
  U32  size  = self->size;
  I32  val_s = self->stride;
  OBJ *val   = self->object;
  OBJ *end   = val + val_s*size;
  OBJ  res   = _3;
  
  while (val != end) {
    end -= val_s;
    res = geval(_2, *end, res);
  }

  retmethod(res);
endmethod

defmethod(OBJ, greduce2, Array, Array, Functor, Object)
  U32  size   = self->size < self2->size ? self->size : self2->size;
  I32  val_s  = self->stride;
  OBJ *val    = self->object;
  I32  val2_s = self2->stride;
  OBJ *val2   = self2->object;
  OBJ *end    = val + val_s*size;
  OBJ  res    = _4;

  while (val != end) {
    res = geval(_3, res, *val, *val2);
    val  += val_s;
    val2 += val2_s;
  }

  retmethod(res);
endmethod

defmethod(OBJ, grreduce2, Array, Array, Functor, Object)
  U32  size   = self->size < self2->size ? self->size : self2->size;
  I32  val_s  = self->stride;
  OBJ *end    = self->object  + val_s*self->size;
  I32  val2_s = self2->stride;
  OBJ *end2   = self2->object + val2_s*self2->size;
  OBJ *val    = end - val_s*size;
  OBJ  res    = _4;

  while (val != end) {
    end  -= val_s;
    end2 -= val2_s;
    res = geval(_3, *end, *end2, res);
  }
  
  retmethod(res);
endmethod

// ----- accumulate

defmethod(OBJ, gaccumulate, Array, Functor)
  U32 size = self->size;
  struct Array* arr = Array_alloc(size);
  OBJ _arr = gautoDelete( (OBJ)arr );

  I32  val_s = self->stride;
  OBJ *val   = self->object;
  U32 *dst_n  = &arr->size;
  OBJ *dst   = arr->object;
  OBJ *end   = dst + size;
  OBJ  res;

  if (dst != end) {
    *dst = gretain(*val), ++*dst_n;
    val += val_s, --end;
    
    while (dst != end) {
      res = geval(_2, *dst, *val);
      *++dst = gretain(res), ++*dst_n;
      val += val_s;
    }
  }
  
  retmethod(_arr);
endmethod

defmethod(OBJ, graccumulate, Array, Functor)
  U32 size = self->size;
  struct Array* arr = Array_alloc(size);
  OBJ _arr = gautoDelete( (OBJ)arr );

  I32  val_s = self->stride;
  OBJ *val   = self->object + val_s*size;
  U32 *dst_n  = &arr->size;
  OBJ *dst   = arr->object;
  OBJ *end   = dst + size;
  OBJ  res;

  if (dst != end) {
    val -= val_s;
    *--end = gretain(*val), ++*dst_n;

    while (dst != end) {
      val -= val_s;
      res = geval(_2, *val, *end);
      *--end = gretain(res), ++*dst_n;
    }
  }
  
  retmethod(_arr);
endmethod

defmethod(OBJ, gaccumulate1, Array, Functor, Object)
  U32 size = self->size;
  struct Array* arr = Array_alloc(size+1);
  OBJ _arr = gautoDelete( (OBJ)arr );

  I32  val_s = self->stride;
  OBJ *val   = self->object;
  U32 *dst_n  = &arr->size;
  OBJ *dst   = arr->object;
  OBJ *end   = dst + size;
  OBJ  res;

  *dst = gretain(_3), ++*dst_n;
  
  while (dst != end) {
    res = geval(_2, *dst, *val);
    *++dst = gretain(res), ++*dst_n;
    val += val_s;
  }

  retmethod(_arr);
endmethod

defmethod(OBJ, graccumulate1, Array, Functor, Object)
  U32 size = self->size;
  struct Array* arr = Array_alloc(size+1);
  OBJ _arr = gautoDelete( (OBJ)arr );

  I32  val_s = self->stride;
  OBJ *val   = self->object + val_s*self->size;
  U32 *dst_n  = &arr->size;
  OBJ *dst   = arr->object;
  OBJ *end   = dst + size;
  OBJ  res;

  *end = gretain(_3), ++*dst_n;

  while (dst != end) {
    val -= val_s,
    res = geval(_2, *val, *end);
    *--end = gretain(res), ++*dst_n;
  }

  retmethod(_arr);
endmethod

defmethod(OBJ, gaccumulate2, Array, Array, Functor, Object)
  U32 size = self->size < self2->size ? self->size : self2->size;
  struct Array* arr = Array_alloc(size+1);
  OBJ _arr = gautoDelete( (OBJ)arr );

  I32  val_s  = self->stride;
  OBJ *val    = self->object;
  I32  val2_s = self2->stride;
  OBJ *val2   = self2->object;
  U32 *dst_n  = &arr->size;
  OBJ *dst    = arr->object;
  OBJ *end    = dst + size;
  OBJ  res;

  *dst = gretain(_4), ++*dst_n;
  
  while (dst != end) {
    res = geval(_3, *dst, *val, *val2);
    *++dst = gretain(res), ++*dst_n;
    val  += val_s;
    val2 += val2_s;
  }

  retmethod(_arr);
endmethod

defmethod(OBJ, graccumulate2, Array, Array, Functor, Object)
  U32 size = self->size < self2->size ? self->size : self2->size;
  struct Array* arr = Array_alloc(size+1);
  OBJ _arr = gautoDelete( (OBJ)arr );

  I32  val_s  = self->stride;
  OBJ *val    = self->object + val_s*self->size;
  I32  val2_s = self2->stride;
  OBJ *val2   = self2->object + val2_s*self2->size;
  U32 *dst_n  = &arr->size;
  OBJ *dst    = arr->object;
  OBJ *end    = dst + size;
  OBJ  res;

  *end = gretain(_4), ++*dst_n;

  while (dst != end) {
    val  -= val_s,
    val2 -= val2_s,
    res = geval(_3, *val, *val2, *end);
    *--end = gretain(res), ++*dst_n;
  }

  retmethod(_arr);
endmethod

// ----- unique (remove contiguous duplicates)

defmethod(OBJ, gunique, Array, Functor)
  U32 size = self->size;
  OBJ _arr = gautoDelete(gnewWith(Array,aInt(size)));
  struct Array* arr = STATIC_CAST(struct Array*, _arr);

  I32  val_s = self->stride;
  OBJ *val   = self->object;
  U32 *dst_n = &arr->size;
  OBJ *dst   = arr ->object;
  OBJ *end   = val + val_s*size;

  if (val != end) {
    *dst = gretain(*val);
    val += val_s;

    while (val != end) {
      if (geval(_2, *dst, *val) != True)
        *++dst = gretain(*val), ++*dst_n;
      val += val_s;
    }
  }

  retmethod(gadjust(_arr));
endmethod

// ----- diff (asymetric diff, self1 - self2)

defmethod(OBJ, gdiff, Array, Container, Functor)
  U32 size = self->size;
  OBJ _arr = gautoDelete(gnewWith(Array,aInt(size)));
  struct Array* arr = STATIC_CAST(struct Array*, _arr);

  I32  val_s  = self->stride;
  OBJ *val    = self->object;
  U32 *dst_n  = &arr->size;
  OBJ *dst    = arr->object;
  OBJ *end    = val + val_s*size;

  while (val != end) {
    if (gfind(_2, aFun(geval2, _3, *val, 0)) == Nil)
      *dst++ = gretain(*val), ++*dst_n;
    val += val_s;
  }

  retmethod(gadjust(_arr));
endmethod

// ----- finding

static OBJ*
findFun(OBJ *val, U32 val_n, I32 val_s, OBJ _2)
{
  if (!val_n) return 0;

  OBJ res = geval(_2, *val); // compare first element

  if (res == True || res == Equal) // found
    return val;

  // linear search
  if (res == False) {
    OBJ *end = val + val_s*val_n;

    val += val_s;
    while (val != end) {
      if (geval(_2, *val) == True) // found
        return val;
      val += val_s;
    }

    return 0; // not found
  }

  // binary search
  if (res == Greater) {
    U32 lo = 1, hi = val_n-1;

    while (lo <= hi) {
      U32 i = (lo + hi) / 2;
      res = geval(_2, val[i*val_s]);

      if (res == Equal)
        return val + i*val_s; // found

      if (res == Lesser)
        hi = i-1;
      else
        lo = i+1;
    }
  }
  
  return 0; // not found
}

// ---

defmethod(OBJ, gfind, Array, Functor)
  U32  val_n = self->size;
  I32  val_s = self->stride;
  OBJ *val   = self->object;

  OBJ *p = findFun(val, val_n, val_s, _2);

  retmethod(p ? *p : Nil);  
endmethod

defmethod(OBJ, gifind, Array, Functor)
  U32  val_n = self->size;
  I32  val_s = self->stride;
  OBJ *val   = self->object;

  OBJ *p = findFun(val, val_n, val_s, _2);

  retmethod(p ? gautoDelete( aInt((p-val)/val_s) ) : Nil);
endmethod

// ----- sorting (in place)

#undef  NETSORT
#define NETSORT(a,s,r,ret) \
  do { \
    switch (r) { \
    case 1: \
      SORT(a[0*s],a[1*s]); \
      ret; \
    \
    case 2: \
      SORT(a[0*s],a[1*s]); \
      SORT(a[0*s],a[2*s]); \
      SORT(a[1*s],a[2*s]); \
      ret; \
    \
    case 3: \
      SORT(a[0*s],a[2*s]); SORT(a[1*s],a[3*s]); \
      SORT(a[0*s],a[1*s]); SORT(a[2*s],a[3*s]); \
      SORT(a[1*s],a[2*s]); \
      ret; \
    \
    case 4: \
      SORT(a[0*s],a[4*s]); SORT(a[1*s],a[3*s]); \
      SORT(a[0*s],a[2*s]); \
      SORT(a[2*s],a[4*s]); SORT(a[0*s],a[1*s]); \
      SORT(a[2*s],a[3*s]); SORT(a[1*s],a[4*s]); \
      SORT(a[1*s],a[2*s]); SORT(a[3*s],a[4*s]); \
      ret; \
    \
    case 5: \
      SORT(a[0*s],a[4*s]); SORT(a[1*s],a[5*s]); \
      SORT(a[0*s],a[2*s]); SORT(a[1*s],a[3*s]); \
      SORT(a[2*s],a[4*s]); SORT(a[3*s],a[5*s]); \
      SORT(a[0*s],a[1*s]); SORT(a[4*s],a[5*s]); \
      SORT(a[1*s],a[4*s]); SORT(a[2*s],a[3*s]); \
      SORT(a[1*s],a[2*s]); SORT(a[3*s],a[4*s]); \
      ret; \
    } \
  } while (0)

#undef  EXCH
#undef  SORT
#define EXCH(a,b) (t=(a),(a)=(b),(b)=t)
#define SORT(a,b) if (GCMP(b,a) == Lesser) EXCH(a,b)

static inline U32
pivot(void)
{
  static U32 x = 1; // no need to be thread safe
  
  return x = x * 2621124293u + 1;
}

/* from "Quicksort Is Optimal", R. Sedgwick & J. Bentley, 2002
   plus some practical improvements.
 */

#undef  GCMP
#define GCMP(a,b) geval(fun,a,b)

static void
qsortFun(OBJ a[], I32 r, OBJ fun)
{
  I32 i, j, p, q;
  OBJ ri, rj;
  OBJ t;

  // nothing to do
  if (r <= 0) return;

  // optimized sort for small sizes
  NETSORT(a,1,r,return);

  // select pivot as the median-of-three taken pseudo-randomly
  i = pivot() % (r+1) + 0, EXCH(a[i],a[0  ]);
  i = pivot() % (r  ) + 1, EXCH(a[i],a[r  ]);
  i = pivot() % (r-1) + 1, EXCH(a[i],a[r-1]);
  SORT(a[0],a[r-1]);
  if ((ri = GCMP(a[r  ],a[0])) == Lesser) EXCH(a[r],a[0  ]);
  if ((rj = GCMP(a[r-1],a[r])) == Lesser) EXCH(a[r],a[r-1]);

  // partitioning initialization
  i = 0, j = r-1;
  p = ri == Equal ? i : -1;
  q = rj == Equal ? j :  r;

  // three-way partitioning
  for (;;) {
    while ((rj = GCMP(a[++i],a[  r])) == Lesser     ) ;
    while ((ri = GCMP(a[  r],a[--j])) == Lesser && j) ;

    if (i >= j) break;

    EXCH(a[i], a[j]);
    if (ri == Equal) ++p, EXCH(a[p],a[i]);
    if (rj == Equal) --q, EXCH(a[q],a[j]);
  }

  // move pivot to center
  EXCH(a[i], a[r]);

  // move equal partition from borders to center
  for (j = i-1; p-- > 0; j--) EXCH(a[p],a[j]);
  for (i = i+1; ++q < r; i++) EXCH(a[q],a[i]);

  // divide & conquer (small first)
  OBJ *small, *large;
  
  if (j < r-i)
    small = a, large = a+i, p = j, q = r-i;
  else
    large = a, small = a+i, q = j, p = r-i;

  qsortFun(small,p,fun);
  qsortFun(large,q,fun); // tail recursion
}

static void
qsortSFun(OBJ a[], I32 r, I32 s, OBJ fun)
{
  I32 i, j, p, q;
  OBJ ri, rj;
  OBJ t;

  // nothing to do
  if (r <= 0) return;

  // optimized sort for small sizes
  NETSORT(a,s,r,return);

  // select pivot as the median-of-three taken pseudo-randomly
  i = pivot() % (r+1) + 0, EXCH(a[i*s],a[ 0   *s]);
  i = pivot() % (r  ) + 1, EXCH(a[i*s],a[ r   *s]);
  i = pivot() % (r-1) + 1, EXCH(a[i*s],a[(r-1)*s]);
  SORT(a[0*s],a[(r-1)*s]);
  if ((ri = GCMP(a[ r   *s],a[0*s])) == Lesser) EXCH(a[r*s],a[ 0   *s]);
  if ((rj = GCMP(a[(r-1)*s],a[r*s])) == Lesser) EXCH(a[r*s],a[(r-1)*s]);

  // partitioning initialization
  i = 0, j = r-1;
  p = ri == Equal ? i : -1;
  q = rj == Equal ? j :  r;

  // three-way partitioning
  for (;;) {
    while ((rj = GCMP(a[++i*s],a[  r*s])) == Lesser     ) ;
    while ((ri = GCMP(a[  r*s],a[--j*s])) == Lesser && j) ;

    if (i >= j) break;

    EXCH(a[i*s], a[j*s]);
    if (ri == Equal) ++p, EXCH(a[p*s],a[i*s]);
    if (rj == Equal) --q, EXCH(a[q*s],a[j*s]);
  }

  // move pivot to center
  EXCH(a[i*s], a[r*s]);

  // move equal partition from borders to center
  for (j = i-1; p-- > 0; j--) EXCH(a[p*s],a[j*s]);
  for (i = i+1; ++q < r; i++) EXCH(a[q*s],a[i*s]);

  // divide & conquer (small first)
  OBJ *small, *large;
  
  if (j < r-i)
    small = a, large = a+i*s, p = j, q = r-i;
  else
    large = a, small = a+i*s, q = j, p = r-i;

  qsortSFun(small,p,s,fun);
  qsortSFun(large,q,s,fun); // tail recursion
}

// ----- indirect sorting (return permutation)

#undef  GCMP
#define GCMP(a,b) geval(fun,o[a*s],o[b*s])

static void
iqsortSFun(I32 a[], I32 r, OBJ o[], I32 s, OBJ fun)
{
  I32 i, j, p, q, t;
  OBJ ri, rj;

  // nothing to do
  if (r <= 0) return;

  // optimized sort for small sizes
  NETSORT(a,1,r,return);

  // select pivot as the median-of-three taken pseudo-randomly
  i = pivot() % (r+1) + 0, EXCH(a[i],a[0  ]);
  i = pivot() % (r  ) + 1, EXCH(a[i],a[r  ]);
  i = pivot() % (r-1) + 1, EXCH(a[i],a[r-1]);
  SORT(a[0],a[r-1]);
  if ((ri = GCMP(a[r  ],a[0])) == Lesser) EXCH(a[r],a[0  ]);
  if ((rj = GCMP(a[r-1],a[r])) == Lesser) EXCH(a[r],a[r-1]);

  // partitioning initialization
  i = 0, j = r-1;
  p = ri == Equal ? i : -1;
  q = rj == Equal ? j :  r;

  // three-way partitioning
  for (;;) {
    while ((rj = GCMP(a[++i],a[  r])) == Lesser     ) ;
    while ((ri = GCMP(a[  r],a[--j])) == Lesser && j) ;

    if (i >= j) break;

    EXCH(a[i], a[j]);
    if (ri == Equal) ++p, EXCH(a[p],a[i]);
    if (rj == Equal) --q, EXCH(a[q],a[j]);
  }

  // move pivot to center
  EXCH(a[i], a[r]);

  // move equal partition from borders to center
  for (j = i-1; p-- > 0; j--) EXCH(a[p],a[j]);
  for (i = i+1; ++q < r; i++) EXCH(a[q],a[i]);

  // divide & conquer (small first)
  I32 *small, *large;
  
  if (j < r-i)
    small = a, large = a+i, p = j, q = r-i;
  else
    large = a, small = a+i, q = j, p = r-i;

  iqsortSFun(small,p,o,s,fun);
  iqsortSFun(large,q,o,s,fun); // tail recursion
}

// ----- sorting methods

defmethod(OBJ, gsort, Array, Functor)
  qsortFun(self->object, self->size-1, _2);
  retmethod(_1);
endmethod

defmethod(OBJ, gsort, ArrayView, Functor)
  struct Array *arr = &self->Array;

  if (arr->stride == 1) {
    qsortFun(arr->object, arr->size-1, _2);
    retmethod(_1);
  }

  if (arr->stride == -1) { // should be faster than strided sort
    qsortFun(arr->object-arr->size+1, arr->size-1, _2);
    retmethod(greverse(_1));
  }

  qsortSFun(arr->object, arr->size-1, arr->stride, _2);
  retmethod(_1);
endmethod

defmethod(OBJ, gisort, Array, Functor)
  useclass(IntVector);

  OBJ _arr = gautoDelete(gnewWith(IntVector, aSlice(0,self->size,1)));
  struct IntVector *arr = STATIC_CAST(struct IntVector*, _arr);

  iqsortSFun(arr->value, arr->size-1, self->object, self->stride, _2);

  retmethod(_arr);
endmethod

// ----- is sorted predicate

defmethod(OBJ, gisSorted, Array, Functor)
  U32 size = self->size;
  if (size < 2) retmethod(True);

  I32  val_s = self->stride;
  OBJ *val   = self->object;
  OBJ *end   = val + val_s*(size-1);

  while (val != end) {
    if (geval(_2, *val, val[val_s]) == Greater)
      retmethod(False);
    val += val_s;
  }

  retmethod(True);
endmethod

