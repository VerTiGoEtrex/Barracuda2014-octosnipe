.PHONY: all

INCLUDE_PATH=-I../jsoncpp/include
LIBRARY_PATH=-L../jsoncpp/lib
USELIBS=-ljsoncpp

ifeq ($(shell uname -s), Darwin)
SOEXT = dylib
else
SOEXT = so
endif

all: libjson_socket.$(SOEXT)

libjson_socket.$(SOEXT): json_socket.cpp json_socket.h
	g++ -g -fpic -shared $(INCLUDE_PATH) $(LIBRARY_PATH) $(USELIBS) -o libjson_socket.$(SOEXT) json_socket.cpp

clean:
	-rm -f libjson_socket.$(SOEXT)
