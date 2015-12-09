//
// Created by Philip Heimböck (Privat) on 09.12.15.
//

#include <stdio.h>
#include "general.h"
#include <stdarg.h>


void error_message(const char* message, ...) {
    va_list argptr;
    va_start(argptr, message);
    vfprintf(stderr, message, argptr);
    va_end(argptr);
}
