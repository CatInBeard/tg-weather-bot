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
#include "text.h"

extern long TG_OFFSET;

bool answer_new_message(const char* TG_TOKEN){
    
    tg_text_message msg;

    if(!get_new_text_message(TG_TOKEN, &msg)){
        return false;
    }
    
    TG_OFFSET = msg.offset+1;

    if(strlen(msg.text) >= 6){ // 6 - "/start" length
        
        char buff[7];
        memcpy(buff, msg.text, 6);
        buff[6] = '\0';

        if(strcmp(buff, "/start") == 0){

            send_simple_message_to_chat(TG_TOKEN, msg.chat_id , get_greeting()); 
            free(msg.text);
            return true;

        }        

    }

    send_simple_message_to_chat(TG_TOKEN, msg.chat_id , msg.text); 

    free(msg.text);
    
    return true;
}


void app_run(const char* TG_TOKEN){

    for(;;){
        answer_new_message(TG_TOKEN);
        printf("Current offset: %ld\n", TG_OFFSET);
    }
}

