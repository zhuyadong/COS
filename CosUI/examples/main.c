#include <stdio.h>
#include <cos/Object.h>
#include <cos/Name.h>

#include <cos/gen/object.h>

useclass(Name);

int main()
{
	cos_init();
	{
		OBJ n = aName("aName");
		OBJ rn = gretain(n);
		grelease(rn);
		rn = gnewWith(Name, n);
		printf("%s\n", gstr(rn));
	}
	return 0;
}
