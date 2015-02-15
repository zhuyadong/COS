#ifndef COS_GEN_MAP_H
#define COS_GEN_MAP_H

/**
 * C Object System
 * COS generics for map
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

#include <cos/Object.h>

defgeneric(OBJ, gkeyIterator, _1);
defgeneric(OBJ, gvalIterator, _1);
defgeneric(OBJ, gkeysForVal,  _1, val); // all keys that mapto val
defgeneric(OBJ, gallKeys,     _1);
defgeneric(OBJ, gallVals,     _1);

#endif // COS_GEN_MAP_H
