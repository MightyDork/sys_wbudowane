// PIC24FJ128GA010 Configuration Bit Settings
// For more on Configuration Bits, consult your device data sheet
// CONFIG2
#pragma config POSCMOD = XT // XT Oscillator mode selected
#pragma config OSCIOFNC = ON // OSC2/CLKO/RC15 as port I/O (RC15)
#pragma config FCKSM = CSDCMD // Clock Switching and Monitor disabled
#pragma config FNOSC = PRI // Primary Oscillator (XT, HS, EC)
#pragma config IESO = ON // Int Ext Switch Over Mode enabled
// CONFIG1
#pragma config WDTPS = PS32768 // Watchdog Timer Postscaler (1:32,768)
#pragma config FWPSA = PR128 // WDT Prescaler (1:128)
#pragma config WINDIS = ON // Watchdog Timer Window Mode disabled
#pragma config FWDTEN = OFF // Watchdog Timer disabled
#pragma config ICS = PGx2 // Emulator/debugger uses EMUC2/EMUD2
#pragma config GWRP = OFF // Writes to program memory allowed
#pragma config GCP = OFF // Code protection is disabled
#pragma config JTAGEN = OFF // JTAG port is disabled
// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
#include <xc.h>
#include <libpic30.h>
#include <math.h>

void configure_CN(void)
{
CNEN1bits.CN3IE = 1; // Enable CN3 pin for interrupt detection
IEC1bits.CNIE = 1; // Enable CN interrupts
IFS1bits.CNIF = 0; // Reset CN interrupt
}
void __attribute__ ((interrupt)) _CNInterrupt(void)
{
// Insert ISR code here
// co ma sie dziac, zmiana programu, sprawdz ktory przycisk wcisniety i if zeby w przod lub tyl
IFS1bits.CNIF = 0; // Clear CN interrupt to make it aviable to happen again
}

void podprogram1(uint16_t a)
{
    // tylko wy?wietl
     LATA = a;
    __delay32(1500000);
    return;
}

void podprogram3(uint16_t a)
{
    uint16_t gray = a ^ (a >> 1);
    LATA = gray;
    __delay32(1500000);
    return;
}

void podprogram5(void)
{
    // 2x4 bitowy BCD
    return;
}

void podprogram7(void)
{
    LATA = 0x07;
    __delay32(1500000);
    LATA = 0x0E;
    __delay32(1500000);
    LATA = 0x1C; 
    __delay32(1500000);
    LATA = 0x38; 
    __delay32(1500000);
    LATA = 0x70; 
    __delay32(1500000);
    LATA = 0xE0; 
    __delay32(1500000);
    LATA = 0x70; 
    __delay32(1500000);   
    LATA = 0x38;
    __delay32(1500000);  
    LATA = 0x1C; 
    __delay32(1500000);   
    LATA = 0x0E;
    __delay32(1500000);   
    LATA = 0x07; 
    return;
}

void p7(void)
{
    uint16_t helper = 0b111;
    LATA = helper;
    __delay32(1500000); 
    for(int i = 0; int < 6; i++)
    {
        LATA = helper;
        __delay32(1500000); 
    }
    return;
}

void podprogram8(void)
{   uint16_t helper2 = 0;
    for(int i = 0; i<9;i++)
    {
        helper2 = helper2 + pow(2, 8-i);
        for(int j = 1; j<9-i;j++)
        {
            // i = duze zapelnione
            // j = latajace
            uint16_t helper1 = pow(2, j-1);
            LATA = helper1 + helper2;
            __delay32(1500000);
        }
    }
    return;
}

void podprogram9(uint16_t a)
{
    // trzeba napisac wlasny generator liczb losowych
    // unicode 1F635 200D 1F4AB
    // lcg ??
    
    
    return a;
}

int main(void) {
T1CON = 0x8010;
// Port A access
AD1PCFG = 0xFFFF; // set to digital I/O (not analog)
TRISA = 0x0000; // set all port bits to be output


uint16_t program = 8;

uint16_t liczba1 = 0;

uint16_t liczba2 = 255;

uint16_t liczba4 = 99;

while(1) {
   
    switch (program)
    {
        case 1:
            podprogram1(liczba1);
            liczba1++;
            break;
        case 2:
            podprogram1(liczba2);
            liczba2--;
            break;
        case 3:
            podprogram3(liczba1);
            liczba1++;
            break;
        case 4:
            podprogram3(liczba2);
            liczba2--;
            break;
        case 5:
            podprogram5(liczba1);
            liczba1++;
            break;
        case 6:
            podprogram5(liczba4);
            liczba4--;
            break;
        case 7:
            podprogram7();
            break;
        case 8:
            podprogram8();
            break;
        case 9:
            podprogram9();
            break;
    }
}
return 0;
}

// RD6 RD13 to rejestry do pinów

// input to tris = 1

// logika guzików: kiedy sprawdzasz czy przycisk wci?ni?ty to porównaj jego warto?? w tym momencie

// kiedy guzik to zeruj liczbe

// pin od przycisku normalnie ma 1, jak naciskamy to mamy 0

// de-bouncing trzeba zrobi? na przycisku, jak pierwszy raz wykryje 0 to przez jaki? czas niech nie s?ucha