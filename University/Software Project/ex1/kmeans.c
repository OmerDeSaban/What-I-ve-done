#define  _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct coordinate {     /* Holds value of current coodinate and pointer to the next one: */
    double value;
    struct coordinate* next;
};

struct datapoint {      /* Holds pointers to the next datapoint and the first coordinat of the current datapoint,
                            as well as the index of the centroid in the centroid matrix to which this datapoint
                            is closest: */
    struct datapoint* next;
    struct coordinate* pointer;
    int cluster;
};

typedef struct coordinate coordinate;
typedef struct datapoint datapoint;

int k;
int num_of_points;
int max_iter;
int dimention;
double **centroids;
double **new_centroids;
const double EPSILON = 0.001;

void print_error(const char *);
datapoint* readText();
void initilize_original_centroids (datapoint*);
double distance(datapoint*, double*);
double distance_doubles(double*, double*);
void closest_cluster(datapoint*);
void update_centroid(datapoint*);
int delta_centroid();
void print_result();
void free_memory(datapoint*, int, int);
void swapCentroids(double ***, double***);

void print_error(const char *error) {      /* Generating an error message and exiting the program: */
    printf("%s\n", error);     /* Prints the error message. */
    exit(1);    /* exit(1) == exit(EXIT_FAILURE) */
}

datapoint* readText() {
    datapoint* head_point;
    datapoint* curr_point;
    datapoint* prev_point;
    coordinate* head_cord;
    coordinate* curr_cord;
    double curr_value;
    char is_char;
    int done;   /* A variable that denotes if we've already found the value of dimention. */
    int curr_dim;     /* A variable that denotes the dimention of the current datapoint. */
    done = 0;
    curr_dim = 0;
    num_of_points = 0;
    dimention = 0;
    /* Allocate memory for the first coordinate: */
    head_cord = malloc(sizeof(coordinate));
    if (head_cord == NULL) {
        print_error("An Error Has Occurred");
    }
    curr_cord = head_cord;
    curr_cord->next = NULL;
    /* Allocate memory for the first datapoint: */
    head_point = malloc(sizeof(datapoint));
    if (head_point == NULL) {
        free(head_cord);
        print_error("An Error Has Occurred");
    }
    curr_point = head_point;
    curr_point->next = NULL;
    /* Read the input and create linked lists that represent the datapoints and their coordinats: */
    while (scanf("%lf%c", &curr_value, &is_char) == 2) {
        if (is_char == '\n') {      /* In case we get to the end of the line - finished the current
                                        datapoint: */
            num_of_points++;
            curr_cord->value = curr_value;
            curr_point->pointer = head_cord;
            head_cord = malloc(sizeof(coordinate));
            if (head_cord == NULL) {
                free(head_cord);
                print_error("An Error Has Occurred");
            }
            curr_point->next = malloc(sizeof(datapoint));
            if (curr_point->next == NULL) {
                free_memory(curr_point->next, 0, 0);
                print_error("An Error Has Occurred");
            }
            prev_point = curr_point;
            curr_point = curr_point->next;
            curr_cord = head_cord;
            curr_cord->next = NULL;
            if (done == 0) {
                dimention = curr_dim + 1;
                done = 1;
            }
            /* If you want to validate that each datapoint has the same dimention, just delete this
            comment and maintain the following code (which is also commented): */
            /*
            else {
                if (dimention != (curr_dim + 1)) {
                    print_error("An Error Has Occurred");
                }
            }
            curr_dim = 0;
            */
            /* Note that there are a few more lines regarding this verification a bit further down! */
            continue;
        }
        else if (is_char == ',') {   /* New coordinate of the datapoint: */
            curr_cord->value = curr_value;
            curr_cord->next = malloc(sizeof(coordinate));
            if (curr_cord->next == NULL) {
                free(curr_cord->next);
                print_error("An Error Has Occurred");
            }
            curr_cord = curr_cord->next;
            curr_cord->next = NULL;
            /* If you want to validate that every datapoint has the same dimention, delete this comment
            and comment the following if statement. Then uncomment the line after it: */
            if (done == 0) {
                curr_dim++;
            }
            /*
            curr_dim++;
            */
        }
        else {  /* is_char isn't ',' or '\n', thus we received a datapoint with an illegal character! */
            print_error("An Error Has Occurred");
        }
    }
    prev_point->next = NULL;
    free(curr_cord);
    free(curr_point);
    if (k >= num_of_points) {
        print_error("Invalid number of clusters!");
    }
    return head_point;
}

void initilize_original_centroids (datapoint* point_head) {     /* Creating 2 2D matrixes for the centroids,
                                                                    putting the first k datapionts in the
                                                                    centroid matrix: */
    int i;
    int j;
    coordinate* curr_coordinate;
    curr_coordinate = point_head->pointer;
    /* Allocate memory for the centriods matrix: */
    centroids = malloc(k * sizeof(double*));
    if (centroids == NULL) {
        free_memory(point_head,0 ,0);
        print_error("An Error Has Occurred");
    }
    new_centroids = malloc(k * sizeof(double*));
    if (new_centroids == NULL) {
        free_memory(point_head,0 ,0);
        print_error("An Error Has Occurred");
    }
    /* Allocate memory for each row in the centroids matrix (each datapoint): */
    for (i = 0; i < k; i++) {
        centroids[i] = malloc(dimention * sizeof(double));
        if (centroids[i] == NULL) {
            free_memory(point_head, i - 1, i - 1);
            print_error("An Error Has Occurred");
        }
        new_centroids[i] = malloc(dimention * sizeof(double));
        if (new_centroids[i] == NULL) {
            free_memory(point_head, i, i - 1);
            print_error("An Error Has Occurred");
        }
    }
    for (i = 0; i < k; i++) {
        for (j = 0; j < dimention; j++) {
            centroids[i][j] = curr_coordinate->value;
            curr_coordinate = curr_coordinate->next;
        }
        point_head = point_head->next;
        curr_coordinate = point_head->pointer;
        }  
    
}

