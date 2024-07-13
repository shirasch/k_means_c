#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct cord {
    double value;
    struct cord *next;
};

struct vector {
    struct vector *next;
    struct cord *cords;
};

void initialize_vector_array(struct vector *head_vec, struct vector **vector_array, int K) {
    struct vector *current = head_vec;
    int i = 0;

    while (current != NULL && i < K) {
        vector_array[i] = current;
        current = current->next;
        i++;
    }
}

void print_vectors(struct vector *head_vec) {
    struct vector *current_vector = head_vec;

    while (current_vector != NULL) {
        struct cord *current_cord = current_vector->cords;

        printf("Vector: ");
        while (current_cord != NULL) {
            printf("%.5lf ", current_cord->value);
            current_cord = current_cord->next;
        }
        printf("\n");

        current_vector = current_vector->next;
    }
}

struct vector* getinput(char *input_file, int *num_vectors) {
    struct vector *head_vec = NULL, *curr_vec = NULL;
    struct cord *head_cord = NULL, *curr_cord = NULL;
    double n;
    char c;
    *num_vectors = 0;

    FILE *fp = fopen(input_file, "r");

    if (fp == NULL) {
        perror("Error opening file");
        return NULL;
    }

    while (fscanf(fp, "%lf%c", &n, &c) == 2) {
        struct cord *new_cord = malloc(sizeof(struct cord));
        new_cord->value = n;
        new_cord->next = NULL;

        if (head_cord == NULL) {
            head_cord = new_cord;
            curr_cord = head_cord;
        } else {
            curr_cord->next = new_cord;
            curr_cord = new_cord;
        }

        if (c == '\n') {
            if (head_cord != NULL) {
                if (head_vec == NULL) {
                    head_vec = malloc(sizeof(struct vector));
                    curr_vec = head_vec;
                } else {
                    curr_vec->next = malloc(sizeof(struct vector));
                    curr_vec = curr_vec->next;
                }
                curr_vec->cords = head_cord;
                curr_vec->next = NULL;
                head_cord = NULL;
                curr_cord = NULL;
                (*num_vectors)++;
            }
        }
    }

    fclose(fp);
    return head_vec;
}

int main(int argc, char **argv) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <K> <iter> <input_file>\n", argv[0]);
        return 1;
    }

    int K = atoi(argv[1]);
    int iter = atoi(argv[2]);
    char *input_file = argv[3];
    int num_vectors;

    struct vector *head_vec = getinput(input_file, &num_vectors);
    if (head_vec == NULL) {
        return 1;
    }


    // Create array of K vectors
    struct vector **vector_array = malloc(K * sizeof(struct vector *));
    initialize_vector_array(head_vec, vector_array, K);

    struct vector **vector_array = malloc(num_vectors * sizeof(struct vector *));
    
    // print_vectors(head_vec);
    


    // Free memory
    struct vector *temp_vec;
    while (head_vec != NULL) {
        temp_vec = head_vec;
        head_vec = head_vec->next;
        struct cord *temp_cord;
        while (temp_vec->cords != NULL) {
            temp_cord = temp_vec->cords;
            temp_vec->cords = temp_vec->cords->next;
            free(temp_cord);
        }
        free(temp_vec);
    }

    free(vector_array);

    return 0;
}
