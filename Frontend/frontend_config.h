#ifndef FRONTEND_CONFIG_H_INCLUDED
#define FRONTEND_CONFIG_H_INCLUDED

const int MAX_PROJECT_TREE_ELEM = 10000;

const int MAX_VAR_NUM           = 100;
const int MAX_VAR_NAME_LENGTH   = 40;
const int MAX_ERROR_TEXT_LENGTH = 10;

const int MAX_FUNC_PARAM      = 15;
const int MAX_VAR_IN_FUNC_NUM = 30;

const int LEFT   = 0;
const int RIGHT  = 1;
const int VERTEX = -1;

enum operators
{
    ADD = '+',
    SUB = '-',
    MUL = '*',
    DIV = '/',
};

enum functions
{
    PRINT  = 1,
    SCAN   = 2,
};

enum type
{
    VAR         = 1,
    NUM         = 2,
    OPER        = 3,
    FUNC        = 4,
    USER_FUNC   = 5,
    SUBST       = 6,
    BRACKET     = 7,
    FIG_BRACKET = 8,
    BUNCH       = 9,
    MAIN        = 10,
    COMMAND     = 11,
    CONDITION   = 12,
    ENUM        = 13,
    RETURN      = 14,
    GLOB_VAR    = 15,
};

enum command
{
    IF     = 1,
    WHILE  = 2,
    ASSIGN = 3,
};

enum condition
{
    JE  = 1,
    JNE = 2,
    JA  = 3,
    JAE = 4,
    JB  = 5,
    JBE = 6,
};

enum brackets
{
    OPEN  = 1000,
    CLOSE = 1001,
};

enum error_state
{
    BRAKETS_ERR     = 1,
    UNKNOWN_DESIGN  = 2,
    END_OF_LINE_ERR = 3,
};

#endif // FRONTEND_CONFIG_H_INCLUDED
