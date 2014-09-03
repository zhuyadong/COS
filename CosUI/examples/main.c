#include <stdio.h>
#include <stdlib.h>
#include <cos/Object.h>
#include <cos/Map.h>
#include <cos/String.h>
#include <cos/Number.h>

#include <cos/gen/object.h>
#include <cos/gen/accessor.h>
#include <cos/gen/value.h>

useclass(Name);

int main(int argc, char **argv)
{
	cos_init();
	{
		OBJ map = aMap();
		I32 count = argc < 2 ? 33 : atoi(argv[1]);
		for (U32 i=0; i<count; ++i) {
			gputAt(map, aInt(i), aInt(i));
		}
		for (U32 i=0; i<count; i+=2) {
			gputAt(map, aInt(i), aInt(999999));
		}
		{
			I32 i555 = gint(ggetAt(map, aInt(30)));
			I32 i556 = gint(ggetAt(map, aInt(31)));
			printf("555=%d 556=%d\n", i555, i556);
		}
	}
	return 0;
}
