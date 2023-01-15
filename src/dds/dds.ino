#include <avr/pgmspace.h>
#include <math.h>
#include <SPI.h>
#include <MD_AD9833.h>

#include "ILI9341.h"
#include "symbols.h"
#include "pwm.h"
#include "bcdFloat.h"

#include "dds.h"

bcdFloat* tVals[4] = {
  new bcdFloat(10000),
  new bcdFloat(10),
  new bcdFloat(50),
  new bcdFloat(0)
};

MD_AD9833 AD(FSYNC); // Hardware SPI

Rect buttonPos[20]; 

void setFullUpdateFlag(boolean f) {
  for (int i = 0; i < updateFlagIndex; i++) updateFlag[i] = f;
}

void encoder() {
  encd = digitalRead(ENC_B_PIN);
  encd = !encd;
  enci = true;
}

void setup() {

  Serial.begin(115200);
  LCD_init();

  pinMode(BUT_PIN,INPUT_PULLUP);
  pinMode(ENC_A_PIN,INPUT_PULLUP);
  pinMode(ENC_B_PIN,INPUT_PULLUP);

  pinMode(RELAY_PIN,OUTPUT);
  
  //setup dla generatora i pga
  AD.begin();
  pinMode(CS_AMPL,OUTPUT);
  //setup PWMa
  pwmInit();

  attachInterrupt(digitalPinToInterrupt(ENC_A_PIN),encoder,RISING);

  //splash sceeen
  
  LCD_clear(0xFF);
  Disp_sprite(isepLogo,128,80,5,32,2,BLACK,WHITE);

  //startowe komendy
  genUpdate(0); //frequency
  delay(1000);
  genUpdate(1); //amplitude

  selectedWave = 3; //przełącz na PWM
  delay(500);
  genUpdate(0); 
  delay(500);

  selectedWave = 0; //z powrotem na sinus
  genUpdate(5);

  Serial.println("[Ready]");
}

void loop() {
  //gdy zmienił się tryb lub początek pracy wykonaj ustawienia startowe
  if (setMod != mod) {
    
    if (setMod == MOD_HOME) homeSetup();
    if (setMod == MOD_PARAM) paramSetSetup(paramMap[selectedWave][selectedParam]);
    if (setMod == MOD_KBD) keyboardSetup(paramMap[selectedWave][selectedParam]);
    
    
    //aktualizuj mod 
    mod = setMod;
    //czyść ekran
    setFullUpdateFlag(true);

    //opóźnienia są w celu ustabilizowaniu pracy wyświetlacza i zmniejszyć ryzyko jego zawieszenia się
    delay(30);
    LCD_clear((byte)bgColor);
    delay(100);
  }
  //zależnie od trybu
  if (mod==MOD_HOME) homeLoop();
  if (mod==MOD_PARAM) paramSetLoop(paramMap[selectedWave][selectedParam]);
  if (mod == MOD_KBD) keyboardLoop(paramMap[selectedWave][selectedParam]);

  //Serial
  SerialReceive();
}

void homeSetup() {
    //układanie przycisków wyboru fali
    for (int i=0; i<4; i++) {
      buttonPos[i]=newRect(80+(i*60),180,50,50);
    }

    //buttonPos[4] zarezerwowany dla on/off
    buttonPos[4] = newRect(10,180,50,50);
    
    //button pos[5] i [6] - pole frequency i amplitude/duty
    buttonPos[5] = newRect(10,30,300,50);
    buttonPos[6] = newRect(10,90,300,50);
    //buttonPos[7] = newRect(10,110,300,50);

    //remote indicator
    buttonPos[8] = newRect(10,10,70,16);

    setButtonMap(buttonPos,9);
    updateFlagIndex = 15;
}

