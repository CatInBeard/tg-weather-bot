bin_name = weather-bot
all: main.o
	gcc main.o -o $(bin_name)
	strip $(bin_name)
	chmod +x $(bin_name)
main.o:
	gcc -c main.c
install:
	cp $(bin_name) /usr/bin/$(bin_name)
clean:
	rm *.o -f
