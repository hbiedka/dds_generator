#include <avr/pgmspace.h>
#include <Timers.h>

#include "fonts/ascii_table.h"
#include "fonts/Font16.h"
#include "fonts/Font32.h"
#include "fonts/Font64.h"
#include "fonts/miscSprites.h"

#define LCD_RST 6
#define LCD_CS 4
#define LCD_RS 2
#define LCD_WR 0
#define LCD_RD 1

#define LCD_COM_PORT PORTF
#define LCD_COM_DDR  DDRF
#define LCD_COM_IOMASK B01010111

// Touchscreen connection:
#define Y1 A4  //(CS) need two analog inputs
#define X1 A2  //(RS)
#define Y2 23   // PA1 (d1)
#define X2 22   // PA0 (d0)

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define ORANGE  0xFE29

#define FONT8 0
#define FONT16 1
#define FONT32 2
#define FONT64 3

// TS calibration
uint16_t ROW_F=116; // TS first row
uint16_t ROW_L=900; // TS last row
uint16_t COL_F=110; // TS first column
uint16_t COL_L=880; // TS last column

//wymiary wyświetlacza
#define LCD_WIDTH 320
#define LCD_HEIGHT 240

int16_t curX=0;
int16_t curY=0;

struct Touch {int16_t x; int16_t y; bool touched; };
struct Rect {int16_t x; int16_t y; int16_t wid; int16_t hei; };

void cSet(byte pin) { LCD_COM_PORT = LCD_COM_PORT | (1 << pin); }
void cReset(byte pin) { LCD_COM_PORT = LCD_COM_PORT & (~(1 << pin)); }


void LCD_write(uint8_t d) {
  // ILI9341 reads data pins when WR rises from LOW to HIGH (A1 pin on arduino)
  cReset(LCD_WR);
  PORTA = d;
  cSet(LCD_WR);
}

void BD_as_input(void) {
  DDRA = 0;
}

void BD_as_output(void) {
  DDRA = 255;
}

void LCD_command_write(uint8_t d) {
  cReset(LCD_RS);
  // write data pins
  LCD_write(d);
}

void LCD_data_write(uint8_t d) {
  cSet(LCD_RS);
  // write data pins
  LCD_write(d);
}

uint8_t LCD_read(void) {
  // CS LOW, WR HIGH, RD HIGH->LOW>HIGH, RS(D/C) HIGH 
  cSet(LCD_RS);  // RS 1
  
  // LCD_RD - arduino pin A0
  // After RD falls from HIGH to LOW ILI9341 outputs data until RD returns to HIGH
  cReset(LCD_RD); // RD 0
  
  BD_as_input(); // Set arduino pins as input

  uint8_t pin = PINA;

  cSet(LCD_RD); // RD 1
  BD_as_output(); // Re-Set arduino pins as output
  return pin;
}

void LCD_clear(byte color) {
  /* 
  Accelerate screen clearing sacrifing color depth. Instead of writing
  to data bits high and low byte of the color for each pixel, which takes more 
  than 300ms to fill the screen, set once data bits to 0's for black or 
  to 1's for white and start changing control bit WR from LOW to HIGH to 
  write whole area. It takes cca 70 ms. In this way the color of screen are
  limited to those with the same high and low byte. For example setting color
  to 0x0C fills the screen with color 0x0C0C.
  Writing two pixels in one cycle lowering cycle count from 76800 (240x320) to 
  38400 clears screen in less then 30ms.
  */
  
  LCD_command_write(0x2a); 
  LCD_data_write(0);
  LCD_data_write(0);
  LCD_data_write(0);
  LCD_data_write(0xEC);
  LCD_command_write(0x2b); 
  LCD_data_write(0); 
  LCD_data_write(0);
  LCD_data_write(1);
  LCD_data_write(0x3F);
  LCD_command_write(0x2c);
  
  //PORTC = PORTC | B00000100; // LCD_RS = 1 - DATA
  cSet(LCD_RS);
  PORTA = color;

  uint16_t x;
  x=38400; // 240*320/2


  byte wr0=PORTF & B11111110; // set WR 0
  byte wr1=PORTF | B00000001; // set WR 1

  for(uint16_t y=0;y<x;y++)
  {
    PORTF = wr0;
    PORTF = wr1;
    PORTF = wr0;
    PORTF = wr1;
    
    PORTF = wr0;
    PORTF = wr1;
    PORTF = wr0;
    PORTF = wr1;
  }
}

