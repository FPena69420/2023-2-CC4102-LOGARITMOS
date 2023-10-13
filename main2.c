#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

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

void printBinaryFile(const char* filename) {
    // Open the binary file for reading
    FILE* file = fopen(filename, "rb");

    if (file == NULL) {
        printf("NO SUCH FILE:\n");
        //perror("Error opening file");
        return;
    }

    printf("START OF BINARY FILE:\n");

    // Read and print the binary data as-is
    int byte;
    while ((byte = fgetc(file)) != EOF) {
        printf("%02X ", byte); // Print each byte as a hexadecimal value
    }

    printf("\nEND OF BINARY FILE:\n");

    // Close the file
    fclose(file);
}

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

int x_write(const char* some_file, // name of the file to be written in
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

int main() {
    const char* filename = "example4.bin";

    int32_t newValues2[]= {16,17,18,19,20,21};
    // replaceIntsInFile(filename, 0, 5, newValues2);
    int32_t newValue[]= {110};

    x_write(filename, 10, 6, newValues2);
    printBinaryFile(filename);

    x_write(filename, 11, 1, newValue);
    printBinaryFile(filename);

    int32_t* file_values= x_get(filename, 11, 5);
    for (int x= 0; x< 5; x++) {
        printf("%d ", file_values[x]);
    }
    free(file_values);

    return 0;
}
