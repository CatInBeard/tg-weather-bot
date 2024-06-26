/*
 *
 * Copyright (c) 2023 Grigoriy EFimov <efimov-gm@newpage.xyz>
 * Licensed under the MIT license
 *
 */

#include <json-c/json.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "headers/tg_functions.h"
#include "logger.h"
#include "mem_buff.h"
#include "openweather_api.h"
#include "text.h"
#include "tg_functions.h"

void proccess_new_message(tg_text_message *msg, const char *TG_TOKEN,
                          const char *OW_TOKEN) {

  size_t message_length = strlen(msg->text);
  if (message_length >= 6) { // 6 - "/start" length

    char buff[7];
    memcpy(buff, msg->text, 6);
    buff[6] = '\0';

    if (strcmp(buff, "/start") == 0) {

      send_simple_message_to_chat(TG_TOKEN, msg->chat_id, get_greeting());
      
      exit(0);
    }
  } else if(message_length == 5){

    char buff[6];
    memcpy(buff, msg->text, 5);
    buff[5] = '\0';

    if (strcmp(buff, "/help") == 0) {

      send_simple_message_to_chat(TG_TOKEN, msg->chat_id, get_help());
      
      exit(0);
    }
  } else if(message_length == 4){
    char buff[5];
    memcpy(buff, msg->text, 4);
    buff[4] = '\0';

    if (strcmp(buff, "/cat") == 0) {

      send_sticker(TG_TOKEN, msg->chat_id, get_cat_sticker());
      exit(0);
    }
  }

  if(msg->text[0] == '/'){
    send_simple_message_to_chat(TG_TOKEN, msg->chat_id, get_command_not_found());
    exit(0);
  }

  city_weather cw;

  if (get_weather_by_city(OW_TOKEN, msg->text, &cw)) {

    char *msgbuff = format_current_weather_message(&cw);

    send_simple_message_to_chat(TG_TOKEN, msg->chat_id, msgbuff);

    free(msgbuff);
  } else {

    send_simple_message_to_chat(TG_TOKEN, msg->chat_id, "City not found!");
  }

  exit(0);
  // No need to free memory in child process
}

bool answer_new_message(const char *TG_TOKEN, const char *OW_TOKEN) {

  tg_text_message msg;
  msg.text = NULL;
  msg.offset=0;

  if (!get_new_text_message(TG_TOKEN, &msg)) {
    if(msg.offset!=0){
      TG_OFFSET = msg.offset + 1;
      pid_t pid = fork();
      if (pid == 0) {
        send_simple_message_to_chat(TG_TOKEN, msg.chat_id, get_not_understand_message());
        send_sticker(TG_TOKEN, msg.chat_id, get_not_understand_sticker());
        exit(0);
      }
    }
    free(msg.text);
    return false;
  }

  TG_OFFSET = msg.offset + 1;

  pid_t pid = fork();
  if (pid == 0) {
    proccess_new_message(&msg, TG_TOKEN, OW_TOKEN); // Subroutine
  }

  if (LOGFILE_FD != NULL) {
    char buffer[50];
    sprintf(buffer, "New message from chat_id %ld, offset %ld", msg.chat_id,
            TG_OFFSET);
    write_log(buffer);
  }

  free(msg.text);

  return true;
}

static const char* commands_json = "[{\"command\":\"start\",\"description\":\"Start the bot\"},{\"command\":\"help\",\"description\":\"Help 🆘\"},{\"command\":\"cat\",\"description\":\"Get cat sticker 🐈\"}]";

void app_run(const char *TG_TOKEN, const char *OW_TOKEN) {

  update_bot_commands(TG_TOKEN, commands_json);

  // Required for profiling, need correct exit
  #ifdef ITERATOR_LIMIT_COUNT
    long iter = 0;
  #endif

  for (;;) {
    
    #ifdef ITERATOR_LIMIT_COUNT
    iter++;
    printf("Iteration count:%ld\n", iter);
    if(iter >= ITERATOR_LIMIT_COUNT){
      exit(0);
    }
    #endif

    answer_new_message(TG_TOKEN, OW_TOKEN);
    printf("Current offset: %ld\n", TG_OFFSET);
  }
}
