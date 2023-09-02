#Can't use alpine, unsolved issue https://stackoverflow.com/questions/77019019/how-to-solve-problem-using-gcc-pg-on-alpine
FROM debian:stable-slim

WORKDIR /app

RUN mkdir /app/data

RUN apt update && apt install gcc musl-dev make curl libcurl4-openssl-dev libjson-c-dev -y 

COPY ./docker/profiler.sh /app/profiler.sh

COPY ./src /app

RUN make build PROFILE=1

ENTRYPOINT ["./profiler.sh"]