void homeLoop() {
  //sprawdź ekran dotykowy
  TouchData dat = checkTouchScreen();

  if(dat.action==PRESSED) {
      if (dat.rectId<4) {
        //wybór fali
        updateFlag[selectedWave] = true;
        updateFlag[dat.rectId] = true;

        if (selectedWave == 3 || dat.rectId == 3) {

            //gdy zmiana widoku międy amplitudą a duty Cycle
            updateFlag[6] = true;
        }

        //aktualizuj ampltudę gdy wybrano falę RMS;
        if (selectedWave != 3 && units[1] == 1) updateFlag[9] = true;

        selectedWave = dat.rectId;

       
        //update generatora
        genUpdate(5);

        //update pga (przy zmianie kształtu zmienia się też amplituda)
        genUpdate(1);
        //prześlij informację do komputera
        SerialTransmit(5);
      }

      if (dat.rectId==4) {
        //on-off
        outputOn = !outputOn;
        digitalWrite(RELAY_PIN,outputOn);
        SerialTransmit(6);
        updateFlag[4]=true;
      }

      if (dat.rectId==5 || dat.rectId==6) {
        updateFlag[selectedParam+5] = true;
        updateFlag[dat.rectId] = true;
        selectedParam = dat.rectId - 5;
      }
  }

  if(dat.action == HOLD) {
    if (dat.rectId==5 || dat.rectId==6) {
        selectedParam = dat.rectId -5;
        if (!remote) setMod = 1;
      }
  }

  //sprawdź enkoder
  if (enci) {
    byte p = paramMap[selectedWave][selectedParam];
    byte mux = UnitPows[p][units[p]] - accur[p];

    if (encd) {
    tVals[p]->add(1,mux);
    } else {
    tVals[p]->sub(1,mux);
    }

    //saturacja
    if (tVals[p]->toInt() > upperLimit[p]) tVals[p]->set(upperLimit[p]);
    if (tVals[p]->toInt() < lowerLimit[p]) tVals[p]->set(lowerLimit[p]);
    
    //dopasowywanie
    if (p == 0) {
      if(tVals[p]->length() <= UnitPows[p][units[p]] && units[p] > 0) {
        units[p]--;
        accur[p]=0;
        updateFlag[selectedParam+11] = true;
      }

      if(tVals[p]->length() > UnitPows[p][units[p]+1] && units[p] < (paramUnitNum[p]-1) ) {
        units[p]++;
        accur[p]=2;
        updateFlag[selectedParam+11] = true;
      }

      byte charNum = tVals[p]->length() - UnitPows[p][units[p]] + accur[p]; 

      if(charNum > 3) accur[p]--;
      if(charNum < 3 && accur[p] < UnitPows[p][units[p]]) accur[p]++;
      
      tVals[p]->approx(UnitPows[p][units[p]] - accur[p]);
    }

    //update ustawien w sprzęcie
    genUpdate(p);

    //prześlij informację do komputera
    SerialTransmit(p);

    updateFlag[selectedParam+8]=true;
    delay(10);
    enci = false;
  }

  //update elementów na wyświetlaczu
  for (int i = 0; i < updateFlagIndex; i++) {
    if (updateFlag[i]) {

    if(i < 4) {
      if(selectedWave == i) {
        //gdy ten przycisk jest aktywny
        LCD_rect_fillet(buttonPos[i],butFocusColor,bgColor);
        Disp_sprite(modeButtonPics[i],(buttonPos[i].x+9),(buttonPos[i].y+5),4,32,1,butSymColor,butFocusColor);
        Disp_string(modeButtonSubs[i],FONT8,(buttonPos[i].x+17),(buttonPos[i].y+37),1,butSymColor,butFocusColor);  

      } else {
        //gdy nie
        LCD_rect_fillet(buttonPos[i],butColor,bgColor); 
        Disp_sprite(modeButtonPics[i],(buttonPos[i].x+9),(buttonPos[i].y+5),4,32,1,butSymColor,butColor);
        Disp_string(modeButtonSubs[i],FONT8,(buttonPos[i].x+17),(buttonPos[i].y+37),1,butSymColor,butColor);  
      }
    }       

    //on/off
    if (i==4) {
      if (!outputOn) {
        //turning off
        LCD_rect_fillet(buttonPos[4],RED,bgColor);
        LCD_rect(buttonPos[4].x+5,buttonPos[4].y+25,40,20,BLACK);
        Disp_string("ON",FONT16,buttonPos[4].x+10,buttonPos[4].y+7,1,BLACK,RED);
        Disp_string("OFF",FONT16,buttonPos[4].x+10,buttonPos[4].y+27,1,WHITE,BLACK);
      } else {
        //turning on
        LCD_rect_fillet(buttonPos[4],GREEN,bgColor);
        LCD_rect(buttonPos[4].x+5,buttonPos[4].y+5,40,20,BLACK);
        Disp_string("ON",FONT16,buttonPos[4].x+10,buttonPos[4].y+7,1,WHITE,BLACK);
        Disp_string("OFF",FONT16,buttonPos[4].x+10,buttonPos[4].y+27,1,BLACK,GREEN);
      }
    }

    if (i >= 5 && i <=13) {
      byte par;
      if(i >= 5 && i <= 7) par = i-5;
      if(i >= 8 && i <= 10) par = i-8;
      if(i >= 11 && i <= 13) par = i-11;

      uint16_t col;
      if (selectedParam == par) {
      col = butFocusColor;
      } else {
      col = bgColor;
      }

      byte p = paramMap[selectedWave][par];           
      
      if(i >= 5 && i <= 6) {
        LCD_rectArea(buttonPos[i],col);
        if (par != 2 || selectedWave != 3) {
          Disp_string(paramSubTitles[paramMap[selectedWave][par]],FONT16,buttonPos[i].x+5,buttonPos[i].y+25,1,subTitleColor,col);
          updateFlag[i+3] = true;
          updateFlag[i+6] = true;       
        } else {
          updateFlag[i+3] = false;
          updateFlag[i+6] = false;
        }
      }

      if (i >=8 && i<= 9) {
      //same liczby

      //gdy ustawiona jest wartość rms przelicz jednostkę
      String s;
      if (p == 1 && units[p] == 1) {
        //pobierz wartość
        int vpp = tVals[p]->toInt();

        //teraz dokonaj przemnożenia
        float vrms = float(vpp) * rmsWave[selectedWave];
        bcdFloat rms((long)vrms);
        s = rms.toString(UnitPows[p][units[p]],accur[p],0);
      } else {
        s = tVals[p]->toString(UnitPows[p][units[p]],accur[p],0);
      }

      //dostosuj wielkość
      byte charNum;
      if (tVals[p]->length() < UnitPows[p][units[p]]) {
        charNum = accur[p] + 1; // np 0.0012
      } else {
        charNum = tVals[p]->length() - UnitPows[p][units[p]] + accur[p]; //np 12.3456
      }

      if (charNum > 4) {
        Disp_string(s,FONT32,buttonPos[i-3].x+75,buttonPos[i-3].y+18,1,TitleColor,col);
        LCD_rect(buttonPos[i-3].x+75,buttonPos[i-3].y+2,145,16,col);
      } else {
        Disp_string(s,FONT64,buttonPos[i-3].x+75,buttonPos[i-3].y+2,1,TitleColor,col);
      }
      //uzupełnienie
      LCD_rect(curX,buttonPos[i-3].y+2,buttonPos[i-3].x+220-curX,45,col);
      }

      if (i >= 11 && i <= 12) {
        //jednostki
        Disp_stringWithIndex(ParamUnitsWithIndex[p][units[p]],FONT32,FONT16,buttonPos[i-6].x+220,buttonPos[i-6].y+18,1,TitleColor,col);
      }

    
    }

    if (i == 14) {
      //remote mode indicator
      if (remote) {
        LCD_rectArea(buttonPos[8],GREEN);
        Disp_string(" REMOTE  ",FONT16,buttonPos[8].x,buttonPos[8].y,1,BLACK,GREEN);
      } else {
        LCD_rectArea(buttonPos[8],bgColor);
      }
    }
    //kasuj flagę
    updateFlag[i] = false;
    }
  }

  //wcisniecie enkodera powoduje przejście do modu 1
  digitalWrite(BUT_PIN,HIGH);
  if (digitalRead(BUT_PIN) == LOW && !remote) setMod = 1;
}

