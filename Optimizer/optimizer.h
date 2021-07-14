#ifndef OPTIMIZER_H_INCLUDED
#define OPTIMIZER_H_INCLUDED

#include "optimizer_config.h"
#include "derivative.h"

#include "tree.h"
#include "text.h"

#define MULTIPLY(left, right)                                                                   \
    create_tree_element(OPER, MUL, left, right)

#define DIVIDE(left, right)                                                                     \
    create_tree_element(OPER, DIV, left, right)

#define ADDITION(left, right)                                                                   \
    create_tree_element(OPER, ADD, left, right)

#define SUBTRACT(left, right)                                                                   \
    create_tree_element(OPER, SUB, left, right)

#define POWER(left, right)                                                                      \
    create_tree_element(OPER, POW, left, right)

#define POWER_NUM(element, value)                                                               \
    create_tree_element(OPER, POW, element, create_tree_element(NUM, value, nullptr, nullptr))

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

void optimizer (FILE *tree_lang, FILE *optimized_tree);

bin_tree_elem *fill_tree (text_t *text, variables *var);

int var_search (variables* var, char* temp_var_name);

void optimizer (FILE *tree_lang, FILE *optimized_tree);

void optimize_tree (bin_tree *tree);

void num_optimize (bin_tree_elem *element, int *optimize_state);

void div_optimize (bin_tree_elem *element, int *optimize_state);

void const_fold_optimize (bin_tree_elem *element, int *optimize_state);

void derivative_optimize (bin_tree_elem *element);

void record_tree (bin_tree *tree, variables *var, FILE *lang_tree);

void write_in_tree_file (bin_tree_elem *element, FILE *lang_tree, variables *var);

const char *func_name (int value);

const char *condition_name (int value);


#endif // OPTIMIZER_H_INCLUDED