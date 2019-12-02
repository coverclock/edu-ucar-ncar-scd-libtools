/*
**	H 2 I
**
**	Copyright 1994	University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	@(#)h2ic.h	1.3 96/06/10 jsloan@ncar.ucar.edu
**
**	Looks weird, I know, but this is some code that's common
**	to functions in h2i.c. Putting it here makes it easier to
**	maintain and eliminates a function call which could be
**	called a bezillion times. This _should_ work with both
**	ASCII and EBCDIC; I've tried not to have any character
**	code dependancies.
*/

#ifdef C_H2I

	{
	register long result, digit;
	register char ch;
	register int sign;

	sign=0;
	while (sign==0)
		switch (*string)
			{
		case '\0':
			return(0);
			break;
		case ' ':
			string++;
			break;
		case '\t':
			string++;
			break;
		case '+':
			sign=1;
			string++;
			break;
		case '-':
			sign=(-1);
			string++;
			break;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case 'a':
		case 'b':
		case 'c':
		case 'd':
		case 'e':
		case 'f':
		case 'A':
		case 'B':
		case 'C':
		case 'D':
		case 'E':
		case 'F':
			sign=1;
			break;
		default:
			return(0);
			}
	if ((*string=='0')&&((*(string+1)=='x')||(*(string+1)=='X')))
		string+=2;
	while ((ch=(*string))!='\0')
		if (ch!='0')
			break;
		else
			string++;
	result=0;
	while ((ch=(*(string++)))!='\0')
		{
		switch (ch)
			{
		case '0':
			digit=0;
			break;
		case '1':
			digit=1;
			break;
		case '2':
			digit=2;
			break;
		case '3':
			digit=3;
			break;
		case '4':
			digit=4;
			break;
		case '5':
			digit=5;
			break;
		case '6':
			digit=6;
			break;
		case '7':
			digit=7;
			break;
		case '8':
			digit=8;
			break;
		case '9':
			digit=9;
			break;
		case 'a':
		case 'A':
			digit=10;
			break;
		case 'b':
		case 'B':
			digit=11;
			break;
		case 'c':
		case 'C':
			digit=12;
			break;
		case 'd':
		case 'D':
			digit=13;
			break;
		case 'E':
		case 'e':
			digit=14;
			break;
		case 'f':
		case 'F':
			digit=15;
			break;
		default:
			return(0);
			}
		result=(result<<4)|digit;
		}
	*pointer=(sign<0)?(-((H2I_TYPE)result)):(H2I_TYPE)result;
	return(1);
	}

#endif /* C_H2I */