bcdFloat pVal(0);
void paramSetSetup(uint8_t p) {
  
    //zakresy
  for (int i = 0; i < paramUnitNum[p]; i++) {
    buttonPos[i] = newRect(260-((paramUnitNum[p]-i-1)*60),180,55,50);
  }

  //przyciski strzałek
  buttonPos[3] = newRect(10,180,50,50);
  buttonPos[4] = newRect(70,180,50,50);

  buttonPos[5] = newRect(10,70,240,70); //pole wyświetlania parametru
  buttonPos[6] = newRect(260,70,50,70); //skrót do klawiatury
  buttonPos[7] = newRect(260,10,50,50); //back

  setButtonMap(buttonPos,8);
  updateFlagIndex = 8;

  //przeliczanie wartości na rms
  if (p == 1 && units[p] == 1) {
    long int rms = tVals[p]->toInt()*rmsWave[selectedWave];
    pVal.set(rms);
  } else {
    pVal.set(tVals[p]->toInt());
  }

  //ustaw kursor
  pointer = UnitPows[p][units[p]];
}

void paramSetLoop(uint8_t p) {

  //sprawdź ekran dotykowy
  TouchData dat = checkTouchScreen();

  if(dat.action==PRESSED) {
    if(dat.rectId < paramUnitNum[p]) {
      //zmiana jednostki

      //obliczanie nowej liczby miejsc po przecinku
      signed char newAccur = accur[p] + UnitPows[p][dat.rectId] - UnitPows[p][units[p]];
      if (newAccur < 0) newAccur = 0;
      accur[p] = newAccur;

      updateFlag[units[p]] = true;
      units[p] = dat.rectId;
      updateFlag[units[p]] = true;

      //RMS
      if (p == 1 && units[p] == 1) {
        long int rms = tVals[p]->toInt()*rmsWave[selectedWave];
        pVal.set(rms);
      } else {
        pVal.set(tVals[p]->toInt());
      }

      updateFlag[7] = true;
      updateFlag[6] = true;
    }

    if (dat.rectId == 3) {
      //strzałka w lewo
    if (pointer < lenLimits[p]) pointer++;

      updateFlag[3] = true;
      updateFlag[6] = true;
    }

    if (dat.rectId == 4) {
      //strzałka w prawo
      if (pointer !=0 ) pointer--;
      if (pointer < (UnitPows[p][units[p]] - accur[p]) && accur[p] < UnitPows[p][units[p]]) accur[p]++;

      updateFlag[4] = true;
      updateFlag[6] = true;
    }

    if (dat.rectId == 6) {
      //przejście do klawiatury
      setMod = MOD_KBD;
    }

    if (dat.rectId == 7) {
      //wyjście
      setMod = MOD_HOME;
    }
  }

  if(dat.action == RELEASED) {
    if (dat.rectId == 3 || dat.rectId == 4) updateFlag[dat.rectId] = true;
  }

  //enkoder
  if (enci) {
    if (encd) {
    pVal.add(1,pointer);
    } else {
    pVal.sub(1,pointer);
    }

    //saturacja
    if (pVal.toInt() > upperLimit[p]) pVal.set(upperLimit[p]);
    if (pVal.toInt() < lowerLimit[p]) pVal.set(lowerLimit[p]);

    //przenoszenie do globalnych zmiennych uwzględniając RMS
    if (p == 1 && units[p] == 1 ) {
      long int vpp = pVal.toInt()/rmsWave[selectedWave];
      if (vpp > upperLimit[p]) vpp = upperLimit[p];
      tVals[p]->set(vpp);
    } else {
      tVals[p]->set(pVal.toInt());
    }

    //update Hardware
    genUpdate(p);
    
    updateFlag[6] = true;
    delay(10);
    enci = false;
  }

  for (int i = 0; i < updateFlagIndex; i++) {
    if (updateFlag[i]) {

      //jednostki
      if (i < paramUnitNum[p]) {
        if (i == units[p]) {
          LCD_rect_fillet(buttonPos[i],butFocusColor,bgColor);
          Disp_stringWithIndex(ParamUnitsWithSpacing[p][i],FONT32,FONT16,(buttonPos[i].x+2),(buttonPos[i].y+13),1,butSymColor,butFocusColor);
        } else {
          LCD_rect_fillet(buttonPos[i],butColor,bgColor);
          Disp_stringWithIndex(ParamUnitsWithSpacing[p][i],FONT32,FONT16,(buttonPos[i].x+2),(buttonPos[i].y+13),1,butSymColor,butColor);          
        }
      }

      if(i==3) {
        //szczałka lewa
        if (dat.action == PRESSED && dat.rectId == 3) {
          LCD_rect_fillet(buttonPos[3],butFocusColor,bgColor);
          Disp_sprite(arrow_l,buttonPos[3].x+9,buttonPos[3].y+9,4,32,1,butSymColor,butFocusColor);
        } else {
          LCD_rect_fillet(buttonPos[3],butColor,bgColor);
          Disp_sprite(arrow_l,buttonPos[3].x+9,buttonPos[3].y+9,4,32,1,butSymColor,butColor);  
        }
      }

      if(i==4) {
        //szczałka prawa
        if (dat.action == PRESSED && dat.rectId == 4) {
          LCD_rect_fillet(buttonPos[4],butFocusColor,bgColor);
          Disp_sprite(arrow_r,buttonPos[4].x+9,buttonPos[4].y+9,4,32,1,butSymColor,butFocusColor);
        } else {
          LCD_rect_fillet(buttonPos[4],butColor,bgColor);
          Disp_sprite(arrow_r,buttonPos[4].x+9,buttonPos[4].y+9,4,32,1,butSymColor,butColor);  
        }
      }

      if(i==5) {
        //pozostałe statyczne dane

         //"falka"
        Disp_sprite(modeButtonPics[selectedWave],210,20,4,32,1,butSymColor,bgColor);

        //klawisz do klawiatury
        LCD_rect_fillet(buttonPos[6],butKbdColor,bgColor);
        Disp_sprite(kbd,buttonPos[6].x+9,buttonPos[6].y+15,5,32,1,BLACK,butKbdColor);

        //back
        LCD_rect_fillet(buttonPos[7],butColor,bgColor);
        Disp_sprite(back,buttonPos[7].x+9,buttonPos[7].y+9,4,32,1,butSymColor,butColor);

        //nazwa parametru
        Disp_string(paramSubTitles[p],FONT32,buttonPos[5].x+5,buttonPos[5].y-30,1,subTitleColor,bgColor);

        //pole wartości
        LCD_rect_fillet(buttonPos[5],butFocusColor,bgColor);
      }

      if(i==6) {
        //liczba
         
        uint8_t len = pVal.length();
        uint8_t last = UnitPows[p][units[p]] - accur[p];
        
        if (len <= UnitPows[p][units[p]]) len = UnitPows[p][units[p]] + 1;
        if (pointer >= len) len = pointer+1;
        
        //dostosowywanie wielkości czcionki do długości wyświetlanego tekstu
        uint8_t numDownSpacing;
        uint8_t numFont;
        if ((len - last) > 5) {           
          numDownSpacing = 30;
          numFont = FONT32;
        } else {            
          numDownSpacing = 15; 
          numFont = FONT64;
        }

        //ustawienie kursora
        curX = buttonPos[5].x+10;
        curY = buttonPos[5].y+numDownSpacing;
        
        char c;
        for(signed char i=len-1; i >= last; i--) {
          c = pVal.digitAt(i) + '0';
          if (i == pointer) {
            Disp_string(String(c),numFont,curX,curY,1,butSelDigColor,butSelDigBgColor);
          } else {
            Disp_string(String(c),numFont,curX,curY,1,TitleColor,butFocusColor);
          }

          if(i == UnitPows[p][units[p]] && accur[p] > 0) {
            Disp_string(".",numFont,curX,curY,1,TitleColor,butFocusColor);
          }
        }
        
        LCD_rect(curX,buttonPos[5].y+10,180-curX,55,butFocusColor);

        if (numFont == FONT32) {
          //dodatkowe prostokąty przykrywające resztki po większej czcionce z góry i z dołu
          LCD_rect(buttonPos[5].x+10,buttonPos[5].y+15,160,15,butFocusColor);
          LCD_rect(buttonPos[5].x+10,buttonPos[5].y+55,160,10,butFocusColor);
          
        }

      }

      if(i==7) {
        //jednostka
        LCD_rect(buttonPos[5].x+180,buttonPos[5].y+30,50,40,butFocusColor);
        Disp_stringWithIndex(ParamUnitsWithIndex[p][units[p]],FONT32,FONT16,buttonPos[5].x+180,buttonPos[5].y+30,1,TitleColor,butFocusColor);
      }

      //kasuj flagę
      updateFlag[i] = false;       
    }
  }
  //wcisniecie enkodera powoduje przejście do modu 0
  
  digitalWrite(BUT_PIN,HIGH);
  if (digitalRead(BUT_PIN) == LOW) setMod = 0;
  
}

