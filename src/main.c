/*
 *
 * Copyright (c) 2023 Grigoriy EFimov <efimov-gm@newpage.xyz>
 * Licensed under the MIT license
 * 
*/

#include <stdio.h>
#include <stdlib.h>

#include "tg_functions.h"

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


   printf("Bot is started...\n");

   for(;;){

   }

   return 0; 
}
