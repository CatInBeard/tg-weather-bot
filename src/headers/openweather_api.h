#ifndef OPENWEATHER_API_H
#define OPENWEATHER_API_H

#include <stdbool.h>

#include "city_weather.h"

bool check_openweather_token(const char* token);
bool get_weather_by_city(const char* OW_TOKEN, const char* city, city_weather* cw);


#endif
