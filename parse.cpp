/* Complete recursive descent parser for the calculator language.
    Builds on figure 2.16.  Prints a trace of productions predicted and
    tokens matched.  Does no error recovery: prints "syntax error" and
    dies on invalid input.
    Michael L. Scott, 2008-2020.
*/

//#include "stdio.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>

#include "scan.h"

using namespace std;

const char *names[] = {"end", "if", "while", "read", "write", "id", "literal", "gets",
                       "add", "sub", "mul", "div", "eq", "not_eq", "less", "great",
                       "less_eq", "great_eq", "lparen", "rparen", "eof", "error"};

static token input_token;

string output = "";

FILE *f;
const char *filename;
string get_line_txt();

bool isError = false;

void error(const char *prod)
{
    cout << "syntax error at " << prod << endl;
    exit(1);
}

void match(token expected)
{
    if (input_token == expected)
    {
        // cout << "matched " << names[input_token];
        if (input_token == t_id || input_token == t_literal)
        {
            output += "\"";
            output += token_image;
            output += "\"";
        }
        // cout << endl;
        input_token = scan();
    }
    else
    {
        cout << "Syntax Error: Line " << line_num << endl;
        cout << "\t" << get_line_txt() << endl;
        cout << "\t\t"
             << "Did not expect to see \"" << names[input_token] << "\" here." << endl;
        isError = true;
        input_token = scan();
        //exit(1);
    }
}

void program();
void stmt_list();
void stmt();
void cond();
void expr();
void term();
void term_tail();
void factor();
void factor_tail();
void row_op();
void add_op();
void mul_op();

//FIRST(A)
void program()
{
    try
    {
        switch (input_token)
        {
        case t_id:
        case t_read:
        case t_write:
        case t_if:
        case t_while:
        case t_eof:
            // cout << "predict program --> stmt_list eof" << endl;
            output += "(program[ ";
            stmt_list();
            match(t_eof);
            break;
        default:
            throw "Did not expect to see: ";
        }
    }
    catch (const char *exp)
    {
        isError = true;
        cout << "\nSyntax Error: Line " << line_num << endl;
        cout << "\t" << get_line_txt() << endl;
        cout << "\t\t" << exp << names[input_token] << endl;
        while (!(input_token == t_eof || input_token == t_while ||
                 input_token == t_if || input_token == t_read || input_token == t_write ||
                 input_token == t_id))
            input_token = scan();
        program();
    }
}

//check
void stmt_list()
{
    try
    {
        switch (input_token)
        {
        case t_id:
        case t_read:
        case t_write:
        case t_while:
        case t_if:
            // cout << "predict stmt_list --> stmt stmt_list" << endl;
            stmt();
            stmt_list();
            break;
        case t_end:
        case t_eof:
            // cout << "predict stmt_list --> epsilon" << endl;
            output += "]) ";
            break; /* epsilon production */
        default:
            throw "Did not expect to see: ";
        }
    }
    catch (const char *exp)
    {
        isError = true;
        cout << "\nSyntax Error: Line " << line_num << endl;
        cout << "\t" << get_line_txt() << endl;
        cout << "\t\t" << exp << names[input_token] << endl;
        // while (!(input_token == t_eof || input_token == t_end))
        //     input_token = scan();
        while (!(input_token == t_eof || input_token == t_end || input_token == t_while ||
                 input_token == t_if || input_token == t_read || input_token == t_write ||
                 input_token == t_id))
            input_token = scan();
        stmt_list();
    }
}

void stmt()
{
    switch (input_token)
    {
    case t_while:
        // cout << "predict stmt --> while cond stmt_list end" << endl;
        output += "(" + string(names[input_token]) + " (";
        match(t_while);
        cond();
        output += ") [ ";
        stmt_list();
        match(t_end);
        break;
    case t_if:
        // cout << "predict stmt --> if cond stmt_list end" << endl;
        output += "(" + string(names[input_token]) + " (";
        match(t_if);
        cond();
        output += ") [ ";
        stmt_list();
        match(t_end);
        break;
    case t_id:
        // cout << "predict stmt --> id gets expr" << endl;
        output += "(";
        match(t_id);
        output += " := ";
        match(t_gets);
        expr();
        output += ") ";
        break;
    case t_read:
        // cout << "predict stmt --> read id" << endl;
        output += "(" + string(names[input_token]) + " ";
        match(t_read);
        match(t_id);
        output += ") ";
        break;
    case t_write:
        // cout << "predict stmt --> write expr" << endl;
        output += "( " + string(names[input_token]) + " ";
        match(t_write);
        expr();
        output += ") ";
        break;
    default:
        error("stmt");
    }
}

