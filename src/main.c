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

    // 2 is the "add 1" instruction the program counter will do to the offset.
    // But this "add 1" should be done manually in assembly-to-opcode translation (e.g. assembly shows `.-4`, opcode does `.-2`)
    // assembly label does not occupy program address 
    int twoscomplement = twosComplement(kkkkkkk + 2, 7);
    
    code[0] = template[0] | twoscomplement >> 5;
    code[1] = template[1] | ((twoscomplement & 0b11111) << 3);
}

/*
    RJMP k
    1100 kkkk kkkk kkkk
*/
void rjmp(unsigned char *code, int size, int kkkkkkkkkkkk) {
    unsigned char template[] = {0xC0, 0x00};  

    int twoscomplement = twosComplement(kkkkkkkkkkkk / 2, 12); // divide by 2 because rjmp works in words, not bytes
    
    code[0] = template[0] | twoscomplement >> 8;
    code[1] = template[1] | ((twoscomplement & 0b111111111111));
}

/*
    DEC k
    1001 010d dddd 1010
*/
void dec(unsigned char *code, int size, int registerN) {
    unsigned char template[] = {0x94, 0x0A};

    code[0] = template[0] | registerN >> 4;
    code[1] = template[1] | ((registerN & 0b1111) << 4);
}

int main() {
    const size_t size = 2; // 1 for the invisible \0
	unsigned char code[size];

    {
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
    }

    {
        memset( code, '\0', size * sizeof(char));
        // 25 9a       	sbi	0x04, 5
        sbi(code, size, 0x04, 5);
        printf("%02x %02x, %02x %02x\n", code[1], code[0], 0x25, 0x9a); // little-endian

        memset( code, '\0', size * sizeof(char));
        // 2d 9a       	sbi	0x05, 5	; 5
        sbi(code, size, 0x05, 5);
        printf("%02x %02x, %02x %02x\n", code[1], code[0], 0x2d, 0x9a); // little-endian
    }

    {
        memset( code, '\0', size * sizeof(char));
        // 2d 98       	cbi	0x05, 5	; 5
        cbi(code, size, 0x05, 5);
        printf("%02x %02x, %02x %02x\n", code[1], code[0], 0x2d, 0x98); // little-endian
    }

    {
        memset( code, '\0', size * sizeof(char));
        // f1 f7       	brne	.-4     ;  0x12 (.-4 means current program counter address 0x14 - 0x4 = 0x10, and then program counter will add 1, which is 1 instruction (2 bytes in length, therefore, 0x10 + 0x2) = 0x12, the answer is 0x12
        brne(code, size, -4);
        printf("%02x %02x, %02x %02x\n", code[1], code[0], 0xf1, 0xf7); // little-endian
    }

    {
        memset( code, '\0', size * sizeof(char));
        // f7 cf       	rjmp	.-18     	;  0x6
        rjmp(code, size, -18);
        printf("%02x %02x, %02x %02x\n", code[1], code[0], 0xf7, 0xcf); // little-endian
    }

    {
        memset( code, '\0', size * sizeof(char));
        // 2a 95       	dec	r18
        dec(code, size, 18);
        printf("%02x %02x, %02x %02x\n", code[1], code[0], 0x2a, 0x95); // little-endian
    }


    return 0;
}
