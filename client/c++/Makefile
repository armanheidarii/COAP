SRC := $(wildcard src/*.cpp)
FLAGS := -std=c++20 -o


main: $(SRC)
	g++ $^ $(FLAGS) $@

run: main
	./main