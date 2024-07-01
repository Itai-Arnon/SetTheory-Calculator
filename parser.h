#ifndef PARSER_H
#define PARSER_H

#include "exclusive_typedef.h"


#define SET_ARRAY_LENGTH 128
#define CMD_LENGTH 13
#define NUM_OF_COMMANDS 9
#define SET_PARSE_LEN 3
#define NUM_SETS 6
#define INPUT_LENGTH 200
#define ERROR_OPER err   /*struct error when no cmd recognized*/
#define ERR_CMD_NOT_RECOGNIZED "Command not found"
#define ERR_SET_NOT_RECOGNIZED " SET Pointer Parse failed"
#define ERR_ILLEGAL_NUMBERS "Illegal numbers"
#define ERR_NUMBER_OUT_BOUND "Number bigger than 127 or smaller than 0"
#define ERR_MINUS_ONE "  Closing -1 is missing"
#define ERR_ILLEGAL_INPUT "Illegal input"
#define ERR_COMMA "Wrong Amount of Commas"
#define ERR_EOF "End Of File Was Reached"

#define STOP_MESSAGE "Stopped the Service, Thank You. Come Back Soon"
#define NEXT_CMD "Due To Error Moving To Next Command\n"
#define WELCOME "Welcome to The Set Caluculator\n Please Enter Your Command\n (Make Sure To Obide All Syntax Rules)\n"
#define MIN_NUM 0
#define MAX_NUM 127



typedef enum{STOP_C, PRINT_C, SET_C, READ_C}comma_type;

typedef enum
{
	READ_SET_TYPE = 1,
	OTHER_TYPE = 3
} SET_PTR_NUM;





typedef struct
{
	char *set_name;
	set s_ptr;
}setobj;

typedef struct
{
	char *cmd_name;
	op_type oper;
} cmdObj;



typedef struct /*struct aid create a string free of commas*/
{
	char* seps[50];
	int seps_count;
	int fault_line;
} sepBase;


void parse(char* line);/*parse the line*/
void parseSet(int type, char** arr);/*recognize and point to all sets continuous*/
sepBase create_sep_arr(char* str);
char* getCmdName(char* cmd);/*TODO add cmdname len to string*/
void parseNum(char** arr , int *numArr);
int getLengthCmd();
int countStrings(char** arr);
void initParserStruct();
void initSepStruct();
void initSetArray();
void initCmdArray();
void initSet(set* s);/*init set done every start of program at least*/

void isSepError(void);
void printParser();
int count_numbers(char* line);
void compare_commas(int type, int sep_count, int num_in_string);
void checkNumInString(int num_in_string);

void cleanAllSets();

void CleanNumsArr();
int findMinusOne(char* str);
void checkifEOF(char* line);
void removeFirstString(char** arr);
void freeAllocations();
int countCommas(char* str); ;

#endif /*PARSER_H*/