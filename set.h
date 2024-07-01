
#ifndef SET_H
#define SET_H


#ifndef M22_SET_H
#define M22_SET_H
#define LEN 8

#include "exclusive_typedef.h"


int is_empty(set* s);
void read_set(set* s, int values[]);
void print_set(set* s);
void union_set(set* s1, set* s2 ,set* s3);
void intersect_set(set* s1, set* s2 ,set* s3);
void sub_set(set* s1, set* s2 ,set* s3);
void symdiff_set(set* s1, set* s2 ,set* s3);
void cleanSet(set* s);





#endif

#endif