String kbdString;
boolean isDot = false;

void keyboardSetup(uint8_t p) {
  byte margX = 5;
  byte margY = 2;

  //kwadraty - 0-9 - numerki, 10 - backspace, 11 - dot, 12-13 - jednostki, 15-ok
  int16_t px, py;
  byte num;

  //tło
  LCD_clear((byte)bgColor); 

  //tworzenie padów
  for (byte i = 0; i < 3; i++) {
    for (byte j = 0; j < 3; j++) {

      //obliczanie px i py 
      px = 170 + (j*50);
      py = 125 - (i*60);
      num = (i*3)+j+1;

      buttonPos[num] = newRect(px,py,40,50);
    }
  }

  //0 . bsp
  buttonPos[10] = newRect(170,185,40,50);
  buttonPos[0] = newRect(220,185,40,50);
  buttonPos[11] = newRect(270,185,40,50);

  //jednostki
  buttonPos[12] = newRect(10,125,40,50);
  buttonPos[13] = newRect(60,125,40,50);
  buttonPos[14] = newRect(110,125,40,50);

  //ok button
  buttonPos[15] = newRect(10,185,140,50);

  //pole liczby
  buttonPos[16] = newRect(10,65,140,50);

  tVals[p]->clearZeros();
  kbdString = tVals[p]->toString(UnitPows[p][units[p]],accur[p],0);
  
  //przeliczanie na RMS
  if (p == 1 && units[p] == 1) {
    float rms = kbdString.toFloat();
    rms *= rmsWave[selectedWave];
    kbdString = String(rms);
  }

  //wyszukiwanie znaku kropki
  isDot = false;
  for(int i = 0; i < kbdString.length();i++) {
    if(kbdString.charAt(i) == '.') {
      isDot = true;
      break;
    }
  }

  setButtonMap(buttonPos,17);
  updateFlagIndex = 18;  

}

