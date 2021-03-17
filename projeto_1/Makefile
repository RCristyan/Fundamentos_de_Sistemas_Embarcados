INCLUDE_PATH?=-I./include
SRC_PATH?=./src/
OBJECT_PATH?=./object/

executable: uart_modbus i2c_read_bme bme main i2c_lcd_write logger gpio pid
	gcc $(OBJECT_PATH)*.o -o executable -lwiringPi

main: $(SRC_PATH)main.c
	gcc $(INCLUDE_PATH) -c $(SRC_PATH)main.c -o $(OBJECT_PATH)main.o

uart_modbus: $(SRC_PATH)uart_modbus.c
	gcc -c $(SRC_PATH)uart_modbus.c -o $(OBJECT_PATH)uart_modbus.o

i2c_read_bme: $(SRC_PATH)i2c_read_bme280.c
	gcc $(INCLUDE_PATH) -c $(SRC_PATH)i2c_read_bme280.c -o $(OBJECT_PATH)i2c_read_bme280.o

bme: $(SRC_PATH)bme.c
	gcc -c $(SRC_PATH)bme.c -o $(OBJECT_PATH)bme.o

i2c_lcd_write: $(SRC_PATH)i2c_lcd_write.c
	gcc $(INCLUDE_PATH) -c $(SRC_PATH)i2c_lcd_write.c -o $(OBJECT_PATH)i2c_lcd_write.o -l wiringPi

logger: $(SRC_PATH)logger.c
	gcc $(INCLUDE_PATH) -c $(SRC_PATH)logger.c -o $(OBJECT_PATH)logger.o

gpio: $(SRC_PATH)gpio.c
	gcc $(INCLUDE_PATH) -c $(SRC_PATH)gpio.c -o $(OBJECT_PATH)gpio.o -lwiringPi -lpthread

pid: $(SRC_PATH)pid.c
	gcc $(INCLUDE_PATH) -c $(SRC_PATH)pid.c -o $(OBJECT_PATH)pid.o

clean:
	rm ./object/*.o
	rm ./executable
