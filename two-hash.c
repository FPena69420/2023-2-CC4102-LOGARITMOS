#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <math.h>

// 2**k - 1 = p
// primo de Mersenne

// 2**l= m

int DEBUG= 1;

// universal hash function
long long uni_hash(int a, int b, int p, int m, int x) {
    return (long long) ((a * x + b) % p ) % m;
}

// fast hash function
long long fast_hash(int a, int b, int k, int l, int x){
    return (long long) ((a * x + b) & ((1 << k) - 1)) >> l;
}

typedef struct {
    float x; 
    float y;
} point;

void print_point_array(point* points, int size) {
    printf("\nPrinting point array:\n");
    for (int i = 0; i < size; i++) {
        printf("Point %d: (%f, %f)\n", i, points[i].x, points[i].y);
    }
}

// gets the quadrant of a point given the length of the quadrant
uint64_t get_quadrant(point xy, double d) {
    uint64_t xfloor= (uint64_t) floor(xy.x/d);
    uint64_t yfloor= (uint64_t) floor(xy.y/d);
    uint64_t cellsperrow= (uint64_t) ceil(1/d);
    uint64_t quad= (uint64_t) xfloor + yfloor * cellsperrow;
    // printf("point (%f, %f) quad: %lu.\nd: %f, xfloor: %lu, yfloor: %lu, cellsperrow: %lu\n", xy.x, xy.y, quad, d, xfloor, yfloor, cellsperrow);
    return quad;
}

// calculates the euclidean distance between two points
double calculate_distance(point p1, point p2) {
    return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
}

// randomizes pairs of points in an array, and returns the minimum distance between all the pairs of points
double find_minimum_distance(point* point_array, int array_size, int n) {
    if (array_size <= 1 || n <= 0) {
        printf("Invalid input parameters.\n");
        return 0.0;
    }

    // Seed for random number generation
    srand((unsigned int)time(NULL));

    double min_distance = 2;

    for (int k = 0; k < n; ++k) {
        // Generate random indices f and g
        int f = rand() % array_size;
        int g = rand() % array_size;

        // Skip if f and g are the same
        if (f == g) {
            --k; // Try again with a different pair
            continue;
        }

        // Calculate distance between points at indices f and g
        double distance = calculate_distance(point_array[f], point_array[g]);

        // Update min_distance if necessary
        if (min_distance < 0 || distance < min_distance) {
            min_distance = distance;
        }

        // printf("Pair %d: Points at indices %d and %d, Distance: %lf\n", k + 1, f, g, distance);
    }

    return min_distance;
}

// Node structure for linked list in each hash table entry
typedef struct Node {
    point data;
    struct Node* next;
} Node;

// Hash table structure
typedef struct {
    int size;
    Node** table;
} HashTable;

// Function to create a new point
point create_point(float x, float y) {
    point p;
    p.x = x;
    p.y = y;
    return p;
}

