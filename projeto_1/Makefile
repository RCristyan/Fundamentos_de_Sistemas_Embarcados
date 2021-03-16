executable: uart_modbus i2c_read_bme bme main i2c_lcd_write logger
	gcc main.o uart_modbus.o i2c_read_bme280.o \
	    bme.o i2c_lcd_write.o logger.o \
		-o executable.o -l wiringPi

main: ./src/main.c
	gcc -I./include -c ./src/main.c -o main.o

uart_modbus: ./src/uart_modbus.c
	gcc -c ./src/uart_modbus.c -o uart_modbus.o

i2c_read_bme: ./src/i2c_read_bme280.c
	gcc -I./include -c ./src/i2c_read_bme280.c -o i2c_read_bme280.o

bme: ./src/bme.c
	gcc -c ./src/bme.c -o bme.o

i2c_lcd_write: ./src/i2c_lcd_write.c
	gcc -I./include -c ./src/i2c_lcd_write.c -o i2c_lcd_write.o -l wiringPi

logger: ./src/logger.c
	gcc -I./include -c ./src/logger.c -o logger.o

clean:
	rm *.o
