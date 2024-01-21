#ifndef TH_FUNCTIONS_H
#define TH_FUNCTIONS_H 1

#include <stdbool.h>

#include "mem_buff.h"
#include "tg_text_message.h"

extern long TG_OFFSET;
bool check_tg_token(const char *);
bool longpoll_get_updates(const char *token, mem_buff *result,
                          const unsigned int timeout,
                          const unsigned int curl_timeout,
                          const unsigned int events_limit,
                          const unsigned long offset);
bool simple_longpoll_get_updates(const char *token, mem_buff *result,
                                 const unsigned long offset);
bool send_simple_message_to_chat(const char *token, long chat_id,
                                 const char *message_text);
bool parse_new_messages(mem_buff *mb, tg_text_message *message);
bool get_new_text_message(const char *TG_TOKEN, tg_text_message *msg);
bool send_sticker(const char *token, long chat_id,
                                 const char *sticker_id);

#endif
