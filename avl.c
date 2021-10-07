#include <stdio.h>
#include <stdlib.h>

typedef struct cell {
    struct cell *left;
    struct cell *right;
    int data;
    int height;
} node, *nodePtr;

typedef nodePtr BSTree;

void display()
{
    printf("%-5s | %-5s\n", "data", "height");
}

void preorder(BSTree T)
{
    if (T != NULL) {
        printf("%-5d | %-5d\n", T->data, T->height);
        preorder(T->left);
        preorder(T->right);
    }
}

int height(nodePtr node)
{
    if (node == NULL)
        return 0;
    return node->height;
}

int max(int a, int b)
{
    return a > b ? a : b;
}

int getBalance(nodePtr node)
{
    if (node == NULL)
        return 0;
    return height(node->left) - height(node->right);
}

void leftRotation(BSTree *T)
{

    /**
     *  Initially...
     *      (*T) - 10
     *            /  \ 
     *                20 - right_node
     * 
     */
    BSTree right_node = (*T)->right;
    (*T)->right = right_node->left;
    right_node->left = *T;


    // need to update height of the rotated nodes
    // why? => node position is changed
    (*T)->height = max(height((*T)->left), height((*T)->right)) + 1;
    right_node->height = max(height(right_node->left), height(right_node->right)) + 1;

    // repoint the root
    *T = right_node;

}

void rightRotation(BSTree *T)
{
    /**
     *  Initially...
     *               (*T) - 30
     *                     /  \ 
     *        left_node - 20
     * 
     */
    BSTree left_node = (*T)->left;
    (*T)->left = left_node->right;
    left_node->right = *T;

    // need to update height of the rotated nodes
    // why? => node position is changed
    (*T)->height = max(height((*T)->left), height((*T)->right)) + 1;
    left_node->height = max(height(left_node->left), height(left_node->right)) + 1;
    
    // repoint the root
    *T = left_node;

}

void insert(BSTree *T, int data)
{
    if (*T == NULL) {
        *T = (BSTree) calloc (1, sizeof(node));
        if (*T != NULL) {
            (*T)->data = data;
            (*T)->height = 1;
        }
        return;
    }

    // data already exists
    if ((*T)->data == data) return;

    // go left if data is less than node->data
    // else, go right
    if ((*T)->data < data)
        insert(&(*T)->right, data);
    else   
        insert(&(*T)->left, data);

    /** 
     *      after insert, we need to update new height
     */

    // update the height of this node
    // by getting the longest path + 1 (we are 1 above the longest path)
    int left_node_height = height((*T)->left);
    int right_node_height = height((*T)->right);
    (*T)->height = max(left_node_height, right_node_height) + 1;


    // get balance factor of this node, 
    // bf = left height - right height
    // bf => [-1, 0, 1] => balanced, else, unbalanced
    int bf = getBalance(*T);

    // Left-Left Scenario, if bf = 2
    // Left first, and data is found on the left
    if (bf > 1 && data < (*T)->left->data) {
        rightRotation(T);
        return;
    }

    // Left - Right
    // Left first, and then data is found on the right
    if (bf > 1 && data > (*T)->left->data) {
        leftRotation(&(*T)->left);
        rightRotation(T);
        return;
    }

    // Right - Left
    // Right first, and then data is found on the left
    if (bf < - 1 && data < (*T)->right->data) {
        rightRotation(&(*T)->right);
        leftRotation(T);
        return;
    }
    
    // Right-Right Scenario, if bf = -2
    // Right first, and then data is found on the right
    if (bf < -1 && data > (*T)->right->data) {
        leftRotation(T);
        return;
    }

}

void populate(BSTree *T, int arr[], int size)
{
    int i;
    for (i = 0; i < size; i++) {
        insert(T, arr[i]);
    }
}

int getPredecessor(BSTree *trav)
{
    if ((*trav)->right == NULL) {
        BSTree temp = *trav;
        *trav = temp->left;
        int retval = temp->data;
        free(temp);
        return retval;
    }

    getPredecessor(&(*trav)->right);
}

int getSuccessor(BSTree *trav)
{
    if ((*trav)->left == NULL) {
        BSTree temp = *trav;
        *trav = temp->right;
        int retval = temp->data;
        free(temp);
        return retval;
    }

    getSuccessor(&(*trav)->left);
}

void delete(BSTree *T, int data)
{
    if (*T == NULL)
        return;
    
    if ((*T)->data == data) {
        BSTree temp = *T;
        if (temp->left == NULL) {
            *T = temp->right;
            free(temp);
        } else if (temp->right == NULL) {
            *T = temp->left;
            free(temp);
        } else {
            temp->data = getPredecessor(&(temp)->left);
        }
        return;
    }

    (*T)->data < data ? delete(&(*T)->right, data) : delete(&(*T)->left, data);

    int left_node_height = height((*T)->left);
    int right_node_height = height((*T)->right);
    (*T)->height = max(left_node_height, right_node_height) + 1;

    int bf = getBalance(*T);

    // left and left
    if (bf > 1 && getBalance((*T)->left) >= 0) {
        rightRotation(T);
        return;
    }

    // right and right
    if (bf < -1 && getBalance((*T)->right) <= 0) {
        leftRotation(T);
        return;
    }

    // left and right
    if (bf > 1 && getBalance((*T)->left) < 0) {
        leftRotation(&(*T)->left);
        rightRotation(T);
        return;
    }
    
    // right and left
    if (bf < - 1 && getBalance((*T)->right > 0)) {
        rightRotation(&(*T)->right);
        leftRotation(T);
        return;
    }

}


int main ()
{
    BSTree T;
    T = NULL;


    // int arr[] = {30, 20, 10};
    // int arr[] = {10, 20, 30};
    // int arr[] = {30, 10, 20};
    // int arr[] = {10, 30, 20};
    
    // int arr[] = {50, 60, 70, 80, 10, 20, 30, 40, 45};
    /**
     *
     *                      60
     *                     /   \
     *                    40    70
     *                  /   \     \
     *                20     50    80
     *               /  \   /
     *              10  30  45
     * 
     * expected output:
     *          60 40 20 10 30 50 45 70 80
     *
     */


    int arr[] = {50, 45, 55, 43, 47, 53, 60, 40, 52, 54};
    /**
     *
     *                         50
     *                     /        \
     *                    45         55
     *                  /   \      /   \
     *                43     47   53   60
     *               /           /  \     
     *              40         52    54 
     * 
     * expected output:
     *          50 45 43 40 47 55 53 52 54 60
     *
     */


    populate(&T, arr, 10);
    display();
    preorder(T);

    delete(&T, 47);
    /**
     *
     *                         50
     *                     /        \
     *                    43         55
     *                  /   \      /   \
     *                40    45    53   60
     *                          /  \     
     *                        52    54 
     * 
     * expected output:
     *          50 43 40 45 55 53 52 54 60
     *
     */
    display();
    preorder(T);


    return 0;
}