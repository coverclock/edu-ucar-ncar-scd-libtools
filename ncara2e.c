/*
**	N C A R A 2 E
**
**	Copyright 1992	University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		NCAR ASCII to EBCDIC Translation Table
**	Program		TAGS libtools
**	Project		TAGS libtools
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Tue Sep 29 11:19:57 MDT 1992
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
**
**	Abstract
**
**	ASCII to EBCDIC translation table, originally developed at
**	NCAR by Basil Irwin (irwin@ncar.ucar.edu) for MASNET on CMS.
*/

static char copyright[]="Copyright 1992 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)ncara2e.c	4.5 96/05/21 jsloan@ncar.ucar.edu";

#include "libtools.h"

unsigned char ncara2e[]=
	{
	0x00,
	0x01,
	0x02,
	0x03,
	0x37,
	0x2D,
	0x2E,
	0x2F,
	0x16,
	0x05,
	0x25,
	0x0B,
	0x0C,
	0x0D,
	0x0E,
	0x0F,
	0x10,
	0x11,
	0x12,
	0x13,
	0x3C,
	0x3D,
	0x32,
	0x26,
	0x18,
	0x19,
	0x3F,
	0x27,
	0x1C,
	0x1D,
	0x1E,
	0x1F,		/* ASCII   ECBDIC			*/
	0x40,		/*    20     40  (SPACE)		*/
	0x5A,		/* |  21   | 5A  (EXCLAMATION)		*/
	0x7F,		/* "  22   " 7F  (DOUBLE QUOTE)		*/
	0x7B,		/* #  23   # 7B  (POUND)		*/
	0x5B,		/* $  24   $ 5B  (DOLLAR)		*/
	0x6C,		/* %  25   % 6C  (PERCENT)		*/
	0x50,		/* &  26   & 50  (AMPERSAND)		*/
	0x7D,		/* '  27   ' 7D  (APOSTROPHE)		*/
	0x4D,		/* (  28   ( 4D  (LEFT PAREN)		*/
	0x5D,		/* )  29   ) 5D  (RIGHT PAREN)		*/
	0x5C,		/* *  2A   * 5C  (STAR)			*/
	0x4E,		/* +  2B   + 4E  (PLUS)			*/
	0x6B,		/* ,  2C   , 6B  (COMMA)		*/
	0x60,		/* -  2D   - 60  (MINUS)		*/
	0x4B,		/* .  2E   . 4B  (PERIOD)		*/
	0x61,		/* /  2F   / 61  (SLASH)		*/
	0xF0,		/* 0  30   0 F0				*/
	0xF1,		/* 1  31   1 F1				*/
	0xF2,		/* 2  32   2 F2				*/
	0xF3,		/* 3  33   3 F3				*/
	0xF4,		/* 4  34   4 F4				*/
	0xF5,		/* 5  35   5 F5				*/
	0xF6,		/* 6  36   6 F6				*/
	0xF7,		/* 7  37   7 F7				*/
	0xF8,		/* 8  38   8 F8				*/
	0xF9,		/* 9  39   9 F9				*/
	0x7A,		/* :  3A   : 7A  (COLON)		*/
	0x5E,		/* ;  3B   ; 5E  (SEMICOLON)		*/
	0x4C,		/* <  3C   < 4C  (LESS THAN)		*/
	0x7E,		/* =  3D   = 7E  (EQUALS)		*/
	0x6E,		/* >  3E   > 6E  (GREATER THAN)		*/
	0x6F,		/* ?  3F   ? 6F  (QUESTION)		*/
	0x7C,		/* @  40   @ 7C  (EACH)			*/
	0xC1,		/* A  41   A C1				*/
	0xC2,		/* B  42   B C2				*/
	0xC3,		/* C  43   C C3				*/
	0xC4,		/* 9  44   D C4				*/
	0xC5,		/* E  45   E C5				*/
	0xC6,		/* F  46   F C6				*/
	0xC7,		/* G  47   G C7				*/
	0xC8,		/* H  48   H C8				*/
	0xC9,		/* I  49   I C9				*/
	0xD1,		/* J  4A   J D1				*/
	0xD2,		/* K  4B   K D2				*/
	0xD3,		/* L  4C   L D3				*/
	0xD4,		/* M  4D   M D4				*/
	0xD5,		/* N  4E   N D5				*/
	0xD6,		/* O  4F   O D6				*/
	0xD7,		/* P  50   P D7				*/
	0xD8,		/* Q  51   Q D8				*/
	0xD9,		/* R  52   R D9				*/
	0xE2,		/* S  53   S E2				*/
	0xE3,		/* T  54   T E3				*/
	0xE4,		/* U  55   U E4				*/
	0xE5,		/* V  56   V E5				*/
	0xE6,		/* W  57   W E6				*/
	0xE7,		/* X  58   X E7				*/
	0xE8,		/* Y  59   Y E8				*/
	0xE9,		/* Z  5A   Z E9				*/
	0xAD,		/* U  5B     AD  (LEFT BRACKET)		*/
	0xE0,		/* \  5C   \ E0  (BACKSLASH)		*/
	0xBD,		/* !  5D   e BD  (RIGHT BRACKET)	*/
	0x5F,		/* ^  5E     5F  (HAT TO NOT)		*/
	0x6D,		/* _  5F   _ 6D  (UNDERSCORE)		*/
	0x79,		/* `  60   ` 79  (ACCENT)		*/
	0x81,		/* A  61   A 81  (LOWER A)		*/
	0x82,		/* B  62   B 82  (LOWER B)		*/
	0x83,		/* C  63   C 83  (LOWER C)		*/
	0x84,		/* D  64   D 84  (LOWER D)		*/
	0x85,		/* E  65   E 85  (LOWER E)		*/
	0x86,		/* F  66   F 86  (LOWER F)		*/
	0x87,		/* G  67   G 87  (LOWER G)		*/
	0x88,		/* H  68   H 88  (LOWER H)		*/
	0x89,		/* I  69   I 89  (LOWER I)		*/
	0x91,		/* J  6A   J 91  (LOWER J)		*/
	0x92,		/* K  6B   K 92  (LOWER K)		*/
	0x93,		/* L  6C   L 93  (LOWER L)		*/
	0x94,		/* M  6D   M 94  (LOWER M)		*/
	0x95,		/* N  6E   N 95  (LOWER N)		*/
	0x96,		/* O  6F   O 96  (LOWER O)		*/
	0x97,		/* P  70   P 97  (LOWER P)		*/
	0x98,		/* Q  71   Q 98  (LOWER Q)		*/
	0x99,		/* R  72   R 99  (LOWER R)		*/
	0xA2,		/* S  73   S A2  (LOWER S)		*/
	0xA3,		/* T  74   T A3  (LOWER T)		*/
	0xA4,		/* U  75   U A4  (LOWER U)		*/
	0xA5,		/* V  76   V A5  (LOWER V)		*/
	0xA6,		/* W  77   W A6  (LOWER W)		*/
	0xA7,		/* X  78   X A7  (LOWER X)		*/
	0xA8,		/* Y  79   Y A8  (LOWER Y)		*/
	0xA9,		/* Z  7A   Z A9  (LOWER Z)		*/
	0xC0,		/* {  7B   { C0  (LEFT BRACE)		*/
	0x4F,		/* e  7C     4F  (SPLT BAR / SH SOL BAR)*/
	0xD0,		/* }  7D   } D0  (RIGHT BRACE)		*/
	0xA1,		/* ~  7E   ~ A1  (TILDE)		*/
	0x07,
	0x20,
	0x21,
	0x22,
	0x23,
	0x24,
	0x15,
	0x06,
	0x17,
	0x28,
	0x29,
	0x2A,
	0x2B,
	0x2C,
	0x09,
	0x0A,
	0x1B,
	0x30,
	0x31,
	0x1A,
	0x33,
	0x34,
	0x35,
	0x36,
	0x08,
	0x38,
	0x39,
	0x3A,
	0x3B,
	0x04,
	0x14,
	0x3E,
	0xE1,
	0x41,
	0x42,
	0x43,
	0x44,
	0x45,
	0x46,
	0x47,
	0x48,
	0x49,
	0x51,
	0x52,
	0x53,
	0x54,
	0x55,
	0x56,
	0x57,
	0x58,
	0x59,
	0x62,
	0x63,
	0x64,
	0x65,
	0x66,
	0x67,
	0x68,
	0x69,
	0x70,
	0x71,
	0x72,
	0x73,
	0x74,
	0x75,
	0x76,
	0x77,
	0x78,
	0x80,
	0x8A,
	0x8B,
	0x8C,
	0x8D,
	0x8E,
	0x8F,
	0x90,
	0x9A,
	0x9B,
	0x9C,
	0x9D,
	0x9E,
	0x9F,
	0xA0,
	0xAA,
	0xAB,
	0xAC,
	0x4A,
	0xAE,
	0xAF,
	0xB0,
	0xB1,
	0xB2,
	0xB3,
	0xB4,
	0xB5,
	0xB6,
	0xB7,
	0xB8,
	0xB9,
	0xBA,
	0xBB,
	0xBC,
	0x6A,
	0xBE,
	0xBF,
	0xCA,
	0xCB,
	0xCC,
	0xCD,
	0xCE,
	0xCF,
	0xDA,
	0xDB,
	0xDC,
	0xDD,
	0xDE,
	0xDF,
	0xEA,
	0xEB,
	0xEC,
	0xED,
	0xEE,
	0xEF,
	0xFA,
	0xFB,
	0xFC,
	0xFD,
	0xFE,
	0xFF
	};