void keyboardLoop(uint8_t p) {

  //sprawdź ekran dotykowy
  TouchData dat = checkTouchScreen();

  if(dat.action==PRESSED) {
    
    if(dat.rectId < 12) {
      //przyciski cyfr

      if(dat.rectId == 10) {
        //backspace
        if(kbdString.charAt(kbdString.length()-1) == '.') isDot = false;
        kbdString.remove((kbdString.length()-1),1);
      
      } else if (dat.rectId == 11) {
        //znak kropki
        if (!isDot && kbdString.length() < 9) {
          //dodaj zero, jeśli łańcuch jest pusty
          if(kbdString.length() == 0) kbdString = "0";
          kbdString.concat(".");
          isDot = true;
        }
      } else {
        //znaki cyfr 0-9
        //dodaj znak do łańcucha
        if (kbdString.length() < 9) kbdString.concat(String(dat.rectId));  
      }
      
      //zaktualizuj wartość na ekranie
      updateFlag[17] = true;
      updateFlag[dat.rectId] = true;
    }

    if (dat.rectId >= 12 && dat.rectId <= 14) {
      //jednostki
      updateFlag[dat.rectId] = true;
      updateFlag[units[p]+12] = true;
      updateFlag[17] = true;

      //obliczanie nowej liczby miejsc po przecinku
      signed char newAccur = accur[p] + UnitPows[p][dat.rectId-12] - UnitPows[p][units[p]];
      if (newAccur < 0) newAccur = 0;

      if (p == 1) {
        float a = kbdString.toFloat();
        if (dat.rectId == 13 && units[p] == 0) {
          //vpp->rms
          a *= rmsWave[selectedWave];
        } else if (dat.rectId == 12 && units[p] == 1) {
          //rms->vpp
          a /= rmsWave[selectedWave];
        }
        kbdString = String(a);
      }

      bcdFloat tmp(0);
      tmp.setFromString(kbdString,UnitPows[p][units[p]]);
      units[p] = dat.rectId - 12;

      kbdString = tmp.toString(UnitPows[p][units[p]],newAccur,0);
    }

    if (dat.rectId == 15) {
      
      //rms
      if (p == 1 && units[p] == 1) {
        float vpp = kbdString.toFloat();
        vpp /= rmsWave[selectedWave];
        kbdString = String(vpp);
      }

      //przeliczanie do bcdFloat
      tVals[p]->setFromString(kbdString,UnitPows[p][units[p]]);

      //saturacja
      if (tVals[p]->toInt() > upperLimit[p]) tVals[p]->set(upperLimit[p]);
      if (tVals[p]->toInt() < lowerLimit[p]) tVals[p]->set(lowerLimit[p]);

      genUpdate(p);

      uint8_t zeros = tVals[p]->getFreeZerosNum();
      if (UnitPows[p][units[p]] - accur[p] > zeros) {
        accur[p] = UnitPows[p][units[p]] - zeros;
      }

      setMod = MOD_PARAM;
    }
  }

  if(dat.action==RELEASED) {
    if(dat.rectId < 12) {
      //przyciski cyfr
      updateFlag[dat.rectId] = true;
    }
  }
  


  for (int i = 0; i < updateFlagIndex; i++) {
    if (updateFlag[i]) {

      if (i < 12) {
        //panel liczbowy
        if (dat.rectId==i && dat.action == PRESSED) {
          LCD_rect_fillet(buttonPos[i],butFocusColor,bgColor);
          Disp_string(String(cc[i]),FONT32,(buttonPos[i].x+5),(buttonPos[i].y+2),1,butKbdColor,butFocusColor);
        } else {
          LCD_rect_fillet(buttonPos[i],butColor,bgColor);
          Disp_string(String(cc[i]),FONT32,(buttonPos[i].x+5),(buttonPos[i].y+2),1,butKbdColor,butColor);
        }
      }

      if (i >= 12 && i-12 < paramUnitNum[p]) {
        //jednostki
        if((i-12) == units[p]) {
          LCD_rect_fillet(buttonPos[i],butFocusColor,bgColor);
          Disp_stringWithIndex(ParamUnitsWithSpacing[p][i-12],FONT16,FONT16,(buttonPos[i].x+5),(buttonPos[i].y+15),1,butKbdColor,butFocusColor);
        } else {
          LCD_rect_fillet(buttonPos[i],butColor,bgColor);
          Disp_stringWithIndex(ParamUnitsWithSpacing[p][i-12],FONT16,FONT16,(buttonPos[i].x+5),(buttonPos[i].y+15),1,butKbdColor,butColor);

        }
      }

      if (i == 15) {
        //OK button
         LCD_rect_fillet(buttonPos[15],butFocusColor,bgColor);
         Disp_string("OK",FONT32,(buttonPos[15].x+53),(buttonPos[15].y+10),1,butKbdColor,butFocusColor);
      }

      if(i == 16) {
        //pole liczby
        LCD_rect_fillet(buttonPos[16],butFocusColor,bgColor);
        Disp_string(paramSubTitles[p],FONT16,buttonPos[16].x+5,buttonPos[16].y-25,1,subTitleColor,bgColor);
      }

      if (i == 17) {
        //liczba
        int16_t numX = buttonPos[16].x +5;
        int16_t numY = buttonPos[16].y+15;  

        Disp_string(kbdString,FONT32,numX,numY,1,TitleColor,butFocusColor);
        Disp_string("_",FONT32,curX,curY,1,TitleColor,butFocusColor);
        LCD_rect(curX,curY,buttonPos[16].x+buttonPos[16].wid-curX,25,butFocusColor);

      }
      //kasuj flagę
      updateFlag[i] = false;       
    }
  } 
}

