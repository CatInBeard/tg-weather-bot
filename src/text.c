/*
 *
 * Copyright (c) 2023 Grigoriy EFimov <efimov-gm@newpage.xyz>
 * Licensed under the MIT license
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "city_weather.h"
#include "weather_converter.h"

char *default_greeting = "Welcome to Telegram weather bot! 🌦️\n\nSend me your "
                         "City and I will tell you about the weather.";

char *get_greeting() { return default_greeting; }

void cw_get_text_weather_type(weather_type wt, char *text) {

  switch (wt) {
  case Thunderstorm:
    strcpy(text, "⛈️ thunderstorm");
    break;
  case Drizzle:
    strcpy(text, "🌧️ drizzle");
    break;
  case Rain:
    strcpy(text, "🌧️ rain");
    break;
  case Snow:
    strcpy(text, "🌨️ snow");
    break;
  case Atmosphere:
    strcpy(text, "🥽 bad vision");
    break;
  case Clear:
    strcpy(text, "☀️ clear");
    break;
  case Clouds:
    strcpy(text, "⛅ cloudy");
    break;
  }
}

char *format_current_weather_message(const city_weather *cw) {
  // You must free returned message after usage

  char *message = malloc(1000);

  char wt[30];
  cw_get_text_weather_type(cw->weather, wt);

  sprintf(message,
          "Currently, it is %s outside.\n🌡️The temperature is %d°C, but it "
          "feels like %d°C.\n⏱️The air pressure is %d mmHg.\n💦The humidity "
          "level is %d%%.\n💨The wind is blowing at a speed of %d m/s.\n☁️The "
          "cloudiness is %d%%\n",
          wt, (int)kelvin_to_celsius(cw->temp_kelvin),
          (int)kelvin_to_celsius(cw->temp_feels_kelvin),
          (int)hpa_to_mmhg(cw->pressure_hpa), cw->humidity_percent,
          (int)cw->wind_speed_mps, cw->cloudiness_percent);

  return message;
}
