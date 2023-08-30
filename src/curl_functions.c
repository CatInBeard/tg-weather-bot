/*
 *
 * Copyright (c) 2023 Grigoriy EFimov <efimov-gm@newpage.xyz>
 * Licensed under the MIT license
 *
 */

#include <curl/curl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "curl_functions.h"

size_t noop_cb(void *ptr __attribute__((unused)), size_t size, size_t nmemb,
               void *data __attribute__((unused))) {
  return size * nmemb;
}

size_t char_buff_copy_cb(void *ptr, size_t size, size_t nmemb, mem_buff *mb) {

  size_t length = size * nmemb;
  char *new;

  new = realloc(mb->buffer, length + 1);
  mb->size = length;

  if (new == NULL) {
    fprintf(stderr, "realloc failed!");
    exit(1);
  }

  mb->buffer = new;

  memcpy(mb->buffer, ptr, length);

  mb->buffer[length] = '\0';

  return size * nmemb;
}

char *escape_link(const char *link) {
  // alloc memory for escaped_link, to free use curl_free() insted of free()
  CURL *curl = curl_easy_init();
  if (curl) {

    char *escaped_link = curl_easy_escape(curl, link, strlen(link));
    curl_easy_cleanup(curl);

    return escaped_link;
  }
  return "\0";
}

bool simple_get(char *link) {

  CURL *curl = curl_easy_init();
  if (curl) {

    CURLcode res;
    curl_easy_setopt(curl, CURLOPT_URL, link);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, noop_cb);

    res = curl_easy_perform(curl);

    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

    curl_easy_cleanup(curl);

    if (http_code >= 200 && http_code < 300 &&
        res != CURLE_ABORTED_BY_CALLBACK) {
      return true;
    }
  }
  return false;
}

bool get_with_responce(const char *link, mem_buff *mb,
                       const unsigned int curl_timeout) {

  CURL *curl = curl_easy_init();
  if (curl) {
    CURLcode res;
    curl_easy_setopt(curl, CURLOPT_URL, link);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, char_buff_copy_cb);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, mb);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, curl_timeout);

    res = curl_easy_perform(curl);

    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
    curl_easy_cleanup(curl);

    if (http_code == 200 && res != CURLE_ABORTED_BY_CALLBACK) {
      return true;
    }
  }

  return false;
}
