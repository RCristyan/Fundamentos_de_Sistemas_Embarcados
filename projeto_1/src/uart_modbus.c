#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <signal.h>

#define ENDERECO                        0x01
#define RECEBER                         0x23
#define RECEBER_VALOR_LM35              0xC1
#define RECEBER_VALOR_POTENCIOMETRO     0xC2

unsigned char rx_buffer[256];
unsigned char tx_buffer[200];
unsigned char *p_tx_buffer;

int uart_filestream;
int tamanho_do_pacote = 0;
int retry_count = 0;

typedef union{
    short crc;
    uint8_t bytes[2];
} bytesCRC;

short CRC16(short crc, char data){
    const short tbl[256] = {
        0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
        0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
        0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
        0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
        0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
        0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
        0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
        0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
        0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
        0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
        0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
        0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
        0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
        0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
        0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
        0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
        0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
        0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
        0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
        0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
        0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
        0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
        0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
        0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
        0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
        0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
        0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
        0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
        0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
        0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
        0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
        0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040};
    return ((crc & 0xFF00) >> 8) ^ tbl[(crc & 0x00FF) ^ (data & 0x00FF)];
}

void escreve_na_uart(int quantidade_de_bytes, int info){
    if(info != 0) printf("Escrevendo na UART...\n");
    int count = write(uart_filestream, &tx_buffer[0], quantidade_de_bytes);
    
    if(count < 0){
        printf("Ocorreu um erro ao escrever na UART. Encerrando...\n");
        exit(2);
    }

    if(info != 0){
        printf("Dados escritos na UART.\n");
        printf("Valor escrito: ");
        for(int i = 0; i < quantidade_de_bytes; i++) printf("0x%x ", tx_buffer[i]);
        printf("\n");
    }
}

void wakeup(){
    // do_nothing
}

void le_na_uart(int info){
    if(info != 0) printf("Aguardando resposta...\n");

    pause();
    
    int rx_length = read(uart_filestream, (void*)rx_buffer, 255);
    int errnum;

    if(rx_length < 0){
        errnum = errno;
        fprintf(stderr, "Erro ao ler dados da UART. Código: %d. Mensagem: %s\n", errno, strerror(errnum));

        if(retry_count == 3){
            printf("%d tentativas de ler UART falharam. Encerrando...\n");
            exit(3);
        }

        printf("Tentando conectar de novo...\n");
        le_na_uart(info);
        retry_count++;

    } else if(rx_length == 0){
        printf("Nenhuma resposta obtida.\n");

        if(retry_count == 3){
            printf("%d tentativas de ler UART falharam. Encerrando...\n");
            exit(4);
        }

        printf("Tentando conectar de novo...\n");
        le_na_uart(info);
        retry_count++;
    }

    rx_buffer[rx_length] = '\0';

    if(info != 0){
        printf("%i bytes lidos: ", rx_length, rx_buffer);
        for(int i = 0; i < rx_length; i++) printf("0x%x ", rx_buffer[i]);
        printf("\n");
    }

    retry_count = 0;
}

void configura_uart(){
    uart_filestream = open("/dev/serial0", O_RDWR | O_NOCTTY | O_NDELAY);

    if(uart_filestream == -1){
        printf("Ocorreu um erro ao inicializar a UART. Encerrando...\n");
        exit(1);
    } else{
        printf("UART inicializada com sucesso.\n");
    }

    struct termios options;
    tcgetattr(uart_filestream, &options);
    options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush(uart_filestream, TCIFLUSH);
    tcsetattr(uart_filestream, TCSANOW, &options);

    p_tx_buffer = &tx_buffer[0];
}

short calcula_CRC(unsigned char *commands, int size) {
	int i;
	short crc = 0;
	for(i=0;i<size;i++) {
		crc = CRC16(crc, commands[i]);
	}
	return crc;
}

void call_calcula_CRC(int last_pos){
    bytesCRC crc;
    crc.crc = calcula_CRC(tx_buffer, last_pos);
    memcpy(&tx_buffer[last_pos], crc.bytes, 2);
    tamanho_do_pacote += 2;
}

void solicita_leitura_sensor_LM35(){
    *p_tx_buffer++ = ENDERECO;
    *p_tx_buffer++ = RECEBER;
    *p_tx_buffer++ = RECEBER_VALOR_LM35;

    // enviar 4 ultimos dígitos da matrícula
    *p_tx_buffer++ = 0x04;
    *p_tx_buffer++ = 0x03;
    *p_tx_buffer++ = 0x08;
    *p_tx_buffer++ = 0x06;

    tamanho_do_pacote = 7;
    call_calcula_CRC(tamanho_do_pacote);
}

void solicita_leitura_potenciometro(){
    *p_tx_buffer++ = ENDERECO;
    *p_tx_buffer++ = RECEBER;
    *p_tx_buffer++ = RECEBER_VALOR_POTENCIOMETRO;

    // enviar 4 ultimos dígitos da matrícula
    *p_tx_buffer++ = 0x04;
    *p_tx_buffer++ = 0x03;
    *p_tx_buffer++ = 0x08;
    *p_tx_buffer++ = 0x06;

    tamanho_do_pacote = 7;
    call_calcula_CRC(tamanho_do_pacote);
}

float converte_pacote_em_float(){
    float dado = 0;
    memcpy(&dado, &rx_buffer[3], 4);

    return dado;
}

void clear_buffer(){
    p_tx_buffer = &tx_buffer[0];
}

float get_LM35_reading(){
    alarm(1);
    signal(SIGALRM, wakeup);

    clear_buffer();
    solicita_leitura_sensor_LM35();
    escreve_na_uart(tamanho_do_pacote, 0);
    le_na_uart(0);

    return converte_pacote_em_float();
}

float get_potenciometro_reading(){
    alarm(1);
    signal(SIGALRM, wakeup);

    clear_buffer();
    solicita_leitura_potenciometro();
    escreve_na_uart(tamanho_do_pacote, 0);
    le_na_uart(0);

    return converte_pacote_em_float();
}

void close_uart(){
    close(uart_filestream);
}
