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

// Global variables
volatile uint16_t program = 1;        // Current program (1-9)
volatile uint16_t timer1_counter = 0; // Timer1 interrupt counter
volatile uint8_t timer1_flag = 0;     // Timer1 period flag
volatile uint8_t debounce_counter = 0; // Button debounce counter

// Timer1 Interrupt Service Routine
void __attribute__((interrupt, auto_psv)) _T1Interrupt(void) {
    timer1_counter++;
    
    // Update debounce counter if active
    if(debounce_counter > 0) debounce_counter--;
    
    // Set flag every 2000 counts (~1s at 8MHz with 1:8 prescaler)
    if(timer1_counter >= 2000) {
        timer1_flag = 1;
        timer1_counter = 0;
    }
    
    IFS0bits.T1IF = 0;  // Clear Timer1 interrupt flag
}

// Change Notification Interrupt for buttons
void __attribute__((interrupt, auto_psv)) _CNInterrupt(void) {
    if (debounce_counter == 0) {
        if (PORTDbits.RD6 == 0) {      // Next program button
            program = (program % 9) + 1; // Wrap around from 9 to 1
            debounce_counter = 20;      // ~20ms debounce
        } 
        else if (PORTDbits.RD13 == 0) { // Previous program button
            program = (program == 1) ? 9 : program - 1;
            debounce_counter = 20;
        }
    }
    IFS1bits.CNIF = 0;  // Clear CN interrupt flag
}

void configure_Timer1(void) {
    T1CON = 0x8010;     // rejestr od zegara 1:8 prescaler
    PR1 = 499;          // Period register for 1ms interrupt at 8MHz
    IFS0bits.T1IF = 0;  // Clear Timer1 interrupt flag
    IEC0bits.T1IE = 1;  // Enable Timer1 interrupt
    IPC0bits.T1IP = 1;  // Set Timer1 interrupt priority
}

void configure_CN(void) {
    CNEN1bits.CN6IE = 1;   // RD6 (next program)
    CNEN2bits.CN13IE = 1;  // RD13 (previous program)
    TRISDbits.TRISD6 = 1;  // RD6 as input
    TRISDbits.TRISD13 = 1; // RD13 as input
    CNPU1bits.CN6PUE = 1;  // Enable pull-up on RD6
    CNPU2bits.CN13PUE = 1; // Enable pull-up on RD13
    IEC1bits.CNIE = 1;     // Enable CN interrupts
    IFS1bits.CNIF = 0;     // Clear CN interrupt flag
}

// Wait for specified number of timer periods (1 period ≈ 1s)
void timer_delay(uint16_t periods) {
    while(periods > 0) {
        timer1_flag = 0;
        while(!timer1_flag); // Wait for timer period
        periods--;
    }
}


void podprogram1(uint16_t a)
{
    // tylko wyswietl
    LATA = a;
    timer_delay(2);
    return;
}

void podprogram3(uint16_t a)
{
    uint16_t gray = a ^ (a >> 1);
    LATA = gray;
    timer_delay(2);
    return;
}

void podprogram5(uint16_t a)
{
    uint16_t dziesiatki = a / 10;
    uint16_t jednosci = a % 10;
    
    uint16_t bcd = (dziesiatki << 4) | jednosci;
    
    LATA = bcd; 
    timer_delay(2);
}

void podprogram7(void)
{
    uint16_t helper = 0b111;
    LATA = helper;
    timer_delay(2);
    for(int i = 0; i < 5; i++)
    {
        helper = helper << 1;
        LATA = helper;
        timer_delay(2);
    }
    for(int i = 0; i < 4; i++)
    {
        helper = helper >> 1;
        LATA = helper;
        timer_delay(2);
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
            timer_delay(2);
        }
    }
    return;
}

uint16_t podprogram9(
uint16_t input, uint16_t m, uint16_t a, uint16_t c)
{
    // trzeba napisac wlasny generator liczb losowych
    // lcg ??
    // 2 diody z lewej nie swiecic ale te bity moga byc w kodzie
    
    uint16_t helper3 = ((input * a) + c) % m;
    uint16_t helper5 = helper3 % 100000;
    helper5 = helper5 >> 2;
    LATA = helper5;
    
    timer_delay(2);
    
    return helper3;
} 
 

int main(void) {
AD1PCFG = 0xFFFF; // set to digital I/O (not analog)
TRISA = 0x0000; // set all port bits to be output

uint16_t liczba1 = 0;

uint16_t liczba2 = 255;

uint16_t liczba4 = 99;

int seed1 = 5; // Seed value 
int m1 = 7; // Modulus parameter 
int a1 = 3; // Multiplier term 
int c1 = 3; // Increment term 
uint16_t helper4; // przechowuje wygenerowany ostatni lcg
helper4 = podprogram9(seed1,m1,a1,c1);

configure_Timer1();

// Configure buttons and interrupts
configure_CN();
    
// Enable interrupts globally
__builtin_enable_interrupts();
    
while(1) {

    // debounce counter
    if (debounce_counter > 0) {
        debounce_counter--;
    }
        
    // Reset counters when program changes
    if (button_pressed) {
        liczba1 = 0;
        liczba2 = 255;
        liczba4 = 99;
        button_pressed = 0;
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
            podprogram7();
            break;
        case 8:
            podprogram8();
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

// logika guzików: kiedy sprawdzasz czy przycisk wcisniety to porównaj jego wartosc w tym momencie

// pin od przycisku normalnie ma 1, jak naciskamy to mamy 0

// de-bouncing trzeba zrobi? na przycisku, jak pierwszy raz wykryje 0 to przez jaki? czas niech nie s?ucha

// w generatorze losowym mozna uzyc and zeby zamaskowac 2 najstarsze bity

// kiedy guzik to zeruj liczbe uzywana w podprogramie