void genUpdate(byte param) {
  
  if (param == 0 || param == 2) {
    //częstotliwość lub duty cycle (tylko dla pwm)
    long int freq = tVals[0]->toInt();

    if (selectedWave == 3) {
      //update PWM
      uint8_t duty = tVals[2]->toInt();
      pwmSet(freq, duty);
    } else {
      //update DDS 
      float hz = freq/10.0;
      AD.setFrequency(MD_AD9833::CHAN_0, hz);
    }
  }

  if (param == 1 && selectedWave != 3) {
    //amplituda lub też offset (chyba ze jestesmy w trybie PWM, wtedy nic nie zmieniaj)
    byte csPin;
    byte data;

    csPin = CS_AMPL;
    
    if (selectedWave == 2) {
      //prostokąt (ma większą amplitudę na wyjściu niż reszta)
      data = tVals[1]->toInt() >> 2;
    } else {
      //sinus/trójkąt
      data = tVals[1]->toInt() << 1;
    }
    
    // ustaw pin Chip Select danego modułu na 0 (aktywuj)
    digitalWrite(csPin,LOW);
    //  wyślij nagłówek (starsze 8 bitów)
    SPI.transfer(B00010001);
    // wyślij wartość
    SPI.transfer(data);
    // ustaw pin Chip Select danego modułu na 1 (dezaktywuj)
    digitalWrite(csPin,HIGH);    
  }

  if (param == 5) {
    //zmiana fali
    switch(selectedWave) {
      case 0: AD.setMode(MD_AD9833::MODE_SINE); break;
      case 1: AD.setMode(MD_AD9833::MODE_TRIANGLE); break;
      case 2: AD.setMode(MD_AD9833::MODE_SQUARE1); break;
    }
  }
}

