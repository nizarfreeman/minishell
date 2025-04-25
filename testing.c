#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct tree_node
{
	int data;
	struct tree_node *left;
	struct tree_node *right;	
} t_node;

t_node *add_new_node(int data)
{
	t_node *new_node = malloc(sizeof(t_node));
	memset(new_node, 0, sizeof(t_node));
	new_node -> data = data;
	return (new_node);
}

int main(int argc, char const *argv[])
{
	t_node *head = add_new_node(1);
	head->right = add_new_node(2);
	head->left = add_new_node(3);

	printf("%d %d %d\n", head->data, head->right->data, head->left->data);
	return 0;
}