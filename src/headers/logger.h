#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>

extern FILE* LOGFILE_FD;

void write_log(const char* error_text);

#endif