/**
 * C Object System
 * COS Map
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

#include <cos/Map.h>
#include <cos/String.h>
#include <cos/Number.h>

#include <cos/gen/init.h>
#include <cos/gen/map.h>
#include <cos/gen/relop.h>
#include <cos/gen/value.h>
#include <cos/gen/object.h>
#include <cos/gen/accessor.h>
#include <cos/gen/sequence.h>

/* borrow from luajit2 */
#if defined(__GNUC__)
#	define cos_fls(x) ((U32)(__builtin_clzll(x)^63))
#else
#	error must define cos_fls() under this platform
#endif

#define hashnode(m,k) (&m->nodes[k & m->mask])
#define hsize2hbits(s)	((s) ? ((s)==1 ? 1 : 1+cos_fls(((s)-1))) : 0)

makclass(Map);

useclass(Map,Int);

struct _MapNode {
	struct _MapNode *next;
	OBJ key;
	OBJ val;
};

struct _MapNode* mapset(struct Map*, OBJ, U64 hash);

static cos_inline U8
keyequal(OBJ k1, OBJ k2)
{
	if (!k1 || !k2) return 0;
	if (gclass(k1) != gclass(k2)) return 0;
	if (gisEqual(k1,k2) != True) return 0;
	return 1;
}

void mapresize(struct Map *map)
{
	struct _MapNode *oldnodes = map->nodes;
	U64 oldmask = map->mask;
	U64 newsize = 1u << (hsize2hbits(map->used) + 1);

	if (newsize < 32) newsize = 32;

	map->used = 0;
	map->mask = newsize - 1;
	map->nodes = calloc(newsize, sizeof(struct _MapNode));
	map->free = &map->nodes[newsize];

	// copy nodes from old to new
	for (U64 i=0; i<=oldmask; ++i) {
		struct _MapNode *n = &oldnodes[i];
		if (n->val) {
			struct _MapNode *nn = mapset(map, n->key, ghash(n->key));
			nn->val = n->val;
			nn->key = n->key;
		}
	}
	free(oldnodes);
}

struct _MapNode*
mapnewkey(struct Map *map, OBJ key, U64 hash)
{
	struct _MapNode *n = hashnode(map, hash);
	if (!n->key)
		return n;

	struct _MapNode *base = map->nodes;
	struct _MapNode *collide, *freenode = map->free;
	do {
		if (freenode == base) {
			mapresize(map);
			return mapset(map, key, hash);
		}
	} while ((--freenode)->key);
	map->free = freenode;

	collide = hashnode(map, ghash(n->key));
	if (collide != n) {
		while (collide->next != n)
			collide = collide->next;
		collide->next  = freenode;
		freenode->val  = n->val;
		freenode->key  = n->key;
		freenode->next = n->next;
		n->next = NULL;
		n->key  = NULL;
		n->val  = NULL;
		while (freenode->next) {
			struct _MapNode *nn = freenode->next;
			if (nn->val && hashnode(map, ghash(nn->key)) == n) {
				freenode->next = nn->next;
				nn->next = n->next;
				n->next = nn;
			} else {
				freenode = nn;
			}
		}
	} else {
		freenode->next = n->next;
		n->next = freenode;
		n = freenode;
	}
	++map->used;
	return n;
}

struct _MapNode*
mapset(struct Map *map, OBJ key, U64 hash)
{
	struct _MapNode *n = hashnode(map, hash);
	do {
		if (!n->key || keyequal(n->key, key)) {
			if (!n->key) {
				++map->used;
			} else {
				grelease(n->key);
				n->key = NULL;
				if (n->val) {
					grelease(n->val);
					n->val = NULL;
				}
			}
			return n;
		}
	} while ((n = n->next));
	return mapnewkey(map, key, hash);
}
// --- alloc

