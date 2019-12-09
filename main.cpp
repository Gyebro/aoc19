#include "days.h"

int main() {

#ifdef DAY01
    day01();
    day01(true);
#endif

#ifdef DAY02
    day02();
    day02(true);
#endif

#ifdef DAY03
    day03();
    day03(true);
#endif

#ifdef DAY04
    day04();
    day04(true);
#endif

#ifdef DAY05
    day05();
    day05(true);
#endif

#ifdef DAY06
    day06();
    day06(true);
#endif

#ifdef DAY07
    day07();
    day07(true);
#endif

#ifdef DAY08
    day08();
    day08(true);
#endif

#ifdef DAY09
    day09();
    day09(true);
#endif

    return 0;
}
