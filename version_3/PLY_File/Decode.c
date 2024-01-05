#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct Node {
    int x, y, z, value;
    struct Node *next;
} Node;

bool startsWith(const char *str, const char *prefix) {
    return strncmp(prefix, str, strlen(prefix)) == 0;
}

void decodeWithNoise(Node *head, int zLevel, const char *startSequence) {
    char decodedString[1000] = {0}; // Initialize all elements to zero for safety
    int index = 0;

    for (int i = 0; ; i += 2) { // Assuming the pattern in the encoder
        Node *current = head;
        int found = 0;

        while (current) {
            if (current->z == zLevel && current->x == i && current->y == i) {
                decodedString[index++] = (char)current->value;
                decodedString[index] = '\0'; // Ensure null-termination after each addition
                found = 1;
                break;
            }
            current = current->next;
        }

        if (!found) {
            // If no matching node is found, check if we already found the start sequence
            if (startsWith(decodedString, startSequence)) {
                printf("Decoded String: %s\n", decodedString + strlen(startSequence));
                return;
            } else {
                printf("Correct string not found\n");
                return;
            }
        }
    }
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

void insert(Node **head, int x, int y, int z, int value) {
    Node *newNode = createNode(x, y, z, value);
    newNode->next = *head;
    *head = newNode;
}

int rgbToChar(int r, int g, int b) {
    // Convert RGB back to ASCII (reverse of charToRGB)
    return ((r << 4) | g) & 0xFF; // Example conversion
}

void readPLY(const char *filename, Node **head) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return;
    }

    char line[100];
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "end_header", 10) == 0) {
            break;
        }
    }

    int x, y, z, r, g, b;
    while (fscanf(file, "%d %d %d %d %d %d", &x, &y, &z, &r, &g, &b) == 6) {
        int value = rgbToChar(r, g, b);
        insert(head, x, y, z, value);
    }

    fclose(file);
}

void decode(Node *head, char *str, int length) {
    for (int i = 0; i < length; i++) {
        Node *current = head;
        while (current) {
            if (current->x == i) {
                str[i] = (char)current->value;
                break;
            }
            current = current->next;
        }
    }
    str[length] = '\0';
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
     Node *matrix = NULL;
    readPLY("output.ply", &matrix);
    decodeWithNoise(matrix, 1, "THIS_ONE");
    freeMatrix(matrix);
    return 0;
}
