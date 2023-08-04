/*
 *
 * Copyright (c) 2023 Grigoriy EFimov <efimov-gm@newpage.xyz>
 * Licensed under the MIT license
 * 
*/


#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "tg_functions.h"

size_t noop_cb(void *ptr, size_t size, size_t nmemb, void *data) {
  return size * nmemb;
}

size_t char_buff_copy_cb(void *ptr, size_t size, size_t nmemb, mem_buff* mb){
    
    size_t length = size*nmemb;
    char* new;

    new = realloc(mb->buffer,length+1);
    mb->size = length;

    if(new == NULL){
        fprintf(stderr, "realloc failed!");
        exit(1);
    }

    mb->buffer = new;

    memcpy(mb->buffer, ptr, length);

    mb->buffer[length] = '\0';

    return size*nmemb;

}

bool check_tg_token(const char* token){

    size_t token_length = 0;
    token_length = strlen(token);

    if(token_length <40 || token_length > 60){
        // Not sure that token length is constant
        // In my case token length was 46
        errno = 1;
        return false;
    }

    char link_buffer[100] = "https://api.telegram.org/bot";
    strcpy(link_buffer+28,token); // 28 - link_buffer current length
    strcpy(link_buffer+28+token_length,"/getMe");

    CURL *curl = curl_easy_init();
    if(curl){
        CURLcode res;
        curl_easy_setopt(curl, CURLOPT_URL, link_buffer);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, noop_cb);

        res = curl_easy_perform(curl);

        long http_code = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

        curl_easy_cleanup(curl);

        if(http_code == 200 && res != CURLE_ABORTED_BY_CALLBACK){
            return true;
        }

    } 

    errno = 2;
    return false;
}

bool simple_longpoll_get_updates(const char* token, mem_buff* result,const unsigned long offset){
    return longpoll_get_updates(token, result, 25, 30, 1, offset);
}

bool longpoll_get_updates(const char* token, mem_buff* result, const unsigned int timeout, const unsigned int curl_timeout, const unsigned int events_limit, const unsigned long offset){
    // Before usage, check token with check_tg_token function to prevent buffer overflow
    
    long link_length = 0;
    char int_buffer[20] = ""; // max long (int64) = +9223372036854775807 - 20 chars with sign
    char link_buffer[350] = "https://api.telegram.org/bot";
    
    link_length += 28;  // 28 - link_buffer current length
    strcpy(link_buffer + link_length, token);

    
    link_length += strlen(token);
    strcpy(link_buffer+link_length, "/getUpdates?timeout=");

    
    sprintf(int_buffer, "%d", timeout);
    link_length += 20; // 20 - prev link part length
    strcpy(link_buffer + link_length, int_buffer);


    link_length += strlen(int_buffer);
    strcpy(link_buffer + link_length, "&limit=1");
    
    sprintf(int_buffer, "%d", events_limit);
    link_length += 7; // 7 - prev link part length
    strcpy(link_buffer + link_length, int_buffer);

    link_length+= strlen(int_buffer);
    strcpy(link_buffer + link_length, "&offset=");

    sprintf(int_buffer, "%lu", offset);
    link_length += 8; // 8 - prev link part length
    strcpy(link_buffer + link_length, int_buffer);

    CURL *curl = curl_easy_init();
    if(curl){
        CURLcode res;
        curl_easy_setopt(curl, CURLOPT_URL, link_buffer);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, char_buff_copy_cb);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, result);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, curl_timeout);

        res = curl_easy_perform(curl);

        long http_code = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
        curl_easy_cleanup(curl);

        if(http_code == 200 && res != CURLE_ABORTED_BY_CALLBACK){
            return true;
        }

    } 

    errno = 2;

    
    return false;

}
