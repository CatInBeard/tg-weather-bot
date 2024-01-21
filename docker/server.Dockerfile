FROM alpine:3.19.0

RUN apk add curl json-c

WORKDIR /app

COPY build/weather-bot /usr/bin/weather-bot

ENTRYPOINT ["weather-bot", "--logfile", "/app/app.log"]
