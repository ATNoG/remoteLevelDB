TARGET  := xstorage
SRCS    := ${wildcard *.cpp} 
OBJS    := ${SRCS:.cpp=.o} 

CFLAGS= -O2 -flto -Wall -pipe -ansi -pedantic
CXXFLAGS=${CFLAGS}
LDFLAGS=
LIBS=-lzmq -lleveldb -lboost_regex -lboost_thread -ljsonobject
CC=g++

.PHONY: all clean 
all:: ${TARGET} 

${TARGET}: ${OBJS} 
	${CC} ${LDFLAGS} -o $@ $^ ${LIBS} 
	
${OBJS}:  %.o: %.cpp
	${CC} ${CFLAGS} -o $@ -c $<
	
clean::
	-rm -f *~ *.o ${TARGET}

distclean:: clean
