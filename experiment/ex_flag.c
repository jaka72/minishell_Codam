#include "../minishell.h"
#define FLAG1 1
#define FLAG2 2
#define FLAG4 4
#define FLAG8 8

int	main()
{
	int	flag = 0;

	printf("FLAG1 is %d, flag is %d\n", FLAG1, flag);

	flag |= FLAG1; // make flag on : BITWISE OR (|)
	printf("flag |= FLAG1 : 	flag is %d\n", flag);

	flag &= ~(FLAG1); // make flag off : BITWISE AND (&) and BITWISE NOT (~)
	printf("flag &= ~(FLAG1) :	flag is %d\n", flag);

	flag ^= FLAG1; // toggle1 : BITWISE XOR(Exclusive-OR))
	printf("flag ^= FLAG1 : 	flag is %d\n", flag);

	flag ^= FLAG1; // toggle2
	printf("flag ^= FLAG1 : 	flag is %d\n", flag);

	flag |= FLAG1 | FLAG2;
	printf("flag |= FLAG1 | FLAG2 : 	flag is %d\n", flag);

	flag |= FLAG1 | FLAG2 | FLAG4;
	printf("flag |= FLAG1 | FLAG2 | FLAG4 : 	flag is %d\n", flag);

	printf("flag & FLAG8 is %d\n", (flag & FLAG8));  // to check the flag

	return (0);
}

// good to know about BIT WISE and flags
// https://blog.podkalicki.com/bit-level-operations-bit-flags-and-bit-masks/#:~:text=This%20is%20done%20by%20using,%E2%80%9C0%E2%80%9D%20(zero).