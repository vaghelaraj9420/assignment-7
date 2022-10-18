#include <arpa/inet.h>
#include <math.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#define MAXSIZE 50
#define MAXROW 50

int batch_size_global;
int num_clients_global;
struct matrix {
  int matrix_num;             // Matrix number
  int matrix_row, matrix_col; // Num of rows, cols in a matrix

  int *arr[MAXROW]; // using an array of pointers to create a dynamic array in
                    // heap
};

void displayInputMatrix(
    struct matrix m[], int num_matrix) // function to display the input matrices
{
  int x, y, a, b, i = 0;
  printf("number of matrices: %d\n", num_matrix);
  for (i = 0; i < num_matrix; i++) {
    a = m[i].matrix_row;
    b = m[i].matrix_col;
    printf("Num rows for matrix %d = %d\n", m[i].matrix_num, a);
    printf("Num cols for matrix %d = %d\n", m[i].matrix_num, b);
    for (x = 0; x < m[i].matrix_row; x++) {
      for (y = 0; y < m[i].matrix_col; y++) {
        printf("%d ", m[i].arr[x][y]);
      }
      printf("\n");
    }
  }
}

void displayMatrix(struct matrix m) // function to display the input matrix
{
  int x, y, a, b, i = 0;

  a = m.matrix_row;
  b = m.matrix_col;
  printf("Num rows for matrix %d = %d\n", m.matrix_num, a);
  printf("Num cols for matrix %d = %d\n", m.matrix_num, b);
  printf("The result matrix is:-\n");
  for (x = 0; x < m.matrix_row; x++) {
    for (y = 0; y < m.matrix_col; y++) {
      printf("%d ", m.arr[x][y]);
    }
    printf("\n");
  }
}

struct matrix multiplyMatrix(struct matrix a, struct matrix b) {
  struct matrix c;
  int sum = 0, i, j, k;
  if (a.matrix_col != b.matrix_row) {
    printf(
        "Invalid format of matrices. matrixA.col is not equal to matrixB.row");
    exit(0);
  }
  c.matrix_row = a.matrix_row;
  c.matrix_col = b.matrix_col;

  for (j = 0; j < a.matrix_row; j++)
    c.arr[j] = (int *)malloc(
        b.matrix_col *
        sizeof(int)); // Initializing the dynamic array of pointers

  c.matrix_num = 0;

  for (i = 0; i < a.matrix_row; i++) {
    for (j = 0; j < b.matrix_col; j++) {
      for (k = 0; k < b.matrix_row; k++) {
        sum += a.arr[i][k] * b.arr[k][j];
      }
      c.arr[i][j] = sum;
      sum = 0;
    }
  }
  return c;
}

// struct matrix unbatchify(struct matrix k[],
//                          int kSize) // function to multiply the matrices
// {

//   static int flag = 0, initialSize;
//   if (flag == 0) {
//     initialSize = kSize;
//     flag = 1;
//   }

//   if ((initialSize % 2 == 0) && (kSize == 1)) {
//     return k[0];
//   } else if ((initialSize % 2 != 0) && (kSize == 2)) { // error line
//     k[0] = multiplyMatrix(k[0], k[1]);
//     return k[0];
//   } else {
//     int i, j;
//     struct matrix Result[kSize];

//     for (i = 0; i < kSize; i++) {
//       Result[i] = k[i];
//       //...............................
//       Result[i].matrix_row =
//           k[i].matrix_row; // storing the num rows in struct matrix
//       Result[i].matrix_col =
//           k[i].matrix_col; // storing the num cols in struct matrix

//       for (j = 0; j < k[i].matrix_row; j++)
//         Result[i].arr[j] = (int *)malloc(
//             k[i].matrix_col *
//             sizeof(int)); // Initializing the dynamic array of pointers

//       //...............................
//     }

//     int temp;
//     for (i = 0, j = 0; i < kSize; i += 2, j++) {
//       if (i + 1 < kSize)
//         Result[j] = multiplyMatrix(
//             k[i],
//             k[i + 1]); // If pair of matrices are availible then multiply them
//       else
//         Result[j] = k[i]; // If only one matrix is availible then take it as it
//                           // is in next block of Result[]

//       temp = j + 1;
//     }


void *PrintHello(void *thredid) {
  long tid;
  tid = (long)thredid;
  printf("Hello world its me Thread #%ld\n", tid);
  pthread_exit(NULL);
}

