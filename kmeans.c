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

struct vector* duplicate_vector(struct vector *original) {
    struct vector *dup = malloc(sizeof(struct vector));
    struct cord *orig_cord = original->cords;
    struct cord *dup_cord_head = NULL, *dup_cord_curr = NULL;

    while (orig_cord != NULL) {
        struct cord *new_cord = malloc(sizeof(struct cord));
        new_cord->value = orig_cord->value;
        new_cord->next = NULL;
        if (dup_cord_head == NULL) {
            dup_cord_head = new_cord;
            dup_cord_curr = dup_cord_head;
        } else {
            dup_cord_curr->next = new_cord;
            dup_cord_curr = new_cord;
        }
        orig_cord = orig_cord->next;
    }
    dup->cords = dup_cord_head;
    dup->next = NULL;
    return dup;
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
void print_vector_array(struct vector **vector_array, int num_vectors) {
    for (int i = 0; i < num_vectors; i++) {
        struct vector *current_vector = vector_array[i];
        printf("Duplicated Vector %d: ", i + 1);
        while (current_vector != NULL) {
            struct cord *current_cord = current_vector->cords;
            while (current_cord != NULL) {
                printf("%.5lf ", current_cord->value);
                current_cord = current_cord->next;
            }
            current_vector = current_vector->next;
        }
        printf("\n");
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

double calculate_distance(struct vector *vec1, struct vector *vec2) {
    struct cord *cord1 = vec1->cords;
    struct cord *cord2 = vec2->cords;
    double sum = 0.0;

    while (cord1 != NULL && cord2 != NULL) {
        double diff = cord1->value - cord2->value;
        sum += diff * diff;
        cord1 = cord1->next;
        cord2 = cord2->next;
    }

    if (cord1 != NULL || cord2 != NULL) {
        fprintf(stderr, "Error: Vectors have different lengths.\n");
        exit(1);
    }

    return sqrt(sum);
}

void find_closest_vectors(struct vector **vector_array, int num_vectors, struct vector **m_array, int K, int *closest_indices) {
    for (int i = 0; i < num_vectors; i++) {
        struct vector *current_vec = vector_array[i];
        int closest_index = -1;
        double min_distance = INFINITY;

        for (int j = 0; j < K; j++) {
            struct vector *m_vec = m_array[j];
            double distance = calculate_distance(current_vec, m_vec);
            if (distance < min_distance) {
                min_distance = distance;
                closest_index = j;
            }
        }

        closest_indices[i] = closest_index;
    }
}

int main(int argc, char **argv) {
    if (argc < 3) {
        fprintf(stderr, "An Error Has Occurred", argv[0]);
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




    struct vector **vector_array = malloc(num_vectors * sizeof(struct vector *));
    struct vector *current = head_vec;
    for (int i = 0; i < num_vectors; i++) {
        if (current == NULL) {
            fprintf(stderr, "Error: Insufficient vectors in input file.\n");
            return 1;
        }
        vector_array[i] = current;
        current = current->next;
    }

    // initilize array of K duplicated vectors
    struct vector **m_array = malloc(K * sizeof(struct vector *));
    for (int i = 0; i < K; i++) {
        if (i >= num_vectors) {
            fprintf(stderr, "Error: K is greater than the number of vectors.\n");
            return 1;
        }
        m_array[i] = duplicate_vector(vector_array[i]);
    }

    int *closest_indices = malloc(num_vectors * sizeof(int));

    // Find the closest vectors and store their indices
    find_closest_vectors(vector_array, num_vectors, m_array, K, closest_indices);


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

    for (int i = 0; i < K; i++) {
        struct vector *dup_vec = m_array[i];
        while (dup_vec != NULL) {
            temp_vec = dup_vec;
            dup_vec = dup_vec->next;
            struct cord *temp_cord;
            while (temp_vec->cords != NULL) {
                temp_cord = temp_vec->cords;
                temp_vec->cords = temp_vec->cords->next;
                free(temp_cord);
            }
            free(temp_vec);
        }
    }

    free(vector_array);
    free(m_array);

    return 0;
}
