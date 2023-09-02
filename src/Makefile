bin_name = weather-bot

include_dirs = -Iheaders -Iheaders/structs
link_libs = -lcurl -ljson-c
objects = main.o tg_functions.o curl_functions.o app.o text.o openweather_api.o weather_converter.o logger.o
flags = -Wall -Wextra

DEBUG?=0
PROFILE?=0

ifneq ($(DEBUG), 0)
	DEBUG_FLAGS = -g
else
	DEBUG_FLAGS = 	
endif

ifneq ($(PROFILE), 0)
	PROFILER_FLAGS = -pg -g -p -DITERATOR_LIMIT_COUNT=10 -fno-omit-frame-pointer -fno-inline-functions -fno-inline
else
	PROFILER_FLAGS = 
endif

all: build strip exec

strip:
	strip $(bin_name)

exec:
	chmod +x $(bin_name)

build: $(objects)
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


install:
	cp $(bin_name) /usr/bin/$(bin_name)
clean:
	rm *.o -f
