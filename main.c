#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

//Struct to represent a rectangle node. 
struct Node {
    int x1;
    int y1;
    int x2;
    int y2;
    int l; // line of child node.
    int c; // number of child nodes
};


//Appends {N} 4-byte zeroes to a binary file.
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

// Prints {count} elements from {some_array}
void printArray(int* some_array, 
                int count
                ) {

    printf("{");
    for (int i= 0; i< count; i++) {
        printf("%d ", some_array[i]);
        if ((i+1)%5== 0){
            printf("\n");
        }
    }
    printf("}");
}

//Prints {size} {Node}s from {some_array}.
void printNodes(struct Node* some_node_array, // the array of nodes to be printed
                int size, // the number of nodes to print
                int vanish_min,
                int vanish_max
                ) {

    if (some_node_array != NULL) {
        printf("PRINTING NODES:\n");
        for (int i = 0; i < size; i++) {
            if (i<= vanish_min || vanish_max<= i) {
                printf("Node %d: x1=%d, y1=%d, x2=%d, y2=%d, l=%d, c=%d\n", 
                i, 
                some_node_array[i].x1, 
                some_node_array[i].y1, 
                some_node_array[i].x2, 
                some_node_array[i].y2, 
                some_node_array[i].l,
                some_node_array[i].c);
            }
        }
        printf("\n");
    }
}

//Prints a binary file of path {filename} that contains ints, in hex format. 
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

//Prints a binary file of path {filename} that contains ints, in int format. Prints {count} 
//elements from the index {index}, {elements per row} elements per row.
void printBinaryFile2(const char *filename, // path of the file to be opened
                      int index, // index from which to start printing
                      int count, // number of elements to print
                      int elements_per_row //elements per row to be printed
                      ) {
    FILE *file = fopen(filename, "rb");

    if (file == NULL) {
        printf("Error opening file: %s\n", filename);
        return;
    }

    // Seek to the desired starting index.
    fseek(file, index * sizeof(int), SEEK_SET);

    int value;
    int printed = 0;
    printf("START OF BINARY FILE: \n");
    while (printed < count && fread(&value, sizeof(int), 1, file) == 1) {
        printf("%d ", value);
        printed++;

        if (printed % elements_per_row == 0) {
            printf("\n");  // Print four numbers per row.
        }
    }
    printf("-EOF-\n\n");

    fclose(file);
}

void printIntsFromFile(const char* filename, 
                       int col, 
                       int vanishing_min, 
                       int vanishing_max
                       ) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Error opening the file");
        return;
    }

    int line_number = 0;
    int int_value;
    int printed = 0;

    printf("PRINTING FILE: \n");

    while (fread(&int_value, sizeof(int_value), 1, file) == 1) {
        if (printed == 0) {
            if (line_number<= vanishing_min || vanishing_max<= line_number) {printf("Line %d: ", line_number);}
        }
        if (line_number<= vanishing_min || vanishing_max<= line_number) {printf("%d ", int_value);}
        printed++;
        if (printed == col) {
            if (line_number<= vanishing_min || vanishing_max<= line_number) {printf("\n");}
            printed = 0;
            line_number++;
        }
    }

    if (printed != 0) {
        printf("\n");
    }

    printf("\n");

    fclose(file);
}

