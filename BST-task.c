#include <stdio.h>
#include <stdlib.h>

// Structure for a node
struct Node {
    int data;
    struct Node *left, *right;
};

// Function to create a new node
struct Node* createNode(int value) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = value;
    newNode->left = newNode->right = NULL;
    return newNode;
}

// Function to insert a new value into BST
struct Node* insert(struct Node* root, int value) {
    if (root == NULL)
        return createNode(value);

    if (value < root->data)
        root->left = insert(root->left, value);
    else if (value > root->data)
        root->right = insert(root->right, value);

    return root;
}

// Function to search for a value in BST
int search(struct Node* root, int key) {
    if (root == NULL)
        return 0; // Not found
    if (root->data == key)
        return 1; // Found
    else if (key < root->data)
        return search(root->left, key);
    else
        return search(root->right, key);
}

// Function for in-order traversal (Left → Root → Right)
void inorder(struct Node* root) {
    if (root != NULL) {
        inorder(root->left);
        printf("%d ", root->data);
        inorder(root->right);
    }
}

int main() {
    struct Node* root = NULL;
    int n, value, key;

    printf("Enter number of nodes to insert: ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        printf("Enter value to insert: ");
        scanf("%d", &value);
        root = insert(root, value);
        printf("In-order after inserting %d: ", value);
        inorder(root);
        printf("\n");
    }

    printf("\nEnter value to search: ");
    scanf("%d", &key);

    if (search(root, key))
        printf("Value %d found in BST.\n", key);
    else
        printf("Value %d not found in BST.\n", key);

    return 0;
}
