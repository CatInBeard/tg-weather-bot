FROM alpine:3.19.0

WORKDIR /app

RUN apk add gcc musl-dev make curl curl-dev json-c json-c-dev

ENTRYPOINT ["make","all"]
