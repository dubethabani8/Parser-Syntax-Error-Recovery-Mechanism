/* Simple ad-hoc scanner for the calculator language.
    Michael L. Scott, 2008-2020.
*/

//#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cctype>

#include "scan.h"

char token_image[MAX_TOKEN_LEN];

using namespace std;

int line = 1;

token scan()
{
    static int c = ' ';
    /* next available char; extra (int) width accommodates EOF */
    int i = 0; /* index into token_image */

    /* skip white space */
    while (isspace(c))
    {
        if (c == '\n') line++;
        c = getchar();
    }

    if (c == EOF)
        return t_eof;
    if (isalpha(c))
    {
        do
        {
            token_image[i++] = c;
            if (i >= MAX_TOKEN_LEN)
            {
                cout << "max token length exceeded" << endl;
                exit(1);
            }
            c = getchar();
        } while (isalpha(c) || isdigit(c) || c == '_');
        token_image[i] = '\0';
        if (!strcmp(token_image, "read"))
            return t_read;
        else if (!strcmp(token_image, "write"))
            return t_write;
        else if (!strcmp(token_image, "while"))
            return t_while;
        else if (!strcmp(token_image, "if"))
            return t_if;
        else if (!strcmp(token_image, "end"))
            return t_end;

        else
            return t_id;
    }
    else if (isdigit(c))
    {
        do
        {
            token_image[i++] = c;
            c = getchar();
        } while (isdigit(c));
        token_image[i] = '\0';
        return t_literal;
    }
    else
        switch (c)
        {
        case ':':
            if ((c = getchar()) != '=')
            {
                fprintf(stderr, "error\n");
                exit(1);
            }
            else
            {
                c = getchar();
                return t_gets;
            }
            break;
        case '<':
            c = getchar();
            if (c == '>')
            {
                c = getchar();
                return t_not_eq;
            }
            else if (c == '=')
            {
                c = getchar();
                return t_less_eq;
            }
            else
            {
                c = getchar();
                return t_less;
            }
            break;
        case '>':
            c = getchar();
            if (c == '=')
            {
                c = getchar();
                return t_great_eq;
            }
            else
            {
                c = getchar();
                return t_great;
            }
            break;
        case '(':
            c = getchar();
            return t_lparen;
        case ')':
            c = getchar();
            return t_rparen;
        case '+':
            c = getchar();
            return t_add;
        case '-':
            c = getchar();
            return t_sub;
        case '*':
            c = getchar();
            return t_mul;
        case '/':
            c = getchar();
            return t_div;
        case '=':
            c = getchar();
            return t_eq;
        default:
            cout << "error" << endl;
            exit(1);
        }
}
