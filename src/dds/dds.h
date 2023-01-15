
#define ENC_A_PIN 18
#define ENC_B_PIN 16
#define BUT_PIN 20

#define RELAY_PIN 14

// Pins for SPI comm with the AD9833 IC
#define FSYNC 48   ///< SPI Load pin number (FSYNC in AD9833 usage)
#define CS_AMPL 47 // MCP41010 chip select - digital potentiometer (amplitude).
#define CS_OFF 46 // MCP41010 chip select - digital potentiometer (offset).

byte paramMap[4][3] = {{0,1,4},{0,1,4},{0,1,4},{0,2,4}};

//mode buttons
unsigned char *modeButtonPics[4] = {sine, tri, squ, pwm}; //from symbols.h
String modeButtonSubs[4] = {"Sin","Tri","Squ","PWM"};

String paramSubTitles[5] = {"Frequency","Amplitude","Duty Cycle","Offset",""};

String ParamUnitsWithIndex[4][3] = {
  {"Hz   ","kHz  ","MHz"},
  {"V_p-p","V_rms",""},
  {"%    ","",""},
  {"V_p-p","V_p-p",""}
};

String ParamUnitsWithSpacing[4][3] = {
  {"  Hz"," kHz","MHz"},
  {" V_p-p","V_rms",""},
  {"   %","",""},
  {" V_p-p"," V_p-p",""}
};

byte paramUnitNum[4] = {3,2,1,1}; //ile parametrów wykorzystujemy

signed int lowerLimit[4] = {10,1,1,1};
signed long int upperLimit[4] = {40000000,120,100,500};

String onOffSwitch[2] = {"ON","OFF"};

//keyboard chars
char cc[12] = {'0' ,'1', '2', '3','4','5','6','7','8','9','<','.'};

int8_t UnitPows[4][3] = {{1,4,7},{1,1,1},{0,0,0},{1,1,1}}; //potęgi wg jednostki
float rmsWave[4] = {0.707,0.577,0.707,1};  //przeliczniki wg rodzaju wybranej fali
int8_t lenLimits[4] = {7,2,1,};  //limit długości zpiennej
//mod
#define MOD_HOME 0
#define MOD_PARAM 1
#define MOD_KBD 2
byte mod = 255;
byte setMod = MOD_HOME; //jeśli setMod i mod nie są równe, ekran automatycznie cały się zupdatuje

//wybrana fala (0 sin 1 tri 2 squ 3 pwm)
byte selectedWave = 0;

//parametr edytiwany (0 freq 1 ampl 2 duty 3 offest)
byte selectedParam = 0;

uint8_t pointer;

//załączenie wyjścia
boolean outputOn = false;

byte units[4] = {1,0,0,0};

//miejsca po przecinku
byte accur[4] = {2,1,0,1};

volatile boolean updateFlag[64];
uint8_t updateFlagIndex = 0;

//encoder
boolean enci = false;
boolean encd;

//remote (serial.cpp)
boolean remote = false;
char serialCmd = 0;
long int serialNum;

//color bindings
int16_t bgColor=0x0000; //0808
int16_t TitleColor = YELLOW;
int16_t subTitleColor=WHITE;
int16_t symColor=WHITE;
int16_t butSymColor=WHITE;
int16_t butColor=0x0808;
int16_t butFocusColor=BLUE; 
int16_t butKbdColor=WHITE;
int16_t butSelDigColor=BLACK;
int16_t butSelDigBgColor=WHITE;

void encoder();
void setFullUpdateFlag(boolean f);
void homeSetup();
void homeLoop();
void paramSetSetup(uint8_t p);
void paramSetLoop(uint8_t p);
void genUpdate(byte param);

void serialTransmit(byte param);
void serialReceive();
