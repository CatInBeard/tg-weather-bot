#ifndef TG_TEXT_MESSAGE_H
#define TG_TEXT_MESSAGE_H

typedef struct TG_text_message {
    long offset;
    char* text;
    long chat_id;
} tg_text_message;

#endif
