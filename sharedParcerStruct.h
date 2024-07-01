
#ifndef SHAREDPARCER_H
#define SHAREDPARCER_H
#include "exclusive_typedef.h"




/*global struct in charge of passing all data b/w the parse part to the set functions*/
typedef struct
{
	char err_buff[200]; /*error report*/
	op_type oper;
	struct
	{
		int length;
		int nums[128];
		set *sets[3];
	} param;
}parserBase;

extern parserBase parser;
/*global sets*/
extern set s1, s2, s3, s4, s5, s6;


#endif /*SHAREDPARCER_H*/