//--- KONIEC FUNKCJI ZALEZNYCH OD PORTOW ----

void LCD_init(void) {
  BD_as_output();
  LCD_COM_DDR = LCD_COM_DDR | LCD_COM_IOMASK;
  
  // LCD_RESET 1 - 0 - 1, arduino pin A4
  cSet(LCD_RST);
  delay(10);
  cReset(LCD_RST);
  delay(20);
  cSet(LCD_RST);
  delay(20);
  
  // CS HIGH, WR HIGH, RD HIGH, CS LOW  
  cSet(LCD_CS);
  cSet(LCD_WR);
  cSet(LCD_RD);
  cReset(LCD_CS);
  
  LCD_command_write(0xF7); // Pump ratio control
  LCD_data_write(0x20); // 
  
  LCD_command_write(0x3A); // COLMOD: Pixel Format Set
  LCD_data_write(0x55); 
  
  LCD_command_write(0x36); // Memory Access Control 
  // MY  - Row Address Order (bit7)
  // MX  - Column Address Order
  // MV  - Row / Column Exchange
  // ML  - Vertical Refresh Order
  // BGR - RGB-BGR Order
  // MH  - Horizontal Refresh ORDER(bit2)
  LCD_data_write(B00001000); 
  
  LCD_command_write(0x11); // Sleep OUT
  LCD_command_write(0x29); // Display ON
  
  delay(50);
}

//creates rectangle object
Rect newRect(int16_t x, int16_t y, int16_t wid, int16_t hei) {
  Rect r;
  r.x = x;
  r.y = y;
  r.wid = wid;
  r.hei = hei;
  
  return r;
}

void LCD_rect(int16_t col,int16_t row, int16_t width, int16_t height, int16_t color) {
  
  LCD_command_write(0x2a); // Column Address Set
  LCD_data_write(row>>8);
  LCD_data_write(row);
  LCD_data_write((row+height-1)>>8);
  LCD_data_write(row+height-1);
  LCD_command_write(0x2b); // Page Address Set
  LCD_data_write(col>>8); 
  LCD_data_write(col);
  LCD_data_write((col+width-1)>>8);
  LCD_data_write(col+width-1);
  LCD_command_write(0x2c); // Memory Write
 
  byte chigh=color >> 8;
  byte clow=color;
  int i,j;
  for(i=0;i<width;i++)
    for(j=0;j<height;j++)
    {
      LCD_data_write(chigh);
      LCD_data_write(clow);
    }
}

void LCD_rectArea(Rect r, int16_t color) {
  LCD_rect(r.x, r.y, r.wid, r.hei, color);
}

void Disp_rle_sprite(byte *sprData, int16_t x, int16_t y, int8_t rows, int8_t cols, int8_t size, int16_t color, int16_t bcolor) {
  byte bchigh=bcolor >> 8;
  byte bclow=bcolor;
  byte fchigh=color >> 8;
  byte fclow=color;
  
  byte c, len;
  byte adr;
  unsigned int cind;  

    //ustalanie pozycji znaku i jego wielkości
    LCD_command_write(0x2a); // Wiersze

    LCD_data_write(y>>8);
    LCD_data_write(y);
    LCD_data_write(((y+rows)-1)>>8);
    LCD_data_write((y+rows)-1);

    LCD_command_write(0x2b); // Kolumny
    LCD_data_write(x>>8); 
    LCD_data_write(x);
    LCD_data_write((x+cols)>>8);
    LCD_data_write(x+cols);
    LCD_command_write(0x2c);

    cind = 0;
    adr = 0;

    //do zamalowania wszystkich pikseli
    while (cind < (rows*cols)) {

      //odczyt wartości z pamięcie flash
      c = pgm_read_byte(sprData+adr);
      adr++;

      //oddzielenie danych o kolorze i ilości piklseli
      len = c & B01111111;
      len++;
      c = c & B10000000;

      //powtórz tyle razy, ile jest pikseli
      for (int i = 0 ; i < len; i++) {
            if (c) {
                //ustaw piksel na kolor znaku
                LCD_data_write(fchigh);
                LCD_data_write(fclow);
            } else {
                //ustaw piksel na kolor tła
                LCD_data_write(bchigh);
                LCD_data_write(bclow);
            }
            //dodaj 1 do licznika pikseli
            cind++;
      }
    }
}

