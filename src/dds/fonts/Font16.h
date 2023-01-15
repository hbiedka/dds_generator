// Font size 2

#include <avr/pgmspace.h>

#define nr_chrs_f16 96
#define chr_hgt_f16 16
#define data_size_f16 8
#define firstchr_f16 32

PROGMEM const unsigned char widtbl_f16[96] =         // character width table
{
        5, 2, 3, 8, 7, 8, 8, 2,             // char 32 - 39
        6, 6, 7, 5, 2, 5, 4, 6,             // char 40 - 47
        7, 7, 7, 7, 7, 7, 7, 7,             // char 48 - 55
        7, 7, 2, 2, 5, 5, 5, 7,             // char 56 - 63
        8, 7, 7, 7, 7, 7, 7, 7,             // char 64 - 71
        6, 3, 7, 7, 6, 9, 7, 7,             // char 72 - 79
        7, 7, 7, 7, 7, 7, 7, 9,             // char 80 - 87
        7, 7, 7, 3, 6, 3, 7, 8,             // char 88 - 95
        3, 6, 6, 6, 6, 6, 5, 6,             // char 96 - 103
        6, 4, 4, 5, 4, 7, 6, 7,             // char 104 - 111
        6, 7, 5, 5, 4, 6, 7, 7,             // char 112 - 119
        5, 6, 6, 4, 2, 4, 7, 5              // char 120 - 127
};

