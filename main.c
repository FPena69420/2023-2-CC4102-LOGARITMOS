#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

//Appends N 4-byte zeroes to a binary file.
void appendZeroesToFile(const char* filename, int N) { 
    
    FILE *file = fopen(filename, "ab"); // "ab" mode for appending in binary
    if (file == NULL) {
        // If the file doesn't exist, create it
        file = fopen(filename, "wb"); // "wb" mode for writing in binary
        if (file == NULL) {
            perror("Error opening file");
            return;
        }
    }

    // Fill a 4-byte buffer with zeroes
    uint32_t zeroes = 0;

    for (int i = 0; i < N; i++) {
        fwrite(&zeroes, sizeof(uint32_t), 1, file);
    }

    // fseek(file, 0L, SEEK_END);
    // uint32_t size= (uint32_t) ftell(file)/sizeof(uint32_t);

    fclose(file);
    return;
}

//Prints a binary file that contains ints, in hex format. 
void printBinaryFile(const char* filename) {
    // Open the binary file for reading
    FILE* file = fopen(filename, "rb");

    if (file == NULL) {
        printf("NO SUCH FILE:\n");
        //perror("Error opening file");
        return;
    }

    printf("PRINTING BINARY FILE:\n");

    // Read and print the binary data as-is
    int byte;
    while ((byte = fgetc(file)) != EOF) {
        printf("%02X ", byte); // Print each byte as a hexadecimal value
    }

    printf("-EOF-\n\n");

    // Close the file
    fclose(file);
}

//Replaces chunks of 4 bytes of a file for {N} elements of {newValues}, starting 
//from index {i}
void replaceIntsInFile(const char* filename, int i, int N, const int32_t* newValues) {
    FILE *file = fopen(filename, "r+b"); // "r+b" mode for reading and writing in binary
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    // Seek to the position in the file
    fseek(file, i * sizeof(int32_t), SEEK_SET);

    // Write the new values from the array
    for (int j = 0; j < N; j++) {
        fwrite(&newValues[j], sizeof(int32_t), 1, file);
    }

    fclose(file);
}

//Writes to a binary file {number_of_elements} elements from array {integer_array}.  
//Starts writing on the file from index {index}.
void x_write(const char* some_file, // name of the file to be written in
             int index, // starting index of the elements to be written (0 onwards) 
             int number_of_elements, // number of elements to be written
             const int32_t* integer_array // array of integers to be written
            ){
    
    int size= 0;
    FILE* check_file= fopen(some_file, "rb");

    if (check_file!= NULL){
        fseek(check_file, 0L, SEEK_END);
        size= (uint32_t) ftell(check_file)/sizeof(uint32_t);
        fclose(check_file);
    }
    

    int write_upper_limit= index + number_of_elements;

    if (size < write_upper_limit) {
        appendZeroesToFile(some_file, write_upper_limit - size);
    }

    replaceIntsInFile(some_file, index, number_of_elements, integer_array);
}

//Deprecated. See x_get2.
int* x_get(const char* some_file, int index, int number_of_elements) {
    FILE *file = fopen(some_file, "rb"); // "rb" mode for reading in binary
    if (file == NULL) {
        perror("Error opening file");
        return NULL;
    }

    // Seek to the starting position
    if (fseek(file, index * sizeof(int32_t), SEEK_SET) != 0) {
        perror("Error seeking to the specified position");
        fclose(file);
        return NULL;
    }

    int* elements = (int*)malloc(sizeof(int) * number_of_elements);
    if (elements == NULL) {
        fclose(file);
        perror("Memory allocation error");
        return NULL;
    }

    // Read the specified number of elements into the array
    size_t elements_read = fread(elements, sizeof(int32_t), number_of_elements, file);
    
    if (elements_read != number_of_elements) {
        perror("Error reading elements from the file");
        free(elements);
        fclose(file);
        return NULL;
    }

    fclose(file);
    return elements;
}

