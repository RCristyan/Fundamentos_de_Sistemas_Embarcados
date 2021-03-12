executable: uart_modbus i2c_read_bme bme main.o
	gcc main.o uart_modbus.o i2c_read_bme280.o bme.o -o executable.o

main: main.c
	gcc -c main.c -o main.o

uart_modbus: uart_modbus.c
	gcc -c uart_modbus.c -o uart_modbus.o

i2c_read_bme: i2c_read_bme280.c
	gcc -c i2c_read_bme280.c -o i2c_read_bme280.o

bme: bme.c
	gcc -c bme.c -o bme.o

clean:
	rm *.o