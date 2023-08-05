/*
 *
 * Copyright (c) 2023 Grigoriy EFimov <efimov-gm@newpage.xyz>
 * Licensed under the MIT license
 * 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <json-c/json.h>

#include "tg_functions.h"
#include "mem_buff.h"
#include "tg_text_message.h"

long TG_OFFSET=0;

bool parse_new_messages(mem_buff *mb, tg_text_message* message){

    json_bool find = false;

    struct json_object* root = json_tokener_parse(mb->buffer);
    struct json_object* result_array;
    find = json_object_object_get_ex(root, "result", &result_array);
    if(!find){
        return false;
    }
    struct json_object* result_element = json_object_array_get_idx(result_array, 0);

    if(!result_element){
        return false;
    }

    struct json_object* message_obj;
    find = json_object_object_get_ex(result_element, "message", &message_obj);
    if(!find){
        return false;
    }
    struct json_object* message_text_obj;
    json_object_object_get_ex(message_obj, "text", &message_text_obj);
    const char* message_text = json_object_get_string(message_text_obj);

    struct json_object* chat_obj;
    find = json_object_object_get_ex(message_obj, "chat", &chat_obj);
    if(!find){
        return false;
    }
    struct json_object* chat_id_obj;
    find = json_object_object_get_ex(chat_obj, "id", &chat_id_obj);
    if(!find){
        return false;
    }

    long chat_id = json_object_get_int(chat_id_obj);

    struct json_object* update_id_obj;
    find = json_object_object_get_ex(result_element, "update_id", &update_id_obj);
    if(!find){
        return false;
    }

    long offset = json_object_get_int64(update_id_obj);

    message->text = malloc(strlen(message_text));
    strcpy(message->text, message_text);
   
    message->offset = offset;
    message->chat_id = chat_id; 

    json_object_put(root);
    
    return true;
}

bool answer_new_message(const char* TG_TOKEN){
    
    mem_buff mb;
    mb.buffer = malloc(1);
    mb.size = 1;

    simple_longpoll_get_updates(TG_TOKEN, &mb, TG_OFFSET); 

    tg_text_message msg;
    bool parsed = parse_new_messages(&mb, &msg);

    if(!parsed){
        return false;
    }
    
    TG_OFFSET = msg.offset+1;

    send_simple_message_to_chat(TG_TOKEN, msg.chat_id , msg.text);


    free(msg.text);
    free(mb.buffer);
    mb.size = 0;

    return true;
}


void app_run(const char* TG_TOKEN){

    for(;;){
        answer_new_message(TG_TOKEN);
        printf("Current offset: %ld\n", TG_OFFSET);
    }
}

