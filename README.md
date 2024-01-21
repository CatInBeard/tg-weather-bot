# tg-weather-bot
[![Build and publish Docker image](https://github.com/CatInBeard/tg-weather-bot/actions/workflows/docker-publish.yaml/badge.svg)](https://github.com/CatInBeard/tg-weather-bot/actions/workflows/docker-publish.yaml) [![Build](https://github.com/CatInBeard/tg-weather-bot/actions/workflows/docker-build.yaml/badge.svg)](https://github.com/CatInBeard/tg-weather-bot/actions/workflows/docker-build.yaml) [![Cppcheck static analysis](https://github.com/CatInBeard/tg-weather-bot/actions/workflows/cpp-check.yaml/badge.svg)](https://github.com/CatInBeard/tg-weather-bot/actions/workflows/cpp-check.yaml)
Simple tg weather bot written in C  

## Description

To get the weather information for a specific city, write to the [bot](https://t.me/weather_sub_bot) with the name of the city you want to know the weather for. The bot will provide you with the weather condition, temperature, humidity, pressure, and wind speed in that location. The bot utilizes the OpenWeather API to fetch the weather data. Bot written in C, wrapped in docker container.

## License

This project is licensed under the terms of the MIT License. See the [LICENSE](LICENSE) file for details.

## Installation

To get started with tg-weather-bot, follow these steps:

1. Clone the repository:

  ```git clone https://github.com/CatInBeard/tg-weather-bot.git```
   

2. Obtain a Telegram Bot API token and set it in the .env file by copying the exmaple.env file and fill TG_TOKEN variable. You can create a bot and obtain the token by following the instructions provided by [BotFather](https://telegram.me/BotFather).

2. Obtain openweather API token and set it in the .env file by copying the exmaple.env file and fill OW_TOKEN variable. You can obtain the token by following the instructions provided by [openweather](https://openweathermap.org/appid).

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

   
```docker run -d -v $PWD/data:/app  catinbeard/tg-weather-bot:latest -e TG_TOKEN='YOUR_BOT_TOKEN' -e OW_TOKEN='YOUR_OW_TOKEN'```

   The tg-weather-bot will can now be running inside the Docker container.
   
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
```make debug```  
If you are not using Docker and simply want to build the debug version, go to the src directory and run:  
```make build DEBUG=1```

## Profiling
You can build the debug version and run it with gprof:  
```make profile```  
If you are not using Docker and simply want to build the profiling version, go to the src directory and run:  
```make build PROFILE=1```
## Valgrind
You can generate valgrind report with:  
```make valgrind```