PROGMEM const unsigned char chr_f16_20[] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
PROGMEM const unsigned char chr_f16_21[] = {0x0, 0x0, 0xF8, 0x17 };
PROGMEM const unsigned char chr_f16_22[] = {0x1C, 0x0, 0x0, 0x0, 0x1C, 0x0 };
PROGMEM const unsigned char chr_f16_23[] = {0x40, 0x2, 0x40, 0x2, 0xF8, 0x1F, 0x40, 0x2, 0x40, 0x2, 0xF8, 0x1F, 0x40, 0x2, 0x40, 0x2 };
PROGMEM const unsigned char chr_f16_24[] = {0x0, 0x10, 0xF0, 0x1F, 0x88, 0x12, 0x88, 0x12, 0x8, 0x10, 0x8, 0x10, 0x10, 0x10 };
PROGMEM const unsigned char chr_f16_25[] = {0x30, 0x18, 0x48, 0x4, 0x48, 0x2, 0x30, 0x1, 0x80, 0xC, 0x40, 0x12, 0x20, 0x12, 0x18, 0xC };
PROGMEM const unsigned char chr_f16_26[] = {0x60, 0xC, 0x90, 0x12, 0x8, 0x11, 0x90, 0x12, 0x60, 0xC, 0x0, 0xC, 0x0, 0x12, 0x0, 0x10 };
PROGMEM const unsigned char chr_f16_27[] = {0x20, 0x0, 0x1C, 0x0 };
PROGMEM const unsigned char chr_f16_28[] = {0xC0, 0x7, 0x30, 0x18, 0x8, 0x20, 0x4, 0x40, 0x2, 0x80, 0x2, 0x80 };
PROGMEM const unsigned char chr_f16_29[] = {0x2, 0x80, 0x2, 0x80, 0x4, 0x40, 0x8, 0x20, 0x30, 0x18, 0xC0, 0x7 };
PROGMEM const unsigned char chr_f16_2A[] = {0x20, 0x2, 0x40, 0x1, 0x80, 0x0, 0xF0, 0x7, 0x80, 0x0, 0x40, 0x1, 0x20, 0x2 };
PROGMEM const unsigned char chr_f16_2B[] = {0x0, 0x1, 0x0, 0x1, 0xC0, 0x7, 0x0, 0x1, 0x0, 0x1 };
PROGMEM const unsigned char chr_f16_2C[] = {0x0, 0x58, 0x0, 0x38 };
PROGMEM const unsigned char chr_f16_2D[] = {0x0, 0x1, 0x0, 0x1, 0x0, 0x1, 0x0, 0x1, 0x0, 0x1 };
PROGMEM const unsigned char chr_f16_2E[] = {0x0, 0x18, 0x0, 0x18, 0x0, 0x0, 0x0, 0x0 };
PROGMEM const unsigned char chr_f16_2F[] = {0x0, 0x30, 0x0, 0xC, 0x0, 0x3, 0xC0, 0x0, 0x30, 0x0, 0xC, 0x0 };
PROGMEM const unsigned char chr_f16_30[] = {0xC0, 0x3, 0x30, 0xC, 0x8, 0x10, 0x8, 0x10, 0x8, 0x10, 0x30, 0xC, 0xC0, 0x3 };
PROGMEM const unsigned char chr_f16_31[] = {0x0, 0x0, 0x20, 0x10, 0x10, 0x10, 0xF8, 0x1F, 0x0, 0x10, 0x0, 0x10, 0x0, 0x0 };
PROGMEM const unsigned char chr_f16_32[] = {0x20, 0x18, 0x10, 0x14, 0x8, 0x12, 0x8, 0x11, 0x8, 0x11, 0x90, 0x10, 0x60, 0x10 };
PROGMEM const unsigned char chr_f16_33[] = {0x10, 0x8, 0x8, 0x10, 0x88, 0x10, 0x88, 0x10, 0x88, 0x10, 0x50, 0x9, 0x20, 0x6 };
PROGMEM const unsigned char chr_f16_34[] = {0x0, 0x3, 0x80, 0x2, 0x40, 0x2, 0x20, 0x2, 0x10, 0x2, 0xF8, 0x1F, 0x0, 0x2 };
PROGMEM const unsigned char chr_f16_35[] = {0xF8, 0x8, 0x88, 0x10, 0x88, 0x10, 0x88, 0x10, 0x88, 0x10, 0x8, 0x9, 0x0, 0x6 };
PROGMEM const unsigned char chr_f16_36[] = {0xE0, 0x7, 0x10, 0x9, 0x88, 0x10, 0x88, 0x10, 0x88, 0x10, 0x8, 0x9, 0x0, 0x6 };
PROGMEM const unsigned char chr_f16_37[] = {0x0, 0x0, 0x8, 0x0, 0x8, 0x0, 0x8, 0x1C, 0x8, 0x3, 0xC8, 0x0, 0x38, 0x0 };
PROGMEM const unsigned char chr_f16_38[] = {0x20, 0x6, 0x50, 0x9, 0x88, 0x10, 0x88, 0x10, 0x88, 0x10, 0x50, 0x9, 0x20, 0x6 };
PROGMEM const unsigned char chr_f16_39[] = {0x60, 0x0, 0x90, 0x10, 0x8, 0x11, 0x8, 0x11, 0x8, 0x11, 0x90, 0x8, 0xE0, 0x7 };
PROGMEM const unsigned char chr_f16_3A[] = {0xC0, 0x6, 0xC0, 0x6 };
PROGMEM const unsigned char chr_f16_3B[] = {0xC0, 0x16, 0xC0, 0xE };
PROGMEM const unsigned char chr_f16_3C[] = {0x0, 0x1, 0x80, 0x2, 0x40, 0x4, 0x20, 0x8, 0x10, 0x10 };
PROGMEM const unsigned char chr_f16_3D[] = {0x80, 0x2, 0x80, 0x2, 0x80, 0x2, 0x80, 0x2, 0x80, 0x2 };
PROGMEM const unsigned char chr_f16_3E[] = {0x10, 0x10, 0x20, 0x8, 0x40, 0x4, 0x80, 0x2, 0x0, 0x1 };
PROGMEM const unsigned char chr_f16_3F[] = {0x20, 0x0, 0x10, 0x0, 0x8, 0x0, 0x8, 0x16, 0x8, 0x1, 0x90, 0x0, 0x60, 0x0 };
PROGMEM const unsigned char chr_f16_40[] = {0xE0, 0x7, 0x10, 0x8, 0xC8, 0x13, 0x28, 0x14, 0x28, 0x14, 0xC8, 0x17, 0x10, 0x14, 0xE0, 0x3 };
PROGMEM const unsigned char chr_f16_41[] = {0x0, 0x1C, 0x80, 0x3, 0x60, 0x2, 0x18, 0x2, 0x60, 0x2, 0x80, 0x3, 0x0, 0x1C };
PROGMEM const unsigned char chr_f16_42[] = {0xF8, 0x1F, 0x88, 0x10, 0x88, 0x10, 0x88, 0x10, 0x88, 0x10, 0x50, 0x9, 0x20, 0x6 };
PROGMEM const unsigned char chr_f16_43[] = {0xE0, 0x7, 0x10, 0x8, 0x8, 0x10, 0x8, 0x10, 0x8, 0x10, 0x8, 0x10, 0x10, 0x8 };
PROGMEM const unsigned char chr_f16_44[] = {0xF8, 0x1F, 0x8, 0x10, 0x8, 0x10, 0x8, 0x10, 0x8, 0x10, 0x10, 0x8, 0xE0, 0x7 };
PROGMEM const unsigned char chr_f16_45[] = {0xF8, 0x1F, 0x88, 0x10, 0x88, 0x10, 0x88, 0x10, 0x88, 0x10, 0x88, 0x10, 0x8, 0x10 };
PROGMEM const unsigned char chr_f16_46[] = {0xF8, 0x1F, 0x88, 0x0, 0x88, 0x0, 0x88, 0x0, 0x88, 0x0, 0x8, 0x0, 0x8, 0x0 };
PROGMEM const unsigned char chr_f16_47[] = {0xE0, 0x7, 0x10, 0x8, 0x8, 0x10, 0x8, 0x11, 0x8, 0x11, 0x8, 0x11, 0x10, 0xE };
PROGMEM const unsigned char chr_f16_48[] = {0xF8, 0x1F, 0x80, 0x0, 0x80, 0x0, 0x80, 0x0, 0x80, 0x0, 0xF8, 0x1F };
PROGMEM const unsigned char chr_f16_49[] = {0x8, 0x10, 0xF8, 0x1F, 0x8, 0x10 };
PROGMEM const unsigned char chr_f16_4A[] = {0x0, 0x4, 0x0, 0x8, 0x0, 0x10, 0x0, 0x10, 0x0, 0x10, 0x0, 0x8, 0xF8, 0x7 };
PROGMEM const unsigned char chr_f16_4B[] = {0xF8, 0x1F, 0x80, 0x0, 0x40, 0x1, 0x20, 0x2, 0x10, 0x4, 0x8, 0x8, 0x0, 0x10 };
PROGMEM const unsigned char chr_f16_4C[] = {0xF8, 0x1F, 0x0, 0x10, 0x0, 0x10, 0x0, 0x10, 0x0, 0x10, 0x0, 0x10 };
PROGMEM const unsigned char chr_f16_4D[] = {0xF8, 0x1F, 0x18, 0x0, 0x60, 0x0, 0x80, 0x1, 0x0, 0x6, 0x80, 0x1, 0x60, 0x0, 0x18, 0x0, 0xF8, 0x1F };
PROGMEM const unsigned char chr_f16_4E[] = {0xF8, 0x1F, 0x18, 0x0, 0x60, 0x0, 0x80, 0x1, 0x0, 0x6, 0x0, 0x18, 0xF8, 0x1F };
PROGMEM const unsigned char chr_f16_4F[] = {0xE0, 0x7, 0x10, 0x8, 0x8, 0x10, 0x8, 0x10, 0x8, 0x10, 0x10, 0x8, 0xE0, 0x7 };
PROGMEM const unsigned char chr_f16_50[] = {0xF8, 0x1F, 0x8, 0x2, 0x8, 0x2, 0x8, 0x2, 0x8, 0x2, 0x10, 0x1, 0xE0, 0x0 };
PROGMEM const unsigned char chr_f16_51[] = {0xE0, 0x7, 0x10, 0x8, 0x8, 0x10, 0x8, 0x10, 0x8, 0x30, 0x10, 0x48, 0xE0, 0x47 };
PROGMEM const unsigned char chr_f16_52[] = {0xF8, 0x1F, 0x8, 0x1, 0x8, 0x1, 0x8, 0x3, 0x8, 0x5, 0x90, 0x8, 0x60, 0x10 };
PROGMEM const unsigned char chr_f16_53[] = {0x60, 0x4, 0x90, 0x8, 0x88, 0x10, 0x8, 0x11, 0x8, 0x11, 0x10, 0x9, 0x20, 0x6 };
PROGMEM const unsigned char chr_f16_54[] = {0x8, 0x0, 0x8, 0x0, 0x8, 0x0, 0xF8, 0x1F, 0x8, 0x0, 0x8, 0x0, 0x8, 0x0 };
PROGMEM const unsigned char chr_f16_55[] = {0xF8, 0x7, 0x0, 0x8, 0x0, 0x10, 0x0, 0x10, 0x0, 0x10, 0x0, 0x8, 0xF8, 0x7 };
PROGMEM const unsigned char chr_f16_56[] = {0x78, 0x0, 0x80, 0x1, 0x0, 0x6, 0x0, 0x18, 0x0, 0x6, 0x80, 0x1, 0x78, 0x0 };
PROGMEM const unsigned char chr_f16_57[] = {0xF8, 0x0, 0x0, 0x7, 0x0, 0x18, 0x0, 0x6, 0xC0, 0x1, 0x0, 0x6, 0x0, 0x18, 0x0, 0x7, 0xF8, 0x0 };
PROGMEM const unsigned char chr_f16_58[] = {0x18, 0x18, 0x20, 0x4, 0x40, 0x2, 0x80, 0x1, 0x40, 0x2, 0x20, 0x4, 0x18, 0x18 };
PROGMEM const unsigned char chr_f16_59[] = {0x38, 0x0, 0x40, 0x0, 0x80, 0x0, 0x0, 0x1F, 0x80, 0x0, 0x40, 0x0, 0x38, 0x0 };
PROGMEM const unsigned char chr_f16_5A[] = {0x8, 0x18, 0x8, 0x14, 0x8, 0x12, 0x88, 0x11, 0x48, 0x10, 0x28, 0x10, 0x18, 0x10 };
PROGMEM const unsigned char chr_f16_5B[] = {0xFC, 0x3F, 0x4, 0x20, 0x4, 0x20 };
PROGMEM const unsigned char chr_f16_5C[] = {0x0, 0x30, 0x0, 0xC, 0x0, 0x3, 0xC0, 0x0, 0x30, 0x0, 0xC, 0x0 };
PROGMEM const unsigned char chr_f16_5D[] = {0x4, 0x20, 0x4, 0x20, 0xFC, 0x3F };
PROGMEM const unsigned char chr_f16_5E[] = {0x0, 0x0, 0x8, 0x0, 0x4, 0x0, 0x2, 0x0, 0x4, 0x0, 0x8, 0x0, 0x0, 0x0 };
PROGMEM const unsigned char chr_f16_5F[] = {0x0, 0x40, 0x0, 0x40, 0x0, 0x40, 0x0, 0x40, 0x0, 0x40, 0x0, 0x40, 0x0, 0x40, 0x0, 0x40 };
PROGMEM const unsigned char chr_f16_60[] = {0x0, 0x0, 0x38, 0x0, 0x40, 0x0 };
PROGMEM const unsigned char chr_f16_61[] = {0x0, 0xC, 0x40, 0x12, 0x40, 0x12, 0x40, 0x12, 0x80, 0xC, 0x0, 0x1F };
PROGMEM const unsigned char chr_f16_62[] = {0xF0, 0x1F, 0x80, 0x8, 0x40, 0x10, 0x40, 0x10, 0x80, 0x8, 0x0, 0x7 };
PROGMEM const unsigned char chr_f16_63[] = {0x0, 0x7, 0x80, 0x8, 0x40, 0x10, 0x40, 0x10, 0x40, 0x10, 0x80, 0x8 };
PROGMEM const unsigned char chr_f16_64[] = {0x0, 0x7, 0x80, 0x8, 0x40, 0x10, 0x40, 0x10, 0x80, 0x8, 0xF0, 0x1F };
PROGMEM const unsigned char chr_f16_65[] = {0x0, 0x7, 0x80, 0xA, 0x40, 0x12, 0x40, 0x12, 0x40, 0x12, 0x80, 0x9 };
PROGMEM const unsigned char chr_f16_66[] = {0x0, 0x1, 0xF0, 0x1F, 0x8, 0x1, 0x8, 0x0, 0x10, 0x0 };
PROGMEM const unsigned char chr_f16_67[] = {0x0, 0x7, 0x80, 0x88, 0x40, 0x90, 0x40, 0x90, 0x80, 0x48, 0x80, 0x3F };
PROGMEM const unsigned char chr_f16_68[] = {0xF8, 0x1F, 0x80, 0x0, 0x40, 0x0, 0x40, 0x0, 0x80, 0x0, 0x0, 0x1F };
PROGMEM const unsigned char chr_f16_69[] = {0x0, 0x0, 0xD0, 0x1F, 0x0, 0x0, 0x0, 0x0 };
PROGMEM const unsigned char chr_f16_6A[] = {0x0, 0x40, 0x0, 0x80, 0x40, 0x80, 0xD0, 0x7F };
PROGMEM const unsigned char chr_f16_6B[] = {0xF8, 0x1F, 0x0, 0x2, 0x0, 0x5, 0x80, 0x8, 0x40, 0x10 };
PROGMEM const unsigned char chr_f16_6C[] = {0x8, 0x0, 0xF8, 0x1F, 0x0, 0x0, 0x0, 0x0 };
PROGMEM const unsigned char chr_f16_6D[] = {0xC0, 0x1F, 0x80, 0x0, 0x40, 0x0, 0x80, 0x1F, 0x40, 0x0, 0x40, 0x0, 0x80, 0x1F };
PROGMEM const unsigned char chr_f16_6E[] = {0xC0, 0x1F, 0x80, 0x0, 0x40, 0x0, 0x40, 0x0, 0x80, 0x0, 0x0, 0x1F };
PROGMEM const unsigned char chr_f16_6F[] = {0x0, 0x7, 0x80, 0x8, 0x40, 0x10, 0x40, 0x10, 0x40, 0x10, 0x80, 0x8, 0x0, 0x7 };
PROGMEM const unsigned char chr_f16_70[] = {0xC0, 0xFF, 0x80, 0x8, 0x40, 0x10, 0x40, 0x10, 0x80, 0x8, 0x0, 0x7 };
PROGMEM const unsigned char chr_f16_71[] = {0x0, 0x7, 0x80, 0x8, 0x40, 0x10, 0x40, 0x10, 0x80, 0x8, 0xC0, 0xFF, 0x0, 0x80 };
PROGMEM const unsigned char chr_f16_72[] = {0xC0, 0x1F, 0x80, 0x0, 0x40, 0x0, 0x40, 0x0, 0x80, 0x0 };
PROGMEM const unsigned char chr_f16_73[] = {0x80, 0x9, 0x40, 0x12, 0x40, 0x12, 0x40, 0x12, 0x80, 0xC };
PROGMEM const unsigned char chr_f16_74[] = {0x40, 0x0, 0xF0, 0xF, 0x40, 0x10, 0x0, 0x10 };
PROGMEM const unsigned char chr_f16_75[] = {0xC0, 0x7, 0x0, 0x8, 0x0, 0x10, 0x0, 0x10, 0x0, 0x8, 0xC0, 0x1F };
PROGMEM const unsigned char chr_f16_76[] = {0xC0, 0x3, 0x0, 0x4, 0x0, 0x8, 0x0, 0x10, 0x0, 0x8, 0x0, 0x4, 0xC0, 0x3 };
PROGMEM const unsigned char chr_f16_77[] = {0xC0, 0xF, 0x0, 0x10, 0x0, 0x8, 0x0, 0x6, 0x0, 0x8, 0x0, 0x10, 0xC0, 0xF };
PROGMEM const unsigned char chr_f16_78[] = {0xC0, 0x18, 0x0, 0x5, 0x0, 0x2, 0x0, 0x5, 0xC0, 0x18 };
PROGMEM const unsigned char chr_f16_79[] = {0xC0, 0x7, 0x0, 0x88, 0x0, 0x90, 0x0, 0x90, 0x0, 0x48, 0xC0, 0x3F };
PROGMEM const unsigned char chr_f16_7A[] = {0x40, 0x18, 0x40, 0x14, 0x40, 0x12, 0x40, 0x12, 0x40, 0x11, 0xC0, 0x10 };
PROGMEM const unsigned char chr_f16_7B[] = {0x0, 0x0, 0x0, 0x1, 0xFC, 0x7E, 0x2, 0x80 };
PROGMEM const unsigned char chr_f16_7C[] = {0x0, 0x0, 0xFC, 0x7F };
PROGMEM const unsigned char chr_f16_7D[] = {0x0, 0x0, 0x2, 0x80, 0xFC, 0x7E, 0x0, 0x1 };
PROGMEM const unsigned char chr_f16_7E[] = {0x0, 0x0, 0x10, 0x0, 0x8, 0x0, 0x8, 0x0, 0x10, 0x0, 0x10, 0x0, 0x8, 0x0 };
PROGMEM const unsigned char chr_f16_7F[] = {0x0, 0x0, 0x18, 0x0, 0x24, 0x0, 0x24, 0x0, 0x18, 0x0 };