struct Map*
Map_alloc(U32 capacity)
{
	U64 nnode;
	OBJ _map = gallocWithSize((OBJ)classref(Map), 0);
	struct Map *map = CAST(struct Map*, _map);
	nnode = 1ull<<hsize2hbits(capacity);
	if (nnode < 32)
		nnode = 32;
	map->mask = nnode - 1;
	map->used = 0;
	map->nodes = (struct _MapNode*)calloc(nnode, sizeof(struct _MapNode));
	map->free = &map->nodes[nnode];

	return map;
}

// ----- allocator

defmethod(OBJ, galloc, pmMap) // lazy alloc
	retmethod(_1);
endmethod

// ----- destructor

defmethod(OBJ, gdeinit, Map)
	struct _MapNode *node = self->nodes;
	struct _MapNode *end = self->nodes + self->mask + 1;

	if (node) {
		while (node != end) {
			if (node->key) grelease(node->key);
			if (node->val) grelease(node->val);
			++node;
		}
		free(self->nodes);
	}

	retmethod(_1);
endmethod

// ----- constructors

defmethod(OBJ, ginit, pmMap)
	retmethod(ginitWith(_1, aInt(0)));
endmethod

defalias(OBJ, (ginitWith)gnewWith, pmMap, Map);
defmethod(OBJ, ginitWith, pmMap, Map)
	PRE
		ensure(self2->mask > 0, "map's mask should not <= 0.");
	BODY
		struct Map *map = Map_alloc(self2->mask - 1);
		ensure(map->mask == self2->mask);
		ptrdiff_t d = (char*)map->nodes - (char*)self2->nodes;
		map->free = (struct _MapNode*)((char*)self2->free + d);
		struct _MapNode *n,*n2;
		U64 i;
		for (i=0,n=map->nodes,n2=self2->nodes; i <= self2->mask; ++i, ++n, ++n2) {
			if (n2->key) n->key = gretain(n2->key);
			if (n2->val) n->val = gretain(n2->val);
			if (n2->next) n->next = (struct _MapNode*)((char*)n2->next + d);
		}
		
		retmethod((OBJ)map);
endmethod

defalias(OBJ, (ginitWith)gnewWith, pmMap, Int);
defmethod(OBJ, ginitWith, pmMap, Int)
	retmethod((OBJ)Map_alloc(self2->value));
endmethod

defalias(OBJ, (ginitWithVaArg)gnewWithVaArg, pmMap, (va_list)va);
defmethod(OBJ, ginitWithVaArg, pmMap, (va_list)va)
	OBJ map = gnewWith(Map, aInt(0));
	do {
		OBJ key = va_arg(va, OBJ);
		if (!key) break;
		OBJ val = va_arg(va, OBJ);
		if (!val) break;
		gputAt((OBJ)map, key, val);
	} while(1);

	retmethod((OBJ)map);
endmethod

// properties
defmethod(U32, gcapacity, Map)
	retmethod(self->mask+1);
endmethod

defmethod(U32, gsize, Map)
	retmethod(self->used);
endmethod

// accessor
defmethod(OBJ, ggetAt, Map, Object)
	struct _MapNode *n = hashnode(self, ghash(_2));
	while (n && n->key) {
		if (keyequal(n->key, _2))
			retmethod(n->val ? n->val : Nil);
		n = n->next;
	}
	retmethod(Nil);
endmethod

defmethod(OBJ, ggetAtKey, Map, (STR)key)
	retmethod(ggetAt(_1, aStringRef(key)));
endmethod

defmethod(OBJ, gputAt, Map, Object, Object)
	struct _MapNode *n = mapset(self, _2, ghash(_2));
	if (_3 && _3 != Nil) {
		n->key = gretain(_2);
		n->val = gretain(_3);
	} else {
		// mapset already cleared node,
		// so we only need update used number here
		// TODO:adjust capacity if needed.
		--self->used;
	}
	retmethod(_1);
endmethod

defmethod(OBJ, gputAtKey, Map, (STR)key, Object)
	retmethod(gputAt(_1, aStringRef(key), _2));
endmethod

// sequence

defmethod(OBJ, gadjust, Map)
	if (self->mask > 31) {
		if (self->used <= self->mask >> 1) {
			mapresize(self);
		}
	}
	retmethod(_1);
endmethod
