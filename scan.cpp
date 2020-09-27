/* Simple ad-hoc scanner for the calculator language.
    Michael L. Scott, 2008-2020.
*/

//#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <string>

#include "scan.h"

using namespace std;

char token_image[MAX_TOKEN_LEN];

int line_num = 1;
string line_txt = "";
char err = ' ';


token scan()
{
    static int c = ' ';
    /* next available char; extra (int) width accommodates EOF */
    int i = 0; /* index into token_image */

    /* skip white space */
    while (isspace(c))
    {
        if (c == '\n') line_num++;
        
        line_txt += (char)c;
        c = getc(f);
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
            line_txt += (char)c;
            c = getc(f);
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
            line_txt += (char)c;
            c = getc(f);
        } while (isdigit(c));
        token_image[i] = '\0';
        return t_literal;
    }
    else
        switch (c)
        {
        case ':':
            line_txt += (char)c;
            if ((c = getc(f)) != '=')
            {
                fprintf(stderr, "error\n");
                exit(1);
            }
            else
            {
                line_txt += (char)c;
                c = getc(f);
                return t_gets;
            }
            break;
        case '<':
            line_txt += (char)c;
            c = getc(f);
            if (c == '>')
            {
                line_txt += (char)c;
                c = getc(f);
                return t_not_eq;
            }
            else if (c == '=')
            {
                line_txt += (char)c;
                c = getc(f);
                return t_less_eq;
            }
            else
            {
                line_txt += (char)c;
                c = getc(f);
                return t_less;
            }
            break;
        case '>':
            line_txt += (char)c;
            c = getc(f);
            if (c == '=')
            {
                line_txt += (char)c;
                c = getc(f);
                return t_great_eq;
            }
            else
            {
                line_txt += (char)c;
                c = getc(f);
                return t_great;
            }
            break;
        case '(':
            line_txt += (char)c;
            c = getc(f);
            return t_lparen;
        case ')':
            line_txt += (char)c;
            c = getc(f);
            return t_rparen;
        case '+':
            line_txt += (char)c;
            c = getc(f);
            return t_add;
        case '-':
            line_txt += (char)c;
            c = getc(f);
            return t_sub;
        case '*':
            line_txt += (char)c;
            c = getc(f);
            return t_mul;
        case '/':
            line_txt += (char)c;
            c = getc(f);
            return t_div;
        case '=':
            line_txt += (char)c;
            c = getc(f);
            return t_eq;
        default:
            err = c;
            return t_err;
        }
}
