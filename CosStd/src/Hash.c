/**
 * C Object System
 * COS hash
 *
 * Copyright 2015+ ZhuYaDong <aeix@foxmail.com>
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
#include <cos/String.h>
#include <cos/Number.h>
#include <cos/gen/relop.h>
#include "xxhash.h"

defmethod(U64, ghash, Object)
	// hash object address
	retmethod(XXH64((const void*)&self, sizeof(void*), 0));
endmethod

defmethod(U64, ghash, String)
	retmethod(XXH64(self->value, self->size, 0));
endmethod

#define DEF_NUM_HASH(cls) \
	defmethod(U64, ghash, cls) \
		retmethod(XXH64((const void*)&self->value, sizeof(self->value), 0)); \
	endmethod

DEF_NUM_HASH(Int)
DEF_NUM_HASH(Long)
DEF_NUM_HASH(Float)
DEF_NUM_HASH(Double)
DEF_NUM_HASH(Complex)