//Writes to a binary file {number_of_elements} elements from array {integer_array}.  
//Starts writing on the file from index {index}.
void x_write(const char* some_file, // name of the file to be written in
             int index, // starting index of the elements to be written (0 onwards) 
             int number_of_elements, // number of ints to be written
             void* integer_array // array of integers to be written
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

//Returns an array with {number_of_elements} 4-byte ints, taken from the binary file {some_file}, 
//starting from index {index}.
int* array_get(const char* some_file, int index, int number_of_elements) {
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

int* array_get_leaves(const char* some_file, int index, int number_of_rectangles) {
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

    // array with x1,y1,x2,y2 of every leaf
    int* elements = (int*)malloc(sizeof(int) * number_of_rectangles * 5); 
    if (elements == NULL) {
        fclose(file);
        perror("Memory allocation error");
        return NULL;
    }

    // Read the specified number of elements into the array
    // size_t elements_read = fread(elements, sizeof(int32_t), number_of_rectangles, file);
    
    for (int i = 0; i < number_of_rectangles * 5; i+=5) {
        // Read 4 integers from the file and create a node
        if (fread(&elements[i], sizeof(int), 4, file) != 4) {
            perror("Error reading data from the file");
            free(elements);
            fclose(file);
            return NULL;
        }
        elements[i + 4]= 0;
    }

    fclose(file);
    return elements;
}

//Parses {N} {Node}s from {some_file}, starting from {index} (measured in 4-byte ints). 
//Returns an array of them. Remember to free with free().
struct Node* node_get(const char* some_file, // file to open
                    int index, // index of file (measured in 4-byte ints) to start extracting nodes
                    int N // number of NODES to extract
                   ) {
    FILE *file = fopen(some_file, "rb"); // "rb" mode for reading in binary

    int size= 0;

    if (file!= NULL){
        fseek(file, 0L, SEEK_END);
        size= (uint32_t) ftell(file)/sizeof(uint32_t);
        if (index + (N * 6) > size) {
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
        // Read six integers from the file and create a node
        if (fread(&nodes[i], sizeof(int), 6, file) != 6) {
            perror("Error reading data from the file");
            free(nodes);
            fclose(file);
            return NULL;
        }
    }

    fclose(file);
    return nodes;
}

// gets {N} leaves from a binary file {some_file}, starts reading from index {index}.
struct Node* node_get_leaves(const char* some_file, // file to open
                    int index, // index of file to start extracting nodes
                    int N // number of NODES to extract
                   ) {
    FILE *file = fopen(some_file, "rb"); // "rb" mode for reading in binary

    int size= 0;

    if (file!= NULL){
        fseek(file, 0L, SEEK_END);
        size= (uint32_t) ftell(file)/sizeof(uint32_t);
        if (index + (N * 6) > size) {
            perror("Segmentation fault. Attempted to create node from memory outside file.\n");
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
        // Read 4 integers from the file and create a node
        if (fread(&nodes[i], sizeof(int), 4, file) != 4) {
            perror("Error reading data from the file");
            free(nodes);
            fclose(file);
            return NULL;
        }
        nodes[i].l= 0;
        nodes[i].c= 0;
    }

    fclose(file);
    return nodes;
}

// Function to create parents from the binary file
int create_parents(const char *file_name, 
                   int M // number of children for each node
                   ) {

    // Initialize variables
    int size= 0;
    int index = 0;
    int nodes_read = M;
    int pos= 0;
    int nodes_created= 0;

    while (nodes_created!= 1) {
        nodes_read= M;
        pos= 0;
        nodes_created= 0;

        FILE *file = fopen(file_name, "rb+");
    
        if (file == NULL) {
            perror("File open error");
            return 0; // Return 0 to indicate failure
        }

        // Find the current size of the file
        fseek(file, 0, SEEK_END);
        size = (uint32_t) ftell(file)/sizeof(uint32_t); // size in 4-byte ints

        // Close the file
        fclose(file);

        while (index < size) {

            // Check if there are enough bytes to read M * 6 integers
            if (index + M * 6 > size) {
                nodes_read = (size - index) / 6;
            }

            // Read nodes_read * 6 4-byte integers from the file
            // int* node_array = (int*)malloc(nodes_read * 6 * sizeof(uint32_t));
            // if (node_array == NULL) {
            //     // Handle memory allocation error
            //     perror("Memory allocation failed");
            //     return 0; // Return 0 to indicate failure
            // }

            // fseek(file, index, SEEK_SET);
            // fread(node_array, sizeof(int), nodes_read * 6, file);

            struct Node* node_array= node_get(file_name, index, nodes_read);

            // Calculate the minimum x1, y1, and maximum x2, y2
            int min_x1 = node_array[0].x1;
            int min_y1 = node_array[1].y1;
            int max_x2 = node_array[2].x2;
            int max_y2 = node_array[3].y2;

            for (int i = 0; i < nodes_read; i++) {
                int node_x1 = node_array[i].x1;
                int node_y1 = node_array[i].y1;
                int node_x2 = node_array[i].x2;
                int node_y2 = node_array[i].y2;

                min_x1 = (node_x1 < min_x1) ? node_x1 : min_x1;
                min_y1 = (node_y1 < min_y1) ? node_y1 : min_y1;
                max_x2 = (node_x2 > max_x2) ? node_x2 : max_x2;
                max_y2 = (node_y2 > max_y2) ? node_y2 : max_y2;
            }

            // Create a Node with the calculated values
            struct Node node;
            node.x1 = min_x1;
            node.y1 = min_y1;
            node.x2 = max_x2;
            node.y2 = max_y2;
            node.l = index;
            node.c = nodes_read * 6;

            // printNodes(&node, 1, 0, 0);

            // Write the Node to the file
            x_write(file_name, size + pos, 6, &node); // modify size
            nodes_created ++;

            // printIntsFromFile(file_name, 6, 50, 1023);

            // Update index and nodes_read
            index += nodes_read * 6 ;
            nodes_read = M;
            pos+= 6;

            // Free the node_array
            free(node_array);
            }
    }

    // Return 1 if exactly one node was written
    return size + pos - 6;
}

//rotar/voltear un cuadrante apropiadamente
void rot(int n, int *x, int *y, int rx, int ry) {
    int t;
    if (ry == 0) {
        if (rx == 1) {
            *x = n-1 - *x;
            *y = n-1 - *y;
        }
        t  = *x;
        *x = *y;
        *y = t;
    }
}

//convierte (x,y) a d
int hillbert (int hillbert_level, int x, int y) {
    int rx, ry, s, d=0;
    for (s = hillbert_level/2; s > 0; s /= 2) {
        rx = (x & s) > 0;
        ry = (y & s) > 0;
        d += s * s * ((3 * rx) ^ ry);
        rot(s, &x, &y, rx, ry);
    }
    return d;
}

//Write your node-sorting function here. (Mine is Hilbert curve).
double some_function(double x1, double y1, double x2, double y2) {

    const int H_L= exp2(19); // 2 ** 19 = 524.288. Enough to cover 500.000 * 500.000 plane
    double x_median= (x1 + x2)/2;
    double y_median= (y1 + y2)/2; 
    return hillbert(H_L, x_median, y_median);
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

int x_size_ints(void* some_struct) {
    return (int) sizeof(some_struct) / sizeof(uint32_t);
}

int x_size_nodes(void* some_struct) {
    return (int) sizeof(some_struct) / sizeof(uint32_t);
}

int main() {
    const char* RECTANGLES = "recs/r_10.bin";
    const char* R_TREE= "r_tree.bin"; 
    const int N_OF_NODES= exp2(10);
    const int M_OF_CHILDREN= 100;
    const int INTS_FOR_NODE= 6;

    remove(R_TREE);

    // int32_t newValues2[]= {16, 17, 18, 19, 20,
    //                        189, 3, 211, 15, 9,
    //                        13, 0, 5, 1, 4};
    // int32_t newValue[]= {1100};

    // x_write(filename, 20, 15, newValues2);
    // printBinaryFile(filename);

    // x_write(filename, 11, 1, newValue);
    // printBinaryFile2(filename, 0, 40);

    // struct Node* nodes = x_get2(filename, 11, 2);
    // struct Node* nodes2 = x_get3(filename, 20, 3);
    // printNodes(nodes, 2);
    // printNodes(nodes2, 3);

    // qsort(nodes2, 3, sizeof(struct Node), compare_nodes);

    // printNodes(nodes2, 3);

    // free(nodes);
    // free(nodes2);

    // return 0;

    struct Node* leaves= node_get_leaves(RECTANGLES, 0, N_OF_NODES);

    qsort(leaves, N_OF_NODES, sizeof(struct Node), compare_nodes);
    
    printNodes(leaves, N_OF_NODES, 50, N_OF_NODES - 50);
    x_write(R_TREE, 0, exp2(10) * INTS_FOR_NODE, leaves);
    printIntsFromFile(R_TREE, INTS_FOR_NODE, 50, N_OF_NODES - 50);
    
    free(leaves);

    int root_index= create_parents(R_TREE, M_OF_CHILDREN);
    printIntsFromFile(R_TREE, INTS_FOR_NODE, 50, N_OF_NODES);
    printf("POSITION OF ROOT OF R-TREE= %d\n", root_index);
    
    return 0;
}
