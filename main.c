// Ewelina Dolega 164461

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

uint16_t program = 9;
uint16_t liczba1 = 0;

uint16_t liczba2 = 255;

uint16_t liczba4 = 99;

int seed1 = 33; // Seed value 
int m1 = 63; // Modulus parameter 
int a1 = 29; // Multiplier term 
int c1 = 13; // Increment term 
uint16_t helper4; // przechowuje wygenerowany ostatni lcg


uint16_t wonsz_kierunek = 1;
uint16_t wonsz = 0b111;

uint16_t kolejka = 1;
uint16_t kolejka_licznik = 0;

uint16_t podprogram9(
uint16_t input, uint16_t m, uint16_t a, uint16_t c)
{
    // trzeba napisac wlasny generator liczb losowych
    // lcg ??
    // 2 diody z lewej nie swiecic ale te bity moga byc w kodzie
    
    uint16_t helper3 = input * a;
    
    helper3 += c;
    helper3 = helper3 % m;
    /*
    if(helper3 > 64){
        uint16_t helper5 = helper3 % 100000;
        helper5 = helper5 >> 2;
        LATA = helper5;
    }
    else
    {
        LATA = helper3;
    }
    */
    LATA = helper3;
    __delay32(2000000);
    
    return helper3;
}

void configure_CN(void)
{
    CNEN1bits.CN15IE = 1;  // RD6
    CNEN2bits.CN19IE = 1;  // RD13
    CNPU1bits.CN15PUE = 1; // pull-up dla RD6
    CNPU2bits.CN19PUE = 1; // pull-up dla RD13

    IFS1bits.CNIF = 0;
    IEC1bits.CNIE = 1;
    IPC4bits.CNIP = 5;
}

void __attribute__ ((__interrupt__)) _CNInterrupt(void)
{
    if(PORTDbits.RD6 == 0)
    {
        program++;
    }
    if(PORTDbits.RD13 == 0)
    {
        program--;
    }
    if(program > 9)
    {
        program = 1;
    }
    
    if(program < 1)
    {
        program = 9;
    }
          
            
    liczba1 = 0;

    liczba2 = 255;

    liczba4 = 99;

    seed1 = 33; // Seed value 
    m1 = 63; // Modulus parameter 
    a1 = 29; // Multiplier term 
    c1 = 13; // Increment term 
    helper4 = podprogram9(seed1,m1,a1,c1);
        
    wonsz = 0b111;
        
    kolejka = 1;
    kolejka_licznik = 0;
    IFS1bits.CNIF = 0; // Clear CN interrupt
}

void podprogram1(uint16_t a)
{
    // tylko wy?wietl
     LATA = a;
    __delay32(2000000);
    return;
}

void podprogram3(uint16_t a)
{
    uint16_t gray = a ^ (a >> 1);
    LATA = gray;
    __delay32(2000000);
    return;
}

void podprogram5(uint16_t a)
{
    uint16_t dziesiatki = a / 10;
    uint16_t jednosci = a % 10;
    
    uint16_t bcd = (dziesiatki << 4) | jednosci;
    
    LATA = bcd; 
    __delay32(2000000);
}

int main(void) {
T1CON = 0x8010; // rejestr od zegara
AD1PCFG = 0xFFFF; // set to digital I/O (not analog)
TRISA = 0x0000; // set all port bits to be output

configure_CN();

helper4 = podprogram9(seed1,m1,a1,c1);

while(1) {
    
    if(PORTDbits.RD6 == 0)
    {
        program++;
        if(program > 9)
        {
            program = 1;
        }
        if(program < 1)
        {
            program = 9;
        }
            
        liczba1 = 0;

        liczba2 = 255;

        liczba4 = 99;

        seed1 = 33; // Seed value 
        m1 = 63; // Modulus parameter 
        a1 = 29; // Multiplier term 
        c1 = 13; // Increment term 
        helper4 = podprogram9(seed1,m1,a1,c1);
        
        wonsz = 0b111;
        
        kolejka = 1;
        kolejka_licznik = 0;
    }
        
    if(PORTDbits.RD13 == 0)
    {
        program--;
        if(program < 1)
        {
            program = 8;
        }
        if(program < 1)
        {
            program = 9;
        }
           
        liczba1 = 0;

        liczba2 = 255;

        liczba4 = 99;

        seed1 = 33; // Seed value 
        m1 = 63; // Modulus parameter 
        a1 = 29; // Multiplier term 
        c1 = 13; // Increment term 
        helper4 = podprogram9(seed1,m1,a1,c1);
        
        wonsz = 0b111;
        
        kolejka = 1;
        kolejka_licznik = 0;
    }
   
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
            if (liczba1 > 99) 
            {
                liczba1 = 0;
            }
            podprogram5(liczba1);
            liczba1++;
            break;
        case 6:
            if (liczba4 > 99) 
            {
                liczba4 = 0;
            }
            if (liczba4 <= 0) 
            {
                podprogram5(0);
                liczba4 = 99;
            }
            podprogram5(liczba4);
            liczba4--;
            break;
        case 7:
            LATA = wonsz;
            if(wonsz == 0b111)
            {
                wonsz_kierunek = 1;
            }
            if(wonsz == 0b11100000)
            {
                wonsz_kierunek = 2;
            }
            if(wonsz_kierunek == 1)
            {
                wonsz = wonsz << 1;
            }
            if(wonsz_kierunek == 2)
            {
                wonsz = wonsz >> 1;
            }
            __delay32(1500000);
            break;
        case 8:
            // mocno nie działa
            if (kolejka > 255)
            {
                kolejka = 1;
            }
            LATA = kolejka;
            kolejka_licznik++;
            kolejka = kolejka + pow(2, kolejka_licznik);
            __delay32(1500000);
            break;
        case 9:
            helper4 = podprogram9(helper4,m1,a1,c1);
            break;
    }
}

return 0;
}


// RD6 RD13 to rejestry do pinów

// input to tris = 1

// logika guzikuw: kiedy sprawdzasz czy przycisk wcisniety to porównaj jego wartosc w tym momencie

// pin od przycisku normalnie ma 1, jak naciskamy to mamy 0

// de-bouncing trzeba zrobic na przycisku, jak pierwszy raz wykryje 0 to przez jakis czas niech nie slucha

// w generatorze losowym mozna uzyc and zeby zamaskowac 2 najstarsze bity

// kiedy guzik to zeruj liczbe uzywana w podprogramie