//odbiór danych przez rs232
void SerialReceive() {
  if (Serial.available() > 0) {
     char c = Serial.read();
    if(serialCmd > 0) {
      if (c >= '0' && c <= '9') {
        serialNum *= 10;
        serialNum+=(c-'0');
      } else if (c == ';' || c == 13 || c == 10) {
        //do something
        if (serialCmd == 'f' && remote) {
          if (serialNum >= lowerLimit[0] && serialNum <= upperLimit[0]) tVals[0]->set(serialNum);

          //dostosowywanie jednostki
          uint8_t len = tVals[0]->length();
          uint8_t newUnit = 0;
          if (len > UnitPows[0][1]) newUnit = 1;
          if (len > UnitPows[0][2]) newUnit = 2;

          if (units[0] != newUnit) {
            units[0] = newUnit;
            updateFlag[11] = true;
          }

          //dostosowywanie zer
          uint8_t zeros = tVals[0]->getFreeZerosNum();
          if (UnitPows[0][units[0]] >= zeros) {
            accur[0] = UnitPows[0][units[0]] - zeros;
          } else {
            accur[0] = 0;
          }


          
          updateFlag[8] = true;
          
          genUpdate(0);
        } else if (serialCmd == 'a' && remote) {
          //amplitude
          if (serialNum >= lowerLimit[1] && serialNum <= upperLimit[1]) tVals[1]->set(serialNum);
          updateFlag[9] = true;

          genUpdate(1);

        } else if (serialCmd == 'd' && remote) {
          //duty
          if (serialNum >= lowerLimit[2] && serialNum <= upperLimit[2]) tVals[2]->set(serialNum);
          updateFlag[9] = true;

          genUpdate(2);
        
        } else if (serialCmd == 'w' && remote) {
          //wave select
          updateFlag[selectedWave]= true;
          updateFlag[serialNum] = true;

          if (selectedWave == 3 || serialNum == 3) {
              //gdy zmiana widoku międy amplitudą a duty Cycle
              updateFlag[6] = true;
              updateFlag[7] = true;
          }

          if (serialNum >=0 && serialNum <= 3) selectedWave = serialNum;

          //gdy wybrany offset to zrzuć na amplitudę
          if ( selectedWave == 3 && selectedParam == 2) selectedParam = 1;      
          
          //update generatora
          genUpdate(5);

        } else if (serialCmd == 'o') {

          //output relay
          if (serialNum) {
            outputOn = true;
          } else {
            outputOn = false;
          }
          digitalWrite(RELAY_PIN,outputOn);
          updateFlag[4]=true;

        } else if (serialCmd == 'r') {
          //turn on/off remote mode
          if(serialNum) {
            remote = true;
            setMod = 0;
          } else {
            remote = false;
          }
          if (mod == 0) updateFlag[14] = true;
        }
        serialCmd = 0;
      } 

    } else {
      if(c >= 'a' && c <= 'z') {
        serialNum = 0;
        serialCmd = c;
      }
    }
  }
}

//nadawanie zmian do komputera
void SerialTransmit(byte param) {
  char cmd;
  long int data;
  if (remote) {
    switch(param) {
      case 0: //frequency
        cmd = 'f';
        data = tVals[0]->toInt();
        break;
      case 1: //amplitude
        cmd = 'a';
        data = tVals[1]->toInt();
        break;
      case 2: //duty 
        cmd = 'd';
        data = tVals[2]->toInt();
        break;
      //case 3,4: reserved
      case 5:
        //wave
        cmd = 'w';
        data = selectedWave;
        break;
      case 6:
        cmd = 'o';
        data = outputOn;
    }

    //and send it!
    Serial.print(cmd);
    Serial.println(data);


  }
}
