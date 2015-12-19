//
// Created by Philip Heimböck (Privat) on 09.12.15.
//

#include "general.h"

#include <stdarg.h>
#include <stdio.h>


void error_message(std::string message, ...) {
    va_list argptr;
    va_start(argptr, message);
    vfprintf(stderr, message.c_str(), argptr);
    va_end(argptr);
}
