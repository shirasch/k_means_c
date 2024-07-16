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

struct vector **vector_array = NULL;
struct vector **m_array = NULL;
int *closest_indices = NULL;
int *counters_array = NULL;
struct vector **sums_array = NULL;
struct vector *head_vec = NULL;
void free_vector_mem(struct vector *vec);
void cleanup(void);

void print_vectors(struct vector *head_vec);
void print_m_array(struct vector **m_array, int K);

void print_m_array(struct vector **m_array, int K) {
    int i;
    for (i = 0; i < K; i++) {
        print_vectors(m_array[i]);
    }
}

struct vector* duplicate_vector(struct vector *original) {
    struct vector *dup = malloc(sizeof(struct vector));
    struct cord *orig_cord = original->cords;
    struct cord *dup_cord_head = NULL, *dup_cord_curr = NULL;
    struct cord *new_cord;

    while (orig_cord != NULL) {
        new_cord = malloc(sizeof(struct cord));
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
    struct cord *current_cord;

    while (current_vector != NULL) {
        current_cord = current_vector->cords;

        while (current_cord->next != NULL) {
            printf("%.4f,", current_cord->value);
            current_cord = current_cord->next;
        }
        printf("%.4f", current_cord->value);
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

// calculate the distance between 2 vectors
double calculate_distance(struct vector *vec1, struct vector *vec2) {
    struct cord *cord1 = vec1->cords;
    struct cord *cord2 = vec2->cords;
    double sum = 0.0;
    double diff;

    while (cord1 != NULL && cord2 != NULL) {
        diff = cord1->value - cord2->value;
        sum += diff * diff;
        cord1 = cord1->next;
        cord2 = cord2->next;
    }

    if (cord1 != NULL || cord2 != NULL) {
        printf("Error: Vectors have different lengths.\n");
        exit(1);
    }

    return sqrt(sum);
}

void find_closest_vectors(struct vector **vector_array, int num_vectors, struct vector **m_array, int K, int *closest_indices) {
    int i;
    int j;
    int closest_index;
    struct vector *current_vec;
    double min_distance;
    struct vector *m_vec;
    double distance;
    for (i = 0; i < num_vectors; i++) {
        current_vec = vector_array[i];
        closest_index = -1;
        min_distance = 1.7976931348623157e+308;

        for (j = 0; j < K; j++) {
            m_vec = m_array[j];
            distance = calculate_distance(current_vec, m_vec);
            if (distance < min_distance) {
                min_distance = distance;
                closest_index = j;
            }
        }
        closest_indices[i] = closest_index;
    }
}

void sum_vectors(struct vector *vec1, struct vector *vec2) {
    struct cord *cord1 = vec1->cords;
    struct cord *cord2 = vec2->cords;

    while (cord1 != NULL && cord2 != NULL) {
        cord1->value = cord1->value + cord2->value;
        cord1 = cord1->next;
        cord2 = cord2->next;
    }
}

struct vector* devide_sum(struct vector *sum_vector, int divisor) {
    struct vector *result = malloc(sizeof(struct vector));
    struct cord *head_result_cord = NULL;
    struct cord *curr_result_cord = NULL;
    struct cord *new_cord;

    struct cord *cord = sum_vector->cords;

    while (cord != NULL) {
        new_cord = malloc(sizeof(struct cord));
        new_cord->value = cord->value / (double) divisor;  // Ensure division is floating-point
        new_cord->next = NULL;

        if (head_result_cord == NULL) {
            head_result_cord = new_cord;
            curr_result_cord = head_result_cord;
        } else {
            curr_result_cord->next = new_cord;
            curr_result_cord = new_cord;
        }

        cord = cord->next;
    }

    result->cords = head_result_cord;
    result->next = NULL;

    return result;
}

int check_convergence(int n, struct vector **new_means, struct vector **old_means) {
    double e = 0.001;
    double total_movement = 0.0;
    int i;
    int result;

    for (i = 0; i < n; i++) {
        total_movement += calculate_distance(new_means[i], old_means[i]);
        
    }
    result = total_movement<e ? 1 : 0;

    return result;
}

void free_vector_mem(struct vector *vec){
    struct vector *temp_vec;
    struct cord *temp_cord;
    while (vec != NULL) {
        temp_vec = vec;
        vec = vec->next;
        while (temp_vec->cords != NULL) {
            temp_cord = temp_vec->cords;
            temp_vec->cords = temp_vec->cords->next;
            free(temp_cord);
        }
        free(temp_vec);
    }
}

void free_array_memory(struct vector **array){
    int i;
    for (i = 0; (array[i] != NULL) ; i++){
        free_vector_mem(array[i]);
    }
    free(array);
}

void cleanup(void) {
    if (vector_array) {
        free_array_memory(vector_array);
    }
    if (m_array) {
        free_array_memory(m_array);
    }
    if (closest_indices) {
        free(closest_indices);
    }
    if (counters_array) {
        free(counters_array);
    }
    if (sums_array) {
        free_array_memory(sums_array);
    }
    if (head_vec) {
        free_vector_mem(head_vec);
    }
}


int main(int argc, char **argv) {
    int K;
    int iter;
    int num_vectors;
    int i;
    int converged = 0;
    int h;
    int j;
    int m_index;
    int *closest_indices;
    int *counters_array;
    struct vector **vector_array;
    struct vector *current;
    struct vector **m_array;
    struct vector **sums_array;
    struct vector **new_m_array;

    if (argc < 2) {
        //fprintf("An Error Has Occurred\n");
        //return 1;
    }
    //K = atoi(argv[1]);

    m_index = 0;
    K = 3;
    iter = 100;
    char input_file[12] = "input_1.txt";

    
    if (argc == 3){
        iter = atoi(argv[2]);
    }
    else{
        iter = 200;
    }

    struct vector *head_vec = getinput(input_file, &num_vectors);
    if (head_vec == NULL) {
        return 1;
    }

    vector_array = malloc(num_vectors * sizeof(struct vector *));
    current = head_vec;
    for (i = 0; i < num_vectors; i++) {
        if (current == NULL) {
            printf("Error: Insufficient vectors in input file.\n");
            return 1;
        }
        vector_array[i] = current;
        current = current->next;
    }

    m_array = malloc(K * sizeof(struct vector *));
    
    for (i = 0; i < K; i++) {
        if (i >= num_vectors) {
            printf("Error: K is greater than the number of vectors.\n");
            return 1;
        }
        m_array[i] = duplicate_vector(vector_array[i]);
    }


    closest_indices = malloc(num_vectors * sizeof(int));
    counters_array = malloc(K * sizeof(int));

    sums_array = malloc(K * sizeof(struct vector *));
    i = 0;

    while((i < iter) && (!converged)){
        find_closest_vectors(vector_array, num_vectors, m_array, K, closest_indices);

        for (j = 0; j < K; j++){
            counters_array[j] = 0;
        }

        for (j = 0; j < K; j++) {
            sums_array[j] = NULL;
        }
        for(h = 0; h < num_vectors; h++){
            m_index = closest_indices[h];
            if (sums_array[m_index] == NULL){
                sums_array[m_index] = duplicate_vector(vector_array[h]);
            }
            else{
                sum_vectors(sums_array[m_index], vector_array[h]);
            }
            counters_array[m_index]++;
        }

        new_m_array = malloc(K * sizeof(struct vector *));

        for(h = 0; h < K; h++){
            new_m_array[h] = devide_sum(sums_array[h], counters_array[h]);
        }

        converged = check_convergence(K ,new_m_array, m_array);
        
        for(int j = 0; j < K; j++){
            free_vector_mem(m_array[j]);
            m_array[j] = new_m_array[j];
        }
        i++;
    }

    print_m_array(m_array, K);

    cleanup();

    return 0;
}