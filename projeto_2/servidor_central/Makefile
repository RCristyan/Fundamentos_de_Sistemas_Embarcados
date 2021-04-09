SRC_PATH?=./src/
OBJECT_PATH?=./object/
INCLUDE_PATH?=-I./include

run: executable
	./executable

executable: main central_socket
	g++ $(OBJECT_PATH)*.o -o executable

main: $(SRC_PATH)main.cpp
	g++ $(INCLUDE_PATH) -c $(SRC_PATH)main.cpp -o $(OBJECT_PATH)main.o

central_socket: $(SRC_PATH)central_socket.cpp
	g++ $(INCLUDE_PATH) -c $(SRC_PATH)central_socket.cpp -o $(OBJECT_PATH)central_socket.o

clean:
	rm $(OBJECT_PATH)*.o
	rm executable
