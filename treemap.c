#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap * new = (TreeMap *)malloc(sizeof(TreeMap));
    if(new == NULL) return NULL;
    new->root = NULL;
    new->current = NULL;
    new->lower_than = lower_than;
    return new;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    if(tree->root == NULL)
    {
        tree->root = createTreeNode(key, value);
        tree->current = tree->root;
    }
    else
    {
        TreeNode * parent = NULL; 
        TreeNode * aux = tree->root;
        while(aux != NULL)
        {
            if(is_equal(tree, key, aux->pair->key) == 1)
            {
                return;
            }
            parent = aux;
            if(tree->lower_than(key, aux->pair->key) == 1)
            {
                aux = aux->left;
            }
            else
            {
                aux = aux->right;
            }
        }
        aux = createTreeNode(key, value);
        aux->parent = parent;
        if(tree->lower_than(key, parent->pair->key) == 1)
        {
            parent->left = aux;
        }
        else
        {
            parent->right = aux;
        }
        tree->current = aux;
    }
}

TreeNode * minimum(TreeNode * x){
    while(x->left != NULL)
    {
        x = x->left;
    }
    return x;
}

void removeNode(TreeMap *tree, TreeNode* node) {
    if (node == NULL) {
        return;
    }

    // Caso 1: Nodo sin hijos
    if (node->left == NULL && node->right == NULL) {
        if (node->parent != NULL) {
            if (node->parent->left == node) {
                node->parent->left = NULL;
            } else {
                node->parent->right = NULL;
            }
        } else {
            tree->root = NULL;
        }
        free(node);
    }

    // Caso 2: Nodo con un hijo
    else if (node->left == NULL || node->right == NULL) {
        TreeNode *child = (node->left != NULL) ? node->left : node->right;
        if (node->parent != NULL) {
            if (node->parent->left == node) {
                node->parent->left = child;
            } else {
                node->parent->right = child;
            }
            if (child != NULL) {
                child->parent = node->parent;
            }
        } else {
            tree->root = child;
            if (child != NULL) {
                child->parent = NULL;
            }
        }
        free(node);
    }

    // Caso 3: Nodo con dos hijos
    else {
        TreeNode *minRight = minimum(node->right);
        node->pair = minRight->pair;
        removeNode(tree, minRight);
    }
}


void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);
}




Pair * searchTreeMap(TreeMap * tree, void* key){
    TreeNode * aux = tree->root;
    while(aux != NULL)
    {
        if(is_equal(tree, key, aux->pair->key) == 1)
        {
            tree->current = aux;
            return aux->pair;
        }
        else if(tree->lower_than(key, aux->pair->key) == 1)
        {
            aux = aux->left;
        }
        else
        {
            aux = aux->right;
        }
    }
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key)
{
    
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    if (tree == NULL || tree->root == NULL)
        return NULL;

    TreeNode *current = tree->root;
    while (current->left != NULL) {
        current = current->left;
    }
    return current->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
    if (tree == NULL || tree->current == NULL) // Verificar si tree o tree->current es NULL
        return NULL;

    TreeNode *current = tree->current;

    if (current->right != NULL) { // Si hay un subárbol derecho, buscar el nodo más a la izquierda
        current = current->right;
        while (current->left != NULL) {
            current = current->left;
        }
        tree->current = current;
        return current->pair;
    } else { // Si no hay subárbol derecho, retroceder hasta el primer ancestro que sea mayor
        TreeNode *parent = current->parent;
        while (parent != NULL && current == parent->right) {
            current = parent;
            parent = parent->parent;
        }
        tree->current = parent; // Actualizar tree->current con el próximo nodo mayor
        return (parent != NULL) ? parent->pair : NULL; // Retornar el par del próximo nodo mayor o NULL si no hay más nodos
    }
}


