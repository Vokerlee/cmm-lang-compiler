#ifndef FRONTEND_REV_H_INCLUDED
#define FRONTEND_REV_H_INCLUDED

#include "tree.h"
#include "frontend_rev_config.h"

#include "text.h"

#define MULTIPLY(left, right)                                                                   \
    create_tree_element(OPER, MUL, left, right)

#define DIVIDE(left, right)                                                                     \
    create_tree_element(OPER, DIV, left, right)

#define ADDITION(left, right)                                                                   \
    create_tree_element(OPER, ADD, left, right)

#define SUBTRACT(left, right)                                                                   \
    create_tree_element(OPER, SUB, left, right)

#define CR_NUM(value)                                                                           \
    create_tree_element(NUM, value, nullptr, nullptr)

#define CR_VAR(value)                                                                           \
    create_tree_element(VAR, value, nullptr, nullptr)

struct variables
{
    int curr_size = 0;

    char var[MAX_VAR_NUM][MAX_VAR_NAME_LENGTH] = {0};
};

struct elements
{
    bin_tree_elem **elements_ = nullptr;

    int curr_size_ = 0;
};

void frontend_rev (FILE *tree_lang, FILE *lang_code);

bin_tree_elem *fill_tree (text_t *text, variables *var);

int var_search (variables* var, char* temp_var_name);

void lang_transfer (bin_tree_elem *element, FILE *lang, variables *var);

void print_body (bin_tree_elem *element, FILE *lang, variables *var, int *n_tab);

void print_expression(bin_tree_elem* element, FILE* lang, variables* var);

void print_tab (int *n_tab, FILE *lang);

#endif // FRONTEND_REV_H_INCLUDED