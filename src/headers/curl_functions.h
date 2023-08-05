#ifndef CURL_FUNCTIONS_H
#define CURL_FUNCTIONS_H

#include <stdbool.h>

#include "mem_buff.h"

size_t noop_cb(void *ptr, size_t size, size_t nmemb, void *data); 
size_t char_buff_copy_cb(void *ptr, size_t size, size_t nmemb, mem_buff* mb);
char* escape_link(const char* link);
bool simple_get(char* link);
bool get_with_responce(const char* link, mem_buff* mb, const unsigned int curl_timeout);


#endif
