#ifndef BST_H
#define BST_H

// подразумевается, что узлы выделены динамически

#include <stdint.h>
#include <stdlib.h>

struct Node;

typedef struct Node Node;

struct Node
{
    uint32_t key;
    uint32_t value;

    Node* left;
    Node* right;

    Node* next; // следующий узел при обходе

    Node* parent;
};

// код ошибки/возвращаемого значение
enum RetCode
{
    OK = 0,      // без ошибок
    NULLPTR,     // указатель на root не валиден
    OVERWRITTEN, // при вставке значение элемента перезаписано
    FILE_ERROR,  // нет такого файла
    NOT_FOUND,   // нет такого ключа
};

// чтобы можно было удобно выводить сообщения об ошибках
// static const char* const RetCodeStr[] = {"OK", "OVERWRITTEN", "NULLPTR", "NO_SUCH_FILE"};

typedef struct Result
{
    int retcode;
    // для возврата из функции либо указателя на узел,
    // либо значение информационного поля
    union
    {
        uint32_t v;
        Node*    n;
    } value;
} Result;

// добавление нового элемента в дерево
// аргументы:
// root - указатель на корень дерева
// key - ключ узла
// value - значение узла
//
// возвращаемое значение:
// информационное поле узла, если обновили существующий узел
Result BST_add(Node** root, uint32_t key, uint32_t value);

// удаление узла из дерева
// аргументы:
// root - указатель на корень дерева
// key - ключ узла
Result BST_remove(Node** root, uint32_t key);


// обход дерева в прямом порядке, вывод элементов в stdout
// аргументы:
// root - указатель на корень дерева
// n - число десятичных разрядов
Result BST_traversal(Node** root, uint32_t n);

// поиск узла по заданному ключу
// аргумменты:
// root - указатель на корень дерева
// key - ключ узла
//
// возвращаемое значение:
// NOT_FOUNT retcode, если узел не найден
// указатель на искомый узел, если найден
Result BST_search(Node** root, uint32_t key);

// поиск узла значение которого максимально отличается от заданного
// аргументы:
// root - указатель на корень дерева
// val - заданное значение
//
// возвращаемое значение:
// указатель на узел, значение которого максимально отличается от заданного
Result BST_specSearch(Node** root, uint32_t key);

// вывод в виде дерева
// аргументы:
// root - указатель на корень дерева
Result BST_print(Node** root);

// графический вывод дерева в файл, требует graphviz
// сохраняет изображение в bst_tree.png
// аргументы:
// root - указатель на корень дерева
// filename - имя выходного файла
Result BST_graphPrint(Node** root);

// считать дерево из файла
// аргументы:
// root - указатель на корень дерева
// filename - имя входного файла
Result BST_read(Node** root, const char* filename);

// очищает память, выделенную под узлы дерева
// аргументы:
// root - указатель на корень дерева
void BST_free(Node** root);

#endif // BST_H
