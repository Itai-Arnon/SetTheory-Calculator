/*This header file is meant to prevent definition clash*/

#ifndef PARSERSTRUCT_H
#define PARSERSTRUCT_H

/*typedef exclusive to this file*/
typedef enum { _read_set, _print_set, _union_set, _intersect_set, _sub_set, _symdiff_set, _stop, _initset,  err }op_type;

typedef struct
{
	unsigned char arr[16];
}set;

#endif /*PARSERSTRUCT_H*/
