FROM alpine

RUN apk add --update --no-cache gcc musl-dev openssl-dev make git

RUN git clone https://github.com/rxi/log.c.git
RUN gcc -fPIC -o log.c/log.o -c log.c/src/log.c
RUN gcc -fPIC -shared -o log.c/liblog.so log.c/log.o
RUN mv log.c/liblog.so /usr/lib
RUN mkdir /usr/include/log
RUN mv log.c/src/log.h /usr/include/log

EXPOSE 2106

COPY . /usr/src/l2auth
WORKDIR /usr/src/l2auth

CMD make && ./build/l2auth
