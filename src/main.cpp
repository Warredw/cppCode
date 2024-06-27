#include <iostream>
#include "TimeTable.h"

int main() {

    std::cout << "Hello, World!" << std::endl;
    TimeTable timeTable;

    std::cout << "There are " << timeTable.numberSchedulesPossible << " schedules possible for the second half of the tournament";
    return 0;
}
