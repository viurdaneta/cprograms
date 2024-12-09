/* COP 3502C Assignment 5
This program is written by: Victor Urdaneta */
#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#define MAX_NAME_LENGTH 26 // 25 characters + 1 for '\0'
#define MAX_FINE 100


struct tree_node {
  char name[MAX_NAME_LENGTH];
  int fine;
  struct tree_node *left;
  struct tree_node *right;
};

// Helper function to create a new node
struct tree_node* create_node(const char *name, int fine) {
    struct tree_node *new_node = malloc(sizeof(struct tree_node));
    strncpy(new_node->name, name, MAX_NAME_LENGTH);
    new_node->fine = fine;
    new_node->left = new_node->right = NULL;
    return new_node;
}

struct tree_node* find(struct tree_node *root, char *name, int *depth) {
    *depth = 0; // Reset depth for each search
    while (root != NULL) {
        if (strcmp(name, root->name) == 0) {
            return root; // Node found
        } else if (strcmp(name, root->name) < 0) {
            root = root->left;
        } else {
            root = root->right;
        }
        (*depth)++; // Increase depth as we go deeper in the tree
    }
    return NULL; // Node not found
}



struct tree_node* add_tree_node(struct tree_node *root, char *name, int fine, int *depth) {
    if (root == NULL) {
        root = create_node(name, fine);
        printf("%s %d %d\n", name, fine, *depth);
        return root;
    }

    if (strcmp(name, root->name) < 0) {
        (*depth)++;
        root->left = add_tree_node(root->left, name, fine, depth);
    }
    else if (strcmp(name, root->name) > 0) {
        (*depth)++;
        root->right = add_tree_node(root->right, name, fine, depth);
    }
    else {
        // If the name already exists, update fine and print original depth without incrementing
        root->fine += fine;
        printf("%s %d %d\n", name, root->fine, *depth);
    }
    return root;
}




struct tree_node* add_owner(struct tree_node *root, const char *name, int fine) {
    int depth = 0;
    return add_tree_node(root, name, fine, &depth);
}


int isLeaf(struct tree_node *node) {
    return (node->left == NULL && node->right == NULL);
}

int hasOnlyLeftChild(struct tree_node *node) {
    return (node->left != NULL && node->right == NULL);
}

int hasOnlyRightChild(struct tree_node *node) {
    return (node->left == NULL && node->right != NULL);
}

// Finds the maximum node in a subtree (used for nodes with two children)
struct tree_node* find_max(struct tree_node *node) {
    while (node->right != NULL) {
        node = node->right;
    }
    return node;
}

// Helper function to find the parent of a given node
struct tree_node* parent(struct tree_node* root, struct tree_node* node) {
    if (root == NULL || root == node) return NULL;

    if ((root->left == node) || (root->right == node)) {
        return root;
    }

    if (strcmp(node->name, root->name) < 0) {
        return parent(root->left, node);
    } else {
        return parent(root->right, node);
    }
}

// Function to find the node by name
struct tree_node* findNode(struct tree_node *current_ptr, char *name) {
    if (current_ptr == NULL) return NULL;

    if (strcmp(current_ptr->name, name) == 0) {
        return current_ptr;
    } else if (strcmp(name, current_ptr->name) < 0) {
        return findNode(current_ptr->left, name);
    } else {
        return findNode(current_ptr->right, name);
    }
}

struct tree_node* delete_node(struct tree_node* root, const char *name) {
    if (root == NULL) return NULL;
    if (strcmp(name, root->name) < 0) {
        root->left = delete_node(root->left, name);
    } else if (strcmp(name, root->name) > 0) {
        root->right = delete_node(root->right, name);
    } else {
        if (root->left == NULL) {
            struct tree_node* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            struct tree_node* temp = root->left;
            free(root);
            return temp;
        } else {
            struct tree_node* temp = root->left;
            while (temp->right) temp = temp->right;
            strncpy(root->name, temp->name, MAX_NAME_LENGTH);
            root->fine = temp->fine;
            root->left = delete_node(root->left, temp->name);
        }
    }
    return root;
}




