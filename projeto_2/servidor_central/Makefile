SRC_PATH?=./src/
OBJECT_PATH?=./object/
INCLUDE_PATH?=-I./include

run: executable
	./executable

executable: main central_socket server_temperature_socket
	g++ $(OBJECT_PATH)*.o -o executable -lpthread

main: $(SRC_PATH)main.cpp
	g++ $(INCLUDE_PATH) -c $(SRC_PATH)main.cpp -o $(OBJECT_PATH)main.o

central_socket: $(SRC_PATH)central_socket.cpp
	g++ $(INCLUDE_PATH) -c $(SRC_PATH)central_socket.cpp -o $(OBJECT_PATH)central_socket.o

server_temperature_socket: $(SRC_PATH)server_temperature_socket.cpp
	g++ $(INCLUDE_PATH) -c $(SRC_PATH)server_temperature_socket.cpp -o $(OBJECT_PATH)server_temperature_socket.o

clean:
	rm $(OBJECT_PATH)*.o
	rm executable
