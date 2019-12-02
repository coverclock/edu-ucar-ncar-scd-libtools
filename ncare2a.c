/*
**	N C A R E 2 A
**
**	Copyright 1992	University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		NCAR EBCDIC to ASCII Translation Table
**	Program		TAGS libtools
**	Project		TAGS libtools
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Tue Sep 29 11:19:57 MDT 1992
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
**
**	Abstract
**
**	EBCDIC to ASCII translation table, originally developed at
**	NCAR by Basil Irwin (irwin@ncar.ucar.edu) for MASNET on CMS.
*/

static char copyright[]="Copyright 1992 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)ncare2a.c	4.5 96/05/21 jsloan@ncar.ucar.edu";

#include "libtools.h"

unsigned char ncare2a[]=
	{
	0x00,
	0x01,
	0x02,
	0x03,
	0x9C,
	0x09,
	0x86,
	0x7F,
	0x97,
	0x8D,
	0x8E,
	0x0B,
	0x0C,
	0x0D,
	0x0E,
	0x0F,
	0x10,
	0x11,
	0x12,
	0x13,
	0x9D,
	0x85,
	0x08,
	0x87,
	0x18,
	0x19,
	0x92,
	0x8F,
	0x1C,
	0x1D,
	0x1E,
	0x1F,
	0x80,
	0x81,
	0x82,
	0x83,
	0x84,
	0x0A,
	0x17,
	0x1B,
	0x88,
	0x89,
	0x8A,
	0x8B,
	0x8C,
	0x05,
	0x06,
	0x07,
	0x90,
	0x91,
	0x16,
	0x93,
	0x94,
	0x95,
	0x96,
	0x04,
	0x98,
	0x99,
	0x9A,
	0x9B,
	0x14,
	0x15,
	0x9E,
	0x1A,		/* ECBDCI  ASCII			*/
	0x20,		/*    40     20   (SPACE)		*/
	0xA0,
	0xA1,
	0xA2,
	0xA3,
	0xA4,
	0xA5,
	0xA6,
	0xA7,
	0xA8,
	0xD5,		/*    4A   U 5B   (CENTS / LEFT BRA.)	*/
	0x2E,		/* .  4B   . 2E   (PERIOD)		*/
	0x3C,		/* <  4C   < 3C   (LESS THAN)		*/
	0x28,		/* (  4D   ( 28   (LEFT PAREN)		*/
	0x2B,		/* +  4E   + 2B   (PLUS)		*/
	0x7C,		/*    4F   e 7C   (SH SOL BAR/SPLT BAR)	*/
	0x26,		/* &  50   & 26   (AMPERSAND)		*/
	0xA9,
	0xAA,
	0xAB,
	0xAC,
	0xAD,
	0xAE,
	0xAF,
	0xB0,
	0xB1,
	0x21,		/* |  5A   | 21   (EXCLAMATION)		*/
	0x24,		/* $  5B   $ 24   (DOLLAR)		*/
	0x2A,		/* *  5C   * 2A   (STAR)		*/
	0x29,		/* )  5D   ) 29   (RIGHT PAREN)		*/
	0x3B,		/* ;  5E   ; 3B   (SEMICOLON)		*/
	0x5E,		/*    5F   ^ 5E   (HAT)			*/
	0x2D,		/* -  60   - 2D   (MINUS)		*/
	0x2F,		/* /  61   / 2F   (SLASH)		*/
	0xB2,
	0xB3,
	0xB4,
	0xB5,
	0xB6,
	0xB7,
	0xB8,
	0xB9,
	0xE5,		/* e  6A   ! E5   (SPLT BAR/RIGHT BRA.)	*/
	0x2C,		/* ,  6B   , 2C   (COMMA)		*/
	0x25,		/* %  6C   % 25   (PERCENT)		*/
	0x5F,		/* _  6D   _ 5F   (UNDERSCORE)		*/
	0x3E,		/* >  6E   > 3E   (GREATER THAN)	*/
	0x3F,		/* ?  6F   ? 3F   (QUESTION)		*/
	0xBA,
	0xBB,
	0xBC,
	0xBD,
	0xBE,
	0xBF,
	0xC0,
	0xC1,
	0xC2,
	0x60,		/* `  79   ` 60   (ACCENT)		*/
	0x3A,		/* :  7A   : 3A   (COLON)		*/
	0x23,		/* #  7B   # 23   (POUND)		*/
	0x40,		/* @  7C   @ 40   (EACH)		*/
	0x27,		/* '  7D   ' 27   (APOSTROPHE)		*/
	0x3D,		/* =  7E   = 3D   (EQUALS)		*/
	0x22,		/* "  7F   " 22   (DOUBLE QUOTE)	*/
	0xC3,
	0x61,		/* A  81   A 61   (LOWER A)		*/
	0x62,		/* B  82   B 62   (LOWER B)		*/
	0x63,		/* C  83   C 63   (LOWER C)		*/
	0x64,		/* D  84   D 64   (LOWER D)		*/
	0x65,		/* E  85   E 65   (LOWER E)		*/
	0x66,		/* F  86   F 66   (LOWER F)		*/
	0x67,		/* G  87   G 67   (LOWER G)		*/
	0x68,		/* H  88   H 68   (LOWER H)		*/
	0x69,		/* I  89   I 69   (LOWER I)		*/
	0xC4,
	0xC5,
	0xC6,
	0xC7,
	0xC8,
	0xC9,
	0xCA,
	0x6A,		/* J  91   J 6A   (LOWER J)		*/
	0x6B,		/* K  92   K 6B   (LOWER K)		*/
	0x6C,		/* L  93   L 6C   (LOWER L)		*/
	0x6D,		/* M  94   M 6D   (LOWER M)		*/
	0x6E,		/* N  95   N 6E   (LOWER N)		*/
	0x6F,		/* O  96   O 6F   (LOWER O)		*/
	0x70,		/* P  97   P 70   (LOWER P)		*/
	0x71,		/* Q  98   Q 71   (LOWER Q)		*/
	0x72,		/* R  99   R 72   (LOWER R)		*/
	0xCB,
	0xCC,
	0xCD,
	0xCE,
	0xCF,
	0xD0,
	0xD1,
	0x7E,		/* ~  A1   ~ 7E   (TILDE)		*/
	0x73,		/* S  A2   S 73   (LOWER S)		*/
	0x74,		/* T  A3   T 74   (LOWER T)		*/
	0x75,		/* U  A4   U 75   (LOWER U)		*/
	0x76,		/* V  A5   V 76   (LOWER V)		*/
	0x77,		/* W  A6   W 77   (LOWER W)		*/
	0x78,		/* X  A7   X 78   (LOWER X)		*/
	0x79,		/* Y  A8   Y 79   (LOWER Y)		*/
	0x7A,		/* Z  A9   Z 7A   (LOWER Z)		*/
	0xD2,
	0xD3,
	0xD4,
	0x5B,		/*    AD     5B   (LEFT BRACKET)	*/
	0xD6,
	0xD7,
	0xD8,
	0xD9,
	0xDA,
	0xDB,
	0xDC,
	0xDD,
	0xDE,
	0xDF,
	0xE0,
	0xE1,
	0xE2,
	0xE3,
	0xE4,
	0x5D,		/*    BD     5D   (RIGHT BRACKET)	*/
	0xE6,
	0xE7,
	0x7B,		/* {  C0   { 7B   (LEFT BRACE)		*/
	0x41,		/* A  C1   A 41				*/
	0x42,		/* B  C2   B 42				*/
	0x43,		/* C  C3   C 43				*/
	0x44,		/* D  C4   9 44				*/
	0x45,		/* E  C5   E 45				*/
	0x46,		/* F  C6   F 46				*/
	0x47,		/* G  C7   G 47				*/
	0x48,		/* H  C8   H 48				*/
	0x49,		/* I  C9   I 49				*/
	0xE8,
	0xE9,
	0xEA,
	0xEB,
	0xEC,
	0xED,
	0x7D,		/* }  D0   } 7D   (RIGHT BRACE)		*/
	0x4A,		/* J  D1   J 4A				*/
	0x4B,		/* K  D2   K 4B				*/
	0x4C,		/* L  D3   L 4C				*/
	0x4D,		/* M  D4   M 4D				*/
	0x4E,		/* N  D5   N 4E				*/
	0x4F,		/* O  D6   O 4F				*/
	0x50,		/* P  D7   P 50				*/
	0x51,		/* Q  D8   Q 51				*/
	0x52,		/* R  D9   R 52				*/
	0xEE,
	0xEF,
	0xF0,
	0xF1,
	0xF2,
	0xF3,
	0x5C,		/* \  E0   \ 5C   (BACKSLASH)		*/
	0x9F,
	0x53,		/* S  E2   S 53				*/
	0x54,		/* T  E3   T 54				*/
	0x55,		/* U  E4   U 55				*/
	0x56,		/* V  E5   V 56				*/
	0x57,		/* W  E6   W 57				*/
	0x58,		/* X  E7   X 58				*/
	0x59,		/* Y  E8   Y 59				*/
	0x5A,		/* Z  E9   Z 5A				*/
	0xF4,
	0xF5,
	0xF6,
	0xF7,
	0xF8,
	0xF9,
	0x30,		/* 0  F0   0 30				*/
	0x31,		/* 1  F1   1 31				*/
	0x32,		/* 2  F2   2 32				*/
	0x33,		/* 3  F3   3 33				*/
	0x34,		/* 4  F4   4 34				*/
	0x35,		/* 5  F5   5 35				*/
	0x36,		/* 6  F6   6 36				*/
	0x37,		/* 7  F7   7 37				*/
	0x38,		/* 8  F8   8 38				*/
	0x39,		/* 9  F9   9 39				*/
	0xFA,
	0xFB,
	0xFC,
	0xFD,
	0xFE,
	0xFF
	};
