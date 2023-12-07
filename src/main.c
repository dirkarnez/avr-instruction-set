// Online C compiler to run C program online
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int twosComplement(int num, int bits) {
    if (num >= 0) {
        return num;
    } else {
        return (1 << bits) + num;
    }
}

/*
    LDI Rd,K
    1110 KKKK dddd KKKK
*/
void ldi(unsigned char *code, int size, int registerN, unsigned char data) {
    unsigned char template[] = {0xE0, 0}; // {0b1110, 0, 0, 0}; binary form takes 4 element

    unsigned char eightbitdataH = data >> 4;
    unsigned char eightbitdataL = data & 15;

    code[0] = template[0] | eightbitdataH;
    code[1] = template[1] | ((registerN - 16) << 4) | eightbitdataL;
}

/*
    SBI A,b 
    1001 1010 AAAA Abbb
*/
void sbi(unsigned char *code, int size, int AAAA, unsigned char bbb) {
    unsigned char template[] = {0x9A, 0}; // {0b1001, 0b1010, 0, 0}; binary form takes 4 element
    code[0] = template[0];
    code[1] = template[1] | (AAAA << 3) | bbb;
}

/*
    CBI A,b 
    1001 1000 AAAA Abbb
*/
void cbi(unsigned char *code, int size, int AAAA, unsigned char bbb) {
    unsigned char template[] = {0x98, 0}; // {0b1001, 0b1010, 0, 0}; binary form takes 4 element
    code[0] = template[0];
    code[1] = template[1] | (AAAA << 3) | bbb;
}

/*
    BRNE k
    1111 01kk kkkk k001
*/
void brne(unsigned char *code, int size, int kkkkkkk) {
    unsigned char template[] = {0xF4, 0x01}; 

    int twoscomplement = twosComplement(kkkkkkk, 7);
    
    code[0] = template[0] | twoscomplement >> 5;
    code[1] = template[1] | ((twoscomplement & 0b0011111) << 3);
}

int main() {
    const size_t size = 2; // 1 for the invisible \0
	unsigned char code[size];

    memset( code, '\0', size * sizeof(char));
    // 00 e2       	ldi	r16, 0x20
    ldi(code, size, 16,  0x20);
    printf("%02x %02x, %02x %02x\n", code[1], code[0], 0x00, 0xe2); // little-endian


    memset( code, '\0', size * sizeof(char));
    // 1f ef ldi	r17, 0xFF
    ldi(code, size, 17,  0xFF);
    printf("%02x %02x, %02x %02x\n", code[1], code[0], 0x1f, 0xef); // little-endian

    
    memset( code, '\0', size * sizeof(char));
    //  2f ef       ldi	r18, 0xFF
    ldi(code, size, 18,  0xFF);
    printf("%02x %02x, %02x %02x\n", code[1], code[0], 0x2f, 0xef); // little-endian

    
    
    
    
    memset( code, '\0', size * sizeof(char));
    // 25 9a       	sbi	0x04, 5
    sbi(code, size, 0x04, 5);
    printf("%02x %02x, %02x %02x\n", code[1], code[0], 0x25, 0x9a); // little-endian


    memset( code, '\0', size * sizeof(char));
    // 2d 9a       	sbi	0x05, 5	; 5
    sbi(code, size, 0x05, 5);
    printf("%02x %02x, %02x %02x\n", code[1], code[0], 0x2d, 0x9a); // little-endian



    memset( code, '\0', size * sizeof(char));
    // 2d 98       	cbi	0x05, 5	; 5
    cbi(code, size, 0x05, 5);
    printf("%02x %02x, %02x %02x\n", code[1], code[0], 0x2d, 0x98); // little-endian




    memset( code, '\0', size * sizeof(char));
    // f1 f7       	brne	.-4
    brne(code, size, -4);
    printf("%02x %02x, %02x %02x\n", code[1], code[0], 0xf1, 0xf7); // little-endian





    return 0;
}
