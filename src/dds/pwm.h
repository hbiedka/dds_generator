//pwm.h - używa timera4 do generowania PWM na pinie nr D6 (OC4A)

//WGMn[3..0] = b1000 (PWM phase and freq correct, TOP in ICRn)

//COMnA[1..0] = b10 (PWM non-invereting mode)
//COMnB[1..0] = b00 (off)
//COMnC[1..0] = b00 (off)

#define F_CPU 16000000

void pwmInit() {
    TCCR4A=B10000000;
    TCCR4B=B00010001; //domyślnie prescaler na 1
    TIMSK4=B00000000; //Wyłączenie przerwań

    DDRH =  DDRH | B00001000;
}

void pwmSetTimer(uint8_t presc, uint16_t icr, uint16_t ocr) {
    
    //set prescaler
    uint8_t tcr = TCCR4B & B11111000;
    presc &= B00000111;
    TCCR4B = tcr | presc;

    //set ICR 
    ICR4H = icr >> 8;
    ICR4L = icr & 255;

     //set OCR
    OCR4AH = ocr >> 8;
    OCR4AL = ocr & 255;
}

void pwmSet(long int freq, uint8_t duty) {

    uint8_t presc = 1;
    uint16_t icr;
    uint16_t ocr;
    
    //częstotliwość obliczamy ze wzoru f_osc = f_clk/(2N*icr)
    //jako że freq jest w hz*10 to n*icr = f_clk*10/2*f_osc -> n*iicr = f_clk*5/f_osc
    float ficr = 5*F_CPU/freq;
    long int nicr = (long int)ficr;

    //następnie dobieramy preskaler
    if (nicr > 0x0FFFF) {
        // 8
        nicr = nicr >> 3;
        presc = 2;
    }

    if (nicr > 0x0FFFF) {
        // 64
        nicr = nicr >> 3;
        presc = 3;
    }
    
    if (nicr > 0x0FFFF) {
        // 256
        nicr = nicr >> 2;
        presc = 4;
    }
    
    if (nicr > 0x0FFFF) {
        // 1024
        nicr = nicr >> 2;
        presc = 5;
    }

    if (nicr > 0x0FFFF) nicr = 0xFFFF;

    icr = (uint16_t)nicr;
    
     
    //obliczanie wypełnienia
    float fduty = duty/100.0;
    float locr = icr*fduty; 
    ocr = (uint16_t)locr;

    //aktualizacja danych
    pwmSetTimer(presc,icr,ocr);
}
