#include "days.h"

int main() {

#ifdef DAY01
    day01();
    day01(true);
#endif

#ifdef DAY05
    day05();
    day05(true);
#endif
    return 0;
}
