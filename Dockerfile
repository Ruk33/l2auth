FROM gcc

# Compile log library
RUN git clone https://github.com/rxi/log.c.git
RUN gcc -fPIC -o log.c/log.o -c log.c/src/log.c -DLOG_USE_COLOR
RUN gcc -fPIC -shared -o log.c/liblog.so log.c/log.o
RUN mv log.c/liblog.so /usr/lib
RUN mkdir /usr/include/log
RUN mv log.c/src/log.h /usr/include/log

# Compile data structures library
RUN git clone https://github.com/Ruk33/data_structures.git
RUN cd data_structures && make lib_shared
RUN cp data_structures/libdatastructures.so /usr/lib
RUN cp -r data_structures /usr/include

COPY . /usr/src/l2auth
WORKDIR /usr/src/l2auth
