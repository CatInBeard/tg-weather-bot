/*
 *
 * Copyright (c) 2023 Grigoriy EFimov <efimov-gm@newpage.xyz>
 * Licensed under the MIT license
 * 
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>

#include "openweather_api.h"
#include "logger.h"
#include "tg_functions.h"
#include "app.h"
#include "mem_buff.h"

int main(int argc, char* argv[]){

   if (signal(SIGCHLD, SIG_IGN) == SIG_ERR) {
        perror("signal");
        exit(EXIT_FAILURE);
   }
   /*
    * Ignore SIGCHLD, for optimization
    * 
    * https://www.man7.org/linux/man-pages/man2/fork.2.html 
   */


   bool enableLogging = true;
   FILE* logfile_fd = NULL;

   for(int i = 1 ; i < argc; i++){ // Skip program name
       if(strcmp("--nolog",argv[i]) == 0){
           enableLogging = false;
       }
       else if(strcmp("--logfile",argv[i]) == 0){
           if(i >= argc - 1){
               fprintf(stderr, "logfile not set");
               return 3;
           }
           else{
               logfile_fd = fopen(argv[i+1], "a+");
               if(logfile_fd == NULL){
                   fprintf(stderr, "Cant write to \"%s\" file",argv[i+1]);
                   return 3;
               }
           }
       }
   }

   if(enableLogging && logfile_fd == NULL){
       logfile_fd = fopen("app.log", "a+");
        if(logfile_fd == NULL){
            fprintf(stderr, "Cant write to \"app.log\" file");
            return 3;
        }
   }
   LOGFILE_FD = logfile_fd;

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

   write_log("Bot is started");

   app_run(TG_TOKEN, OW_TOKEN);

   printf("Exit!\n");

   return 0; 
}
