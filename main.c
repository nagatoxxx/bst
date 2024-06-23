#include <stdio.h>
#include <string.h>

#include "include/bst.h"

#define FILENAMELEN 255

#define stdinFlush()                                  \
    {                                                 \
        int c;                                        \
        while ((c = getchar()) != '\n' && c != EOF) { \
        }                                             \
    }

int main()
{
    Node* root = NULL;

    while (1) {
        int choose = 0;

        printf("Choose an option:\n"
               "1. Add element\n"
               "2. Delete element\n"
               "3. Traverse\n"
               "4. Search\n"
               "5. Special search\n"
               "6. Print tree\n"
               "7. Output tree to png\n"
               "8. Load from file\n\n"
               "9. Exit\n"
               "> ");

        if (scanf("%d", &choose) != 1) {
            printf("Invalid input\n");
            stdinFlush();
            continue;
        }

        switch (choose) {
            case 1:
            {
                uint32_t key = 0, value = 0;
                printf("Enter key and value: ");
                if (scanf("%u %u", &key, &value) != 2) {
                    printf("Invalid input\n");
                    break;
                }
                BST_add(&root, key, value);
                printf("Element added\n");
                break;
            }
            case 2:
            {
                uint32_t key = 0;
                printf("Enter key to delete: ");
                if (scanf("%u", &key) != 1) {
                    printf("Invalid input\n");
                    break;
                }
                Result r = BST_remove(&root, key);
                if (r.retcode == NOT_FOUND) {
                    printf("Element not found\n");
                    break;
                }
                printf("Element deleted\n");
                break;
            }
            case 3:
            {
                uint32_t n = 0;
                printf("Enter n (decimal places): ");
                if (scanf("%u", &n) != 1) {
                    printf("Invalid input\n");
                    break;
                }
                Result r = BST_traversal(&root, n);
                if (r.retcode == NULLPTR) {
                    printf("Tree is empty\n");
                    break;
                }
                break;
            }
            case 4:
            {
                uint32_t key = 0;
                printf("Enter key to search: ");
                if (scanf("%u", &key) != 1) {
                    printf("Invalid input\n");
                    break;
                }
                Result r = BST_search(&root, key);
                if (r.retcode == NOT_FOUND) {
                    printf("Element not found\n");
                    break;
                }
                printf("Element found: \n");
                printf("Value: %u\n", r.value.n->value);
                break;
            }
            case 5:
            {
                uint32_t key = 0;
                printf("Enter key to search: ");
                if (scanf("%u", &key) != 1) {
                    printf("Invalid input\n");
                    break;
                }
                Result r = BST_specSearch(&root, key);
                printf("Element with max difference:\n");
                printf("Key: %u\n", r.value.n->key);
                printf("Value: %u\n", r.value.n->value);
                break;
            }
            case 6:
            {
                Result r = BST_print(&root);
                if (r.retcode == NULLPTR) {
                    printf("Tree is empty\n");
                }
                break;
            }
            case 7:
            {
                Result r = BST_graphPrint(&root);
                if (r.retcode == NULLPTR) {
                    printf("Tree is empty\n");
                }
                printf("Tree saved to bst_tree.png\n");
                break;
            }
            case 8:
            {
                stdinFlush();
                char filename[FILENAMELEN];

                printf("Enter filename: ");
                fgets(filename, FILENAMELEN, stdin);
                size_t len = strlen(filename);
                if (len > 0 && filename[len - 1] == '\n') {
                    filename[len - 1] = '\0';
                }

                Result r = BST_read(&root, filename);
                if (r.retcode == FILE_ERROR) {
                    printf("Error while reading file\n");
                    break;
                }
                printf("Tree loaded successfully\n");
                break;
            }
            case 9:
                goto exit;
        }
    }

exit:
    BST_free(&root);
    return 0;
}
