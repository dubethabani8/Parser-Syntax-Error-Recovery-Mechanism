/* Complete recursive descent parser for the calculator language.
    Builds on figure 2.16.  Prints a trace of productions predicted and
    tokens matched.  Does no error recovery: prints "syntax error" and
    dies on invalid input.
    Michael L. Scott, 2008-2020.
*/

//#include "stdio.h"
#include <iostream>
#include <cstdlib>

#include "scan.h"

using namespace std;

const char *names[] = {"end", "if", "while", "read", "write", "id", "literal", "gets",
                       "add", "sub", "mul", "div", "eq", "not_eq", "less", "great",
                       "less_eq", "great_eq", "lparen", "rparen", "eof"};

static token input_token;

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
            cout << "\"" << token_image << "\"";
        // cout << endl;
        input_token = scan();
    }
    else
        error("match");
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

void program()
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
        cout << "(program[ ";
        stmt_list();
        match(t_eof);
        break;
    default:
        error("program");
    }
}

void stmt_list()
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
        cout << "]) ";
        break; /* epsilon production */
    default:
        error("stmt_list");
    }
}

void stmt()
{
    switch (input_token)
    {
    case t_while:
        // cout << "predict stmt --> while cond stmt_list end" << endl;
        cout << "(" << names[input_token] << " (";
        match(t_while);
        cond();
        cout << ") [ ";
        stmt_list();
        match(t_end);
        break;
    case t_if:
        // cout << "predict stmt --> if cond stmt_list end" << endl;
        cout << "(" << names[input_token] << " (";
        match(t_if);
        cond();
        cout << ") [ ";
        stmt_list();
        match(t_end);
        break;
    case t_id:
        // cout << "predict stmt --> id gets expr" << endl;
        cout << "(";
        match(t_id);
        cout << " := ";
        match(t_gets);
        expr();
        cout << ") ";
        break;
    case t_read:
        // cout << "predict stmt --> read id" << endl;
        cout << "(" << names[input_token] << " ";
        match(t_read);
        match(t_id);
        cout << ") ";
        break;
    case t_write:
        // cout << "predict stmt --> write expr" << endl;
        cout << "( " << names[input_token] << " ";
        match(t_write);
        expr();
        cout << ") ";
        break;
    default:
        error("stmt");
    }
}

void cond()
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
        error("cond");
    }
}

void expr()
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
        error("expr");
    }
}

void term()
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
        error("term");
    }
}

void term_tail()
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
    case t_eof:
        // cout << "predict term_tail --> epsilon" << endl;
        break; /* epsilon production */
    default:
        error("term_tail");
    }
}

void factor()
{
    switch (input_token)
    {
    case t_literal:
        // cout << "predict factor --> literal" << endl;
        cout << "(" << names[input_token] << " ";
        match(t_literal);
        cout << ")";
        break;
    case t_id:
        // cout << "predict factor --> id" << endl;
        cout << "(" << names[input_token] << " ";
        match(t_id);
        cout << ")";
        break;
    case t_lparen:
        // cout << "predict factor --> lparen expr rparen" << endl;
        cout << "(";
        match(t_lparen);
        expr();
        cout << ")";
        match(t_rparen);
        break;
    default:
        error("factor");
    }
}

void factor_tail()
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
    case t_eof:
        // cout << "predict factor_tail --> epsilon" << endl;
        break; /* epsilon production */
    default:
        error("factor_tail");
    }
}

void row_op()
{
    switch (input_token)
    {
    case t_eq:
        // cout << "predict row_op --> eq" << endl;
        cout << " = ";
        match(t_eq);
        break;
    case t_not_eq:
        // cout << "predict row_op --> not_eq" << endl;
        cout << " <> ";
        match(t_not_eq);
        break;
    case t_less:
        // cout << "predict row_op --> less" << endl;
        cout << " < ";
        match(t_less);
        break;
    case t_great:
        // cout << "predict row_op --> great" << endl;
        cout << " > ";
        match(t_great);
        break;
    case t_less_eq:
        // cout << "predict row_op --> less_eq" << endl;
        cout << " <= ";
        match(t_less_eq);
        break;
    case t_great_eq:
        // cout << "predict row_op --> great_eq" << endl;
        cout << " >= ";
        match(t_great_eq);
        break;
    default:
        error("row_op");
    }
}

void add_op()
{
    switch (input_token)
    {
    case t_add:
        // cout << "predict add_op --> add" << endl;
        cout << " + ";
        match(t_add);
        break;
    case t_sub:
        // cout << "predict add_op --> sub" << endl;
        cout << " - ";
        match(t_sub);
        break;
    default:
        error("add_op");
    }
}

void mul_op()
{
    switch (input_token)
    {
    case t_mul:
        // cout << "predict mul_op --> mul" << endl;
        cout << " * ";
        match(t_mul);
        break;
    case t_div:
        // cout << "predict mul_op --> div" << endl;
        cout << " / ";
        match(t_div);
        break;
    default:
        error("mul_op");
    }
}

int main()
{
    input_token = scan();
    program();
    return 0;
}