// struct matrix multiplyMatrixThread(void *threadid , struct matrix a, struct matrix b) {
void *multiplyMatrixThread(struct matrix a, struct matrix b) 
{
  struct matrix c;
  int sum = 0, i, j, k;
  if (a.matrix_col != b.matrix_row) {
    printf("Invalid format of matrices. matrixA.col is not equal to matrixB.row");
    exit(0);
  }
  c.matrix_row = a.matrix_row;
  c.matrix_col = b.matrix_col;

  for (j = 0; j < a.matrix_row; j++)
    c.arr[j] = (int *)malloc(
        b.matrix_col *
        sizeof(int)); // Initializing the dynamic array of pointers

  c.matrix_num = 0;

  for (i = 0; i < a.matrix_row; i++) {
    for (j = 0; j < b.matrix_col; j++) {
      for (k = 0; k < b.matrix_row; k++) {
        sum += a.arr[i][k] * b.arr[k][j];
      }
      c.arr[i][j] = sum;
      sum = 0;
    }
  }

  printf("\nMultiplied matrices through threads: \n");
  displayMatrix(c);

  pthread_exit(NULL);
  // return c;
}


struct matrix unbatchify(struct matrix k[],
                         int kSize) // function to multiply the matrices
{

  static int flag = 0, initialSize;
  if (flag == 0) {
    initialSize = kSize;
    flag = 1;
  }

  if ((initialSize % 2 == 0) && (kSize == 1)) {
    return k[0];
  } else if ((initialSize % 2 != 0) && (kSize == 2)) { // error line
    k[0] = multiplyMatrix(k[0], k[1]);
    return k[0];
  } else {
    int i, j;
    struct matrix Result[kSize];      // Declaring the Result Matrix in which multiplication results will be stored

  // Initializing the Result matrix dynamically
    for (i = 0; i < kSize; i++) {
      Result[i] = k[i];
 
      Result[i].matrix_row =
          k[i].matrix_row; // storing the num rows in struct matrix
      Result[i].matrix_col =
          k[i].matrix_col; // storing the num cols in struct matrix

      for (j = 0; j < k[i].matrix_row; j++)
        Result[i].arr[j] = (int *)malloc(
            k[i].matrix_col *
            sizeof(int)); // Initializing the dynamic array of pointers
    }

    pthread_t threads[kSize];     // Declaring Threads array
    int temp;
    for (i = 0, j = 0; i < kSize; i += 2, j++) {
      if (i + 1 < kSize)
      {       // Implement threading here........................................
         Result[j] = multiplyMatrix(k[i], k[i + 1]); // If pair of matrices are availible then multiply them


          // pthread_t threads[NUM_THREADS];
          int rc;
          long t;
          // for (t = 0; t < NUM_THREADS; t++) {
            printf("In main, creating thread %d\n", i);
void * (*)(struct matrix,  struct matrix)
            rc = pthread_create(&threads[i], NULL, multiplyMatrixThread, (void *)(k[i],  k[i+1]));

            if (rc) {
              printf("ERROR, return code from pthread_create() is %d\n", rc);
              exit(-1);
            }
          // }

          pthread_exit(NULL);
      }//................................................................
      else
        Result[j] = k[i]; // If only one matrix is availible then take it as it
                          // is in next block of Result[]

      temp = j + 1;
    }

    printf("j = %d\n", j);
    if (initialSize % 2 == 0)
      return unbatchify(
          Result, kSize - j); // Recurse on the basis of j obtained previously
    else if (j == 1)
      return unbatchify(Result, kSize - j);
    else
      return unbatchify(Result, kSize - j + 1);
  }
}



int main(int argc, char *argv[]) {
  // char jobFilePath[100];
  // strcpy(jobFilePath, argv[5]);

  FILE *file = fopen("input2.txt", "r");

  if (file == NULL) {
    perror("Unable to open file");
    exit(1);
  }

  int i, j, a, b;
  int num_matrix, row, col;

  char line[100];
  char *token;
  fscanf(file, "%d\n", &num_matrix); // Input number of matrices

  struct matrix m[num_matrix];

  for (i = 0; i < num_matrix; i++) {
    m[i].matrix_num = i;
    fscanf(file, "%d %d\n", &row,
           &col); // Input num of rows and num of cols of a matrix
    // printf("row: %d, col:%d\n", row, col);
    m[i].matrix_row = row; // storing the num rows in struct matrix
    m[i].matrix_col = col; // storing the num cols in struct matrix

    for (j = 0; j < row; j++)
      m[i].arr[j] = (int *)malloc(
          col * sizeof(int)); // Initializing the dynamic array of pointers

    for (a = 0; a < row; a++) {
      fgets(line, 100, file);
      token = strtok(line, " ");

      for (b = 0; b < col; b++) {

        m[i].arr[a][b] =
            atoi(token); // storing each matrix element in its position

        // printf("%d ", m[i].arr[a][b]);
        token = strtok(
            NULL,
            " "); // Moving on to next value in the same row of an input matrix
      }
      printf("\n");
    }
  }
  // num_clients_global = atoi(argv[4]);

  // display the input matrices taken in the struct matrix
  // displayInputMatrix(m, num_matrix);

  struct matrix result = unbatchify(m, 5);
  printf("\n----------------------------------------------\n");
  displayMatrix(result);

  


  fclose(file);
  return 0;
}
