FROM alpine:3.18.2

WORKDIR /app

RUN apk add gdb gcc musl-dev make curl curl-dev json-c json-c-dev

COPY ./src /app

RUN make build DEBUG=1

ENTRYPOINT ["gdb", "weather-bot"]
