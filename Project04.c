#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX_LEVEL 5

typedef struct SkipListNode {
    int data;
    struct SkipListNode** forward;
} SkipListNode;

typedef struct SkipList {
    int level;
    SkipListNode* header;
} SkipList;

SkipListNode* createNode(int data, int level) {
    SkipListNode* newNode = (SkipListNode*)malloc(sizeof(SkipListNode));
    newNode->data = data;
    newNode->forward = (SkipListNode**)malloc((level + 1) * sizeof(SkipListNode*));
    return newNode;
}

SkipList* createSkipList() {
    SkipList* list = (SkipList*)malloc(sizeof(SkipList));
    list->level = 0;
    list->header = createNode(-1, MAX_LEVEL);
    for (int i = 0; i <= MAX_LEVEL; i++) {
        list->header->forward[i] = NULL;
    }
    return list;
}

int randomLevel() {
    int level = 0;
    while (rand() < RAND_MAX / 2 && level < MAX_LEVEL) {
        level++;
    }
    return level;
}

void insert(SkipList* list, int data) {
    SkipListNode* update[MAX_LEVEL + 1];
    SkipListNode* current = list->header;

    for (int i = list->level; i >= 0; i--) {
        while (current->forward[i] != NULL && current->forward[i]->data < data) {
            current = current->forward[i];
        }
        update[i] = current;
    }

    current = current->forward[0];

    if (current == NULL || current->data != data) {
        int newLevel = randomLevel();

        if (newLevel > list->level) {
            for (int i = list->level + 1; i <= newLevel; i++) {
                update[i] = list->header;
            }
            list->level = newLevel;
        }

        SkipListNode* newNode = createNode(data, newLevel);

        for (int i = 0; i <= newLevel; i++) {
            newNode->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = newNode;
        }
    }
}

void delete(SkipList* list, int data) {
    SkipListNode* update[MAX_LEVEL + 1];
    SkipListNode* current = list->header;

    for (int i = list->level; i >= 0; i--) {
        while (current->forward[i] != NULL && current->forward[i]->data < data) {
            current = current->forward[i];
        }
        update[i] = current;
    }

    current = current->forward[0];

    if (current != NULL && current->data == data) {
        for (int i = 0; i <= list->level; i++) {
            if (update[i]->forward[i] != current)
                break;
            update[i]->forward[i] = current->forward[i];
        }
        free(current);
        while (list->level > 0 && list->header->forward[list->level] == NULL) {
            list->level--;
        }
    }
}

SkipListNode* search(SkipList* list, int data) {
    SkipListNode* current = list->header;

    for (int i = list->level; i >= 0; i--) {
        while (current->forward[i] != NULL && current->forward[i]->data < data) {
            current = current->forward[i];
        }
    }

    current = current->forward[0];

    if (current != NULL && current->data == data) {
        return current;
    } else {
        return NULL;
    }
}

void display(SkipList* list) {
    printf("Skip List (level %d):\n", list->level);

    for (int i = list->level; i >= 0; i--) {
        SkipListNode* current = list->header->forward[i];
        printf("Level %d: ", i);

        while (current != NULL) {
            printf("%d ", current->data);
            current = current->forward[i];
        }

        printf("\n");
    }
}

int main() {
    int choice, data, n;

    SkipList* list = NULL;

    while (1) {
        printf("\nSkip List Operations:\n");
        printf("1. Create SkipList\n2. Insert in SkipList\n3. Delete from SkipList\n4. Search in SkipList\n5. Display SkipList\n6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                srand(time(NULL));
                if (list != NULL) {
                    printf("SkipList already created. Please choose another option.\n");
                    break;
                }
                list = createSkipList();
                printf("Enter the number of values in SkipList: ");
                scanf("%d", &n);
                printf("Enter data to insert: ");
                while (n--) {
                    scanf("%d", &data);
                    insert(list, data);
                }
                break;
            case 2:
                if (list == NULL) {
                    printf("SkipList not created. Please choose option 1 to create a SkipList.\n");
                    break;
                }
                printf("Enter the data to insert: ");
                scanf("%d", &data);
                insert(list, data);
                break;
            case 3:
                if (list == NULL) {
                    printf("SkipList not created. Please choose option 1 to create a SkipList.\n");
                    break;
                }
                printf("Enter the data to delete: ");
                scanf("%d", &data);
                delete(list, data);
                break;
            case 4:
                if (list == NULL) {
                    printf("SkipList not created. Please choose option 1 to create a SkipList.\n");
                    break;
                }
                printf("Enter the data to search: ");
                scanf("%d", &data);
                if (search(list, data) != NULL) {
                    printf("Data %d found in the SkipList.\n", data);
                } else {
                    printf("Data %d not found in the SkipList.\n", data);
                }
                break;
            case 5:
                if (list == NULL) {
                    printf("SkipList not created. Please choose option 1 to create a SkipList.\n");
                    break;
                }
                display(list);
                break;
            case 6:
                printf("Exiting the program.\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
