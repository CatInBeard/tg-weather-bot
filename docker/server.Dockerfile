FROM alpine:3.18.2

RUN apk add curl

WORKDIR /app

COPY build/weather-bot /usr/bin/weather-bot

ENTRYPOINT weather-bot
