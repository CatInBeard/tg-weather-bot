#ifndef TEXT_H
#define TEXT_H

#include "city_weather.h"

const char *get_greeting();
const char *get_help();
char *format_current_weather_message(const city_weather *cw);
const char *get_not_understand_message(); 
const char *get_not_understand_sticker(); 
const char *get_cat_sticker();
const char *get_command_not_found();
#endif
