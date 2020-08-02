FROM gcc

# Compile log library
RUN git clone https://github.com/rxi/log.c.git
RUN gcc -fPIC -o log.c/log.o -c log.c/src/log.c -DLOG_USE_COLOR
RUN gcc -fPIC -shared -o log.c/liblog.so log.c/log.o
RUN mv log.c/liblog.so /usr/lib
RUN mkdir /usr/include/log
RUN mv log.c/src/log.h /usr/include/log

COPY . /usr/src/l2auth
WORKDIR /usr/src/l2auth
