
/**********************************************************************************************/
/*the file holds the commnand for all parsing purposes. The fill the parser struct and invokes*/
/* the set cmds
/**********************************************************************************************/

#include <stdio.h>
#include<string.h>
#include <stdlib.h>
#include <ctype.h>  // for isdigit function
#include "sharedParcerStruct.h"
#include "parser.h"
#include "set.h"


static setobj setArray[NUM_SETS];/*array that holds all pointer and their string names*/

static cmdObj cmdArray[NUM_OF_COMMANDS];/*cmd array that holds all commands and their string names*/



static sepBase sep = {0};/*seperator struct that enables parsing of the various elements*/
static int comma_count;/*comma counter, important to check legal input*/

void parse(char* line)/*main function, leads the entire process through the various parsing funcs*/
{
	int num_in_string = 0;
	char* _line = malloc(sizeof(char) * INPUT_LENGTH);/*copy of the string input*/
	comma_count = countCommas(_line);
	initParserStruct();/*initaites the parser struct*/
	initSepStruct();/*initaites the seperator struct*/

	strcpy(_line, line); /*makes a back up*/

	_line = getCmdName(_line);/*cmd name is identified*/
	num_in_string = count_numbers(_line);
	/*seperates the rest of the input into a string array*/
	/*later to be parsed further*/
	sep = create_sep_arr(_line);


/*main switch dividies into set function, treating each cmd accordingly*/
	switch (parser.oper)/*parser employs enum to direct to commands*/
	{
	case _read_set:
		checkNumInString(num_in_string);
		compare_commas(READ_C, sep.seps_count, num_in_string);
		/*error enum that will report error and skip current input*/
		if (parser.oper != err)
		{
			parseSet(READ_SET_TYPE, sep.seps); /*takes of first elem of set*/
			removeFirstString(sep.seps);
			parseNum(sep.seps, parser.param.nums);
			read_set(parser.param.sets[0], parser.param.nums);
			CleanNumsArr();
		}
		else printf(NEXT_CMD);
		break;

	case _print_set:
		compare_commas(PRINT_C, sep.seps_count, num_in_string);
		/*error enum that will report error and skip current input*/
		if (parser.oper != err)
		{
			parseSet(READ_SET_TYPE, sep.seps); /*takes of first elem of set*/
			print_set(parser.param.sets[0]);
		}
		else printf(NEXT_CMD);
		break;
	case _union_set:
		compare_commas(SET_C, sep.seps_count, num_in_string);
		/*error enum that will report error and skip current input*/
		if (parser.oper != err)
		{
			parseSet(OTHER_TYPE, sep.seps);
			union_set(parser.param.sets[0], parser.param.sets[1], parser.param.sets[2]);
		}
		else printf(NEXT_CMD);
		break;
	case _intersect_set:
		compare_commas(SET_C, sep.seps_count, num_in_string);
		/*error enum that will report error and skip current input*/
		if (parser.oper != err)
		{
			parseSet(OTHER_TYPE, sep.seps);
			intersect_set(parser.param.sets[0], parser.param.sets[1], parser.param.sets[2]);
		}
		else 	printf(NEXT_CMD);
		break;
	case _sub_set:
		compare_commas(SET_C, sep.seps_count, num_in_string);
		/*error enum that will report error and skip current input*/
		if (parser.oper != err)
		{
			parseSet(OTHER_TYPE, sep.seps);
			sub_set(parser.param.sets[0], parser.param.sets[1], parser.param.sets[2]);
		}
			else printf(NEXT_CMD);
		break;
	case _symdiff_set:
		compare_commas(SET_C, sep.seps_count, num_in_string);
		/*error enum that will report error and skip current input*/
		if (parser.oper != err)
		{
			parseSet(OTHER_TYPE, sep.seps);
			symdiff_set(parser.param.sets[0], parser.param.sets[1], parser.param.sets[2]);
		}else
			printf(NEXT_CMD);
		break;
	case _stop:/*stop command will exit the program*/
		printf(STOP_MESSAGE);
		free(_line);
		freeAllocations();/*part of the end is freeing memory allocations*/
		return;

	case _initset:/*meant to init the sets in case of need*/
		initSet(&s1);
		initSet(&s2);
		initSet(&s3);
		initSet(&s4);
		initSet(&s5);
		initSet(&s6);
		break;
	default:
		break;
	}


	return;
}

