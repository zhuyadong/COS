#ifndef COS_MAP_H
#define COS_MAP_H

#include <cos/Object.h>

defclass(Map)
	U32 mask; /* node count mask */
	U32 used; /* used node count */
	struct _Node *free; /* top free node */ 
	struct _Node *nodes; /* allocated nodes */
endclass

#define aMap() \
	( &(struct Map) { cos_object_auto(Map), 0, 0, NULL, NULL } )

#endif