// Function to create a new node
Node* create_node(point data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

// Function to initialize a hash table
HashTable* create_hash_table(uint64_t size) {
    HashTable* table = (HashTable*)malloc(sizeof(HashTable));
    if (table == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    table->size = size;
    table->table = (Node**)malloc(size * sizeof(Node*));
    if (table->table == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    // Initialize each entry in the hash table to NULL
    for (uint64_t i = 0; i < size; ++i) {
        table->table[i] = NULL;
    }

    return table;
}

// Function to insert a point into the hash table using a specified hash function
void insert_point(HashTable* table, point p, uint64_t index) {

    // Create a new node with the point data
    Node* newNode = create_node(p);

    // Insert the new node at the beginning of the linked list at the calculated index
    newNode->next = table->table[index];
    table->table[index] = newNode;
}

// Function to print all points in the hash table
void print_hash_table(HashTable* table) {
    printf("\nPrinting hash table:\n");
    for (uint64_t i = 0; i < table->size; ++i) {
        Node* current = table->table[i];
        printf("Index %lu:\n", i);
        while (current != NULL) {
            printf("    (%f, %f)\n", current->data.x, current->data.y);
            current = current->next;
        }
        // printf("\n");
    }
}

// Function to free memory allocated for the hash table
void free_hash_table(HashTable* table) {
    for (uint64_t i = 0; i < table->size; ++i) {
        Node* current = table->table[i];
        while (current != NULL) {
            Node* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(table->table);
    free(table);
}

// finds the smallest power of two that is bigger than a number and returns the exponent
int find_next_power_of_two(int number) {
    if (number <= 0) {
        return -1;  // Error: invalid input
    }

    int exponent = 0;
    unsigned long long result = 1;

    // Left shift until the result is greater than or equal to the input number
    while (result < number) {
        result <<= 1;  // Bitwise left shift by 1
        exponent++;
    }

    return exponent;
}

// compares a point in a hash table against all other element in the cells surrounding it and in the same cell
float compare_to_neighbors(HashTable* the_hash_table, point the_point, uint64_t index_of_point, int cells_for_row) {
    if (DEBUG) {printf("\nCompare to neighbors-> point (%f, %f):\n", the_point.x, the_point.y);}
    if (index_of_point < 0 || index_of_point >= the_hash_table->size) {
        printf("Invalid index\n");
        return 0.0;
    }

    float minimum_distance= 2;

    for (int sum1= -1; sum1 < 2; sum1++) {
        for (int sum2= -1; sum2 < 2; sum2++) {

            unsigned long long current_index= (unsigned long long) index_of_point + sum1 * cells_for_row + sum2;

            if (current_index < 0 || current_index >= the_hash_table-> size) {
                ;
            }

            else {
                if (DEBUG) {printf("index %llu, ", current_index);}
                Node* current_node_in_index= the_hash_table->table[index_of_point];

                while (current_node_in_index != NULL){

                    point point_data= current_node_in_index->data;
                    if (point_data.x != the_point.x && point_data.y != the_point.y) {

                        float current_distance= calculate_distance(point_data, the_point);
                        minimum_distance= current_distance < minimum_distance ? current_distance : minimum_distance;

                        if (DEBUG) {printf("distance: %f\n", minimum_distance);}
                    }

                    //printf("\n");
                    current_node_in_index= current_node_in_index-> next;
                }
            }
        }
    }
    return minimum_distance;

}

int main() {
    srand((unsigned int)time(NULL));

    long long n= 50 * 1;  // number of points to be added to the grid
    point* point_array = (point*) malloc(n * sizeof(point));

    // Initialize points with random coordinates
    for (int i = 0; i < n; ++i) {
        point_array[i].x = (double)rand() / RAND_MAX; // Random x-coordinate between 0 and 1
        point_array[i].y = (double)rand() / RAND_MAX; // Random y-coordinate between 0 and 1
    }

    print_point_array(point_array, n);

    double d= find_minimum_distance(point_array, n, n);  // finds the minimum distance per 2.3.1 
    int cells_for_row= (uint64_t) ceil(1/d);  // number of cells in each row/column (square grid)

    uint64_t k = 13; // mersenne: 31, 61, 89, 127.
    uint64_t p= (1 << k) - 1;
    // p is equal to 2^k - 1

    int l = find_next_power_of_two(pow(cells_for_row, 2)); // the smallest exponent needed to fulfill 
                                                      // 2^l >= number of cells in grid
    uint64_t m = 1 << l;
    // m is equal to 2^l

    HashTable* the_hash_table= create_hash_table(m);  // hash table of m= 2^l cells

    uint64_t a = 1 + rand() % p;  // a is randomized for every iteration
    uint64_t b = rand() % p;  // b is also

    printf("\nRunning hash table with:\np=%lu\nm= %lu\na= %lu\nb= %lu\n\n", p, m, a, b);

    // inserts every point in the point array in the hash table
    for(int i= 0; i< n; i++) {
        uint64_t quadrant= get_quadrant(point_array[i], d);
        long long index_to_insert= uni_hash(a, b, p, m, quadrant);
        insert_point(the_hash_table, point_array[i], index_to_insert);
    }

    print_hash_table(the_hash_table);
    
    float minimum_distance_global= 2; 

    // compares every point against all the points in surrounding cells in the hash table, and returns the 
    // smallest distance between them
    for(int i= 0; i< n; i++) {
        uint64_t quadrant= get_quadrant(point_array[i], d);
        long long hashed_index= uni_hash(a, b, p, m, quadrant);
        float minimum_distance_9_square= compare_to_neighbors(the_hash_table, point_array[i], hashed_index, cells_for_row);

        if (minimum_distance_9_square < minimum_distance_global) {minimum_distance_global= minimum_distance_9_square;}
    }

    printf("minimum distance global: %f\n", minimum_distance_global);

    free(point_array);
    free_hash_table(the_hash_table);
}