/*seperates the rest of the input into a string array*/
/*later to be parsed further*/
sepBase create_sep_arr(char* str)
{
	/*type changes between read_set and all other function enum for read_set others*/
	sepBase sep_res = {0};
	char* s;
	int seps_count = 0;
	int comma_count = countCommas(str);
	printf("%d\n", comma_count);


	if (*str == '\0')
	{
		return sep_res;
	}
	do
	{
		sep_res.seps[seps_count] = str;
		seps_count++;
		s = strchr(str, ',');
		if (s)
		{
			*s = '\0';
			s++;
			if (*s == ',')
			{
				sep_res.fault_line = 1;

				strcpy(parser.err_buff, ERR_COMMA);
				printf("%s\n", parser.err_buff);
				parser.oper = ERROR_OPER;
				sep_res.seps[seps_count] = '\0';
				return sep_res;
			}
		}
		str = s;
	}
	while (s);
	sep_res.seps_count = seps_count;
	if (comma_count + 1 < seps_count)
	{
		sep_res.fault_line = 1;
		strcpy(parser.err_buff, ERR_COMMA);
		printf("%s\n", parser.err_buff);
		parser.oper = ERROR_OPER;
		return sep;
	}

	return sep_res;
}

/*exert out the commnad using the cmdArray*/
char* getCmdName(char* line)
{
	char cmd[CMD_LENGTH] = {'\0'};
	int i = 0;

	while (isspace(*line)) line++; /*reduce white space*/

	if (sscanf(line, "%s", cmd) == 1)/*helps removes whitespace*/
	{
		for (i = 0; i < NUM_OF_COMMANDS; i++)
		{
			if (strcmp(cmd, cmdArray[i].cmd_name) == 0)
			{
				parser.oper = cmdArray[i].oper;/* command is determined and set in parser*/
				line = line + getLengthCmd();/*removing the cmd from the main line*/
				return line;
			}
		}
	}
	else
	{
		strcpy(parser.err_buff,ERR_CMD_NOT_RECOGNIZED);
		printf("%s\n", parser.err_buff);
		parser.oper = ERROR_OPER;
		return line;
	}
	return line;
}
/*this cmd connect the parser with the wanted SET*/
/*it use the setArray,and compares names to sets*/
/*type varies b/w READ_SET_TYPE =1  and set command i.e OTHER_TYPE =3  */
void parseSet(int type, char** arr)
{
	int i = 0;
	int j = 0;
	int recognized = 0;
	/*i for setArray, j for arr,*/
	char stripped[4] = {0};


	for (j = 0; j < type; j++)
	{
		if (sscanf(arr[j], "%s", stripped) == 1)
		{
			for (i = 0; i < NUM_SETS; ++i)
			{
				if (strncmp(stripped, setArray[i].set_name, 4) == 0)
				{
					parser.param.sets[j] = &setArray[i].s_ptr;

					recognized = (j == (type - 1)) ? 1 : 0;
				}
			}
		}
		else
		{
			/*error automatically change the current cmd to ERR*/
			/* it tells the users what's the eror will stop the parsing */
			strcpy(parser.err_buff, ERR_SET_NOT_RECOGNIZED);
			printf("%s\n", parser.err_buff);
			parser.param.sets[0] = parser.param.sets[1] = parser.param.sets[2] = NULL;
			parser.oper = ERROR_OPER;
			return;
		}
		stripped[0] = '\0';
	}
     /*failure to recognize sets*/
	if (recognized == 0) /*out of the loop if not all sets were found*/
	{
		strcpy(parser.err_buff, ERR_SET_NOT_RECOGNIZED); //TODO: move to the parser struct
		printf("%s\n", parser.err_buff);

		parser.oper = ERROR_OPER;
		return;
	}
}