struct tree_node *deduct_owner(struct tree_node *root, const char *name, int fine) {
    int depth = 0;
    struct tree_node *node = root;
    while (node && strcmp(name, node->name) != 0) {
        depth++;
        node = (strcmp(name, node->name) < 0) ? node->left : node->right;
    }
    if (node == NULL) {
        printf("%s not found\n", name);
    } else {
        if (node->fine <= fine) {
            printf("%s removed\n", name);
            root = delete_node(root, name);
        } else {
            node->fine -= fine;
            printf("%s %d %d\n", name, node->fine, depth);
        }
    }
    return root;
}


int count(struct tree_node *root) {
  if (root==NULL)
  {
    return 0;
  }
  return 1 + count(root->left) + count(root->right);
}

double calculate_total_fine(struct tree_node *root) {
    if (root == NULL) {
        return 0.0;
    }
    return root->fine + calculate_total_fine(root->left) + calculate_total_fine(root->right);
}

void average(struct tree_node *root) {
    if (root == NULL) {
        printf("0.00\n");  // Ensure it prints 0.00 if the tree is empty
        return;
    }

    double sum = calculate_total_fine(root);
    int owner_count = count(root);

    if (owner_count > 0) {
        double avg = sum / owner_count;
        printf("%.2f\n", avg);  // Print with two decimal places
    } else {
        printf("0.00\n");
    }
}


int calculate_height(struct tree_node *node) {
    if (node == NULL) return -1;
    int left_height = calculate_height(node->left);
    int right_height = calculate_height(node->right);
    return 1 + (left_height > right_height ? left_height : right_height);
}

void height_balance(struct tree_node *root) {
    if (root == NULL) {
        printf("left height = 0 right height = 0 balanced\n");
        return;
    }
    int left_height = calculate_height(root->left);
    int right_height = calculate_height(root->right);
    printf("left height = %d right height = %d %s\n", left_height, right_height,
           (left_height == right_height) ? "balanced" : "not balanced");
}

void calculate_total_fine_below(struct tree_node *node, const char *name, double *total) {
    if (node == NULL) {
        return;
    }
    if (strcmp(node->name, name) <= 0) {
        *total += node->fine;  // Add this node's fine to the total
        calculate_total_fine_below(node->right, name, total);  // Traverse right subtree
    }
    calculate_total_fine_below(node->left, name, total);  // Always traverse left subtree
}

void calc_below(struct tree_node *root, const char *name) {
    double total = 0.0;
    calculate_total_fine_below(root, name, &total);

    if (total == (int)total) {
        printf("%d\n", (int)total);  // Print as integer if it's a whole number
    } else {
        printf("%.2f\n", total);  // Print with two decimal places otherwise
    }
}


void free_tree(struct tree_node *root) {
    if (root == NULL) return;
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

int main(void) {
    int n;
    scanf("%d", &n);
    struct tree_node *root = NULL;
    char command[10];
    char name[MAX_NAME_LENGTH];
    int fine;

    for (int i = 0; i < n; i++) {
        scanf("%s", command);
        
        if (strcmp(command, "add") == 0) {
            scanf("%s %d", name, &fine);  // Capture name and fine
            root = add_owner(root, name, fine);
        }
        
        if (strcmp(command, "deduct") == 0) {
            scanf("%s %d", name, &fine);
            root = deduct_owner(root, name, fine);
        }
        
        if (strcmp(command, "search") == 0) {
            int depth = 0;
            scanf("%s", name);
            struct tree_node *node = find(root, name, &depth);
            if (node == NULL) {
                printf("%s not found\n", name);
            } else {
                printf("%s %d %d\n", name, node->fine, depth);
            }
        }
        
        if (strcmp(command, "average") == 0) {
            average(root);
        }
        
        if (strcmp(command, "height_balance") == 0) {
            height_balance(root);
        }
        
        if (strcmp(command, "calc_below") == 0) {
            scanf("%s", name);
            calc_below(root, name);
        }
        
    }
    free_tree(root);

    return 0;
}
