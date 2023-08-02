FROM alpine:3.18.2

WORKDIR /app

RUN apk add gcc musl-dev make curl curl-dev

ENTRYPOINT ["make","all"]
