#ifndef COS_NAME_H
#define COS_NAME_H

#include <cos/Object.h>
#include <cos/gen/value.h>

defclass(Name)
	void *astr;
endclass

#define NAME_RC_AUTO ((I32)-100)
#define NAME_RC_STATIC ((I32)-101)
#define NAME_RC2COS(rc) (rc+100)

#define aName(...) ((OBJ)atName  (NAME_RC_AUTO,   __VA_ARGS__))
#define sName(...) ((OBJ)atName  (NAME_RC_STATIC, __VA_ARGS__))

#define atName(rc, cstr) \
	( &(struct Name) { {{COS_CLS_NAME(Name).Behavior.id, rc}}, (cstr) })

#endif
