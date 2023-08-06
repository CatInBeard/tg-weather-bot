/*
 *
 * Copyright (c) 2023 Grigoriy EFimov <efimov-gm@newpage.xyz>
 * Licensed under the MIT license
 * 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "openweather_api.h"
#include "tg_functions.h"
#include "app.h"
#include "mem_buff.h"

int main(){


   const char *TG_TOKEN = getenv("TG_TOKEN");
   const char *OW_TOKEN = getenv("OW_TOKEN");


   if(!TG_TOKEN){
       fprintf(stderr, "Telegram token not set. Please fill TG_TOKEN environment variable. Get bot token following instruction: https://core.telegram.org/bots/features#botfather\n");
       return 1;
   }

   printf("Checking telegram bot token...\n");

   if(!check_tg_token(TG_TOKEN)){
       fprintf(stderr, "Invalid telegram token\n");

       return 1;
   }
   else{
       printf("Telegram token valid!\n");
   }

   if(!OW_TOKEN){
       fprintf(stderr, "OpenWeather token not set. Please fill OW_TOKEN environment variable. Get api token following instructions https://openweathermap.org/appid\n");
       return 2;
   }
    
   printf("Checking OpenWeather token...\n");

   if(!check_openweather_token(OW_TOKEN)){
       fprintf(stderr, "OpenWeather token invalid!\n");
       return 2;
   }

   printf("Openweather token valid!\n");

   printf("Bot is started...\n");

   app_run(TG_TOKEN, OW_TOKEN);

   printf("Exit!\n");

   return 0; 
}
