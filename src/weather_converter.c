/*
 *
 * Copyright (c) 2023 Grigoriy EFimov <efimov-gm@newpage.xyz>
 * Licensed under the MIT license
 * 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

double kelvin_to_celsius(double temp){
    return temp - 273.15; // https://en.wikipedia.org/wiki/Kelvin
}

double hpa_to_mmhg(double pressure){
    return pressure * 0.750062; //https://en.wikipedia.org/wiki/Millimetre_of_mercury
}
