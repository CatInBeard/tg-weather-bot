/*
 *
 * Copyright (c) 2023 Grigoriy EFimov <efimov-gm@newpage.xyz>
 * Licensed under the MIT license
 *
 */

#include <curl/curl.h>
#include <json-c/json.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define JSON_FIND_HELPER                                                       \
  if (!find) {                                                                 \
    json_object_put(root);                                                     \
    return false;                                                              \
  }

#include "city_weather.h"
#include "curl_functions.h"

bool check_openweather_token(const char *token) {

  if (strlen(token) != 32) {
    return false;
  }

  char link[96];

  sprintf(link,
          "https://api.openweathermap.org/data/2.5/weather?q=London&appid=%s",
          token);

  return simple_get(link);
}

weather_type get_weather_type_by_weather_id(int id) {
  // https://openweathermap.org/weather-conditions

  weather_type wt;

  if (id < 300) {
    wt = Thunderstorm;
  } else if (id < 500) {
    wt = Drizzle;
  } else if (id < 600) {
    wt = Rain;
  } else if (id < 700) {
    wt = Snow;
  } else if (id < 800) {
    wt = Atmosphere;
  } else if (id == 800) {
    wt = Clear;
  } else {
    wt = Clouds;
  }

  return wt;
}

bool parse_raw_weather_to_city_weather(mem_buff *mb, city_weather *cw) {

  json_bool find = false;

  struct json_object *root = json_tokener_parse(mb->buffer);

  struct json_object *coord_obj;
  find = json_object_object_get_ex(root, "coord", &coord_obj);
  JSON_FIND_HELPER
  struct json_object *lon_obj;
  find = json_object_object_get_ex(coord_obj, "lon", &lon_obj);
  JSON_FIND_HELPER
  struct json_object *lat_obj;
  find = json_object_object_get_ex(coord_obj, "lat", &lat_obj);
  JSON_FIND_HELPER
  cw->lon = json_object_get_double(lon_obj);
  cw->lat = json_object_get_double(lat_obj);

  struct json_object *city_id_obj;
  find = json_object_object_get_ex(root, "id", &city_id_obj);
  JSON_FIND_HELPER
  cw->city_id = json_object_get_int64(city_id_obj);

  struct json_object *weather_obj;
  find = json_object_object_get_ex(root, "weather", &weather_obj);
  JSON_FIND_HELPER
  struct json_object *result_element =
      json_object_array_get_idx(weather_obj, 0);

  if (!result_element) {
    json_object_put(root);
    return false;
  }

  struct json_object *weather_id;
  find = json_object_object_get_ex(result_element, "id", &weather_id);

  cw->weather = get_weather_type_by_weather_id(json_object_get_int(weather_id));
  JSON_FIND_HELPER

  struct json_object *main_obj;
  find = json_object_object_get_ex(root, "main", &main_obj);
  JSON_FIND_HELPER

  struct json_object *temp_obj;
  find = json_object_object_get_ex(main_obj, "temp", &temp_obj);
  JSON_FIND_HELPER

  cw->temp_kelvin = json_object_get_double(temp_obj);

  struct json_object *temp_feels_obj;
  find = json_object_object_get_ex(main_obj, "feels_like", &temp_feels_obj);
  JSON_FIND_HELPER

  cw->temp_feels_kelvin = json_object_get_double(temp_feels_obj);

  struct json_object *pressure_obj;
  find = json_object_object_get_ex(main_obj, "pressure", &pressure_obj);
  JSON_FIND_HELPER

  cw->pressure_hpa = json_object_get_double(pressure_obj);

  struct json_object *humidity_obj;
  find = json_object_object_get_ex(main_obj, "humidity", &humidity_obj);
  JSON_FIND_HELPER

  cw->humidity_percent = json_object_get_int(humidity_obj);

  struct json_object *visibility_obj;
  find = json_object_object_get_ex(root, "visibility", &visibility_obj);
  JSON_FIND_HELPER

  cw->visibility_meters = json_object_get_double(visibility_obj);

  struct json_object *wind_obj;
  find = json_object_object_get_ex(root, "wind", &wind_obj);
  JSON_FIND_HELPER

  struct json_object *wind_speed_obj;
  find = json_object_object_get_ex(wind_obj, "speed", &wind_speed_obj);
  JSON_FIND_HELPER

  cw->wind_speed_mps = json_object_get_double(wind_speed_obj);

  struct json_object *wind_deg_obj;
  find = json_object_object_get_ex(wind_obj, "deg", &wind_deg_obj);
  JSON_FIND_HELPER

  cw->wind_deg = json_object_get_double(wind_deg_obj);

  struct json_object *clouds_obj;
  find = json_object_object_get_ex(root, "clouds", &clouds_obj);
  JSON_FIND_HELPER

  struct json_object *clouds_all_obj;
  find = json_object_object_get_ex(clouds_obj, "all", &clouds_all_obj);
  JSON_FIND_HELPER

  cw->cloudiness_percent = json_object_get_int(clouds_all_obj);

  json_object_put(root);

  return true;
}

bool get_weather_by_city(const char *OW_TOKEN, const char *city,
                         city_weather *cw) {

  size_t city_len = strlen(city);

  if (city_len > 100) {
    return false;
  }

  char *escaped_city = escape_link(city);

  char link_buffer[500];

  sprintf(link_buffer,
          "https://api.openweathermap.org/data/2.5/weather?q=%s&appid=%s",
          escaped_city, OW_TOKEN);

  curl_free(escaped_city);

  mem_buff mb;
  mb.buffer = malloc(1);
  mb.size = 1;

  if (!get_with_responce(link_buffer, &mb, 30)) {
    return false;
  }
  parse_raw_weather_to_city_weather(&mb, cw);

  free(mb.buffer);

  return true;
}
