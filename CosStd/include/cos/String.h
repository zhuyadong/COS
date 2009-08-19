#ifndef COS_STRING_H
#define COS_STRING_H

/*
 o---------------------------------------------------------------------o
 |
 | COS String, Dynamic String, Lazy String and String View
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
 | $Id: String.h,v 1.2 2009/08/17 12:57:12 ldeniau Exp $
 |
*/

#include <cos/Sequence.h>

/* NOTE-USER: String class cluster constructors

   aString    (cstring)            -> Block string    (automatic)
   aStringRef (buffer,size)        -> String          (automatic)
   aStringView(string,slice)       -> String view     (automatic)

   gnewWith (String,string)        -> Block string    (clone)
   gnewWith2(String,size,chr)      -> Block string    (element)
   gnewWith2(String,size,fun)      -> Block string    (generator)
   gnewWith2(String,string,slice)  -> Block string    (substring)
   gnewWith2(String,string,range)  -> Block string    (substring)
   gnewWith2(String,string,intvec) -> Block string    (sequence)

   gnew     (String)               -> Dynamic string
   gnewWith (String,capacity)      -> Dynamic string  (pre-allocated)

   gnewWith (String,fun)           -> Lazy string     (generator)
   gnewWith2(String,fun,string)    -> Lazy string     (generator)

   gnewWith2(View,string,slice)    -> String view     (view)
   gnewWith2(View,string,range)    -> String view     (view)

   where:
   - All strings are mutable
   - String are not sliced (ignored in views construction)
   - Dynamic strings can shrink and grow (gappend, gpreprend)
   - Dynamic strings can be converted to fixed string (gfix, gadjust)
   - Lazy strings are dynamic strings growing automatically using a generator
   - String views work only on non-dynamic strings
   - String views clone are block strings (copy), not views
*/

defclass(String, ValueSequence)
  U8* value;
  U32 size;
endclass

// ----- automatic constructors

#define aString(...)     ( (OBJ)atString    (__VA_ARGS__) )
#define aStringRef(...)  ( (OBJ)atStringRef (__VA_ARGS__) )
#define aStringView(...) ( (OBJ)atStringView(__VA_ARGS__) )

// --- shortcuts

#ifndef COS_NOSHORTCUT

#define aStr(...)  aString(__VA_ARGS__)

#endif

/***********************************************************
 * Implementation (private)
 */

#include <string.h>

// ----- Block string

defclass(StringN, String)
  U8 _value[];
endclass

// ----- Fixed string, Dynamic string and Lazy string

defclass(StringFix, String)
  U8* _value;
  U32 _cls;
  U32 capacity;
endclass

defclass(StringDyn, StringFix)
endclass

defclass(StringLzy, StringDyn)
  OBJ generator;
  I32 arity;
endclass

// ----- String view

defclass(StringView, String)
  struct String *string;
endclass

// ----- initializers, allocators and utilities (for the class cluster)

struct Slice;
struct String* String_alloc(U32);
struct String* StringView_init(struct StringView*, struct String*, struct Slice*);

// ----- automatic constructors

#define atString(cstr) ( (struct String*) \
  ( &(struct StringN) {{ {{{ cos_object_auto(StringN) }}}, \
  (U8[]){ cstr }, strlen( cstr ) }} ))

// --- StringRef

#define atStringRef(buffer,size) \
  ( &(struct String) { {{{ cos_object_auto(String) }}}, (buffer), (size) } )

// --- StringView

#define atStringView(string,slice) StringView_init( \
  (&(struct StringView) {{ {{{ cos_object_auto(StringView) }}}, 0, 0 }, 0 }), \
  string,slice)

#endif // COS_ARRAY_H