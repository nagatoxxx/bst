#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "bst.h"

Result BST_add(Node** root, uint32_t key, uint32_t value)
{
    Node* new = (Node*)malloc(sizeof(Node));

    new->key   = key;
    new->value = value;
    new->left  = NULL;
    new->right = NULL;
    new->next  = NULL;

    if (*root == NULL) {
        *root = new;
        return (Result){.retcode = OK};
    }

    int temp = -1;

    Node* parent  = NULL;
    Node* current = *root;

    int right = 1; // является ли узел самым правым
    while (current != NULL) {
        parent = current;
        if (new->key < current->key) {
            right   = 0;
            current = current->left;
        }
        else
            current = current->right;
    }
    new->parent = parent;
    if (new->key < parent->key) {
        parent->left = new;
        new->next    = parent;
    }
    else if (new->key > parent->key) {
        parent->right = new;
        if (!right)
            new->next = *root;
        else
            new->next = NULL;
    }
    else {
        temp          = parent->value;
        parent->value = new->value;
        free(new);
    }

    Result ret;
    if (temp == -1)
        ret.retcode = OK;
    else {
        ret.retcode = OVERWRITTEN;
        ret.value.v = (uint32_t)temp;
    }

    return ret;
}

// перемещает вершину v на место u
static void shift(Node** root, Node* u, Node* v)
{
    if (u->parent == NULL)
        *root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;

    if (v != NULL) v->parent = u->parent;
}

// возвращает указатель на самый правый узел дерева
// static Node* maximum(Node** root)
// {
//     Node* x = *root;
//     while (x->right != NULL) {
//         x = x->right;
//     }
//
//     return x;
// }

// возвращает указатель на самый левый узел
static Node* minimum(Node* root)
{
    Node* x = root;
    while (x->left != NULL) {
        x = x->left;
    }

    return x;
}

static Node* successor(Node* x)
{
    if (x->right != NULL) return minimum(x->right);
    Node* y = x->parent;
    while (y != NULL && x == y->right) {
        x = y;
        y = y->parent;
    }

    return y;
}

// рекурсивный обход, не использующий прошивку
// нужно для обновления next указателей
static void updateNext(Node* n, Node* oldRoot, Node* newRoot)
{
    if (n != NULL) {
        updateNext(n->left, oldRoot, newRoot);
        if (n->right == NULL && n->next == oldRoot) n->next = newRoot;
        updateNext(n->right, oldRoot, newRoot);
    }
}

Result BST_remove(Node** root, uint32_t key)
{
    if (*root == NULL) return (Result){.retcode = NULLPTR};

    Result res = BST_search(root, key);

    if (res.retcode == NOT_FOUND) return (Result){.retcode = NOT_FOUND};

    Node* d = res.value.n;

    Node* oldRoot = *root;
    Node* newRoot = NULL;

    if (d->left == NULL) {
        newRoot = d->right;
        shift(root, d, d->right);
    }

    else if (d->right == NULL) {
        newRoot = d->right;
        shift(root, d, d->left);
    }
    else {
        Node* e = successor(d);
        if (e->parent != d) {
            shift(root, e, e->right);
            e->right         = d->right;
            e->right->parent = e;
        }
        shift(root, d, e);
        e->left         = d->left;
        e->left->parent = e;
        newRoot         = e;
    }

    updateNext(*root, oldRoot, newRoot);

    free(d);

    return (Result){.retcode = OK};
}

// находит максимальный ключ в дереве
static uint32_t maxKey(Node** root)
{
    Node*    cur = *root;
    uint32_t max = 0;
    while (cur != NULL) {
        max = cur->key;
        cur = cur->right;
    }
    return max;
}

// возвращает число десятичных разрядов в числе
static uint32_t decimalPlaces(uint32_t n)
{
    if (n == 0) return 1;
    return (uint32_t)log10(n) + 1;
}

Result BST_traversal(Node** root, uint32_t n)
{
    if (*root == NULL) return (Result){.retcode = NULLPTR};

    Node* cur = *root;

    uint8_t* list = malloc(maxKey(root) + 1); // не совсем эффективно по памяти
                                              // можно сделать через битовые поля etc
    memset(list, 0, maxKey(root) + 1);

    while (cur != NULL) {
        if (list[cur->key] == 0 && decimalPlaces(cur->key) == n) {
            printf("%d: %d \n", cur->key, cur->value);
        }
        if (cur->left != NULL && list[cur->key] == 0) {
            list[cur->key] = 1;
            cur            = cur->left;
        }
        else if (cur->right == NULL && list[cur->key] == 0) {
            list[cur->key] = 1;
            cur            = cur->next;
        }
        else {
            list[cur->key] = 1;
            cur            = cur->right;
        }
    }

    free(list);

    return (Result){.retcode = OK};
}

