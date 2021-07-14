#ifndef BACKEND_H_INCLUDED
#define BACKEND_H_INCLUDED

#include "tree.h"
#include "backend_config.h"
#include "exe_creator.h"

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

void backend (FILE *tree_lang, FILE *exe_file);

bin_tree_elem *fill_tree (text_t *text, variables *var);

int var_search (variables *var, char *temp_var_name);

void fill_text_sec (bin_tree *tree, BYTE *text_section, variables *var);

void find_glob_vars (bin_tree_elem *element, int n_glob_vars);

void main_var_optimize (bin_tree_elem *element, int n_glob_vars);

void encode_body (bin_tree_elem *vertex, BYTE *text_section, variables *var);

void encode_command (bin_tree_elem *element, BYTE *text_section, variables *var);

void encode_expr (bin_tree_elem *element, BYTE *text_section, variables *var);

void find_func_var (bin_tree_elem *element, int *param, char *n_param, int first_loc_var);

void encode_func (bin_tree_elem *vertex, BYTE *text_section, variables *var);

void param_pass_func (BYTE *text_section, char n_param);

void user_func_optimize (bin_tree_elem *element, int *param, int n_param);

void call_exit_program (BYTE *text_section);

void call_get_number (BYTE *text_section);

void call_print_number (BYTE *text_section);


#endif // BACKEND_H_INCLUDED