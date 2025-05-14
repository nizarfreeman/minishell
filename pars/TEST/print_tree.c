#include "minishell.h"

// Function to determine the maximum depth of the tree
int get_max_depth(t_tree *root) {
    if (!root)
        return 0;
    
    int left_depth = get_max_depth(root->left);
    int right_depth = get_max_depth(root->right);
    
    return (left_depth > right_depth ? left_depth : right_depth) + 1;
}

// Helper function to get string representation of node data
char *get_node_str(t_tree *node) {
    static char buffer[256];
    
    if (!node) {
        snprintf(buffer, sizeof(buffer), "[NULL]");
        return buffer;
    }
    
    buffer[0] = '\0';
    
    // Add type information
    snprintf(buffer, sizeof(buffer), "[Type: %d", node->type);
    
    // Add command if available
    if (node->cmd)
        snprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer), 
                " Cmd: %s", node->cmd);
    
    // Add arguments if available
    if (node->args && node->args[0]) {
        strcat(buffer, " Args:");
        for (int i = 0; node->args[i]; i++) {
            snprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer), 
                    " %s", node->args[i]);
            if (node->args[i + 1])
                strcat(buffer, ",");
        }
    }
    
    strcat(buffer, "]");
    return buffer;
}

// Function to print connecting lines between parent and child nodes
void print_branches(int spaces, int width, int start_pos, int has_right_sibling) {
    print_spaces(spaces);
    
    if (start_pos == 0) { // Left branch
        printf("│");
        for (int i = 1; i < width / 2; i++)
            printf("─");
        printf("┌");
        for (int i = width / 2 + 1; i < width; i++)
            printf("─");
    } else { // Right branch
        for (int i = 0; i < width / 2; i++)
            printf("─");
        printf("┐");
        for (int i = width / 2 + 1; i < width; i++)
            printf("─");
        if (has_right_sibling)
            printf("│");
    }
    printf("\n");
}

// Function to print spaces
void print_spaces(int count) {
    for (int i = 0; i < count; i++)
        printf(" ");
}

// Main tree visualization function using a level-order traversal approach
void print_clean_tree(t_tree *root) {
    if (!root) {
        printf("Empty tree\n");
        return;
    }
    
    printf("\n===== SYNTAX TREE =====\n\n");
    
    // Prepare queue for level-order traversal
    t_tree *queue[1000]; // Adjust size as needed
    int level_markers[1000]; // Track which nodes are on which level
    int front = 0, rear = 0;
    
    // Enqueue root
    queue[rear] = root;
    level_markers[rear] = 0;
    rear++;
    
    int current_level = 0;
    int node_width = 20; // Adjust based on your typical node content
    int total_width = node_width * (1 << get_max_depth(root));
    
    while (front < rear) {
        // Dequeue a node
        t_tree *node = queue[front];
        int level = level_markers[front];
        front++;
        
        // If we're at a new level, print a newline
        if (level > current_level) {
            printf("\n\n");
            current_level = level;
        }
        
        // Calculate position for this node at this level
        int level_width = total_width / (1 << level);
        int position = front - (1 << level);
        int indent = position * level_width;
        
        // Print the node
        print_spaces(indent);
        printf("%s", get_node_str(node));
        
        // Enqueue children if they exist
        if (node->left) {
            queue[rear] = node->left;
            level_markers[rear] = level + 1;
            rear++;
            
            // Print branch to left child
            print_branches(indent, level_width / 2, 0, node->right != NULL);
        }
        
        if (node->right) {
            queue[rear] = node->right;
            level_markers[rear] = level + 1;
            rear++;
            
            // Print branch to right child
            print_branches(indent + level_width / 2, level_width / 2, 1, 0);
        }
    }
    
    printf("\n===== END OF TREE =====\n");
}

// Alternative simpler tree display function that works well for command trees
void print_simple_tree(t_tree *root, int depth) {
    if (!root)
        return;
    
    // Print indentation
    for (int i = 0; i < depth; i++)
        printf("    ");
    
    // Print node information
    printf("%s\n", get_node_str(root));
    
    // Print connection lines to children
    if (root->left || root->right) {
        for (int i = 0; i < depth; i++)
            printf("    ");
        printf("│\n");
    }
    
    // Print left subtree
    if (root->left) {
        for (int i = 0; i < depth; i++)
            printf("    ");
        printf("├───");
        print_simple_tree(root->left, depth + 1);
    }
    
    // Print right subtree
    if (root->right) {
        for (int i = 0; i < depth; i++)
            printf("    ");
        printf("└───");
        print_simple_tree(root->right, depth + 1);
    }
}

// Wrapper function
void print_ast(t_tree *root) {
    printf("\n===== ABSTRACT SYNTAX TREE =====\n\n");
    print_simple_tree(root, 0);
    printf("\n================================\n");
}