/*this extracts the nums for read set and checks if their legal*/
void parseNum(char** arr, int* numArr)
{
	int i = 0;
	int size = countStrings(arr);


	for (i = 0; i < size; i++)
	{
		if (sscanf(arr[i], "%d", &numArr[i]) == 1)
		{
			if (i < (size - 1) && (numArr[i] < MIN_NUM || numArr[i] > MAX_NUM))
			{
				strcpy(parser.err_buff, ERR_ILLEGAL_NUMBERS);
				printf("%s\n", parser.err_buff);
				parser.oper = ERROR_OPER;
				return;
			}
		}
		else
		{
			strcpy(parser.err_buff, ERR_ILLEGAL_NUMBERS);
			printf("%s\n", parser.err_buff);
			parser.oper = ERROR_OPER;
			return;
		}
	}
	if (numArr[size - 1] != -1)
	{
		strcpy(parser.err_buff, ERR_MINUS_ONE);
		printf("%s\n", parser.err_buff);
		parser.oper = ERROR_OPER;
		return;
	}
	numArr[size - 1] = 0;
	parser.param.length = size - 1;


}

/*removes the first strng entry for READ_SET*/
void removeFirstString(char** arr)
{
	int i = 0;
	int size = countStrings(arr);
	for (i = 1; i < size; i++)
	{
		arr[i - 1] = arr[i];
	}
	arr[size - 1] = '\0';
	sep.seps_count = sep.seps_count - 1;
}

int countStrings(char** arr)
{
	int count = 0;
	while (arr[count] != NULL)
	{
		count++;
	}
	return count;
}


int getLengthCmd()
{
	char* name = (cmdArray[parser.oper].cmd_name);
	return strlen(name);
}
/*initalizations functions for the two structs,sets and cmdArray and seArray*/
void initParserStruct()
{
	memset(parser.err_buff, 0, sizeof(parser.err_buff));

	parser.oper = ERROR_OPER;

	parser.param.sets[0] = (set*)malloc(sizeof(set));
	parser.param.sets[1] = (set*)malloc(sizeof(set));
	parser.param.sets[2] = (set*)malloc(sizeof(set));
}
/*initalizations functions for the two structs,sets and cmdArray and seArray*/
void initSepStruct()
{
	sep.fault_line = 0;
	memset(sep.seps, '\0', sizeof(sep.seps));
	sep.seps_count = 0;
}
/*initalizations functions for the two structs,sets and cmdArray and seArray*/
void initSetArray()
{
	int i = 0;
	set s_array[NUM_SETS] = {s1, s2, s3, s4, s5, s6};
	char* set_names[NUM_SETS] = {"SETA", "SETB", "SETC", "SETD", "SETE", "SETF"};
	for (i = 0; i < NUM_SETS; i++)
	{
		initSet(&setArray[i].s_ptr);
		setArray[i].set_name = (char*)malloc(sizeof(char) * 4);
		setArray[i].set_name = set_names[i];
		setArray[i].s_ptr = s_array[i];
	}
}
/*initalizations functions for the two structs,sets and cmdArray and seArray*/
void initCmdArray()
{
	int i = 0;

	char* cmd_names[NUM_OF_COMMANDS] = {
		"read_set", "print_set", "union_set", "intersect_set", "sub_set", "symdiff_set", "stop", "initset", "err"
	};
	for (i = 0; i < NUM_OF_COMMANDS; i++)
	{
		cmdArray[i].cmd_name = malloc(sizeof(char) * 4);
		cmdArray[i].cmd_name = cmd_names[i];
		cmdArray[i].oper = i;
	}
}
/*initalizations functions for the two structs,sets and cmdArray and seArray*/
void initSet(set* s)
{
	memset(s->arr, 0, sizeof(unsigned char) * 16);
}