void Disp_sprite(unsigned char *sprData, int16_t x, int16_t y, int8_t x8rows, int8_t cols, int8_t size, int16_t color, int16_t bcolor) {
  
  byte bchigh=bcolor >> 8;
  byte bclow=bcolor;
  byte fchigh=color >> 8;
  byte fclow=color;
  byte rind, cind, nbit, i, j;
    
    //ustaw kursor
    LCD_command_write(0x2a); // ROWS

    LCD_data_write(y>>8);
    LCD_data_write(y);
    
    LCD_data_write(((y+(x8rows*size*8))-1)>>8);
    LCD_data_write((y+(x8rows*size*8))-1);


    LCD_command_write(0x2b); // COLUMNS
    LCD_data_write(x>>8); 
    LCD_data_write(x);
    LCD_data_write((x+(size*cols))>>8);
    LCD_data_write(x+(size*cols));
    LCD_command_write(0x2c);

    //maluj (LSB first)

    byte col;
    byte adr;

    for (cind = 0; cind < cols; cind++ ) {
      for (i= 0; i<size; i++) {
        for (rind=0; rind < x8rows; rind++) {
          //char col=sprData[((cind*x8rows)+rind)];
          adr = (cind*x8rows)+rind;
          col = pgm_read_byte(sprData+adr);
          byte mask=1;
          for (nbit = 0; nbit < 8; nbit++) {
            if (col & mask) {
              for (j=0; j<size; j++){
                LCD_data_write(fchigh);
                LCD_data_write(fclow);
              }
            }
            else {
              for (j=0; j<size; j++){
                LCD_data_write(bchigh);
                LCD_data_write(bclow);
              }
            }
            mask=mask<<1;
          }
        }
      }
    }
}

void Disp_string(String str, byte font, int16_t x, int16_t y, int8_t size, int16_t color, int16_t bcolor) {
  
  int16_t px=x;
  int16_t py=y;
  
  byte wid, hei;
  byte marg = 0;
  unsigned char *c;


  if (font == FONT8) {
    //czcionka 5x8, standardowa
    hei = 8;
    marg = 1;
  }
  else if (font == FONT16) {
    //czcionka 16
    hei = chr_hgt_f16;
    marg = 1;
  }
  else if (font == FONT32) {
    //czcionka 32 RLE
    hei = chr_hgt_f32;
    marg = 0;
  }
  else if (font == FONT64) {
    //czcionka 64 RLE
    hei = chr_hgt_f64;
    marg = 0;
  }

  for(int n=0; n<str.length(); n++) {

    //zawijanie wierszy
    //przeniesione z disp_char
    if( (px+(size*6)) > LCD_WIDTH) {
      px=0;
      py+=size*(hei+1);
    }

    //drukuj znak
    //dla czcionek RLE size zaspawany jako 1
    if (font == FONT8) {
      wid = 5; //stała szerokość
      c = &ASCII[(str.charAt(n) - 0x20)*5];
      Disp_sprite(c,px,py,1,5,size,color,bcolor);
    } 
    else if (font == FONT16) {
      wid = pgm_read_byte(widtbl_f16+str.charAt(n)-firstchr_f16);
      c = pgm_read_word(&chrtbl_f16[str.charAt(n)-firstchr_f16]);
      Disp_sprite(c,px,py,2,wid,size,color,bcolor);
    } 
    else if (font == FONT32) {
      wid = pgm_read_byte(widtbl_f32+str.charAt(n)-firstchr_f32);
      c = pgm_read_word(&chrtbl_f32[str.charAt(n)-firstchr_f32]);
      Disp_rle_sprite(c,px,py,hei,wid,1,color,bcolor);
    }
    else if (font == FONT64) {
      wid = pgm_read_byte(widtbl_f64+str.charAt(n)-firstchr_f64);
      c = pgm_read_word(&chrtbl_f64[str.charAt(n)-firstchr_f64]);
      Disp_rle_sprite(c,px,py,hei,wid,1,color,bcolor);
    }
    //przenienie z Disp_char - przesunięcie
    px+=size*(wid + marg); //zzmienna marg zależy od czcionki
  }


  //wyrzut wartości do globalnych ustawień kursora
  curX = px;
  curY = py;
}


