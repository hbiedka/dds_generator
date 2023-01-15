/*
miscSprites.h
this file contains some sprites
*/

#include <avr/pgmspace.h>

//round cornes
PROGMEM const unsigned char LDcorner[8] = { B00000000,
                                            B00000011,
                                            B00001111,
                                            B00011111,
                                            B00111111,
                                            B00111111,
                                            B01111111,
                                            B01111111 };

PROGMEM const unsigned char RDcorner[8] = { B01111111, 
                                            B01111111,
                                            B00111111,
                                            B00111111,
                                            B00011111,
                                            B00001111,
                                            B00000011,
                                            B00000000 };

PROGMEM const unsigned char LUcorner[8] = { B00000000,
                                            B11000000,
                                            B11110000,
                                            B11111000,
                                            B11111100,
                                            B11111100,
                                            B11111110,
                                            B11111110 };

PROGMEM const unsigned char RUcorner[8] = { B11111110,
                                            B11111110,
                                            B11111100,
                                            B11111100,
                                            B11111000,
                                            B11110000,
                                            B11000000,
                                            B00000000 };

//testing sprite
PROGMEM const unsigned char Tsprite[16] =  { B00000001, B10000000,
                                            B00100011, B11000000,
                                            B00000111, B11100000,
                                            B11111110, B01111111,
                                            B11111110, B01111111,
                                            B00000111, B11100000,
                                            B00000011, B11000000,
                                            B00000001, B10000000 };
