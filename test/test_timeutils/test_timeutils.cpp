#include "time/time_utils.h"

#include <iostream>

int main() {
    std::cout << "current time : " << get_current_time() << std::endl;
    std::cout << "0 clock time : " << get_zero_clock_time() << std::endl;
    return 0;
}
