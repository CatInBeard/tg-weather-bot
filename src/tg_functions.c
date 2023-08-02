#include <curl/curl.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "tg_functions.h"

size_t noop_cb(void *ptr, size_t size, size_t nmemb, void *data) {
  return size * nmemb;
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

    char link_buffer[250] = "https://api.telegram.org/bot";
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

        printf("Result code : %d", res);

    } 

    errno = 2;
    return false;
}
