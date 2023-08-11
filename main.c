// Online C compiler to run C program online
#include <stdio.h>
#include <stdlib.h>

void ldi(unsigned char *teml, int size, int rN, unsigned char eightbitdataH, unsigned char eightbitdataL) {
    // unsigned char* a = (unsigned char*)malloc(2 * sizeof(unsigned char));
    // if (a == NULL) {
    //     // Handle allocation failure
    //     return NULL;
    // }
    // a[0] = 0xe2;
    // a[1] = 0x00;
    // return a;
   // teml[2] =  ;
    teml[0] |= eightbitdataH;
    teml[1] |= ((rN - 16) << 4) | eightbitdataL;
   
}

int main() {
    unsigned char a[] = {0xE0, 0};//{0b1110, 0, 0, 0};
    printf("%02x %02x\n", a[1], a[0]); // little-endian
    
    printf("%d\n", sizeof(a) / sizeof(unsigned char));
    int data = 0xFF;
    ldi(a, sizeof(a) / sizeof(unsigned char), 17, data >> 4, data & 15);
    
    printf("%02x %02x\n", a[1], a[0]); // little-endian
    
    

    return 0;
}
