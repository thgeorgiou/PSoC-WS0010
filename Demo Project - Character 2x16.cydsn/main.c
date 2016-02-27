/**
 * @file main.c
 * @author Thanasis Georgiou
 * @brief Demo for the WS0010 display component.
 */

#include <project.h>

int digitCount(unsigned long n);
unsigned long time = 0;

CY_ISR(TimerInterrupt) {
    time++;
    
    unsigned long currentTime = time;
    int digits = digitCount(currentTime);
    WS0010_Position(16 - digits, 1);
       
    char timeString[digits + 1];
    
    digits--;
    timeString[digits] = '\0';
    
    while (digits != -1) {
        timeString[digits] = (currentTime % 10) + 48;
        currentTime /= 10;
        digits--;
    }
    
    WS0010_PrintString(timeString);
}

int main() {
    CyGlobalIntEnable; /* Enable global interrupts. */

    // Initialize display
    WS0010_Init();
    
    WS0010_PrintString("Hello PSoC! ");
    WS0010_Position(0, 1);
    WS0010_PrintString("Uptime (s): ");
    
    // Start timer
    ISR_Timer_StartEx(TimerInterrupt);
    Timer_1Hz_Start();
    
    // Main loop
    for(;;) {
    // Uncomment block to try out sleep functionality!  
        /*
        CyDelay(2 * 1000);
        WS0010_Sleep();
        CyDelay(2 * 1000);
        WS0010_Wakeup();
        */
    }
}

int digitCount(unsigned long n) {
    if (n == 0) return 0;
    if (n < 10) return 1;
    if (n < 100) return 2;
    if (n < 1000) return 3;
    if (n < 10000) return 4;
    if (n < 100000) return 5;
    if (n < 1000000) return 6;
    if (n < 10000000) return 7;
    if (n < 100000000) return 8;
    if (n < 1000000000) return 9;
    return 10;
}

/* [] END OF FILE */