void Disp_stringWithIndex(String str,byte font, byte indexFont, int16_t x, int16_t y, int8_t size, int16_t color, int16_t bcolor) {
  //explode string by _
  String mainStr = str;
  String indexStr = str;
  
  boolean isIndex = false;
  for (int i = 0; i < str.length(); i++) {
    if (str.charAt(i) == '_' ) {
      mainStr.remove(i);
      indexStr.remove(0,i+1);
      isIndex = true;
      break;
    }
  }

  Disp_string(mainStr,font,x,y,size,color,bcolor);
  if(isIndex) Disp_string(indexStr,indexFont,curX,curY+15,size,color,bcolor);
}
 
void Disp_float(float n, byte prec, byte font, int16_t x, int16_t y, int8_t size, int16_t color, int16_t bcolor) {
  String str=String(n,prec);
  Disp_string(str,font,x,y,size,color,bcolor);
}

void Disp_integer(int16_t n, byte font, int16_t x, int16_t y, int8_t size, int16_t color, int16_t bcolor) {
  String str=String(n);
  Disp_string(str,font,x,y,size,color,bcolor);
}

//rectangle with round corners
void LCD_rect_fillet(Rect r, int16_t color, int16_t bgColor) {
  LCD_rect(r.x, r.y, r.wid, r.hei, color);
  Disp_sprite(LUcorner,r.x,r.y,1,8,1,color,bgColor);
  Disp_sprite(RUcorner,(r.x+r.wid-8),r.y,1,8,1,color,bgColor);
  Disp_sprite(LDcorner,r.x,(r.y+r.hei-8),1,8,1,color,bgColor);
  Disp_sprite(RDcorner,(r.x+r.wid-8),(r.y+r.hei-8),1,8,1,color,bgColor);
}

Touch getTouch(void) {
   
  int16_t row, col;
  int8_t touch;
  Touch tData;
  tData.x=0; tData.y=0; tData.touched=0;

    pinMode(Y1, INPUT); 
    pinMode(Y2, INPUT_PULLUP); 
    
    pinMode(X1, OUTPUT);
    pinMode(X2, OUTPUT);
    digitalWrite(X1, LOW);
    digitalWrite(X2, LOW);

    delay(2);
    
    touch = !digitalRead(Y1); // 0 - touched
    if (touch) {
      digitalWrite(X2, HIGH); 
      delay(1);
      row = analogRead(Y1);
      delay(4); 
      if (abs(analogRead(Y1)-row)>3) { return tData;}
      delay(3);
      if (abs(analogRead(Y1)-row)>3) { return tData;}
      
      pinMode(X1, INPUT); 
      pinMode(X2, INPUT_PULLUP); 
      
      pinMode(Y1, OUTPUT);
      pinMode(Y2, OUTPUT);

      //odczyt przetwornika ADC
      digitalWrite(Y1, LOW);  // Y variant B
      digitalWrite(Y2, HIGH);  // Y variant B
      delay(1);
      col = analogRead(X1);
      delay(4);  
      if (abs(analogRead(X1)-col)>3) { return tData;}  //zwroci daną(0,0,FALSE)
      delay(3);
      if (abs(analogRead(X1)-col)>3) { return tData;}

      digitalWrite(Y2, LOW);  // Y variant B

      touch = !digitalRead(X1); // 0 - dotyk
      if (touch) {
        int16_t rows=ROW_L-ROW_F;
        int16_t cols=COL_L-COL_F;
        float row1=float(row-ROW_F)/rows*240;
        float col1=float(col-COL_F)/cols*320;
        
        if(col1 < 320 && row1 < 240) {
          tData.y=int(row1);
          tData.x=int(col1);
          tData.touched=1;
        }
      }
  }
  // Re-Set A2 A3 8 9 for ILI9341

  delay(2);
  BD_as_output();


  //resetuje rejestry DDR
  LCD_COM_DDR = LCD_COM_DDR | LCD_COM_IOMASK;

  return tData;
}


