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
#include <json-c/json.h>


#include "curl_functions.h"
#include "tg_functions.h"
#include "tg_text_message.h"

#define JSON_FIND_HELPER if(!find){\
        json_object_put(root);\
        return false;\
    }


long TG_OFFSET=0;

bool check_tg_token(const char* token){

    size_t token_length = 0;
    token_length = strlen(token);

    if(token_length <40 || token_length > 60){
        // Not sure that token length is constant
        // In my case token length was 46
        errno = 1;
        return false;
    }

    char link_buffer[100];
    sprintf(link_buffer,"https://api.telegram.org/bot%s/getMe", token);

    if(simple_get(link_buffer)){
        return true;
    }

    errno = 2;
    return false;
}

bool simple_longpoll_get_updates(const char* token, mem_buff* result,const unsigned long offset){
    return longpoll_get_updates(token, result, 25, 30, 1, offset);
}

bool longpoll_get_updates(const char* token, mem_buff* mb, const unsigned int timeout, const unsigned int curl_timeout, const unsigned int events_limit, const unsigned long offset){
    // Before usage, check token with check_tg_token function to prevent buffer overflow
    
    char link_buffer[350];
    
    sprintf(link_buffer, "https://api.telegram.org/bot%s/getUpdates?timeout=%d&limit=%d&offset=%lu", token,timeout,events_limit,offset);


    
    return get_with_responce(link_buffer, mb, curl_timeout);

}

bool send_simple_message_to_chat(const char* token, long chat_id, const char* message_text){

    char* escaped_message_text = escape_link(message_text);


    size_t token_length = strlen(token);

    size_t alloc_size = 145 + token_length + strlen(escaped_message_text); // 28 + 27 = 125
                               // 28 - base url
                               // 27 - get params
                               // 70 - max token length
                               // max long length


    char* link_buffer = malloc(alloc_size);

    sprintf(link_buffer, "https://api.telegram.org/bot%s/sendMessage?chat_id=%ld/&text=%s", token, chat_id, escaped_message_text);
    
    curl_free(escaped_message_text);

    return simple_get(link_buffer);
}

bool parse_new_messages(mem_buff *mb, tg_text_message* message){

    json_bool find = false;

    struct json_object* root = json_tokener_parse(mb->buffer);
    struct json_object* result_array;
    find = json_object_object_get_ex(root, "result", &result_array);
    JSON_FIND_HELPER
    struct json_object* result_element = json_object_array_get_idx(result_array, 0);

    if(!result_element){
        return false;
    }

    struct json_object* message_obj;
    find = json_object_object_get_ex(result_element, "message", &message_obj);
    JSON_FIND_HELPER
    struct json_object* message_text_obj;
    json_object_object_get_ex(message_obj, "text", &message_text_obj);
    const char* message_text = json_object_get_string(message_text_obj);

    struct json_object* chat_obj;
    find = json_object_object_get_ex(message_obj, "chat", &chat_obj);
    JSON_FIND_HELPER
    struct json_object* chat_id_obj;
    find = json_object_object_get_ex(chat_obj, "id", &chat_id_obj);
    JSON_FIND_HELPER

    long chat_id = json_object_get_int(chat_id_obj);

    struct json_object* update_id_obj;
    find = json_object_object_get_ex(result_element, "update_id", &update_id_obj);
    JSON_FIND_HELPER

    long offset = json_object_get_int64(update_id_obj);

    message->text = malloc(strlen(message_text));
    strcpy(message->text, message_text);
    message->offset = offset;
    message->chat_id = chat_id; 

    json_object_put(root);
    
    return true;
}


bool get_new_text_message(const char* TG_TOKEN, tg_text_message* msg){

    // You must free msg.text after success new message

    mem_buff mb;
    mb.buffer = malloc(1);
    mb.size = 1;

    simple_longpoll_get_updates(TG_TOKEN, &mb, TG_OFFSET); 

    
    bool parsed = parse_new_messages(&mb, msg);

    free(mb.buffer);

    return parsed;
}
