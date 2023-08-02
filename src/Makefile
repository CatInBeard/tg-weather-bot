bin_name = weather-bot

include_dirs = -Iheaders -Iheaders/structs
link_libs = -lcurl
objects = main.o tg_functions.o

all: $(objects)
	gcc $(objects) -o $(bin_name) $(include_dirs) $(link_libs)
	strip $(bin_name)
	chmod +x $(bin_name)
main.o:
	gcc -c main.c $(include_dirs)
tg_functions.o:
	gcc -c tg_functions.c $(include_dirs)
install:
	cp $(bin_name) /usr/bin/$(bin_name)
clean:
	rm *.o -f
