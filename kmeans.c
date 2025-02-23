#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct cord
{
    double value;
    struct cord *next;
};

struct vector
{
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
void cleanup(int k);

void print_vectors(struct vector *head_vec);
void print_m_array(struct vector **m_array, int K);

void print_m_array(struct vector **m_array, int K)
{
    int i;
    for (i = 0; i < K; i++)
    {
        print_vectors(m_array[i]);
    }
}

struct vector *duplicate_vector(struct vector *original)
{
    struct vector *dup = malloc(sizeof(struct vector));
    struct cord *orig_cord = original->cords;
    struct cord *dup_cord_head = NULL, *dup_cord_curr = NULL;
    struct cord *new_cord;
    while (orig_cord != NULL)
    {
        new_cord = malloc(sizeof(struct cord));
        new_cord->value = orig_cord->value;
        new_cord->next = NULL;
        if (dup_cord_head == NULL)
        {
            dup_cord_head = new_cord;
            dup_cord_curr = dup_cord_head;
        }
        else
        {
            dup_cord_curr->next = new_cord;
            dup_cord_curr = new_cord;
        }
        orig_cord = orig_cord->next;
    }
    dup->cords = dup_cord_head;
    dup->next = NULL;
    return dup;
}

void print_vectors(struct vector *head_vec)
{
    struct vector *current_vector = head_vec;
    struct cord *current_cord;

    while (current_vector != NULL)
    {
        current_cord = current_vector->cords;

        while (current_cord->next != NULL)
        {
            printf("%.4f,", current_cord->value);
            current_cord = current_cord->next;
        }
        printf("%.4f", current_cord->value);
        printf("\n");

        current_vector = current_vector->next;
    }
}

struct vector *getinput(int *num_vectors)
{
    struct vector *head_vec = NULL, *curr_vec = NULL;
    struct cord *head_cord = NULL, *curr_cord = NULL;
    double n;
    char c;
    *num_vectors = 0;

