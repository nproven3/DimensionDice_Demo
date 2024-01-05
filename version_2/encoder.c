#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int x, y, z, value;
    struct Node *next;
} Node;

Node *createNode(int x, int y, int z, int value) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (!newNode) return NULL;
    newNode->x = x;
    newNode->y = y;
    newNode->z = z;
    newNode->value = value;
    newNode->next = NULL;
    return newNode;
}

void insert(Node **head, int x, int y, int z, int value) {
    Node *newNode = createNode(x, y, z, value);
    newNode->next = *head;
    *head = newNode;
}

void pseudoRandomPosition(int i, int *x, int *y, int *z) {
    // This function should be the same as in the decoder
    *x = i;
    *y = i + 1;
    *z = i + 2;
}

void encode(Node **head, const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        int x, y, z;
        pseudoRandomPosition(i, &x, &y, &z);
        insert(head, x, y, z, (int)str[i]);
    }
}

void writeToFile(Node *head, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Error opening file");
        return;
    }

    Node *current = head;
    while (current != NULL) {
        fprintf(file, "%d %d %d %d\n", current->x, current->y, current->z, current->value);
        current = current->next;
    }

    fclose(file);
}

void freeMatrix(Node *head) {
    Node *current = head;
    while (current != NULL) {
        Node *temp = current;
        current = current->next;
        free(temp);
    }
}

int main() {
    char inputStr[100];
    printf("Enter a string to encode: ");
    fgets(inputStr, 100, stdin);

    Node *matrix = NULL;

    encode(&matrix, inputStr);

    writeToFile(matrix, "encoded_data.txt");

    freeMatrix(matrix);

    return 0;
}
