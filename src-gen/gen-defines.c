#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>

enum type
{
	unsigned_integer,
	integer,
	floating_point
};

void print_typedef(const char *type_name, size_t byte_size, enum type type)
{
	switch (type)
	{
		case unsigned_integer:
			if (sizeof(unsigned char) == byte_size)
			{
				printf("typedef unsigned char   %s;\n", type_name);
			}
			else if (sizeof(unsigned short) == byte_size)
			{
				printf("typedef unsigned short  %s;\n", type_name);
			}
			else if (sizeof(unsigned int) == byte_size)
			{
				printf("typedef unsigned int    %s;\n", type_name);
			}
			else if (sizeof(unsigned long) == byte_size)
			{
				printf("typedef unsigned long   %s;\n", type_name);
			}
			else
			{
				fprintf(stderr, "could not find any unsigned integer type of size %lu\n", byte_size);
				exit(1);
			}
			break;

		case integer:
			if (sizeof(char) == byte_size)
			{
				printf("typedef          char   %s;\n", type_name);
			}
			else if (sizeof(short) == byte_size)
			{
				printf("typedef          short  %s;\n", type_name);
			}
			else if (sizeof(int) == byte_size)
			{
				printf("typedef          int    %s;\n", type_name);
			}
			else if (sizeof(long) == byte_size)
			{
				printf("typedef          long   %s;\n", type_name);
			}
			else
			{
				fprintf(stderr, "could not find any integer type of size %lu\n", byte_size);
				exit(1);
			}
			break;

		case floating_point:
			if (sizeof(float) == byte_size)
			{
				printf("typedef          float  %s;\n", type_name);
			}
			else if (sizeof(double) == byte_size)
			{
				printf("typedef          double %s;\n", type_name);
			}
			else
			{
				fprintf(stderr, "could not find any floating point type of size %lu\n", byte_size);
				exit(1);
			}
			break;
	}
}

int main(int argc, char *argv[])
{
	printf("#ifndef __GEN_DEFINES_H__\n");
	printf("#define __GEN_DEFINES_H__\n");

	printf("/* THIS FILE IS GENERATED. DO NOT EDIT. */\n");

	printf("#define MAX_PATH         %d\n", PATH_MAX);

	printf("#define SIZE_BYTE_CHAR   %lu\n", sizeof(char));
	printf("#define SIZE_BYTE_SHORT  %lu\n", sizeof(short));
	printf("#define SIZE_BYTE_INT    %lu\n", sizeof(int));
	printf("#define SIZE_BYTE_LONG   %lu\n", sizeof(long));
	printf("#define SIZE_BYTE_FLOAT  %lu\n", sizeof(float));
	printf("#define SIZE_BYTE_DOUBLE %lu\n", sizeof(double));
	printf("#define SIZE_BYTE_PTR    %lu\n", sizeof((void *)0));

	print_typedef("u8", 1, unsigned_integer);
	print_typedef("i8", 1, integer);
	print_typedef("u16", 2, unsigned_integer);
	print_typedef("i16", 2, integer);
	print_typedef("u32", 4, unsigned_integer);
	print_typedef("i32", 4, integer);
	print_typedef("u64", 8, unsigned_integer);
	print_typedef("i64", 8, integer);
	print_typedef("usize", sizeof((void *)0), unsigned_integer);
	print_typedef("isize", sizeof((void *)0), integer);
	print_typedef("f32", 4, floating_point);
	print_typedef("f64", 8, floating_point);

	printf("#endif /* ! __GEN_DEFINES_H__ */\n");

	return 0;
}
