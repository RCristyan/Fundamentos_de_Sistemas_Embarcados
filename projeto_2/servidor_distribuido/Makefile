SRC_PATH?=./src/
OBJECT_PATH?=./object/
INCLUDE_PATH?=-I./include

run: executable
	./executable

executable: main bme i2c_read_bme280 gpio_control servidor temperature_reading_client
	g++ $(OBJECT_PATH)*.o -o executable -lpthread -lwiringPi

main: $(SRC_PATH)main.cpp
	g++ $(INCLUDE_PATH) -c $(SRC_PATH)main.cpp -o $(OBJECT_PATH)main.o

bme: $(SRC_PATH)bme.c
	gcc -c $(SRC_PATH)bme.c -o $(OBJECT_PATH)bme.o

i2c_read_bme280: $(SRC_PATH)i2c_read_bme280.c
	gcc $(INCLUDE_PATH) -c $(SRC_PATH)i2c_read_bme280.c -o $(OBJECT_PATH)i2c_read_bme280.o

gpio_control: $(SRC_PATH)gpio_control.cpp
	g++ $(INCLUDE_PATH) -c $(SRC_PATH)gpio_control.cpp -o $(OBJECT_PATH)gpio_control.o

servidor: $(SRC_PATH)servidor.cpp
	g++ $(INCLUDE_PATH) -c $(SRC_PATH)servidor.cpp -o $(OBJECT_PATH)servidor.o

temperature_reading_client: $(SRC_PATH)temperature_reading_client.cpp
	g++ $(INCLUDE_PATH) -c $(SRC_PATH)temperature_reading_client.cpp -o $(OBJECT_PATH)temperature_reading_client.o

clean:
	rm $(OBJECT_PATH)*.o
	rm executable
