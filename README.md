# tg-weather-bot

Simple tg weather bot written in C  

## Description

Simple tg weather bot written in C  

## License

This project is licensed under the terms of the MIT License. See the [LICENSE](LICENSE) file for details.

## Installation

To get started with tg-weather-bot, follow these steps:

1. Clone the repository:

  ```git clone https://github.com/CatInBeard/tg-weather-bot.git```
   

2. Obtain a Telegram Bot API token and set it in the .env file by copying the exmaple.env file and fill variables. You can create a bot and obtain the token by following the instructions provided by [BotFather](https://telegram.me/BotFather).

3. Build container:
   
```make build-server```

4. Start bot:

   
```make run```
   
   The bot should now be up and running.

## Docker Hub

You can run tg-weather-bot using Docker Hub. Follow these steps:

1. Pull the Docker image from Docker Hub:

   
   ```docker pull catinbeard/tg-weather-bot:latest```
   
2. Run the container:

   
```docker run -d catinbeard/tg-weather-bot:latest -e TG_TOKEN='YOUR_BOT_TOKEN' -e OW_TOKEN='YOUR_OW_TOKEN'```

   The tg-weather-bot will can  now be running inside the Docker container.
   
## Building without Docker 
Alternatively, you can do without a container builder and work without a server container. Follow these steps:

1. Clone the repository:

  ```git clone https://github.com/CatInBeard/tg-weather-bot.git```
   

2. Install the required dependencies using package manager:

  * Debian/Ubuntu
   ```apt update && apt install gcc musl-dev make curl libjson-c-dev```
  * Arch ```sudo pacman -S gcc make curl json-c```
  * Fedora ```dnf install gcc make curl json-c```
  * Alpine ```apk add gdb gcc musl-dev make curl curl-dev json-c json-c-dev```

3. Compile: go to the src directory and run ```make all```

4. Install:
   ```make install```
5. Run: ``` tg-weather-bot``` or ```nohup tg-weather-bot >/dev/null 2>&1 &``` for background usage
6. Cron (optional) You can add cronjob after system startup:
   open crontab with ```crontab -e``` and add ``` @reboot nohup tg-weather-bot >/dev/null 2>&1 &```

## Debug
You can build the debug version and open it with gdb: 

  ```make debug```.
  
  If you are not using Docker and simply want to build the debug version, go to the src directory and run ```make build DEBUG=1```.
