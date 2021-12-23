//https://github.com/Patrickek
#include <stdio.h>
#include <stdlib.h>

#define MAXOPSIZE       20      /* maximum size of operand, operator */
#define NUMBER          '0'     /* indicates number found */
#define TOOBIG          '9'     /* indicates string is too big */
#define MAXSIZE 100             /* stack size */

int getop(char* dest, int limit);

static int count = 0; // count for the stack items, so we can detect if its under or overflowed.
static double values[MAXSIZE];

double push(double _item)
{
    if (count < MAXSIZE)
    {
        values[count++] = _item;
    }
    else
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

double pop()
{
    if (count > 0)
    {
        return values[--count];
    }
    else
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
    while ((type = getop(input, MAXOPSIZE)) != EOF)
    {
        switch (type)
        {
            case NUMBER:
                push(strtod(input, NULL));//Convert string to float with strtod and push it to stack.
                break;
            case '+':
                push(pop() + pop());
                break;
            case '*':
                push(pop() * pop());
                break;
            case '-':
                op2 = pop();
                push(pop() - op2);
                break;
            case '/':
                op2 = pop();
                if (op2 == 0.0)
                    printf("Divide by zero.\n");
                else
                    push(pop() / op2);
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
                break;
            default:
                printf("Unknown command: '%c'\n", type);
        }
    } // end while
}

int getop(char* dest, int limit)
{
    int i;
    int c;

    while ((c = getchar()) == ' ' || c == '\t' || c == '\n');//Get chars if between spaces '' or tabs'  ' or new lines.
    if (c != '.' && (c < '0' || c > '9')) { return c; }
    dest[0] = c;
    for (i = 1; (c = getchar()) >= '0' && c <= '9'; ++i)
    {
        if (i < limit) { dest[i] = c; }
    }
    if (c == '.')
    {
        // collect fractional portion
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
        return NUMBER;
    }
    else
    { /* input is too big; skip rest of line */
        while (c != '\n' && c != EOF)
            c = getchar();
        dest[limit - 1] = '\0';
        return TOOBIG;
    }
}
