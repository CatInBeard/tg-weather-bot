/*
 *
 * Copyright (c) 2023 Grigoriy EFimov <efimov-gm@newpage.xyz>
 * Licensed under the MIT license
 * 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tg_functions.h"
#include "mem_buff.h"

int main(int argc, char* argv[]){


   const char *TG_TOKEN = getenv("TG_TOKEN");
   const char *OW_TOKEN = getenv("OW_TOKEN");


   if(!TG_TOKEN){
       fprintf(stderr, "Telegram token not set. Please fill TG_TOKEN environment variable. Get bot token following instruction: https://core.telegram.org/bots/features#botfather\n");
       return 1;
   }

   printf("Checking telegram bot token\n");

   if(!check_tg_token(TG_TOKEN)){
       fprintf(stderr, "Invalid telegram token\n");

       return 1;
   }
   else{
       printf("Telegram token valid!\n");
   }

   mem_buff mb;
   mb.buffer = malloc(1);
   mb.size = 1;


   simple_longpoll_get_updates(TG_TOKEN, &mb, 0); 

   printf("%s", mb.buffer); 

   free(mb.buffer);
   mb.size = 0;

   return 0;

   printf("Bot is started...\n");

   for(;;){

   }

   return 0; 
}
