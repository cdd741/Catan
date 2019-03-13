#ifndef STATUS_H
#define STATUS_H

#include <iostream>

enum class Status { OK, cantBuildHere, notEnoughResources, cantPutHere};

std::ostream &operator<<(std::ostream &out, const Status &s);

#endif // !STATUS_H]

