#ifndef COS_MAP_H
#define COS_MAP_H

#include <cos/Object.h>

defclass(Map)
	U64    mask;    // node count mask
	U64    used;    // used node count
	struct _MapNode *free;  // top free node
	struct _MapNode *nodes; // allocated nodes
endclass

#define aMap() \
	( &(struct Map) { cos_object_auto(Map), 0, 0, NULL, NULL } )

#endif
