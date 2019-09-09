#include <time.h>

#include "util.h"

struct timespec dur;
dur.tv_sec = time_t 1;

int main()
{
    busy_wait(dur);
}
