FROM alpine:3.18.2

WORKDIR /app

RUN apk add gcc musl-dev make curl curl-dev json-c json-c-dev

ENTRYPOINT ["make","all"]
