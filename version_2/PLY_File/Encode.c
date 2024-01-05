#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct Node {
    int x, y, z, value;
    struct Node *next;
} Node;

void insert(Node **head, int x, int y, int z, int value);

void addNoise(Node **head, int zLevel, int noiseAmount) {
    for (int i = 0; i < noiseAmount; ++i) {
        int x = rand() % 1000;  // Larger range for randomness
        int y = rand() % 1000;
        int value = rand() % 128;
        if (x % 2 == 0 && y % 2 == 0) continue;  // Avoid the pattern used for real data
        insert(head, x, y, zLevel, value);
    }
}

void encodeWithNoise(Node **head, const char *str, int zLevel, int noiseAmount) {
    addNoise(head, zLevel, noiseAmount);
    for (int i = 0; str[i] != '\0'; i++) {
        int x = i * 2;  // Even indices for real data
        int y = i * 2;
        insert(head, x, y, zLevel, (int)str[i]);
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

void pseudoRandomPosition(int i, int *x, int *y, int *z) {
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

void charToRGB(int ascii, int *r, int *g, int *b) {
    // Convert ASCII value to RGB (simple demonstration)
    *r = (ascii & 0xF0) >> 4;
    *g = (ascii & 0x0F);
    *b = (ascii % 256) / 16;
}

void writePLY(Node *head, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Error opening file");
        return;
    }

    // Count the number of nodes in the linked list
    int nodeCount = 0;
    Node *current = head;
    while (current) {
        nodeCount++;
        current = current->next;
    }

    // PLY header
    fprintf(file, "ply\n");
    fprintf(file, "format ascii 1.0\n");
    fprintf(file, "element vertex %d\n", nodeCount);
    fprintf(file, "property float x\n");
    fprintf(file, "property float y\n");
    fprintf(file, "property float z\n");
    fprintf(file, "property uchar red\n");
    fprintf(file, "property uchar green\n");
    fprintf(file, "property uchar blue\n");
    fprintf(file, "end_header\n");

    // Writing vertices with color data
    current = head;
    while (current) {
        int r, g, b;
        charToRGB(current->value, &r, &g, &b);
        fprintf(file, "%d %d %d %d %d %d\n", current->x, current->y, current->z, r, g, b);
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
    Node *matrix = NULL;
    const char *realData = "THIS_ONETHIS IS THE FLAG: jfoierjaiosjhefdoklnaeriofjahwe4i890ofj3490iowej";
    int zLevel = 1;
    int noiseAmount = 1000;

    srand(time(NULL)); 
    encodeWithNoise(&matrix, realData, zLevel, noiseAmount);
    writePLY(matrix, "output.ply");
    freeMatrix(matrix);

    return 0;
}
