/*
examples of scanf() function and how it works when using %c and %[^\n]

The main problem is how scanf reads white space in different senarios

we use *c to ignore the white space after the string

* is used to read and ignore the input data so it is not stored in any variable

*/
#include <stdio.h>

int main(void)
{
    char c;
    char s[80];

    // give different combinations of spaces as input
    printf("Enter a character and a string: ");
    printf("\nscanf return: %d\n",scanf("%c %[^\n]", &c, s));
    printf("c = %c, s = %s\n", c, s);

    // give different combinations of spaces as input
    printf("Enter a character and a string: ");
    printf("\nscanf return: %d\n", scanf("%c%[^\n]", &c, s));
    printf("c = %c, s = %s\n", c, s);

    // give different combinations of spaces as input
    printf("Enter a character and a string: ");
    printf("\nscanf return: %d\n", scanf("%c%[^\n]*c", &c, s));
    printf("c = %c, s = %s\n", c, s);
}

/*
Output:
#### Try 1:
Enter a character and a string: hello mohammed

scanf return: 2
c = h, s = ello mohammed
Enter a character and a string: hello mohammed

scanf return: 2
c =
, s = hello mohammed

#### Try 2:

Enter a character and a string: h hello mohammed

scanf return: 2
c = h, s = hello mohammed
Enter a character and a string: h hello mohammed

scanf return: 2
c =
, s = h hello mohammed