//Struct to represent a rectangle node. 
struct Node {
    int x1;
    int y1;
    int x2;
    int y2;
    int l; //line of son node.
};

//Parses {N} {Node}s from {some_file}, starting from {index}. Returns an array of them.
//Remember to free with free().
struct Node* x_get2(const char* some_file, // file to open
                    int index, // index of file to start extracting nodes
                    int N // number of NODES to extract
                   ) {
    FILE *file = fopen(some_file, "rb"); // "rb" mode for reading in binary

    int size= 0;

    if (file!= NULL){
        fseek(file, 0L, SEEK_END);
        size= (uint32_t) ftell(file)/sizeof(uint32_t);
        if (index + (N * 5) > size) {
            perror("Segmentation fault. Attempted to create node from memory outside array.\n");
            fclose(file);
            return NULL;
        }
    }
    else {
        perror("No such file");
        return NULL;
    }

    // Seek to the starting position
    if (fseek(file, index * sizeof(int), SEEK_SET) != 0) {
        perror("Error seeking to the specified position");
        fclose(file);
        return NULL;
    }

    struct Node* nodes = (struct Node*)malloc(sizeof(struct Node) * N);
    if (nodes == NULL) {
        fclose(file);
        perror("Memory allocation error");
        return NULL;
    }

    for (int i = 0; i < N; i++) {
        // Read five integers from the file and create a node
        if (fread(&nodes[i], sizeof(int), 5, file) != 5) {
            perror("Error reading data from the file");
            free(nodes);
            fclose(file);
            return NULL;
        }
    }

    fclose(file);
    return nodes;
}

//Prints {size} {Node}s from an array.
void printNodes(struct Node* some_node_array, // the array of nodes to be printed
                int size // the number of nodes to print
               ) {
    if (some_node_array != NULL) {
        printf("PRINTING NODES:\n");
        for (int i = 0; i < size; i++) {
            printf("Node %d: x1=%d, y1=%d, x2=%d, y2=%d, l=%d\n", 
            i, 
            some_node_array[i].x1, 
            some_node_array[i].y1, 
            some_node_array[i].x2, 
            some_node_array[i].y2, 
            some_node_array[i].l);
        }
        printf("\n");
    }
}

//Write your node-sorting function here. (Mine is Hilbert curve).
double some_function(double x1, double y1, double x2, double y2) {
    // Implement your custom function here
    // For example, you can return the Euclidean distance 
    // (Or not since C doesn't seem to recognize sqrt()).
    return (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1);
}

//Defines a comparison function for qsort
int compare_nodes(const void *a, const void *b) {
    const struct Node *node_a = (const struct Node *)a;
    const struct Node *node_b = (const struct Node *)b;

    // Calculate the result of some_function for the first 4 elements
    double result_a = some_function(node_a->x1, node_a->y1, node_a->x2, node_a->y2);
    double result_b = some_function(node_b->x1, node_b->y1, node_b->x2, node_b->y2);

    if (result_a < result_b) {
        return -1;
    } else if (result_a > result_b) {
        return 1;
    } else {
        return 0;
    }
}

int main() {
    const char* filename = "example.bin";

    remove(filename);

    int32_t newValues2[]= {16, 17, 18, 19, 20,
                           189, 3, 211, 15, 9,
                           13, 0, 5, 1, 4};
    int32_t newValue[]= {1100};

    x_write(filename, 20, 15, newValues2);
    printBinaryFile(filename);

    x_write(filename, 11, 1, newValue);
    printBinaryFile(filename);

    struct Node* nodes = x_get2(filename, 11, 2);
    struct Node* nodes2 = x_get2(filename, 20, 3);
    printNodes(nodes, 2);
    printNodes(nodes2, 3);

    qsort(nodes2, 3, sizeof(struct Node), compare_nodes);

    printNodes(nodes2, 3);

    free(nodes);
    free(nodes2);

    return 0;
}
