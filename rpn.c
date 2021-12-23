//https://github.com/Patrickek
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> // library for checking if char is a number or letter.

#define MAXOPSIZE       20      /* maximum size of operand, operator */
#define TOOBIG          '9'     /* indicates string is too big */
#define MAXSIZE 100             /* stack size */

int getop(char* dest, int limit);

static int count = 0; // count for the stack items, so we can detect if its under or overflowed.
static double stack[MAXSIZE]; // The Stack himself :)

double push(double _item)//Push item to stack.
{
    if (count < MAXSIZE)//If stack is not bigger then its defined size(MAXSIZE) then push the item to stack.
    {
        stack[count++] = _item;
    }
    else//Else print error that stack overflowed and exit the program.
    {
        printf("Error: Stack overflow.\n");
        exit(1);
    }
    return _item;
}

void clear_stack()
{
    count = -1;
}

double pop()//Get item from stack.
{
    if (count > 0)//if stack has item then return it.
    {
        return stack[--count];
    }
    else//else stack underflow. It doesn't have any item.
    {
        printf("Error: Stack underflow.\n");
        clear_stack();
        exit(1);
    }
}

int main()
{
    int  type;
    char input[MAXOPSIZE];
    double op2;
    printf("Enter an RPN expression: ");
    while ((type = getop(input, MAXOPSIZE)) != EOF)//Get input with getop function we made.
    {
        switch (type)
        {
            case '0':
                push(strtod(input, NULL));//Convert string to float with strtod and push it to stack.
                break;
            case '+': // In case of an operator pop items from stack do the math and then push them to stack.
                push(pop() + pop());
                break;
            case '*':
                push(pop() * pop());// Same with the above one.
                break;
            case '-':
                op2 = pop();
                push(pop() - op2);// Same with the above one.
                break;
            case '/':
                op2 = pop();
                if (op2 == 0.0)// If input second operand is 0 then give error.
                {
                    printf("Divide by zero.\n");
                    exit(1);
                } else// else do the math.
                {
                    push(pop() / op2);
                }
                break;
            case '=':
                printf("Result: %.1f\n", push(pop()));
                printf("Enter an RPN expression: ");
                break;
            case 'c':
                clear_stack();
                break;
            case TOOBIG:
                printf("Input too large: '%s'\n", input);
                printf("Enter an RPN expression: ");
                break;
            default:
                printf("Unknown command: '%c'\n", type);
                printf("Enter an RPN expression: ");
        }
    } // end while
}

int getop(char* dest, int limit)
{
    int i;
    int c;

    while ((c = getchar()) == ' ' || c == '\t' || c == '\n');//Get chars if between spaces '' or tabs'  ' or new lines.
    if (c != '.' && !isdigit(c)) { return c; }
    dest[0] = c;
    for (i = 1; isdigit(c = getchar()); ++i)
    {
        if (i < limit) { dest[i] = c; }
    }

    // collect fractional portion
    if (c == '.')
    {
        if (i < limit) { dest[i] = c; }
        for (i++; (c = getchar()) >= '0' && c <= '9'; ++i)
        {
            if (i < limit)
            {
                dest[i] = c;
            }
        }
    }

    if (i < limit)
    { /* number is ok */
        ungetc(c, stdin);
        dest[i] = '\0';
        return '0';
    }
    else
    { /* input is too big; skip rest of line */
        while (c != '\n' && c != EOF)
            c = getchar();
        dest[limit - 1] = '\0';
        return TOOBIG;
    }
}
