SRC_PATH?=./src/
OBJECT_PATH?=./object/
INCLUDE_PATH?=-I./include

run: executable
	./executable

executable: main
	g++ $(OBJECT_PATH)*.o -o executable

main: $(SRC_PATH)main.cpp
	g++ -c $(SRC_PATH)main.cpp -o $(OBJECT_PATH)main.o

clean:
	rm $(OBJECT_PATH)*.o
	rm executable
