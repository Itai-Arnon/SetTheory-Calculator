myset: 		myset.o set.o parser.o
		gcc -ansi -Wall -pedantic myset.o set.o parcer.o -o myset

myset.o:	myset.c  parser.h set.h  exclusive_typedef.h
		gcc -ansi -Wall -pedantic myset.c -o myset.o

parcer.o:	parser.c parser.h sharedParserStruct.h set.h exclusive_typedef.h
		gcc -ansi -Wall -pedantic parser.c -o parser.o

set.o:		set.c set.h exclusive_typedef.h
		gcc -ansi -Wall -pedantic set.c -o set.o
		
	