void cond()
{
    try
    {
        switch (input_token)
        {
        case t_id:
        case t_literal:
        case t_lparen:
            // cout << "predict cond --> expr row_op expr" << endl;
            expr();
            row_op();
            expr();
            break;
        default:
            throw "Did not expect to see: ";
        }
    }
    catch (const char *exp)
    {
        isError = true;
        cout << "\nSyntax Error: Line " << line_num << endl;
        cout << "\t" << get_line_txt() << endl;
        cout << "\t\t" << exp << names[input_token] << endl;
        while (!(input_token == t_eof || input_token == t_end || input_token == t_while ||
                 input_token == t_if || input_token == t_read || input_token == t_write ||
                 input_token == t_id))
            input_token = scan();
        return;
    }
}

void expr()
{
    try
    {
        switch (input_token)
        {
        case t_id:
        case t_literal:
        case t_lparen:
            // cout << "predict expr --> term term_tail" << endl;
            term();
            term_tail();
            break;
        default:
            throw "Did not expect to see: ";
        }
    }
    catch (const char *exp)
    {
        isError = true;
        cout << "\nSyntax Error: Line " << line_num << endl;
        cout << "\t" << get_line_txt() << endl;
        cout << "\t\t" << exp << names[input_token] << endl;
        while (!(input_token == t_eof || input_token == t_end || input_token == t_while ||
                 input_token == t_if || input_token == t_read || input_token == t_write ||
                 input_token == t_id || input_token == t_eq || input_token == t_not_eq ||
                 input_token == t_less || input_token == t_great || input_token == t_less_eq ||
                 input_token == t_great_eq || input_token == t_rparen))
            input_token = scan();
        return;
    }
}

void term()
{
    try
    {
        switch (input_token)
        {
        case t_id:
        case t_literal:
        case t_lparen:
            // cout << "predict term --> factor factor_tail" << endl;
            factor();
            factor_tail();
            break;
        default:
            throw "Did not expect to see: ";
        }
    }
    catch (const char *exp)
    {
        isError = true;
        cout << "\nSyntax Error: Line " << line_num << endl;
        cout << "\t" << get_line_txt() << endl;
        cout << "\t\t" << exp << names[input_token] << endl;
        while (!(input_token == t_eof || input_token == t_end || input_token == t_while ||
                 input_token == t_if || input_token == t_read || input_token == t_write ||
                 input_token == t_id || input_token == t_eq || input_token == t_not_eq ||
                 input_token == t_less || input_token == t_great || input_token == t_less_eq ||
                 input_token == t_great_eq || input_token == t_rparen || input_token == t_add ||
                 input_token == t_sub))
            input_token = scan();
    }
}

void term_tail()
{
    try
    {
        switch (input_token)
        {
        case t_add:
        case t_sub:
            // cout << "predict term_tail --> add_op term term_tail" << endl;
            add_op();
            term();
            term_tail();
            break;
        case t_rparen:
        case t_id:
        case t_read:
        case t_write:
        case t_eq:
        case t_not_eq:
        case t_less:
        case t_great:
        case t_less_eq:
        case t_great_eq:
        case t_end:
        case t_if:
        case t_while:
        case t_eof:
            // cout << "predict term_tail --> epsilon" << endl;
            break; /* epsilon production */
        default:
            throw "Did not expect to see: ";
        }
    }
    catch (const char *exp)
    {
        isError = true;
        cout << "\nSyntax Error: Line " << line_num << endl;
        cout << "\t" << get_line_txt() << endl;
        cout << "\t\t" << exp << names[input_token] << endl;
        while (!(input_token == t_eof || input_token == t_end || input_token == t_while ||
                 input_token == t_if || input_token == t_read || input_token == t_write ||
                 input_token == t_id || input_token == t_eq || input_token == t_not_eq ||
                 input_token == t_less || input_token == t_great || input_token == t_less_eq ||
                 input_token == t_great_eq || input_token == t_rparen))
            input_token = scan();
    }
}

void factor()
{
    try
    {
        switch (input_token)
        {
        case t_literal:
            // cout << "predict factor --> literal" << endl;
            output += "(" + string(names[input_token]) + " ";
            match(t_literal);
            output += ")";
            break;
        case t_id:
            // cout << "predict factor --> id" << endl;
            output += "(" + string(names[input_token]) + " ";
            match(t_id);
            output += ")";
            break;
        case t_lparen:
            // cout << "predict factor --> lparen expr rparen" << endl;
            output += "(";
            match(t_lparen);
            expr();
            output += ")";
            match(t_rparen);
            break;
        default:
            throw "Did not expect to see: ";
        }
    }
    catch (const char *exp)
    {
        isError = true;
        cout << "\nSyntax Error: Line " << line_num << endl;
        cout << "\t" << get_line_txt() << endl;
        cout << "\t\t" << exp << names[input_token] << endl;
        while (!(input_token == t_eof || input_token == t_end || input_token == t_while ||
                 input_token == t_if || input_token == t_read || input_token == t_write ||
                 input_token == t_id || input_token == t_eq || input_token == t_not_eq ||
                 input_token == t_less || input_token == t_great || input_token == t_less_eq ||
                 input_token == t_great_eq || input_token == t_rparen || input_token == t_add ||
                 input_token == t_sub || input_token == t_mul || input_token == t_div))
            input_token = scan();
        return;
    }
}

