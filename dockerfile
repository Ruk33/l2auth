FROM alpine

RUN apk add --update --no-cache gcc musl-dev openssl-dev

EXPOSE 2106

COPY . /usr/src/l2auth
WORKDIR /usr/src/l2auth

RUN gcc -o l2auth main.c -std=c99 -I. -lcrypto -lssl
