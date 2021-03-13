executable: uart_modbus i2c_read_bme bme main i2c_lcd_write logger
	gcc main.o uart_modbus.o i2c_read_bme280.o \
	    bme.o i2c_lcd_write.o logger.o \
		-o executable.o -l wiringPi

main: main.c
	gcc -c main.c -o main.o

uart_modbus: uart_modbus.c
	gcc -c uart_modbus.c -o uart_modbus.o

i2c_read_bme: i2c_read_bme280.c
	gcc -c i2c_read_bme280.c -o i2c_read_bme280.o

bme: bme.c
	gcc -c bme.c -o bme.o

i2c_lcd_write: i2c_lcd_write.c
	gcc -c i2c_lcd_write.c -o i2c_lcd_write.o -l wiringPi

logger: logger.c
	gcc -c logger.c -o logger.o

clean:
	rm *.o