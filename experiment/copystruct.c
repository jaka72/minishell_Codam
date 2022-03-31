# include <stdio.h>

typedef struct	s_test
{
	int		i;
	char	*text;
}t_test;

int	main()
{
	t_test	test;
	t_test	newtest;

	test.i = 5;
	test.text = "hello";
	newtest.i = 4;
	
	newtest = test;
	newtest.text = "hello world";
	test.i = 100;
	test.text = "changing";
	printf("%d %s\n", test.i, test.text);
	printf("%d %s\n", newtest.i, newtest.text);


	char 	*str1;
	char	*str2;

	str1 = "hello";
	str2 = str1;

	printf("%p %s\n", str1, str1);
	printf("%p %s\n", str2, str2);;

	str1 = "qweqweqweqwe";

	printf("%p %s\n", str1, str1);
	printf("%p %s\n", str2, str2);;
	return (0);
}