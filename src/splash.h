/* splash.h */
/* Title screen function definitions */

#ifndef SPLASH_H
#define SPLASH_H

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "appinfo.h"

#define TITLE_COUNT 14

void splash_init_titles(char*** titles);
void splash_show();

#endif