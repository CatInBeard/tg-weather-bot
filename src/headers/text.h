#ifndef TEXT_H
#define TEXT_H

#include "city_weather.h"

char *get_greeting();
char *format_current_weather_message(const city_weather *cw);
char *get_not_understand_message(); 
char *get_not_understand_sticker(); 
#endif
