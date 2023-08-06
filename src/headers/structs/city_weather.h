#ifndef CITY_WEATHER_H
#define CITY_WEATHER_H

enum Weather_type {Thunderstorm, Drizzle, Rain, Snow, Atmosphere, Clear, Clouds};

typedef enum Weather_type weather_type;

typedef struct City_weather {
    long city_id;
    double lon;
    double lat;
    weather_type weather;
    double temp_kelvin;
    double temp_feels_kelvin;
    double pressure_hpa;
    int humidity_percent;
    int visibility_meters;
    double wind_speed_mps;
    int wind_deg;
    int cloudiness_percent;    
} city_weather;

#endif
