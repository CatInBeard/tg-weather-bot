bin_name = weather-bot

include_dirs = -Iheaders -Iheaders/structs
link_libs = -lcurl
objects = main.o tg_functions.o curl_functions.o

DEBUG?=0

ifneq ($(DEBUG), 0)
	DEBUG_FLAGS = -g
else
	DEBUG_FLAGS = 
endif

all: build strip exec

strip:
	strip $(bin_name)

exec:
	chmod +x $(bin_name)

build: $(objects)
	gcc $(objects) $(DEBUG_FLAGS) -o $(bin_name) $(include_dirs) $(link_libs) 
	
main.o:
	gcc -c main.c $(include_dirs) $(DEBUG_FLAGS) 
tg_functions.o:
	gcc -c tg_functions.c $(include_dirs) $(DEBUG_FLAGS) 
curl_functions.o:
	gcc -c curl_functions.c $(include_dirs) $(DEBUG_FLAGS) 
install:
	cp $(bin_name) /usr/bin/$(bin_name)
clean:
	rm *.o -f
