/*
 *
 * Copyright (c) 2023 Grigoriy EFimov <efimov-gm@newpage.xyz>
 * Licensed under the MIT license
 *
 */

#include <stdio.h>
#include <time.h>

#include "logger.h"

FILE *LOGFILE_FD;

void get_current_datetime(char *buffer) {
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);
  strftime(buffer, 30, "%F %T %Z", &tm);
}

void write_log(const char *error_text) {
  if (LOGFILE_FD != NULL) {
    char datebuffer[30];
    get_current_datetime(datebuffer);

    fprintf(LOGFILE_FD, "[%s]: %s\n", datebuffer, error_text);
    fflush(LOGFILE_FD);
  }
}