PROGMEM const unsigned char* const chrtbl_f16[96] =       // character pointer table
{
        chr_f16_20, chr_f16_21, chr_f16_22, chr_f16_23, chr_f16_24, chr_f16_25, chr_f16_26, chr_f16_27, 
        chr_f16_28, chr_f16_29, chr_f16_2A, chr_f16_2B, chr_f16_2C, chr_f16_2D, chr_f16_2E, chr_f16_2F, 
        chr_f16_30, chr_f16_31, chr_f16_32, chr_f16_33, chr_f16_34, chr_f16_35, chr_f16_36, chr_f16_37, 
        chr_f16_38, chr_f16_39, chr_f16_3A, chr_f16_3B, chr_f16_3C, chr_f16_3D, chr_f16_3E, chr_f16_3F, 
        chr_f16_40, chr_f16_41, chr_f16_42, chr_f16_43, chr_f16_44, chr_f16_45, chr_f16_46, chr_f16_47, 
        chr_f16_48, chr_f16_49, chr_f16_4A, chr_f16_4B, chr_f16_4C, chr_f16_4D, chr_f16_4E, chr_f16_4F, 
        chr_f16_50, chr_f16_51, chr_f16_52, chr_f16_53, chr_f16_54, chr_f16_55, chr_f16_56, chr_f16_57, 
        chr_f16_58, chr_f16_59, chr_f16_5A, chr_f16_5B, chr_f16_5C, chr_f16_5D, chr_f16_5E, chr_f16_5F, 
        chr_f16_60, chr_f16_61, chr_f16_62, chr_f16_63, chr_f16_64, chr_f16_65, chr_f16_66, chr_f16_67, 
        chr_f16_68, chr_f16_69, chr_f16_6A, chr_f16_6B, chr_f16_6C, chr_f16_6D, chr_f16_6E, chr_f16_6F, 
        chr_f16_70, chr_f16_71, chr_f16_72, chr_f16_73, chr_f16_74, chr_f16_75, chr_f16_76, chr_f16_77, 
        chr_f16_78, chr_f16_79, chr_f16_7A, chr_f16_7B, chr_f16_7C, chr_f16_7D, chr_f16_7E, chr_f16_7F
};
