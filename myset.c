#include <stdio.h>



#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "sharedParcerStruct.h"


/**********************************************************************************************/
/*welcome to the set theory calculator, it perform all the set actions . It loads number the*/
/*users inputs and can apply all set relations on the inpu									*/

/*the file holds the commands that accepts input from the user                            */
/* this where all of the differenet commands come together									*/
/**********************************************************************************************/



/*the main struct, it interconnects all information*/
parserBase parser;
set s1, s2, s3, s4, s5, s6;

int main(void)
{
	char line[200];
/*initiation of all main sets and arrays*/
	initCmdArray();
	initSetArray();
	initSet(&s1);
	initSet(&s2);
	initSet(&s3);
	initSet(&s4);
	initSet(&s5);
	initSet(&s6);
	initParserStruct();
	initSepStruct();


	printf(WELCOME);
	printf("\n \n ");
	/*fgets will contiuously allow the  to input his commands*/
	while (fgets(line, 200, stdin) != NULL && parser.oper != _stop)
	{
	/*to allow continuous input it allows syntax mistakes as well as empty sentences*/
		if (line[0] == '\0')
		{
			continue;
		}
		/*the main parsing cmd*/
		parse(line);
	}
	return 0;
}