double distance(datapoint* p1, double* p2) {    /* Calculating eclidean distance between datapoint p1 and
                                                    array of doubles p2: */
    double sum;
    int i;
    coordinate* value1;
    sum = 0.0;
    i = 0;
    value1 = p1->pointer;
    while (value1 != NULL) {
        sum += (value1->value - p2[i]) * (value1->value - p2[i]);
        i++;
        value1 = value1->next;
    }
    return sqrt(sum);
}

double distance_doubles(double* p1, double* p2) {       /* Calculating eclidean distance between array of
                                                            doubles p1 and array of doubles p2: */
    double sum;
    int i;
    sum = 0.0;
    for (i = 0; i < dimention; i++) {
        sum += (p1[i] - p2[i]) * (p1[i] - p2[i]);
    }
    return sqrt(sum);
}

void closest_cluster(datapoint* point_head) {       /* Find the closest centroid for datapoint and assiging
                                                        the cluster: */
    double min_distance;
    double curr_distance;
    int min_index;
    int i;
    min_distance = distance(point_head, centroids[0]);
    min_index = 0;
    for (i = 1; i < k; i++){
        curr_distance = distance(point_head, centroids[i]);
        if (curr_distance < min_distance) {
            min_distance = curr_distance;
            min_index = i;
        }
    }
    point_head->cluster = min_index;
}

void update_centroid(datapoint* point_head) {       /* Updatde the new centroids: */
    int i;
    int j;
    int point_counter;
    datapoint* curr_point;
    coordinate* curr_coordinate;
    double* sums;
    point_counter = 0;
    curr_point = point_head;
    sums = calloc(dimention, sizeof(double*));
    if (sums == NULL) {
        free_memory(point_head, k, k);
        print_error("An Error Has Occurred");
    }
    for (i = 0; i < k; i++) {
        while (curr_point != NULL) {
            if (curr_point->cluster == i) {
                curr_coordinate = curr_point->pointer;
                point_counter++;
                for (j = 0; j < dimention; j++) {
                    sums[j] += curr_coordinate->value;
                    curr_coordinate = curr_coordinate->next;
                }
            }
            curr_point = curr_point->next;
        }
        for (j = 0; j < dimention; j++) {
            new_centroids[i][j] = sums[j] / point_counter;
            sums[j] = 0.0;
        }
        curr_point = point_head;
        point_counter = 0;
    }
    free(sums);
}

int delta_centroid() {      /* Calculating the distance betwwen the old centroisd and the new: */
    int i;
    for (i = 0; i < k; i++) {

        if (distance_doubles(centroids[i], new_centroids[i]) >= EPSILON) {
            return 0;
        }
    }
    return 1;
}

void print_result() {       /* Printing the outcome: */
    int i;
    int j;
    for (i = 0; i < k; i++) {
        for (j = 0; j < dimention; j++) {
            if (j == dimention - 1) {
                printf("%.4f\n", centroids[i][j]);
            }
            else {
                printf("%.4f,", centroids[i][j]);
            }
        }
    }
}

void free_memory(datapoint* head, int centroids_size, int new_centroids_size) {
    int i;
    datapoint* tmp_datapoint;
    coordinate* tmp_cord;
    while (head != NULL) {
        while (head->pointer != NULL) {
            tmp_cord = head->pointer;
            head->pointer = head->pointer->next;
            free(tmp_cord);
        }
        tmp_datapoint = head;
        head = head->next;
        free(tmp_datapoint);
    }

    /* Free centroids: */
    for (i = 0; i < centroids_size; i++) {
        free(centroids[i]);
    }
    free(centroids);

    /* Free new_centroids: */
    for (i = 0; i < new_centroids_size; i++) {
        free(new_centroids[i]);
    }
    free(new_centroids);
}

void swapCentroids(double ***old_centroids, double ***centroids_new) {
    double **tmp = *old_centroids;
    *old_centroids = *centroids_new;
    *centroids_new = tmp;
}

int main(int argc, char* argv[]) {
    int num_iteration;
    datapoint* head;
    datapoint* curr_point;
    double v1;
    char *ptr;
    num_iteration = 0;
    if (argc < 2 || argc > 3) {
        print_error("An Error Has Occurred");
    }
    v1 = strtod(argv[1], &ptr);
    if (v1 != floor(v1) || *ptr != '\0' || v1 <= 1) {
        print_error("Invalid number of clusters!");
    }
    k = atoi(argv[1]);      /* The only thing left to validate abouot k is that it is smaller than the number
                                of datapoints! */

    if (argc == 2) {    /* If there is no argument passed for max number of iterations... */
        max_iter = 200;
    }
    if (argc == 3) {
        v1 = strtod(argv[2], &ptr);
        if (v1 != floor(v1) || *ptr != '\0' || v1 <= 1 || v1 >= 1000) {
            print_error("Invalid maximum iteration!");
        }
        max_iter = atoi(argv[2]);
    }
    head = readText();
    curr_point = head;
    initilize_original_centroids(head);
    while (num_iteration < max_iter) {
        while (curr_point != NULL) {
            closest_cluster(curr_point);
            curr_point = curr_point->next;
        }
        update_centroid(head);
        if (delta_centroid() == 1) {
            swapCentroids(&centroids, &new_centroids);
            break;
        }
        swapCentroids(&centroids, &new_centroids);
        num_iteration++;
        curr_point = head;
    }
    print_result();
    free_memory(head, k, k);
    return 0;
}
