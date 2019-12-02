/*
**	S 2 I
**
**	Copyright 1993	University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	@(#)s2ic.h	4.2 96/06/10 jsloan@ncar.ucar.edu
**
**	Looks weird, I know, but this is some code that's common
**	to functions in s2i.c. Putting it here makes it easier to
**	maintain and eliminates a function call which could be
**	called a bezillion times. This _should_ work with both
**	ASCII and EBCDIC; I've tried not to have any character
**	code dependancies.
*/

#ifdef C_S2I

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
			sign=1;
			break;
		default:
			return(0);
			}
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
		default:
			return(0);
			}
		result=((result<<3)+result+result)+digit;
		}
	*pointer=(sign<0)?(-((S2I_TYPE)result)):(S2I_TYPE)result;
	return(1);
	}

#endif /* C_S2I */
