#include <stdio.h>
#include <stdlib.h>

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

int main(int argc, char **argv) {
    struct vector *head_vec = NULL, *curr_vec = NULL;
    struct cord *head_cord = NULL, *curr_cord = NULL;
    double n;
    char c;

    // add validation tests

    int K = atoi(argv[1]);
    int iter = atoi(argv[2]);
    char *input_file = argv[3];
    FILE *fp = fopen(input_file, "r");

    if (fp == NULL) {
        perror("Error opening file");
        return 1;
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
            }
        }
    }

    fclose(fp);

    // Create array of K vectors
    struct vector **vector_array = malloc(K * sizeof(struct vector *));
    initialize_vector_array(head_vec, vector_array, K);

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

    return 0;
}
