FROM alpine

RUN apk add --update --no-cache gcc musl-dev openssl-dev make

EXPOSE 2106

COPY . /usr/src/l2auth
WORKDIR /usr/src/l2auth

CMD make && ./build/l2auth
