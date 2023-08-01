FROM alpine:3.18.2

WORKDIR /app

COPY build/weather-bot /usr/bin/weather-bot

ENTRYPOINT weather-bot
