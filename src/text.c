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

static const char *default_greeting = "Welcome to Telegram weather bot! 🌦️\n\nSend me your "
                         "City and I will tell you about the weather.";

static const char *help = "Welcome to Telegram weather bot! 🌦️\n\nSend me your "
                         "City and I will tell you about the weather. Or send /cat to get cat sticker 🐈";

static const char *not_understand = "Sorry, i can't understand your query 😞";

static const char *command_not_found = "Unknown command 🤔, try help 🆘";

static const char *not_understand_sticker_id = "CAACAgIAAxkBAAELOetlrVGmz-4Kbnmy0-lFJY9KGHl2JAACjycAAulVBRiFTapo6EI3QDQE"; 
static const char *cat_sticker_id = "CAACAgIAAxUAAWYTAYytFvpiiY9yyTYCQOOkn8JXAAIKSAACGbOYSPjrNq2CCu-mNAQ"; 

const char *get_greeting() { return default_greeting; }

const char *get_help() {return help;}

const char *get_not_understand_message(){ return not_understand; }

const char *get_not_understand_sticker(){ return not_understand_sticker_id; }

const char *get_cat_sticker(){ return cat_sticker_id; }

const char *get_command_not_found() {return command_not_found;}

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
