#include <arpa/inet.h>
#include <math.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#define MAXSIZE 50
#define MAXROW 50

int batch_size_global;
int num_clients_global;
struct matrix
{
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
    for (i = 0; i < num_matrix; i++)
    {
        a = m[i].matrix_row;
        b = m[i].matrix_col;
        printf("Num rows for matrix %d = %d\n", m[i].matrix_num, a);
        printf("Num cols for matrix %d = %d\n", m[i].matrix_num, b);
        for (x = 0; x < m[i].matrix_row; x++)
        {
            for (y = 0; y < m[i].matrix_col; y++)
            {
                printf("%d ", m[i].arr[x][y]);
            }
            printf("\n");
        }
    }
}
// struct matrix multiplyMatrix(struct matrix a, struct matrix b) {
//   struct matrix c;
//   int sum = 0;
//   if (a.matrix_col != b.matrix_row) {
//     printf(
//         "Invalid format of matrices. matrixA.col is not equal to
//         matrixB.row");
//     exit(0);
//   }

//   for (int i = 0; i < a.matrix_row; i++) {
//     for (int j = 0; j < b.matrix_col; j++) {
//       for (int k = 0; k < b.matrix_row; k++) {
//         sum += a.arr[i][k] * b.arr[k][j];
//       }
//       c.arr[i][j] = sum;
//       sum = 0;
//     }
//   }
//   return c;
// }

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
//   } else if ((initialSize % 2 != 0) && (kSize == 2)) {
//     return k[0];
//   } else {
//     int i, j;
//     struct matrix Result[kSize];

//     for (i = 0; i < kSize; i++)
//       Result[i] = k[i];

//     for (i = 0, j = 0; i < kSize, j < kSize; i += 2, j++) {
//       if (i + 1 < kSize)
//         Result[j] = multiplyMatrix(Result[i], Result[i + 1]);
//       else
//         Result[j] = Result[i];
//     }
//     if (initialSize % 2 == 0)
//       return unbatchify(Result, kSize - j);
//     else
//       return unbatchify(Result, kSize - j + 1);
//   }
// }

void displayMatrix(struct matrix m) // function to display the input matrix
{
    int x, y, a, b, i = 0;

    a = m.matrix_row;
    b = m.matrix_col;
    printf("Num rows for matrix %d = %d\n", m.matrix_num, a);
    printf("Num cols for matrix %d = %d\n", m.matrix_num, b);
    printf("The result matrix is:-\n");
    for (x = 0; x < m.matrix_row; x++)
    {
        for (y = 0; y < m.matrix_col; y++)
        {
            printf("%d ", m.arr[x][y]);
        }
        printf("\n");
    }
}

