#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#define ENDERECO        0x01

#define RECEBER         0x23
#define RECEBER_INT     0xA1
#define RECEBER_FLOAT   0xA2
#define RECEBER_STRING  0xA3

#define ENVIAR          0x16
#define ENVIAR_INT      0xB1
#define ENVIAR_FLOAT    0xB2
#define ENVIAR_STRING   0xB3

unsigned char tx_buffer[200];
unsigned char *p_tx_buffer;

unsigned char rx_buffer[256];

int uart_filestream;

char* string_convertida[100];
int tamanho_do_pacote = 0;

typedef union
{
    int inteiro;
    float flutuante;
    unsigned char bytes[4];
} valores;

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

void escreve_na_uart(int quantidade_de_bytes){
    printf("Escrevendo na UART...\n");
    int count = write(uart_filestream, &tx_buffer[0], quantidade_de_bytes);
    
    if(count < 0){
        printf("Ocorreu um erro ao escrever na UART. Encerrando...\n");
        exit(2);
    } else{
        printf("Dados escritos na UART.\n");
        printf("Valor escrito: ");
        for(int i = 0; i < quantidade_de_bytes; i++) printf("0x%x ", tx_buffer[i]);
        printf("\n");
    }
}

void le_na_uart(){
    printf("Aguardando resposta...\n");
    sleep(1);
    
    int rx_length = read(uart_filestream, (void*)rx_buffer, 255);
    int errnum;

    if(rx_length < 0){
        errnum = errno;
        fprintf(stderr, "Erro ao ler dados da UART. Código: %d. Mensagem: %s\n", errno, strerror(errnum));
        exit(3);
    } else if(rx_length == 0){
        printf("Nenhuma resposta obtida.\n");
        exit(4);
    } 
    else {
        rx_buffer[rx_length] = '\0';
        printf("%i bytes lidos: ", rx_length, rx_buffer);
        for(int i = 0; i < rx_length; i++) printf("0x%x ", rx_buffer[i]);
        printf("\n");
    }
}

// TODO: conversor decimal - hexadecimal
void convert_dec_to_hex(){}

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

void solicita_int(){
    *p_tx_buffer++ = ENDERECO;
    *p_tx_buffer++ = RECEBER;
    *p_tx_buffer++ = RECEBER_INT;

    tamanho_do_pacote = 3;
    call_calcula_CRC(tamanho_do_pacote);
}

void solicita_float(){
    *p_tx_buffer++ = ENDERECO;
    *p_tx_buffer++ = RECEBER;
    *p_tx_buffer++ = RECEBER_FLOAT;

    tamanho_do_pacote = 3;
    call_calcula_CRC(tamanho_do_pacote);
}

void solicita_string(){
    *p_tx_buffer++ = ENDERECO;
    *p_tx_buffer++ = RECEBER;
    *p_tx_buffer++ = RECEBER_STRING;

    tamanho_do_pacote = 3;
    call_calcula_CRC(tamanho_do_pacote);
}

void envia_int(){
    *p_tx_buffer++ = ENDERECO;
    *p_tx_buffer++ = ENVIAR;
    *p_tx_buffer++ = ENVIAR_INT;

    valores num;
    printf("Digite um inteiro para enviar:\n");
    scanf("%d",&num.inteiro);
    memcpy(&tx_buffer[3],num.bytes,4);

    tamanho_do_pacote = 7;
    call_calcula_CRC(tamanho_do_pacote);
}

void envia_float(){
    *p_tx_buffer++ = ENDERECO;
    *p_tx_buffer++ = ENVIAR;
    *p_tx_buffer++ = ENVIAR_FLOAT;

    valores num;
    printf("Digite um float para enviar:\n");
    scanf("%f",&num.flutuante);
    memcpy(&tx_buffer[3],num.bytes,4);

    tamanho_do_pacote = 7;
    call_calcula_CRC(tamanho_do_pacote);
}

void envia_string(){
    *p_tx_buffer++ = ENDERECO;
    *p_tx_buffer++ = ENVIAR;
    *p_tx_buffer++ = ENVIAR_STRING;

    char string[100];

    printf("Digite uma string para enviar (máximo 100 bytes): ");
    scanf("%[^\n]", string);

    valores num;
    int tamanho_da_string = strlen(string);
    printf("String = %s\nTamanho = %d\n", string, tamanho_da_string);

    num.inteiro = tamanho_da_string;
    memcpy(&tx_buffer[3], num.bytes, 4);

    // *p_tx_buffer++ = 0x0D; // tamanho da string
    memcpy(&tx_buffer[4], &string, tamanho_da_string);
    tamanho_do_pacote = 3 + tamanho_da_string;

    call_calcula_CRC(tamanho_do_pacote+1);
}

int converte_pacote_em_int(){
    int dado = 0;
    memcpy(&dado, &rx_buffer[3], 4);

    return dado;
}

float converte_pacote_em_float(){
    float dado = 0;
    memcpy(&dado, &rx_buffer[3], 4);

    return dado;
}

void converte_pacote_em_string(){
    int tamanho_da_string = 0;
    memcpy(&tamanho_da_string, &rx_buffer[3], 1);

    memcpy(&string_convertida, &rx_buffer[4], tamanho_da_string);
    string_convertida[tamanho_da_string+1] = '\0';
}

int main(){

    configura_uart();
    envia_string();
    escreve_na_uart(tamanho_do_pacote);
    le_na_uart();

    converte_pacote_em_string();
    printf("String recebida = %s\n", string_convertida);

    close(uart_filestream);

    return 0;
}