void isSepError(void)
{
	if (sep.fault_line)
	{
		printf("%s\n", parser.err_buff);
		return;
	}
}
/*utility to print parser data*/
void printParser()
{
	int i = 0;
	printf("error_buff content:%s\n", (parser.err_buff));
	printf("oper_code:%d\n", parser.oper);
	for (i = 0; i < parser.param.length; ++i)
	{
		printf("param:%d\n", parser.param.nums[i]);
	}
	for (i = 0; i < 3; ++i)
	{
		printf("sets[%d]%p\n", i, (parser.param.sets[i] != NULL) ? parser.param.sets[i] : NULL);
	}
}

/*counts amount of numbers when parsing for read_set*/
int count_numbers(char* str)
{
	int count = 0;

	while (*str)
	{
		while (isdigit(*str))
		{
			if (!isdigit(*(str + 1)))
				count++;
			str++;
		}
		str++;
	}
	return count;
}
/*function overseeing sensbilie commas,num etc*/
void compare_commas(int type, int sep_count, int num_in_string)
{
	/*usess ENUM "STOP_C, PRINT_C, SET_C, READ_C*/
	switch (type)
	{
	case STOP_C:
		if (sep_count != 0)
		{
			strcpy(parser.err_buff, ERR_COMMA);
			printf("%s\n", parser.err_buff);
			parser.oper = ERROR_OPER;
		}
		break;
	case PRINT_C:
		if (sep_count != 1)
		{
			strcpy(parser.err_buff, ERR_COMMA);
			printf("%s\n", parser.err_buff);
			parser.oper = ERROR_OPER;
		}
		break;
	case SET_C:
		if (sep_count != 3 || comma_count != 2)
		{
			strcpy(parser.err_buff, ERR_COMMA);
			printf("%s\n", parser.err_buff);
			parser.oper = ERROR_OPER;
		}
		break;
	case READ_C:
		if (num_in_string == sep_count)
		{
			strcpy(parser.err_buff, ERR_COMMA);
			printf("%s\n", parser.err_buff);
			parser.oper = ERROR_OPER;
		}
		break;
	default:
		break;
	}
}

/*function specifically for read_set lack of number means error*/
void checkNumInString(int num)
{
	if (num == 0)
	{
		strcpy(parser.err_buff, ERR_ILLEGAL_NUMBERS);
		printf("%s\n", parser.err_buff);
		parser.oper = ERROR_OPER;
	}
}

/*TURNS all beats off*/
void cleanAllSets()
{
	cleanSet(&s1);
	cleanSet(&s2);
	cleanSet(&s3);
	cleanSet(&s4);
	cleanSet(&s5);
	cleanSet(&s6);
}
/*resets the parser num array*/
void CleanNumsArr()
{
	int i = 0;
	int size = parser.param.length;
	for (i = 0; i < size; i++)
	{
		parser.param.nums[i] = 0;
	}
}



int findMinusOne(char* str)
{
	if (strstr(str, "-1") != NULL)
	{
		printf("FOUND EOF IN STRING\n");
		return 1; // "-1" found in the string
	}
	else
	{
		return 0; // "-1" not found in the string
	}
}
/*checks if eof was found*/
void checkifEOF(char* line)
{
	while (*line)
	{
		if (*line == EOF)
		{
			strcpy(parser.err_buff,ERR_EOF);
			printf(ERR_EOF);
			parser.oper = ERROR_OPER;
			return;
		}
		line++;
	}
	return;
}
/*frees all allocations once stop cmd is given*/
void freeAllocations()
{
	int i;

	// Free set allocations
	for (i = 0; i < NUM_SETS; i++)
	{
		free(setArray[i].set_name);
	}

	// Free cmd_name allocations
	for (i = 0; i < NUM_OF_COMMANDS; i++)
	{
		free(cmdArray[i].cmd_name);
	}

	// Free parser set allocations
	for (i = 0; i < 3; i++)
	{
		free(parser.param.sets[i]);
	}
}

int countCommas(char* str)
{
	int count = 0;
	int i;
	for (i = 0; str[i] != '\0'; i++)
	{
		if (str[i] == ',')
		{
			count++;
		}
	}
	return count;
}