Result BST_search(Node** root, uint32_t key)
{
    if (*root == NULL) return (Result){.retcode = NULLPTR};

    Node* cur = *root;
    while (cur != NULL && key != cur->key) {
        if (key < cur->key)
            cur = cur->left;
        else
            cur = cur->right;
    }

    Result res;
    if (cur == NULL) {
        res.retcode = NOT_FOUND;
    }
    else {
        res.retcode = OK;
        res.value.n = cur;
    }

    return res;
}

void BST_free(Node** root)
{
    if (*root == NULL) return;

    BST_free(&(*root)->left);
    BST_free(&(*root)->right);

    free(*root);
}

Result BST_specSearch(Node** root, uint32_t key)
{
    if (*root == NULL) return (Result){.retcode = NULLPTR};

    Node* cur = *root;

    uint8_t* list = malloc(maxKey(root) + 1);
    memset(list, 0, maxKey(root) + 1);

    Node*    max    = NULL;
    uint32_t maxDif = 0;

    while (cur != NULL) {
        if (list[cur->key] == 0) {
            uint32_t dif = abs((int32_t)key - (int32_t)cur->key);
            if (dif > maxDif) {
                maxDif = dif;
                max    = cur;
            }
        }
        if (cur->left != NULL && list[cur->key] == 0) {
            list[cur->key] = 1;
            cur            = cur->left;
        }
        else if (cur->right == NULL && list[cur->key] == 0) {
            list[cur->key] = 1;
            cur            = cur->next;
        }
        else {
            list[cur->key] = 1;
            cur            = cur->right;
        }
    }

    free(list);

    return (Result){.retcode = OK, .value.n = max};
}

static void print(Node* n, uint32_t nspaces)
{
    const uint32_t COUNT = 6;

    if (n == NULL) return;

    nspaces += COUNT;

    print(n->right, nspaces);

    printf("\n");
    for (uint32_t i = COUNT; i < nspaces; ++i) {
        printf(" ");
    }
    printf("%d\n", n->key);

    print(n->left, nspaces);
}

Result BST_print(Node** root)
{
    if (*root == NULL) return (Result){.retcode = NULLPTR};

    print(*root, 0);

    return (Result){.retcode = OK};
}

static void generateEdges(Node* root, FILE* fp)
{
    if (root == NULL) return;

    if (root->left != NULL) {
        fprintf(fp, "%d -> %d;\n", root->key, root->left->key);
        generateEdges(root->left, fp);
    }
    if (root->right != NULL) {
        fprintf(fp, "%d -> %d;\n", root->key, root->right->key);
        generateEdges(root->right, fp);
    }
}

static void generateNodes(Node* root, FILE* fp)
{
    if (root == NULL) return;


    if (root->left != NULL) {
        generateNodes(root->left, fp);
    }

    fprintf(fp, "%u [label=\"{<key>%u|<value>%u}\"];\n", root->key, root->key, root->value);

    if (root->right != NULL) {
        generateNodes(root->right, fp);
    }
}

Result BST_graphPrint(Node** root)
{
    if (*root == NULL) return (Result){.retcode = NULLPTR};

    FILE* fp;
    fp = fopen("bst_tree.dot", "w");

    if (fp == NULL) {
        return (Result){.retcode = FILE_ERROR};
    }

    fprintf(fp, "digraph BST {\n");
    fprintf(fp, "node [fontname=\"Arial\"];\n");
    fprintf(fp, "node [shape=\"record\"];\n");

    generateNodes(*root, fp);
    generateEdges(*root, fp);

    fprintf(fp, "}\n");
    fclose(fp);

    system("dot -Tpng bst_tree.dot -o bst_tree.png");

    return (Result){.retcode = OK};
}

Result BST_read(Node** root, const char* filename)
{
    FILE* file = fopen(filename, "r");

    if (file == NULL) return (Result){.retcode = FILE_ERROR};

    uint32_t key, value;
    while (fscanf(file, "%u", &key) != EOF && fscanf(file, "%u", &value) != EOF) {
        BST_add(root, key, value);
    }

    fclose(file);

    return (Result){.retcode = OK};
}
