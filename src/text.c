/*
 *
 * Copyright (c) 2023 Grigoriy EFimov <efimov-gm@newpage.xyz>
 * Licensed under the MIT license
 * 
*/

#include <stdio.h>
#include <string.h>

char* default_greeting = "Welcome to Telegram weather bot! 🌦️\n\nSend me your City and I will tell you about the weather.";

char* get_greeting(){
    return default_greeting;
}
