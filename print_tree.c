#include "minishell.h"

// Function to get the height of the tree
int get_height(t_tree *root)
{
    if (root == NULL)
        return 0;
    
    int left_height = get_height(root->left);
    int right_height = get_height(root->right);
    
    return (left_height > right_height) ? left_height + 1 : right_height + 1;
}

// Function to print the node data
void print_node_data(t_tree *node)
{
    if (node == NULL)
        return;
    
    printf("Type: %d", node->type);
    
    if (node->cmd)
        printf(" Cmd: %s", node->cmd);
    
    if (node->args && node->args[0])
    {
        printf(" Args: ");
        for (int i = 0; node->args[i] != NULL; i++)
        {
            printf("%s", node->args[i]);
            if (node->args[i + 1] != NULL)
                printf(", ");
        }
    }
}

// Function to print spaces
void print_spaces(int count)
{
    for (int i = 0; i < count; i++)
        printf(" ");
}

// Helper function to print the tree at a given level
void print_level(t_tree *root, int level, int space_width)
{
    if (root == NULL)
    {
        // Print empty node for better visualization
        print_spaces(space_width);
        printf("NULL");
        return;
    }
    
    if (level == 1)
    {
        // Print the current node with indentation
        print_node_data(root);
    }
    else if (level > 1)
    {
        // Recursively print left and right subtrees
        print_level(root->left, level - 1, space_width);
        printf("\n");
        print_spaces(space_width / 2);
        printf("|");
        printf("\n");
        
        print_level(root->right, level - 1, space_width);
    }
}

// Function to print the tree in a visual format
void print_tree(t_tree *root)
{
    if (root == NULL)
    {
        printf("Empty tree\n");
        return;
    }
    
    int height = get_height(root);
    int space_width = 10;  // Adjust based on your node content
    
    printf("Tree Structure:\n");
    printf("==============\n\n");
    
    for (int i = 1; i <= height; i++)
    {
        print_level(root, i, space_width);
        printf("\n\n");  // Add extra lines between levels
    }
}

// Alternative function to print tree in a more compact horizontal format
void print_tree_horizontal(t_tree *root, int level, char *prefix, int is_left)
{
    if (root == NULL)
    {
        printf("%sNULL\n", prefix);
        return;
    }
    
    printf("%s", prefix);
    
    if (is_left)
        printf("├── ");
    else
        printf("└── ");
    
    // Print the current node data
    print_node_data(root);
    printf("\n");
    
    // Prepare the prefix for children
    char *new_prefix = malloc(strlen(prefix) + 10);
    if (new_prefix == NULL)
        return;
    
    strcpy(new_prefix, prefix);
    if (is_left)
        strcat(new_prefix, "│   ");
    else
        strcat(new_prefix, "    ");
    
    // Print the children recursively
    print_tree_horizontal(root->left, level + 1, new_prefix, 1);
    print_tree_horizontal(root->right, level + 1, new_prefix, 0);
    
    free(new_prefix);
}

// Wrapper function for horizontal tree printing
void print_tree_h(t_tree *root)
{
    if (root == NULL)
    {
        printf("Empty tree\n");
        return;
    }
    
    printf("Tree Structure (Horizontal):\n");
    printf("==========================\n\n");
    
    print_tree_horizontal(root, 0, "", 0);
}