boolean isTouchedIn(Touch pos, Rect area, signed char margin) {
  //if not touched
  if (!pos.touched) return 0;

  //touching borderlines
  int16_t bLeft  = area.x - margin;
  int16_t bRight = area.x + area.wid + margin;

  int16_t bUp   = area.y - margin;
  int16_t bDown = area.y + area.hei + margin;

  return (pos.x >= bLeft && pos.x <= bRight && pos.y >= bUp && pos.y <= bDown);

}

//obsługa ekranu dotykowego
#define NONE 0
#define PRESSED 1
#define HOLD 2
#define RELEASED 3

Timer timer;

struct TouchData {byte rectId; byte action; };
Rect * buttonSet;
volatile byte buttonSetSize = 0;
volatile byte pressedId = 255;

volatile boolean pir = false; //wskazuje, czy timer teraz liczy czas opadania (f) czy trzymania(t)
volatile boolean timerStarted = false; //gdy true, timer1 pracuje

volatile boolean pressed = false;
volatile boolean hold = false;

volatile unsigned long timerBuf = 0;

void setButtonMap(Rect* bSet, byte size) {
  //zaktualizuj wskaźnik na zestaw przycisków i rozmiar
  buttonSet = bSet;
  buttonSetSize = size;
  pressedId = 255;
}

TouchData checkTouchScreen(void) {

  int pressTime = 20; //20ms
  int releaseTime = 50;
  int holdTime = 500;

  //odczyt pozycji ekranu
  Touch pos = getTouch();
  TouchData dat;
  dat.rectId=0;
  dat.action = NONE; 
  //sprawdzanie
  
  byte pressedButton = 255;

  //na początku przelatujemy przez wszystkie przyciski i sprawdzamy, czy któryś jest wciśnięty i czy nie nastąpiło "zjechanie" na inny przycisk
  for (byte i = 0; i < buttonSetSize; i++) {
    //gdy przycisk wciśnięty i nie zjechano na inny przycisk
    if (isTouchedIn(pos,buttonSet[i],0) && (pressedId == 255 || pressedId == i)) {
      pressedButton = i;
      pressedId = i;
      break;
    }

  }

  if(pressedButton != 255) {
    //press action
    if(!pressed) {
      //gdy timer nie jest uruchomiony albo jest uruchomiony ale dla opadania
      if (!timerStarted || (timerStarted && !pir)) {
        //wystartuj timer 
        timer.begin(pressTime);

        timerStarted = true;
        pir = true;
      }

      if(timerStarted && pir && timer.available()) {
        //gdy przyszło nieobsłużone przerwanie
        timerStarted = false;
        pressed = true;

        dat.action = PRESSED;
        dat.rectId = pressedButton;
        pressedId = pressedButton;
      }
    } else {
      //gdy przycisk już wciśnięty
      pressedId = pressedButton;
      
      //gdy zaczął działać timer opadający
      if(timerStarted && !pir) {
        //przywróć timer
        timer.time(timerBuf);
        pir = true;
      }
      
      //gdy już jest wciśnięty, zbuforuj wartość zegara. 
      //Gdy na chwile zwolnimy przycisk po zarejestrowaniu wciśnięcia, wartość ta zostanie przywrócona
      timerBuf = timer.time();

      //gdy timer jeszcze nie wystartował
      if(!timerStarted && !hold) {
        //przywróć bufor i wystartuj
        timer.begin(holdTime);
        timerStarted = true;
        pir = true;
      }

      if(timerStarted && pir && timer.available()) {
        //zatrzymaj timer
        timerStarted = false;
        hold = true;

        dat.action = HOLD;
        dat.rectId = pressedButton;
      }
    }
    
    
  } else {
    
    //gdy przycisk jest zarejestrowany jako wciśnięty
    if(pressed) {
      //gdy timer nie jest uruchomiony lub jest urubhomiony ale dla na trzymanie
      if(!timerStarted || (timerStarted && pir)) {
        //ustaw przerwanie
        timer.begin(releaseTime);
        timerStarted = true;
        pir = false;
      }


      if(timerStarted && !pir && timer.available()) {
        timerStarted = false;
        pressed = false;
        hold = false;

        dat.action = RELEASED;
        dat.rectId = pressedId;
        pressedId = 255;
      }        
    } else {
      //Zatrzymaj liczenie
      timer.time(STOP);
      timerStarted = false;

    }
  }
return dat;
}
