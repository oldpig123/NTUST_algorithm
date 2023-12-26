CC=g++
LDFLAGS=-std=c++11 -O3 -lm
SOURCES=*.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=m12345678
INCLUDES=*.h

all: $(SOURCES) ./$(EXECUTABLE)

./$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

%.o:  %.c  ${INCLUDES}
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o ./$(EXECUTABLE)
