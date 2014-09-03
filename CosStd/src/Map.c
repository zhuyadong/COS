#include <stdint.h>
#include <cos/Map.h>
#include <cos/Number.h>
#include <cos/String.h>

#include <cos/gen/object.h>
#include <cos/gen/relop.h>
#include <cos/gen/value.h>
#include <cos/gen/accessor.h>

/* some default ghash implement */
defmethod(U64, ghash, String)
	STR str = gstr(_1);
	U64 hash = 0;

	if (str && str[0]==0) {
		while (*str)
			hash = (hash << 5) + hash + *str++;
	}
	retmethod(hash);
endmethod

/* borrow from luajit2 */
#if defined(__GNUC__)
#	define cos_fls(x) ((U32)(__builtin_clz(x)^31))
#else
#	error must define cos_fls() under this platform
#endif

#define HASH_BIAS (-0x04c11db7)
#define HASH_ROT1 14
#define HASH_ROT2 5
#define HASH_ROT3 13
#define hash_rol(x,n) (((x)<<(n)) | ((x)>>(-(int)(n)&(8*sizeof(x)-1))))
#define u32ptr(p) ((U32)(intptr_t)(void*)(p))
#define hashptr(p) hashrot(u32ptr(p), u32ptr(p) + HASH_BIAS)
#define hashnum(o) hashrot((o).lo, ((o).hi << 1))
#define hashnode(m,k) (&m->nodes[k & m->mask])
#define hsize2hbits(s)	((s) ? ((s)==1 ? 1 : 1+cos_fls((uint32_t)((s)-1))) : 0)

static cos_inline U32 hashrot(U32 lo, U32 hi)
{
	lo ^= hi; hi  = hash_rol(hi, HASH_ROT1);
	lo -= hi; hi  = hash_rol(hi, HASH_ROT2);
	hi ^= lo; hi -= hash_rol(lo, HASH_ROT3);
	return hi;
}

typedef union _Hash {
		I64 hash;
		U32 lo,hi;
} _Hash;

defmethod(U64, ghash, Any)
	retmethod(hashptr(_1));
endmethod

defmethod(U64, ghash, Int)
	_Hash hash;
	hash.hash = self->value;
	retmethod(hashnum(hash));
endmethod

defmethod(U64, ghash, Long)
	_Hash hash;
	hash.hash = self->value;
	retmethod(hashnum(hash));
endmethod

defmethod(U64, ghash, Float)
	_Hash hash;
	*(F64*)&hash.hash = self->value;
	retmethod(hashnum(hash));
endmethod


/* Map implement */
makclass(Map);

typedef struct _Node {
	OBJ key;
	OBJ val;
	U32 hash;
	struct _Node *next;
} _Node;

_Node* mapset(struct Map*, OBJ, U32);


void mapresize(struct Map *map)
{
	_Node *oldnodes = map->nodes;
	U32 oldmask = map->mask;
	U32 newsize = 1u << hsize2hbits(map->used);

	if (newsize < 32) newsize = 32;

	map->used = 0;
	map->mask = newsize - 1;
	map->nodes = calloc(newsize, sizeof(_Node));

	/* copy nodes from old to new */
	for (U32 i=1; i<=oldmask; ++i) {
		_Node *n = &oldnodes[i];
		if (n->val) {
			_Node *nn = mapset(map, n->key, n->hash);
			nn->val = n->val;
			nn->hash = n->hash;
		}
	}
	free(oldnodes);
}

_Node*
mapnewkey(struct Map *map, OBJ key, U32 hash)
{
	_Node *n;
	if (map->mask == 0)
		ginit((OBJ)map);

	n = hashnode(map, hash);
	if (!n->key)
		return n;

	_Node *base = map->nodes;
	_Node *collide, *freenode = map->free;
	do {
		if (freenode == base) {
			mapresize(map);
			return mapset(map, key, hash);
		}
	} while ((--freenode)->key);
	map->free = freenode;

	collide = hashnode(map, n->hash);
	if (collide != n) {
		while (collide->next != n)
			collide = collide->next;
		collide->next  = freenode;
		freenode->val  = n->val;
		freenode->key  = n->key;
		freenode->hash = n->hash;
		freenode->next = n->next;
		n->next = NULL;
		n->val = NULL;
		while (freenode->next) {
			_Node *nn = freenode->next;
			if (nn->val && hashnode(map, nn->hash) == n) {
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
	n->key = key;
	n->hash = hash;
	++map->used;
	return n;
}

_Node*
mapset(struct Map *map, OBJ key, U32 hash)
{
	_Node *n;
	if (!map->nodes)
		ginit((OBJ)map);
	n = hashnode(map, hash);
	do {
		if (!n->key || gisEqual(n->key, key) == True) {
			if (!n->key) ++map->used;
			return n;
		}
	} while ((n = n->next));
	return mapnewkey(map, key, hash);
}

defmethod(OBJ, ginit, Map)
	U32 nnode;

	if (self->mask == 0) {
		nnode = 32; /* default 32 node */
		self->mask = nnode -1;
	} else {
		nnode = self->mask + 1;
	}

	if (!self->nodes)
		self->nodes = calloc(nnode, sizeof(_Node));
	
	self->free = &self->nodes[nnode];
	retmethod(_1);
endmethod

defmethod(OBJ, ggetAt, Map, Object)
	if (!self->nodes) {
		retmethod(NULL);
	} else {
		U32 hash = ghash(_2);
		_Node *n = hashnode(self, hash);
		do {
			if (n->key && gisEqual(n->key, _2) == True) {
				retmethod(n->val);
			}
		} while ((n = n->next));
		retmethod(NULL);
	}
endmethod

defmethod(OBJ,gputAt, Map, Object, Object)
	U32 hash = ghash(_2);
	_Node *n = mapset(self, _2, hash);
	n->key = gretain(_2);
	n->val = gretain(_3);
	n->hash = hash;
	retmethod(_1);	
endmethod

defmethod(OBJ, ggetAtKey, Map, (STR)key)
	if (!self->nodes) {
		retmethod(NULL);
	} else {
		OBJ okey = aStringRef(key);
		retmethod(ggetAt(_1, okey));
	}
endmethod
