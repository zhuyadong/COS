#include <cos/signal.h>
#include <cos/String.h>
#include <cos/Number.h>

#include <cos/gen/object.h>
#include <cos/gen/accessor.h>
#include <cos/gen/sequence.h>
#include <cos/gen/value.h>
#include <cos/gen/map.h>

#include <stdio.h>
#include <stdlib.h>
useclass(Map, AutoRelease);

int main(int argc, char **argv)
{
    if (argc < 2) {
        printf("no argument!");
        exit(1);
    }
	cos_init();
	cos_signal_std();
	OBJ pool = gnew(AutoRelease);

	OBJ map = gnew(Map);

    int count = atoi(argv[1]);
	for (int i=0; i<count; ++i) {
		gputAt(map, aInt(i%1023), aInt(i%1023));
	}

	OBJ clone = gnewWith(Map, map);

	for (int i=0; i<1023; ++i) {
		printf("%d\n", gint(ggetAt(clone, aInt(i))));
	}


	printf("capacity:%d\nsize:%d\n", gcapacity(map), gsize(map));

	grelease(pool);
	return 0;
}