struct matrix multiplyMatrix(struct matrix a, struct matrix b)
{
    struct matrix c;
    int sum = 0, i, j, k;
    if (a.matrix_col != b.matrix_row)
    {
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

    for (i = 0; i < a.matrix_row; i++)
    {
        for (j = 0; j < b.matrix_col; j++)
        {
            for (k = 0; k < b.matrix_row; k++)
            {
                sum += a.arr[i][k] * b.arr[k][j];
            }
            c.arr[i][j] = sum;
            sum = 0;
        }
    }
    return c;
}

struct matrix unbatchify(struct matrix k[],
                         int kSize) // function to multiply the matrices
{

    static int flag = 0, initialSize;
    if (flag == 0)
    {
        initialSize = kSize;
        flag = 1;
    }

    if ((initialSize % 2 == 0) && (kSize == 1))
    {
        return k[0];
    }
    else if ((initialSize % 2 != 0) && (kSize == 2))
    { // error line
        k[0] = multiplyMatrix(k[0], k[1]);
        return k[0];
    }
    else
    {
        int i, j;
        struct matrix Result[kSize];

        for (i = 0; i < kSize; i++)
        {
            Result[i] = k[i];
            //...............................
            Result[i].matrix_row =
                k[i].matrix_row; // storing the num rows in struct matrix
            Result[i].matrix_col =
                k[i].matrix_col; // storing the num cols in struct matrix

            for (j = 0; j < k[i].matrix_row; j++)
                Result[i].arr[j] = (int *)malloc(
                    k[i].matrix_col *
                    sizeof(int)); // Initializing the dynamic array of pointers

            //...............................
        }

        int temp;
        for (i = 0, j = 0; i < kSize; i += 2, j++)
        {
            if (i + 1 < kSize)
                Result[j] = multiplyMatrix(
                    k[i],
                    k[i + 1]); // If pair of matrices are availible then multiply them
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

int main(int argc, char *argv[])
{
    //   char jobFilePath[100];
    //   strcpy(jobFilePath, argv[5]);

    FILE *file = fopen("input2.txt", "r");
    //   FILE *file = fopen(jobFilePath, "r");

    if (file == NULL)
    {
        perror("Unable to open file");
        exit(1);
    }

    int i, j, a, b;
    int num_matrix, row, col;

    char line[100];
    char *token;
    fscanf(file, "%d\n", &num_matrix); // Input number of matrices

    struct matrix m[num_matrix];

    for (i = 0; i < num_matrix; i++)
    {
        m[i].matrix_num = i;
        fscanf(file, "%d %d\n", &row,
               &col); // Input num of rows and num of cols of a matrix
        printf("row: %d, col:%d\n", row, col);
        m[i].matrix_row = row; // storing the num rows in struct matrix
        m[i].matrix_col = col; // storing the num cols in struct matrix

        for (j = 0; j < row; j++)
            m[i].arr[j] = (int *)malloc(
                col * sizeof(int)); // Initializing the dynamic array of pointers

        for (a = 0; a < row; a++)
        {
            fgets(line, 100, file);
            token = strtok(line, " ");

            for (b = 0; b < col; b++)
            {

                m[i].arr[a][b] =
                    atoi(token); // storing each matrix element in its position

                printf("%d ", m[i].arr[a][b]);
                token = strtok(
                    NULL,
                    " "); // Moving on to next value in the same row of an input matrix
            }
            printf("\n");
        }
    }
    // num_clients_global = atoi(argv[4]);

    // display the input matrices taken in the struct matrix
    displayInputMatrix(m, num_matrix);

    // struct matrix result = unbatchify(m, 5);          // unbatchifying the
    // input array of structures and simulataneously multiplying them
    //    struct matrix result =  multiplyMatrix(m[0], m[1]);

    // displayMatrix(result);                // Display the Resultant matrix
    fclose(file);
    // Network Programming starts from
    // here........................................ int num_clients =
    // atoi(argv[4]); int batchSize = ceil(num_matrix / num_clients) + 1;;
    // batch_size_global = batchSize;
    // int NUM_THREADS;
    if (!strcmp(argv[1], "-s")) // Write server code here
    {
        // SERVER STARTS
        // HERE.................................................................................

        int sockfd, newsockfd; /* Socket descriptors */
        int clilen;
        struct sockaddr_in cli_addr, serv_addr;
        int i;
        char buf[100], temp[50]; /* We will use this buffer for communication */

        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) // Openeing a socket
        {
            printf("Cannot create socket\n");
            exit(0);
        }

        serv_addr.sin_family = AF_INET; // AF_INET for the internet family
        // serv_addr.sin_addr.s_addr = INADDR_ANY;     //"sin_addr" specifies the
        // internet address of the server and is set to INADDR_ANY for machines
        // having a single IP address.
        serv_addr.sin_addr.s_addr = atoi(argv[2]);
        // strcpy(serv_addr.sin_addr.s_addr, argv[2]);
        // serv_addr.sin_port = 6000;      // port number of the server
        serv_addr.sin_port = atoi(argv[3]);

        if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) <
            0) // binding is done here (associating the server with its port)
        {
            printf("Unable to bind local address\n");
            exit(0);
        }

        int num_clients = atoi(argv[4]);
        num_clients_global = num_clients;
        // listen(sockfd, 5);
        /* This specifies that up to 5 concurrent client
     requests will be queued up while the system is
     executing the "accept" system call below. */
        listen(sockfd, num_clients);

        int batchSize =
            ceil(num_matrix / num_clients); // calculating the input batch size for each client to be given
        batch_size_global = batchSize;
        int flag = 0, j, x, k, z = 0;

        // for(i=0, j=0; i<num_matrix, j<num_clients ; i += batchSize, j++)
        // {
        // flag++;
        // if((m[j].matrix_num + 1)* batchSize > num_matrix)

        while (1)
        {
            for (i = 0, j = 0; i < num_matrix, j < num_clients; i += batchSize, j++)
            {
                clilen = sizeof(cli_addr);
                // accepting a client connection
                newsockfd =
                    accept(sockfd, (struct sockaddr *)&cli_addr,
                           &clilen); // the new socket descriptor returned by the
                                     // accept() system call is stored in "newsockfd".

                if (newsockfd < 0)
                {
                    printf("Accept error");
                    exit(0);
                }

                struct matrix k[batchSize]; // Introducing a new variable for doing
                                            // multiplication on
                // static int z = 0;
                // for(z, x=0; z < (j+1)*batchSize , x < batchSize; z += (1 +
                // j*batchSize), x++)
                // {
                //   k[x] = m[z + j*batchSize];              // fitting the batch to be
                //   processed into the new variable if(z == num_matrix - 1)
                //     flag = num_matrix - j*batchSize;
                // }
                for (z, x = 0; (z < ((j + 1) * batchSize)) && (z < num_matrix), x < batchSize; z += 1, x++)
                {
                    k[x] = m[z];                // fitting the batch to be processed into the new variable
                    if (z == num_matrix - 1) // initializing a flag if there are matrices left without fully filled batch
                        flag = num_matrix - j * batchSize;
                }

                if (fork() == 0)
                {                  /* This child process will now communicate with the
            client through the send() and recv() system calls. */
                    close(sockfd); /* Close the old socket since all communications
                    will be through the new socket. */
                    for (i = 0; i < 100; i++)
                        buf[i] = '\0'; /* Initialize buffer */

                    // strcpy(buf, "Message from server");         /* Copy message */
                    if (flag != 0) // If the last batch of input matrices is not fully
                                   // contained then process only some matrices
                    {
                        batch_size_global = flag;
                        for (x = 0; x < flag; x++)
                        {
                            sprintf(temp, "%d %d ", k[x].matrix_row, k[x].matrix_col);
                            strcat(buf, temp); // the matrix number, row and col are being
                                               // stored in the buf

                            for (a = 0; a < k[x].matrix_row; a++)
                            {
                                for (b = 0; b < k[x].matrix_col; b++)
                                {
                                    sprintf(temp, "%d ", k[x].arr[a][b]);
                                    strcat(
                                        buf,
                                        temp); // the matrix elements are being stored in the buf
                                }
                            }
                        }
                        flag = 0;
                    }
                    else
                    {
                        for (x = 0; x < batchSize; x++)
                        {
                            sprintf(temp, "%d %d ", k[x].matrix_row, k[x].matrix_col);
                            strcat(buf, temp); // the matrix number, row and col are being
                                               // stored in the buf

                            for (a = 0; a < k[x].matrix_row; a++)
                            {
                                for (b = 0; b < k[x].matrix_col; b++)
                                {
                                    sprintf(temp, "%d ", k[x].arr[a][b]);
                                    strcat(
                                        buf,
                                        temp); // the matrix elements are being stored in the buf
                                }
                            }
                        }
                    }

                    send(newsockfd, buf, 100, 0); /* Send message */

                    for (i = 0; i < 100; i++)
                        buf[i] = '\0'; /* Initialize buffer */

                    recv(newsockfd, buf, 100, 0); /* Receive message */
                    printf("%s\n", buf);
                    close(newsockfd);
                    exit(0);
                }
                close(newsockfd);
            } // End of for loop
            close(newsockfd);
        } // End of while loop
        return 0;
        // SERVER ENDS
        // HERE.................................................................................
    }
    else if (!strcmp(argv[1], "-c")) // Write client code here
    {
        // CLIENT STARTS
        // HERE.................................................................................
        int sockfd;
        struct sockaddr_in serv_addr;
        int i;
        char buf[100];

        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) // opening a socket
        {
            printf("Unable to create socket\n");
            exit(0);
        }

        serv_addr.sin_family = AF_INET;
        // serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        serv_addr.sin_addr.s_addr = atoi(argv[2]);
        // serv_addr.sin_port = 6000;
        serv_addr.sin_port = atoi(argv[3]);

        // printf("client Num: %d\n" ,atoi(argv[4]));

        if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) <
            0) // connect with the server
        {
            printf("Unable to connect to server\n");
            exit(0);
        }

        for (i = 0; i < 100; i++)
            buf[i] = '\0';

        recv(sockfd, buf, 100, 0);
        printf("%s\n", buf);

        char *token;
        token = strtok(
            buf, " "); // dividing the buf input we got from the server into tokens

        int num_clients = atoi(argv[4]);
        // int batchSize = num_matrix/num_clients;
        int batchSize =
            ceil(num_matrix / num_clients); // calculating the input batch size for each client to be given
        // struct matrix resultChild[2];       // define 2 matrixes to perform
        // multiplication on struct matrix resultChild[batch_size_global];       //
        // define 2 matrixes to perform multiplication on

        //..........................................
        int j, x, z = 0, flag = 0;
        for (i = 0, j = 0; i < num_matrix, j < num_clients; i += batchSize, j++)
        {
            for (z, x = 0;
                 (z < ((j + 1) * batchSize)) && (z < num_matrix), x < batchSize;
                 z += 1, x++)
            {
                if (z == num_matrix - 1) // initializing a flag if there are matrices
                                         // left without fully filled batch
                    flag = num_matrix - j * batchSize;
            }

            //............................................
            printf("Flag = %d, batchSize = %d", flag, batchSize);

            struct matrix resultChild[batchSize]; // define |batchsize| matrixes to perform multiplication on

            if (flag != 0)
            {
                struct matrix
                    resultChild[flag]; // define only some matrixes to perform multiplication on as the number of matrices left is < batchsize
            }
        
            int y;
            if (flag == 0)              // Filling those batches whose num of matrices = batchSize
            {
                for (y = 0; y < batchSize; y++)
                {
                    resultChild[y].matrix_row = atoi(token);
                    token = strtok(NULL, " ");
                    resultChild[y].matrix_col = atoi(token);
                    token = strtok(NULL, " ");

                    // printf("\n%d %d\n", resultChild[y].matrix_row,
                    // resultChild[y].matrix_col);

                    for (x = 0; x < resultChild[y].matrix_row; x++)
                        resultChild[y].arr[x] = (int *)malloc(
                            resultChild[y].matrix_col *
                            sizeof(int)); // Initializing the dynamic array of pointers

                    for (a = 0; a < resultChild[y].matrix_row; a++)
                    {
                        for (b = 0; b < resultChild[y].matrix_col; b++)
                        {
                            resultChild[y].arr[a][b] = atoi(token);     // Filling the Result matrix
                            // printf("\n%d", resultChild[y].arr[a][b]);
                            token = strtok(NULL, " ");
                        }
                    }
                }
            }
            else                // Filling those batches whose num of matrices < batchSize and equal to flag
            {
                for (y = 0; y < flag; y++)
                {
                    resultChild[y].matrix_row = atoi(token);
                    token = strtok(NULL, " ");
                    resultChild[y].matrix_col = atoi(token);
                    token = strtok(NULL, " ");

                    // printf("\n%d %d\n", resultChild[y].matrix_row,
                    // resultChild[y].matrix_col);

                    for (x = 0; x < resultChild[y].matrix_row; x++)
                        resultChild[y].arr[x] = (int *)malloc(
                            resultChild[y].matrix_col *
                            sizeof(int)); // Initializing the dynamic array of pointers

                    for (a = 0; a < resultChild[y].matrix_row; a++)
                    {
                        for (b = 0; b < resultChild[y].matrix_col; b++)
                        {
                            resultChild[y].arr[a][b] = atoi(token);
                            // printf("\n%d", resultChild[y].arr[a][b]);
                            token = strtok(NULL, " ");
                        }
                    }
                }
            }

            // HERE MULTIPLY RESULTCHILD[0] TILL RESULTCHILD[2] AND SEND THE FINAL
            // RESULT MATRIX TO SERVER THREADS
            // START...................................................................

            //   num_clients_global = atoi(argv[4]);
            //   printf("num_clients_global = %d\n", num_clients_global);

            // pthread_t threads[num_clients_global];
            //   int rc;
            //   long t;
            //   for (t = 0; t < num_clients_global; t++) {
            //     printf("In main, creating thread %ld\n", t);
            //     rc = pthread_create(&threads[t], NULL, MatrixMultiplication, (void
            //     *)t); if (rc) {
            //       printf("ERROR, return code from pthread_create() is %d\n", rc);
            //       exit(-1);
            //     }
            //   }

            //   pthread_exit(NULL);

            // THREADS
            // END......................................................................

            for (i = 0; i < 100; i++)
                buf[i] = '\0';
            char temp[50];

            // strcpy(buf, "Message from client");
            if (flag == 0)
            {
                for (y = 0; y < batchSize; y++)
                {
                    sprintf(temp, " %d %d ", resultChild[y].matrix_row,
                            resultChild[y].matrix_col);
                    strcat(buf, temp); // the matrix number, row and col are being stored
                                       // in the buf

                    for (a = 0; a < resultChild[y].matrix_row; a++)
                    {
                        for (b = 0; b < resultChild[y].matrix_col; b++)
                        {
                            sprintf(temp, " %d", resultChild[y].arr[a][b]);
                            strcat(buf,
                                   temp); // the matrix elements are being stored in the buf
                        }
                    }
                }
            }
            else
            {
                for (y = 0; y < flag; y++)
                {
                    sprintf(temp, " %d %d ", resultChild[y].matrix_row,
                            resultChild[y].matrix_col);
                    strcat(buf, temp); // the matrix number, row and col are being stored
                                       // in the buf

                    for (a = 0; a < resultChild[y].matrix_row; a++)
                    {
                        for (b = 0; b < resultChild[y].matrix_col; b++)
                        {
                            sprintf(temp, " %d", resultChild[y].arr[a][b]);
                            strcat(buf,
                                   temp); // the matrix elements are being stored in the buf
                        }
                    }
                }
            }

            send(sockfd, buf, 100, 0);

            close(sockfd);

            return 0;
        }

        return 0;
    }
}
