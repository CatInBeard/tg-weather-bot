bin_name = weather-bot

include_dirs = -Iheaders -Iheaders/structs
link_libs = -lcurl -ljson-c
objects = main.o tg_functions.o curl_functions.o app.o text.o openweather_api.o weather_converter.o logger.o

DEBUG?=0
STRICT?=1
PROFILE?=0
ITERLIMIT?=0

ifneq ($(DEBUG), 0)
	DEBUG_FLAGS = -g
else
	DEBUG_FLAGS = 	
endif

ifneq ($(STRICT), 0)
	flags = -Wall -Wextra -Werror
else
	flags = -Wall -Wextra
endif

ifneq ($(PROFILE), 0)
	PROFILER_FLAGS = -pg -g -p -DITERATOR_LIMIT_COUNT=10 -fno-omit-frame-pointer -fno-inline-functions -fno-inline
else
	PROFILER_FLAGS = 
endif

ifneq ($(ITERLIMIT), 0)
	PROFILER_FLAGS = -DITERATOR_LIMIT_COUNT=$(ITERLIMIT)
endif

default: help

.PHONY: help
help: # Show help for each of the Makefile recipes.
	@grep -E '^[a-zA-Z0-9 -]+:.*#'  Makefile | sort | while read -r l; do printf "\033[1;32m$$(echo $$l | cut -f 1 -d':')\033[00m:$$(echo $$l | cut -f 2- -d'#')\n"; done
.PHONY: all
all: build strip exec # Prepare install-ready binary

strip: # Delete all build information for decrease file size and imporve performance
	strip $(bin_name)

exec: # Add executable permission
	chmod +x $(bin_name)

build: $(objects) # Build binary
	gcc $(objects) $(DEBUG_FLAGS) $(PROFILER_FLAGS) -o $(bin_name) $(include_dirs) $(link_libs) $(flags)
	
main.o:
	gcc -c main.c $(include_dirs) $(DEBUG_FLAGS) $(PROFILER_FLAGS) $(flags)
tg_functions.o:
	gcc -c tg_functions.c $(include_dirs) $(DEBUG_FLAGS) $(PROFILER_FLAGS) $(flags)
curl_functions.o:
	gcc -c curl_functions.c $(include_dirs) $(DEBUG_FLAGS) $(PROFILER_FLAGS) $(flags)
app.o:
	gcc -c app.c $(include_dirs) $(DEBUG_FLAGS) $(PROFILER_FLAGS) $(flags)
text.o:
	gcc -c text.c $(include_dirs) $(DEBUG_FLAGS) $(PROFILER_FLAGS) $(flags)
openweather_api.o:
	gcc -c openweather_api.c $(include_dirs) $(DEBUG_FLAGS) $(PROFILER_FLAGS) $(flags)
weather_converter.o:
	gcc -c weather_converter.c $(include_dirs) $(DEBUG_FLAGS) $(PROFILER_FLAGS) $(flags)
logger.o:
	gcc -c logger.c $(include_dirs) $(DEBUG_FLAGS)  $(PROFILER_FLAGS) $(flags)


install: # Install binary to system
	cp $(bin_name) /usr/bin/$(bin_name)
clean: # Delete all object files
	rm *.o -f
