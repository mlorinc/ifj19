#include <string.h>
#include "parser_ast.h"
#include "stack.h"
#include "iterator.h"


ast_t ast_node_init_empty()
{
    ast_t ast = malloc(sizeof(struct ast));
    if (ast != NULL)
    {
        ast->nodes = queue_init();
    }
    return ast;
}

ast_t ast_node_init(ast_node_type_t type, void *data)
{
    ast_t tree = ast_node_init_empty();
    if (tree != NULL)
    {
        tree->node_type = type;
        tree->data = data;
    }
    return tree;
}

bool ast_delete(ast_t root) {
    if (root == NULL)
	{
		return;
	}

	stack_t nodes = stack_init();

    if (nodes == NULL) {
        return false;
    }

    stack_push(nodes, root);
	
	while (!stack_empty(nodes))
	{
		ast_t node = stack_top(nodes);
        stack_pop(nodes);

		if (node == NULL)
		{
			continue;
		};

        iterator_t end = queue_end(node->nodes);
        for (iterator_t ite = queue_begin(node->nodes); !iterator_equal(ite, end); ite = iterator_next(ite))
        {
            stack_push(nodes, deque_iterator_value(ite));
        }
        
        queue_destroy(node->nodes);

        if (node->data != NULL) {
            free(node->data);
        }
        
		free(node);
	}

    return true;
}

bool ast_add_node(ast_t parent, ast_t node) {
    return queue_push(parent->nodes, node);
}

void *allocate_value_to_heap(void *value, size_t size) {
    void *new = malloc(size);

    if(new == NULL) {
        return NULL;
    }

    // copy all bits from value
    memmove(new, value, size);
    return new;
}
