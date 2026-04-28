# Makefile ALTERDUNE
# Usage :
#   make          -> compile
#   make run      -> compile + lance
#   make clean    -> nettoie les .o et l'exe

CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
TARGET   = alterdune

SRCS = main.cpp Game.cpp Player.cpp Monster.cpp Entity.cpp Item.cpp ActAction.cpp BestiaryEntry.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all run clean
