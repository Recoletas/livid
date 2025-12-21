#ifndef RETURN_H
#define RETURN_H

#include <any>

struct ReturnException {
    const std::any value;
};

#endif