#include <stdio.h>
#include <stdlib.h>

/* Node structure for BST */
struct Node {
    int data;
    struct Node *left;
    struct Node *right;
};

/* Create and initialize a new node */
struct Node* createNode(int value) {
    struct Node *n = (struct Node*)malloc(sizeof(struct Node));
    if (n == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    n->data = value;
    n->left = n->right = NULL;
    return n;
}

/* Insert value into BST (recursive). Duplicates are ignored. */
struct Node* insert(struct Node* root, int value) {
    if (root == NULL) {
        return createNode(value);
    }
    if (value < root->data) {
        root->left = insert(root->left, value);
    } else if (value > root->data) {
        root->right = insert(root->right, value);
    }
    return root;
}

/* Find minimum value in BST (left-most node) */
int findMin(struct Node* root) {
    if (root == NULL) {
        fprintf(stderr, "Tree is empty — no minimum.\n");
        exit(EXIT_FAILURE);
    }
    struct Node* cur = root;
    while (cur->left != NULL) cur = cur->left;
    return cur->data;
}

/* Find maximum value in BST (right-most node) */
int findMax(struct Node* root) {
    if (root == NULL) {
        fprintf(stderr, "Tree is empty — no maximum.\n");
        exit(EXIT_FAILURE);
    }
    struct Node* cur = root;
    while (cur->right != NULL) cur = cur->right;
    return cur->data;
}

/* Height of tree measured as number of nodes on longest root->leaf path.
   height(NULL) = 0; height(single node) = 1. */
int height(struct Node* root) {
    if (root == NULL) return 0;
    int lh = height(root->left);
    int rh = height(root->right);
    return (lh > rh ? lh : rh) + 1;
}

/* Optional helper: in-order traversal prints values in sorted order */
void inorder(struct Node* root) {
    if (root == NULL) return;
    inorder(root->left);
    printf("%d ", root->data);
    inorder(root->right);
}

/* Free all nodes (post-order) */
void freeTree(struct Node* root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int main(void) {
    const char *filename = "input.txt"; /* change if you want another name */
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "Could not open file '%s' for reading.\n", filename);
        return EXIT_FAILURE;
    }

    struct Node* root = NULL;
    int value;
    int count = 0;

    /* Read integers until EOF. Accepts spaces/newlines/tabs as separators. */
    while (fscanf(fp, "%d", &value) == 1) {
        root = insert(root, value);
        count++;
    }

    fclose(fp);

    if (count == 0) {
        printf("No integers found in file '%s'. Exiting.\n", filename);
        return 0;
    }

    printf("Values read and inserted into BST (%d values):\n", count);
    printf("In-order (sorted): ");
    inorder(root);
    printf("\n");

    int mn = findMin(root);
    int mx = findMax(root);
    int h = height(root);

    printf("Minimum value = %d\n", mn);
    printf("Maximum value = %d\n", mx);
    printf("Height of tree (nodes on longest path) = %d\n", h);

    freeTree(root);
    return 0;
}
