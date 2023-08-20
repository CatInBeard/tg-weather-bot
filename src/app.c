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
#include "openweather_api.h"
#include "logger.h"

bool answer_new_message(const char* TG_TOKEN, const char* OW_TOKEN){
    
    tg_text_message msg;

    if(!get_new_text_message(TG_TOKEN, &msg)){
        return false;
    }
    
    TG_OFFSET = msg.offset+1;

    if(LOGFILE_FD != NULL){
        char buffer [50];
        sprintf(buffer, "New message from chat_id %ld, offset %ld", msg.chat_id, TG_OFFSET);
        write_log(buffer);
        
    }

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

    city_weather cw;

    if(get_weather_by_city(OW_TOKEN, msg.text, &cw)){   

        char* msgbuff = format_current_weather_message(&cw); 

        send_simple_message_to_chat(TG_TOKEN, msg.chat_id , msgbuff); 

	free(msgbuff);
    }
    else{

        send_simple_message_to_chat(TG_TOKEN, msg.chat_id , "City not found!"); 

    }



    //send_simple_message_to_chat(TG_TOKEN, msg.chat_id , msg.text); 

    free(msg.text);
    
    return true;
}


void app_run(const char* TG_TOKEN, const char* OW_TOKEN){

    for(;;){
        answer_new_message(TG_TOKEN, OW_TOKEN);
        printf("Current offset: %ld\n", TG_OFFSET);
    }

}

