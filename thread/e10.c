#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct thread_data {
	int thread_id;
	int result;
};

int row_size;
int col_size;

int **matrix;
int *vector;

void print_matrix(int **mat, int rows, int cols) {
	printf(" *** Matrix ***\n");
	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < cols; c++) {
			printf("[ %d ] ", mat[r][c]);
		}
		printf("\n");
	}
}
void print_vector(int *vec, int size) {
	printf(" *** Vector ***\n");
	for (int i = 0; i < size; i++) {
		printf("[ %d ]\n", vec[i]);
	}
}

void generate_matrix(int **mat, int rows, int cols) {
	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < cols; c++) {
			mat[r][c] = rand() % 10;
		}
	}
}
void generate_vector(int *vec, int size) {
	for (int i = 0; i < size; i++) {
		vec[i] = rand() % 10;
	}
}

/* DO NOT MODIFY ABOVE */

void *thread_mvm(void *arg) {
	((struct thread_data*)arg)->result = 0;
	for(int i=0; i < col_size; i++)
		((struct thread_data*)arg)->result += matrix[((struct thread_data*)arg)->thread_id][i] * vector[i];
}

/* DO NOT MODIFY MAIN CODE BELOW
** JUST REPLACE COMMENTED LINE WITH YOUR OWN CODE 
** DO NOT ADD/REMOVE BLANK LINE
*/
int main(int argc, char *argv[]) {

	if (argc != 3) {
		printf("Usage: %s <row> <column>\n", argv[0]);
		exit(1);
	}

	row_size = atoi(argv[1]);
	col_size = atoi(argv[2]);
	pthread_t tid[row_size];
	struct thread_data* t_data[row_size];

	srand(time(NULL));
	matrix = malloc(sizeof(int*) * row_size);
	vector = malloc(sizeof(int) * col_size);
	for (int i = 0; i < row_size; i++) {
		matrix[i] = malloc(sizeof(int) * col_size);
	}

	generate_matrix(matrix, row_size, col_size);
	generate_vector(vector, col_size);

	for (int t = 0; t < row_size; t++) {
		t_data[t]->thread_id = t;
		if(pthread_create(&tid[t], NULL, thread_mvm, (void*)t_data[t])){
			printf("THREAD CREAT ERR\n");
			exit(1);
		}
	}
	for (int t = 0; t < row_size; t++) {
		pthread_join(tid[t], NULL);
	}

	print_matrix(matrix, row_size, col_size);
	print_vector(vector, col_size);
	printf("\n *** Result ***\n");
	for (int r = 0; r < row_size; r++) {
		printf("[ %d ]\n", t_data[r]->result);
	}

	for (int r = 0; r < row_size; r++) {
		free(matrix[r]);
	}
	free(matrix);
	free(vector);

	return 0;
}
