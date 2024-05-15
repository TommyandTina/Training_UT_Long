/*
 * $QNXLicenseC:
 * Copyright 2018, QNX Software Systems. All Rights Reserved.
 *
 * You must obtain a written license from and pay applicable license fees to QNX
 * Software Systems before you may reproduce, modify or distribute this software,
 * or any work that includes all or part of this software.   Free development
 * licenses are available for evaluation and non-commercial purposes.  For more
 * information visit http://licensing.qnx.com or email licensing@qnx.com.
 *
 * This file may contain contributions from others.  Please review this entire
 * file for other proprietary rights or license notices, as well as the QNX
 * Development Suite License Guide at http://licensing.qnx.com/license-guide/
 * for other information.
 * $
 */

#ifndef __RB_TREE_H
#define __RB_TREE_H

#include <inttypes.h>       // for uintptr_t

typedef struct rb_tree_node     rb_tree_node_t;
typedef struct rb_tree_func     rb_tree_func_t;
typedef struct rb_tree          rb_tree_t;

/**
 * Comparison functions used during tree lookups.
 */
struct rb_tree_func {
    /**
     * Compares two nodes.
     * The callback function is used when a new node is inserted to the tree.
     * @param   node1   The first node to compare
     * @param   node2   The second node to compare
     * @param   tree    A pointer to the tree structure
     * @retval  Negative integer if node1 is considered as smaller than node2
     * @retval  Positive integer if node1 is considered as greater than node2
     * @retval  0 if the two nodes are considered as equal
     */
    int (*cmp_nodes)(void const *__node1, void const *__node2, rb_tree_t const *__tree);
    /**
     * Compares a node with a value
     * The callback function is used when looking up a node by its value.
     * @param   node    The node to compare
     * @param   value   The value to compare with
     * @param   tree    A pointer to the tree structure
     * @retval  Negative integer if node is considered as smaller than value
     * @retval  Positive integer if node is considered as greater than value
     * @retval  0 if the node matches the value
     */
    int (*cmp_value)(void const *__node, uintptr_t __value, rb_tree_t const *__tree);
    /**
     * Compares a node with a range.
     * The callback function is used when locating a node within a value range
     * [start,end].
     * @param   node    The node to compare
     * @param   start   The first value in the range (inclusive)
     * @param   end     The last value in the range (inclusive)
     * @param   tree    A pointer to the tree structure
     * @retval  Negative integer if node is considered as smaller than start
     * @retval  Positive integer if node is considered as greater than end
     * @retval  0 if the node is within the range
     */
    int (*cmp_range)(void const *__node, uintptr_t __start, uintptr_t __end, rb_tree_t const *__tree);
};

struct rb_tree {
    rb_tree_node_t          *t_root;
    rb_tree_func_t const    *t_func;
    unsigned                t_offset;
};

/**
 * Node in a red-black tree.
 * The node consists of links to the parent (which also encodes the colour) and
 * the left and right children.
 * Note that the node should be embedded in a a structure defining an element
 * stored in a tree, and therefore does not provide any data itself. The
 * embedding element is obtained by subtracting the value stored in the tree's
 * offset field from the address of the node.
 */
struct rb_tree_node {
    uintptr_t       n_parent;
    rb_tree_node_t  *n_link[2];
};

#define RB_LEFT         0
#define RB_RIGHT        1

#define rb_tree_first(t)    (rb_tree_minmax((t), RB_LEFT))
#define rb_tree_last(t)     (rb_tree_minmax((t), RB_RIGHT))
#define rb_tree_next(t, n)  (rb_tree_iterate((t), (n), RB_RIGHT))
#define rb_tree_prev(t, n)  (rb_tree_iterate((t), (n), RB_LEFT))

void    rb_tree_init(rb_tree_t *tree, rb_tree_func_t const *func, unsigned offset);
void *  rb_tree_insert(rb_tree_t *tree, rb_tree_node_t *node);
void    rb_tree_remove(rb_tree_t *tree, rb_tree_node_t *node);
void *  rb_tree_minmax(const rb_tree_t *tree, int dir);
void *  rb_tree_iterate(const rb_tree_t *tree, const rb_tree_node_t *node, int dir);
void *  rb_tree_find_value(const rb_tree_t *tree, uintptr_t val);
void *  rb_tree_find_range(const rb_tree_t *tree, uintptr_t start, uintptr_t end);

#endif  /* __RB_TREE_H */

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.x_dev/trunk/lib/c/public/private/rb_tree.h $ $Rev: 873433 $")
#endif