void factor_tail()
{
    try
    {
        switch (input_token)
        {
        case t_mul:
        case t_div:
            // cout << "predict factor_tail --> mul_op factor factor_tail" << endl;
            mul_op();
            factor();
            factor_tail();
            break;
        case t_add:
        case t_sub:
        case t_eq:
        case t_not_eq:
        case t_less:
        case t_great:
        case t_less_eq:
        case t_great_eq:
        case t_end:
        case t_rparen:
        case t_id:
        case t_read:
        case t_write:
        case t_while:
        case t_if:
        case t_eof:
            // cout << "predict factor_tail --> epsilon" << endl;
            break; /* epsilon production */
        default:
            throw "Did not expect to see: ";
        }
    }
    catch (const char *exp)
    {
        isError = true;
        cout << "\nSyntax Error: Line " << line_num << endl;
        cout << "\t" << get_line_txt() << endl;
        cout << "\t\t" << exp << names[input_token] << endl;
        while (!(input_token == t_eof || input_token == t_end || input_token == t_while ||
                 input_token == t_if || input_token == t_read || input_token == t_write ||
                 input_token == t_id || input_token == t_eq || input_token == t_not_eq ||
                 input_token == t_less || input_token == t_great || input_token == t_less_eq ||
                 input_token == t_great_eq || input_token == t_rparen || input_token == t_add ||
                 input_token == t_sub))
            input_token = scan();
        return;
    }
}

void row_op()
{
    try
    {
        switch (input_token)
        {
        case t_eq:
            // cout << "predict row_op --> eq" << endl;
            output += " = ";
            match(t_eq);
            break;
        case t_not_eq:
            // cout << "predict row_op --> not_eq" << endl;
            output += " <> ";
            match(t_not_eq);
            break;
        case t_less:
            // cout << "predict row_op --> less" << endl;
            output += " < ";
            match(t_less);
            break;
        case t_great:
            // cout << "predict row_op --> great" << endl;
            output += " > ";
            match(t_great);
            break;
        case t_less_eq:
            // cout << "predict row_op --> less_eq" << endl;
            output += " <= ";
            match(t_less_eq);
            break;
        case t_great_eq:
            // cout << "predict row_op --> great_eq" << endl;
            output += " >= ";
            match(t_great_eq);
            break;
        default:
            throw "Did not expect to see: ";
        }
    }
    catch (const char *exp)
    {
        isError = true;
        cout << "\nSyntax Error: Line " << line_num << endl;
        cout << "\t" << get_line_txt() << endl;
        cout << "\t\t" << exp << names[input_token] << endl;
        while (!(input_token == t_eof || input_token == t_lparen || input_token == t_id || input_token == t_literal))
            input_token = scan();
        cout << "---" << token_image;
        return;
    }
}

void add_op()
{
    try
    {
        switch (input_token)
        {
        case t_add:
            // cout << "predict add_op --> add" << endl;
            output += " + ";
            match(t_add);
            break;
        case t_sub:
            // cout << "predict add_op --> sub" << endl;
            output += " - ";
            match(t_sub);
            break;
        default:
            throw "Did not expect to see";
        }
    }
    catch (const char *exp)
    {
        isError = true;
        cout << "\nSyntax Error: Line " << line_num << endl;
        cout << "\t" << get_line_txt() << endl;
        cout << "\t\t" << exp << names[input_token] << endl;
        while (!(input_token == t_eof || input_token == t_lparen || input_token == t_id || input_token == t_literal))
            input_token = scan();
        return;
    }
}

void mul_op()
{
    try
    {
        switch (input_token)
        {
        case t_mul:
            // cout << "predict mul_op --> mul" << endl;
            output += " * ";
            match(t_mul);
            break;
        case t_div:
            // cout << "predict mul_op --> div" << endl;
            output += " / ";
            match(t_div);
            break;
        default:
            throw "Did not expect to see: ";
        }
    }
    catch (const char *exp)
    {
        isError = true;
        cout << "\nSyntax Error: Line " << line_num << endl;
        cout << "\t" << get_line_txt() << endl;
        cout << "\t\t" << exp << names[input_token] << endl;
        while (!(input_token == t_eof || input_token == t_lparen || input_token == t_id || input_token == t_literal))
            input_token = scan();
        return;
    }
}

//returns text at specific line number
string get_line_txt()
{

    string line_txt;
    ifstream in(filename);

    for (int i = 1; i < line_num; ++i)
        getline(in, line_txt);
    getline(in, line_txt);

    return line_txt;
}

int main(int argc, char **argv)
{
    filename = argv[1];
    f = fopen(filename, "r");

    input_token = scan();
    program();
    if (!isError)
    cout << output;
    cout << endl;
    return 0;
}
