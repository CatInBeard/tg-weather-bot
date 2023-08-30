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
  sprintf(buffer, "%d-%02d-%02d %02d:%02d:%02d", tm.tm_year + 1900,
          tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
}

void write_log(const char *error_text) {
  if (LOGFILE_FD != NULL) {
    char datebuffer[20] = ""; // max date text length with \0 = 20
    get_current_datetime(datebuffer);

    fprintf(LOGFILE_FD, "[%s]: %s\n", datebuffer, error_text);
    fflush(LOGFILE_FD);
  }
}