    while (scanf("%lf%c", &n, &c) == 2)
    {
        struct cord *new_cord = malloc(sizeof(struct cord));
        new_cord->value = n;
        new_cord->next = NULL;

        if (head_cord == NULL)
        {
            head_cord = new_cord;
            curr_cord = head_cord;
        }
        else
        {
            curr_cord->next = new_cord;
            curr_cord = new_cord;
        }

        if (c == '\n')
        {
            if (head_cord != NULL)
            {
                if (head_vec == NULL)
                {
                    head_vec = malloc(sizeof(struct vector));
                    curr_vec = head_vec;
                }
                else
                {
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

    return head_vec;
}

int is_int(const char *str) {
    double k;
    char *endptr;

    if (*str == '\0' || *str == '-') {
        return 0;
    }

    k = strtod(str, &endptr);

    if (*endptr == '\0' && k == floor(k)) {
        return 1;
    }

    return 0;
}

double calculate_distance(struct vector *vec1, struct vector *vec2)
{
    struct cord *cord1 = vec1->cords;
    struct cord *cord2 = vec2->cords;
    double sum = 0.0;
    double diff;

    while (cord1 != NULL && cord2 != NULL)
    {
        diff = cord1->value - cord2->value;
        sum += diff * diff;
        cord1 = cord1->next;
        cord2 = cord2->next;
    }

    if (cord1 != NULL || cord2 != NULL)
    {
        printf("An Error Has Occurred\n");
        exit(1);
    }

    return sqrt(sum);
}

void find_closest_vectors(struct vector **vector_array, int num_vectors, struct vector **m_array, int K, int *closest_indices)
{
    int i;
    int j;
    int closest_index;
    struct vector *current_vec;
    double min_distance;
    struct vector *m_vec;
    double distance;
    for (i = 0; i < num_vectors; i++)
    {
        current_vec = vector_array[i];
        closest_index = -1;
        min_distance = -1.0;

        for (j = 0; j < K; j++)
        {
            m_vec = m_array[j];
            distance = calculate_distance(current_vec, m_vec);
	    
            if (distance < min_distance || min_distance < 0)
            {
                min_distance = distance;
                closest_index = j;
            }
        }
        closest_indices[i] = closest_index;
    }
}

void sum_vectors(struct vector *vec1, struct vector *vec2)
{
    struct cord *cord1 = vec1->cords;
    struct cord *cord2 = vec2->cords;

    while (cord1 != NULL && cord2 != NULL)
    {
        cord1->value = cord1->value + cord2->value;
        cord1 = cord1->next;
        cord2 = cord2->next;
    }
}

struct vector *devide_sum(struct vector *sum_vector, int divisor)
{
    struct vector *result = malloc(sizeof(struct vector));
    struct cord *head_result_cord = NULL;
    struct cord *curr_result_cord = NULL;
    struct cord *new_cord;

    struct cord *cord = sum_vector->cords;

    while (cord != NULL)
    {
        new_cord = malloc(sizeof(struct cord));
        new_cord->value = cord->value / (double)divisor;
        new_cord->next = NULL;

        if (head_result_cord == NULL)
        {
            head_result_cord = new_cord;
            curr_result_cord = head_result_cord;
        }
        else
        {
            curr_result_cord->next = new_cord;
            curr_result_cord = new_cord;
        }

        cord = cord->next;
    }

    result->cords = head_result_cord;
    result->next = NULL;

    return result;
}

int check_convergence(int n, struct vector **new_means, struct vector **old_means)
{
    double e = 0.001;
    double total_movement = 0.0;
    int i;
    int result;

    for (i = 0; i < n; i++)
    {
        total_movement += calculate_distance(new_means[i], old_means[i]);
    }
    result = total_movement < e ? 1 : 0;

    return result;
}

void free_vector_mem(struct vector *vec)
{
    struct vector *temp_vec;
    struct cord *temp_cord;
    while (vec != NULL)
    {
        temp_vec = vec;
        vec = vec->next;
        while (temp_vec->cords != NULL)
        {
            temp_cord = temp_vec->cords;
            temp_vec->cords = temp_vec->cords->next;
            free(temp_cord);
        }
        free(temp_vec);
    }
}

void free_array_memory(struct vector **array,int length)
{
    int i;
    for (i = 0; i<length; i++)
    {
        free_vector_mem(array[i]);
    }
    free(array);
}

void cleanup(int K)
{
    if (vector_array)
    {
        free(vector_array);

    }
    if(head_vec){
        free_vector_mem(head_vec);
    }
    if (m_array)
    {
        free_array_memory(m_array, K);

    }
    if (closest_indices)
    {
        free(closest_indices);
    }
    if (counters_array)
    {
        free(counters_array);
    }
    if (sums_array)
    {
        free(sums_array);
    }
}

int main(int argc, char **argv)
{
    int K;
    int iter;
    int num_vectors;
    int i;
    int converged = 0;
    int h;
    int j;
    int m_index;
    struct vector **new_m_array; /*new centroids array*/
    struct vector *current;

    if (argc < 2)
    {
        printf("An Error Has Occurred\n");
        return 1;
    }

    if (argc > 3)
    {
        printf("An Error Has Occurred\n");
        return 1;
    }

    if (!is_int(argv[1])){
        printf("Invalid number of clusters!\n");
        return 1;
    }
    K = atoi(argv[1]);

    if (argc == 2)
    {
        iter = 200;
    }
    else
    {
        if(!is_int(argv[2])){
            printf("Invalid maximum iteration!\n");
            return 1;
        }
        iter = atoi(argv[2]);
        if (iter < 2 || iter > 999)
        {
            printf("Invalid maximum iteration!\n");
            return 1;
        }
    }

    head_vec = getinput(&num_vectors);
    if (head_vec == NULL)
    {
        printf("An Error Has Occurred\n");
        cleanup(0);
        return 1;
    }


    if (K > num_vectors - 1 || K < 2)
    {
        printf("Invalid number of clusters!\n");
        cleanup(0);
        return 1;
    }

    vector_array = malloc(num_vectors * sizeof(struct vector *));
    if (vector_array == NULL) {
        printf("An Error Has Occurred\n");
        cleanup(K);
        return 1;
    }

    current = head_vec;
    for (i = 0; i < num_vectors; i++)
    {
        vector_array[i] = current;
        current = current->next;
    }

    m_array = malloc(K * sizeof(struct vector *));
    if (m_array == NULL) {
        printf("An Error Has Occurred\n");
        cleanup(K);
        return 1;
    }

    for (i = 0; i < K; i++)
    {
        if (i >= num_vectors)
        {
            printf("An Error Has Occurred\n");
            return 1;
        }
        m_array[i] = duplicate_vector(vector_array[i]);
    }

    closest_indices = malloc(num_vectors * sizeof(int));
    if (closest_indices == NULL) {
        printf("An Error Has Occurred\n");
        cleanup(K);
        return 1;
    }
    counters_array = malloc(K * sizeof(int));
    if (counters_array == NULL) {
        printf("An Error Has Occurred\n");
        cleanup(K);
        return 1;
    }

    sums_array = malloc(K * sizeof(struct vector *));
    if (sums_array == NULL) {
        printf("An Error Has Occurred\n");
        cleanup(K);
        return 1;
    }
    i = 0;

    new_m_array = malloc(K * sizeof(struct vector *));
    if (new_m_array == NULL) {
        printf("An Error Has Occurred\n");
        cleanup(K);
        return 1;
    }

    while ((i < iter) && (!converged))
    {
        find_closest_vectors(vector_array, num_vectors, m_array, K, closest_indices);

        for (j = 0; j < K; j++)
        {
            counters_array[j] = 0;
        }

        for (j = 0; j < K; j++)
        {
            sums_array[j] = NULL;
        }
        for (h = 0; h < num_vectors; h++)
        {
            m_index = closest_indices[h];
            if (sums_array[m_index] == NULL)
            {
                sums_array[m_index] = duplicate_vector(vector_array[h]);
            }
            else
            {
                sum_vectors(sums_array[m_index], vector_array[h]);
            }
            counters_array[m_index]++;
        }

        for (h = 0; h < K; h++)
        {
            new_m_array[h] = devide_sum(sums_array[h], counters_array[h]);
            free_vector_mem(sums_array[h]);
        }

        converged = check_convergence(K, new_m_array, m_array);

        for (j = 0; j < K; j++)
        {
            free_vector_mem(m_array[j]);
            m_array[j] = new_m_array[j];
        }
        i++;
    }
    
    free(new_m_array);
    print_m_array(m_array, K);

    cleanup(K);

    return 0;
}
