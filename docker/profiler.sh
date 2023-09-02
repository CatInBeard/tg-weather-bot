#!/bin/sh
echo "Wait 10 iterations..."
./weather-bot > /dev/null 2>&1
echo "App finieshed, gprof starting"
gprof weather-bot gmon.out -plLz > weather-bot.txt
cp weather-bot.txt data/weather-bot.txt
cp app.log data/app.log
echo "Profiling finished, you can see result in weather-bot.txt"