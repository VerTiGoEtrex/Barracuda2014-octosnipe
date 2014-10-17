.PHONY: all .cpp.o

CXX=game_io.cpp \
	error_msg.cpp \
	game_result.cpp \
	game_state.cpp \
	greeting.cpp \
	json_response.cpp \
	move_request.cpp \
	move_result.cpp \
	take_space_response.cpp \
	wait_response.cpp

OBJECTS=$(CXX:.cpp=.o)

INCLUDE_PATH=-I../jsoncpp/include \
			 -I../json_socket

LIBRARY_PATH=-L../json_socket \
			 -L../jsoncpp/lib
USELIBS=-ljsoncpp \
		 -ljson_socket

ifeq ($(shell uname -s), Darwin)
SOEXT = dylib
else
SOEXT = so
endif

all: libgame_io.$(SOEXT)

libgame_io.$(SOEXT): $(OBJECTS)
	g++ -g -shared -Wall -fpic $(INCLUDE_PATH) $(LIBRARY_PATH) $(USELIBS) $(OBJECTS) -o $@

.cpp.o:
	g++ -g -c -Wall -fpic $(INCLUDE_PATH) $(LIBRARY_PATH) $< -o $@

clean:
	-rm -f *.o *.$(SOEXT)
