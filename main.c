#include <stdio.h>
#include <stdlib.h>

int NODE_SIZE= (int) 5 * sizeof(int);

void writeIntegersToLine(const char* filename, int line_number, const int* some_data, int M) {
    FILE* file = fopen(filename, "r+"); // Open the file in read and write mode

    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    int current_line = 0;

    while (current_line < line_number && fgetc(file)) {
        char ch;
        while ((ch = fgetc(file)) != EOF && ch != '\n') {
            // Count lines until the desired line is reached
        }
        current_line++;
    }

    if (current_line == line_number) {
        // Found the line to write data to

        // Move the file pointer back to the beginning of the line
        fseek(file, -1, SEEK_CUR);

        // Write the array of integers in little-endian format to the binary file
        for (int i = 0; i < M; i++) {
            fwrite(&some_data[i], sizeof(int), 1, file);
        }
        fwrite("\n", sizeof(char), 1, file);
    }

    fclose(file);
}

void printIntegersFromBinaryFile(const char* filename) {
    // Open the binary file for reading
    FILE* file = fopen(filename, "rb");

    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    int value;

    while (fread(&value, sizeof(int), 1, file) == 1) {
        printf("%d ", value); // Print each integer value
    }

    printf("\n");

    // Close the file
    fclose(file);
}


void printBinaryFile(const char* filename) {
    // Open the binary file for reading
    FILE* file = fopen(filename, "rb");

    if (file == NULL) {
        //perror("Error opening file");
        return;
    }

    // Read and print the binary data as-is
    int byte;
    while ((byte = fgetc(file)) != EOF) {
        printf("%02X ", byte); // Print each byte as a hexadecimal value
    }

    printf("\n");

    // Close the file
    fclose(file);
}

// int* parseLine2(const char* filename, int line_number, int M) {
//     FILE* file= fopen(filename, "rb");

//     if (file== NULL) {
//         perror("Error opening file");
//         return;
//     }
    
//     int* integers= (int*) malloc(M* sizeof(int));

//     file += M * (); 
// }


// int* parseLine(const char* filename, int line_number, int K) {
//     FILE* file = fopen(filename, "rb");
    
//     if (file == NULL) {
//         perror("Error opening file");
//         return NULL;
//     }

//     int current_line = 0;
//     int* integers = (int*)malloc(K * sizeof(int));

//     while (current_line < line_number) {
//         char ch;
//         while ((ch = fgetc(file)) != EOF) {
//             if (ch == '\n') {
//                 current_line++;
//                 if (current_line >= line_number) {
//                     break;
//                 }
//             }
//         }

//         if (ch == EOF) {
//             free(integers);
//             fclose(file);
//             return NULL; // Line number out of range
//         }
//     }

//     for (int i = 0; i < K; i++) {
//         if (fread(&integers[i], sizeof(int), 1, file) != 1) {
//             free(integers);
//             fclose(file);
//             return NULL; // Failed to read all K integers
//         }
//     }

//     fclose(file);
//     return integers;
// }


int main() {
    printf("INT SIZE: %lu\n", sizeof(int)); //32 bits por entero

    const char* filename = "example.bin";
    FILE* file = fopen(filename, "w+"); // Open the file in read and write mode
    fclose(file);

    int line_number = 7; // Replace with the desired line number
    int some_data[] = {1, 2, 3, 4, 5}; // Replace with your array of 32-bit integers
    int some_data_2[] = {16, 17, 18, 19, 20};
    int some_data_3[] = {10, 11, 12, 13, 14};
    int M = sizeof(some_data) / sizeof(some_data[0]); // Calculate the length of the array


    writeIntegersToLine(filename, line_number, some_data, M);
    writeIntegersToLine(filename, line_number, some_data_3, M);
    writeIntegersToLine(filename, line_number, some_data_2, M);

    printBinaryFile(filename);
    printIntegersFromBinaryFile(filename);

    // int* result = parseLine(filename, line_number, M);
    // if (result != NULL) {
    //     for (int i = 0; i < M; i++) {
    //         printf("%d ", result[i]);
    //     }
    //     free(result);
    // } else {
    //     printf("Failed to parse the line.\n");
    // }

    
    return 0;
}
