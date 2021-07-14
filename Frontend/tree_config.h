#ifndef TREE_CONFIG_H_INCLUDED
#define TREE_CONFIG_H_INCLUDED

const char GRAPH_BASE[]  = {"dot -Tpdf -o graph.pdf graph.dot"};
const char START_GRAPH[] = {"start graph.pdf"};
const char START_TEXT[]  = {"start tree_log.txt"};

enum diff_tree_error
{
    TREE_NULL_PTR               = 1,
    TREE_ROOT_NULL_PTR          = 2,
    TREE_INVALID_SIZE           = 3,
    TREE_REPEAT_INSERT_LEFT     = 4,
    TREE_REPEAT_INSERT_RIGHT    = 5,
    TREE_INVALID_INSERT_ELEMENT = 6,
};

#endif // TREE_CONFIG_H_INCLUDED
