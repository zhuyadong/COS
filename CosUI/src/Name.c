#include <string.h>

#include <cos/Name.h>
#include <cos/String.h>
#include <cos/gen/relop.h>
#include <cos/gen/object.h>
#include <libwapcaplet/libwapcaplet.h>

makclass(Name);
useclass(Name);

defmethod(OBJ, ginit, Name)
	I32 rc = cos_object_rc(_1);
	if (rc >= COS_RC_STATIC)
		retmethod(_1);
	cos_object_setRc(_1, NAME_RC2COS(rc));
	if (self->astr) {
		lwc_intern_string((STR)self->astr, strlen((STR)self->astr), &self->astr);
	}
	retmethod(_1);
endmethod

defmethod(OBJ, ginitWith, Name, Name)
	ginit(_2);
	self->astr = self2->astr;
	retmethod(_1);
endmethod

defmethod(STR, gstr, Name)
	ginit(_1);
	retmethod(lwc_string_data((lwc_string*)self->astr));
endmethod

defmethod(OBJ, gretain, Name)
	ginit(_1);
	next_method(self);
	retmethod(_1);
endmethod

defmethod(U64, ghash, Name)
	ginit(_1);
	retmethod((U64)self->astr);
endmethod

