#ifndef FRONTEND_H_INCLUDED
#define FRONTEND_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include "frontend_config.h"

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

int frontend (FILE *program, FILE *lang_tree);

void fill_tree (bin_tree *tree, FILE *formula, variables *var, elements *elem);

void record_tree (bin_tree *tree, variables *var, FILE *lang_tree);

void write_in_tree_file (bin_tree_elem *element, FILE *lang_tree, variables *var);

int is_keyword (char *temp_var_name, int *type);

int var_search (variables *var, char *temp_var_name);

bin_tree_elem *create_prog_tree       (elements *elem, int *counter);

bin_tree_elem *create_user_func_tree  (elements *elem, int *counter);

bin_tree_elem *create_var_func_tree   (elements *elem, int *counter);

bin_tree_elem *create_param_func_tree (elements *elem, int *counter);

bin_tree_elem *create_main_tree       (elements *elem, int *counter);

bin_tree_elem *create_body_tree       (elements *elem, int *counter);
 
bin_tree_elem *create_cmd_tree        (elements *elem, int *counter);

bin_tree_elem *create_e_tree          (elements *elem, int *counter);

bin_tree_elem *create_t_tree          (elements *elem, int *counter);

bin_tree_elem *create_w_tree          (elements *elem, int *counter);

bin_tree_elem *read_w_tree            (elements *elem, int *counter);

bin_tree_elem *create_p_tree          (elements *elem, int *counter);

bin_tree_elem *create_n_tree          (elements *elem, int *counter);

void syntax_error(text_t *text, elements *elem, int line, const char *file);

const char *func_name (int value);

const char *condition_name (int value);


#endif // FRONTEND_H_INCLUDED