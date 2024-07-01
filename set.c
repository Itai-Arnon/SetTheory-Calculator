#include <stdio.h>

#include "set.h"
#include "parser.h"
#include "sharedParcerStruct.h"

#define amount 16


/*reports the number in the array according the given value*/
/*there are 16 char in every set via modulo and integer division */
/*every bit can be reached making a total array of 128 numbers */
static int get_bit(set* s, int n)
{
	int mod_n = (n % 8);
	int set_sequence = n / 8;

	return (((s->arr[set_sequence]) & (1<< mod_n) ) >0 ? 1 : 0);
}
/*sets on  the number in the array according the given value*/
static void set_on(set* s, int n)
{
	int mod_n = (n % 8);
	int set_sequence = n / 8;

	s->arr[set_sequence] |= (1 << mod_n);
}

/*sets off the number in the array according the given value*/
static void set_off(set* s, int n)
{
	int mod_n = (n % 8);
	int set_sequence = n / 8;

	s->arr[set_sequence] &= ~(1 << mod_n);
}
/*sets off all the bits in the set hence erasing the array number*/
 void cleanSet(set* s)
{
	int i = 0;
	for (i = 0; i < 128; ++i)
	{
		set_off(s,i);
	}
}


void read_set(set* s, int values[])
{
	int i = 0;
	int was_zero = 0;
	int size = parser.param.length;
	for (i = 0; i < size; i++)
	{

		if (was_zero == 0 && values[i] == 0)
		{
			set_on(s, values[i]);
			was_zero = 1;
		}
		if (values[i] != 0) set_on(s, values[i]);

	}
}


void print_set(set* s)
{
	int i = 0;

	if (is_empty(s) == 1)
	{
		return;
	}
	for (i = 0; i < 128; i++)
	{
		printf("bit %2d=%2d  ", i, (get_bit(s, i)));
		if (i % 20 == 0)
			printf("\n");
	}
printf("\n");
}

/*reports if the array is empty*/
/*if one number is identified it immdiately returns falls*/
/*other it reports empty sets*/
int is_empty(set* s)
{
	int i = 0;
	for (i = 0; i < 128; i++)
	{
		if (get_bit(s, i)==1)
		{
			return 0;
		}
	}
	printf("\nThe Set is Empty\n");
	return 1;
}
/*all of this function employ the set_bit and get_bit function*/
/*they are very handy in creating the relations */


/*perfroms a union b/w two sets*/

void union_set(set* s1, set* s2, set* s3)
{
	int i = 0;
	for (i = 0; i < 128; i++)
	{
		if (get_bit(s1, i) || get_bit(s2, i))
			set_on(s3, i);
	}
}

/*perfroms an intersection b/w two sets*/
void intersect_set(set* s1, set* s2, set* s3)
{
	int i = 0;
	for (i = 0; i < 128; i++)
	{
		if (get_bit(s1, i) && get_bit(s2, i))
		{
			set_on(s3, i);
		}
	}
}
/*perfroms a subtraction b/w two sets*/
void sub_set(set* s1, set* s2, set* s3)
{
	int i = 0;
	for (i = 0; i < 128; i++)
	{
		if (get_bit(s1, i) && !get_bit(s2, i))
		{
			set_on(s3, i);
		}
	}
}
/*perfroms a symmetrical difference b/w two sets*/
void symdiff_set(set* s1, set* s2, set* s3)
{
	int i = 0;
	for (i = 0; i < 128; ++i)
	{
		if (get_bit(s1, i) && !get_bit(s2, i))
		{
			set_on(s3, i);
		}
		if (!get_bit(s1, i) && get_bit(s2, i))
		{
			set_on(s3, i);
		}
	}
}
