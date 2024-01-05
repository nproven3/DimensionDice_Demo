#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int x, y, z, value;
    struct Node *next;
} Node;


Node *findNode(Node *head, int x, int y, int z);
Node *createNode(int x, int y, int z, int value);
Node *readFromFile(const char *filename);
char *decode(Node *head, int length);
void pseudoRandomPosition(int i, int *x, int *y, int *z);
void freeMatrix(Node *head);

Node *findNode(Node *head, int x, int y, int z) {
    Node *current = head;
    while (current != NULL) {
        if (current->x == x && current->y == y && current->z == z) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

Node *createNode(int x, int y, int z, int value) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->x = x;
    newNode->y = y;
    newNode->z = z;
    newNode->value = value;
    newNode->next = NULL;
    return newNode;
}

Node *readFromFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return NULL;
    }

    Node *head = NULL;
    int x, y, z, value;
    while (fscanf(file, "%d %d %d %d", &x, &y, &z, &value) == 4) {
        Node *newNode = createNode(x, y, z, value);
        newNode->next = head;
        head = newNode;
    }

    fclose(file);
    return head;
}

char *decode(Node *head, int length) {
    char *str = (char *)malloc(length + 1);
    if (!str) return NULL;

    // This loop assumes you have a way to determine the order of characters
    for (int i = 0; i < length; i++) {
        int x, y, z;
        // Generate pseudo-random x, y, z based on i (the position in the string)
        // This should be the same algorithm used in encoding
        pseudoRandomPosition(i, &x, &y, &z);

        Node *node = findNode(head, x, y, z);
        str[i] = node ? (char)node->value : '?'; // '?' denotes missing value
    }

    str[length] = '\0';
    return str;
}

void pseudoRandomPosition(int i, int *x, int *y, int *z) {
    // This function should be the same as in the decoder
    *x = i;
    *y = i + 1;
    *z = i + 2;
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
    Node *matrix = readFromFile("encoded_data.txt");

    int length = 10; // You need to know the length of the original string
    char *decodedString = decode(matrix, length);
    if (decodedString) {
        printf("Decoded String: %s\n", decodedString);
        free(decodedString);
    }

    freeMatrix(matrix);

    return 0;
}
