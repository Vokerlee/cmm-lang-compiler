#ifndef DERIVATIVE_H_INCLUDED
#define DERIVATIVE_H_INCLUDED

#include "tree.h"
#include <ctype.h>
#include <time.h>

const int NO_VAR = -1;

const int BRACKETS_ON  = 1;
const int BRACKETS_OFF = 0;

struct elements;
struct variables;

bin_tree_elem *derivate_tree (bin_tree_elem *element, int var_value);

void print_func_text (int value, FILE *tex);

int is_elem_func (bin_tree_elem *element);


#endif // DERIVATIVE_H_INCLUDED