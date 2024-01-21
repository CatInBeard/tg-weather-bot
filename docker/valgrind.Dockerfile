FROM alpine:3.19.0

WORKDIR /app

RUN ulimit -n 100000

RUN apk add gdb gcc musl-dev make curl curl-dev json-c json-c-dev valgrind

COPY ./src /app

RUN make build ITERLIMIT=3 DEBUG=1

COPY ./docker/valgrind.sh /app/valgrind.sh

ENTRYPOINT ["sh","/app/valgrind.sh"]