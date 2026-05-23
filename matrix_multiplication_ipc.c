#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>



int main ()
{
   int rows1, cols1, rows2, cols2, i, j, k, matrix3_size;
   int fd_parent[2], fd_child[2]; // fd_parent sends the row and column number, 
                                  // fd_child returns the result
   int row_num=0, col_num=0, rows3, cols3;
   int parent_value;
   
   srand(time(NULL)); 
   
   printf("Enter the number of rows for the first matrix:\n");
   scanf("%d", &rows1); // We get the matrix size from the user
   printf("Enter the number of columns for the first matrix:\n");
   scanf("%d", &cols1);
   
   printf("The second matrix rows must match the first matrix columns.\n");
   printf("Enter the number of rows for the second matrix:\n");
   scanf("%d", &rows2);
   
   while(rows2 != cols1) // If the number of rows of the second matrix is not equal to
   {                 // the  number of columns of the first, we ask the user for a new number 
      printf("The second matrix rows must match the first matrix columns.\n");
      printf("Enter the number of rows for the second matrix:\n");
      scanf("%d", &rows2);
   }
   
   printf("Enter the number of columns for the second matrix:\n");
   scanf("%d", &cols2);
   
   matrix3_size = rows1 * cols2; // Size of the multiplied matrix

   int matrix1[rows1][cols1], matrix2[rows2][cols2], matrix3[rows1][cols2];
   
   for(i = 0; i < rows1; i++)  // Initialization of the matrices
   {
      for(j = 0; j < cols1; j++)
      {
         matrix1[i][j] = (rand() % 20) + 1; // The first matrix is filled with random numbers (1-20)
         printf("Matrix1[%d][%d] = %d\n", i, j, matrix1[i][j]);
      }
   }
   printf("\n\n");
   
   for(i = 0; i < rows2; i++)
   {
      for(j = 0; j < cols2; j++)
      {
         matrix2[i][j] = (rand() % 20) + 1; // The second matrix is filled with random numbers (1-20)
         printf("Matrix2[%d][%d] = %d\n", i, j, matrix2[i][j]);
      }
   }   
   
   printf("\n");
  
   for(i = 0; i < matrix3_size; i++) // Repeat until all elemets of the third matrix are calculated
   {      
      if (pipe(fd_parent) < 0)
      {
          printf("Error number: %d\n", errno);
          perror("Error Description");
          return(-1);
      }

      if (pipe(fd_child) < 0)
      {
          printf("Error number: %d\n", errno);
          perror("Error Description");
          return(-2);
      }
      
      int pid = fork();    
      
      if (pid < 0) 
      {
         printf("Error number: %d\n", errno);
         perror("Error Description");
         return(0);
      }  
    
      if (pid == 0) // Child process
      {
         int value = 0; // Saves the result of the multiplication
         int prev_value; // Value for print
  
         close(fd_parent[1]); // Closing write
         
         read(fd_parent[0], &rows3, sizeof(rows3)); // Reading the rows and columns
         read(fd_parent[0], &cols3, sizeof(cols3));
         
         printf("Child %d read the row = %d and the column = %d.\n\n", i, rows3, cols3);
         
         for(j = 0; j < cols1; j++) // Columns of the first matrix = rows of the second matrix
         {
           prev_value = value;
           value = value + matrix1[rows3][j] * matrix2[j][cols3];
           printf("Matrix1[%d][%d] = %d, Matrix2[%d][%d] = %d\n", rows3, j, matrix1[rows3][j], j, cols3, matrix2[j][cols3]);
           printf("Value = (value) %d + %d * %d = %d \n", prev_value, matrix1[rows3][j], matrix2[j][cols3], value);
         }
  
         close(fd_child[0]); // Closing read
         close(fd_parent[0]); // Closing read
         
         printf("\nChild %d sent the value = %d to parent.\n", i, value);
         
         write(fd_child[1], &value, sizeof(value)); // Writing the result
         
         close(fd_child[1]); // Closing write
         
         printf("Child %d is exiting.\n", i);

         exit(0);
      }
   
      else // Parent process
      {
       
         close(fd_parent[0]); // Closing parent's read so we can write
         
         printf("Parent sent the row = %d and column = %d.\n", row_num, col_num);
         
         write(fd_parent[1], &row_num, sizeof(row_num)); // Parent is writing the number of rows to the child.
         write(fd_parent[1], &col_num, sizeof(col_num)); // Parent is writing the number of columns to the child.
      
         close(fd_child[1]); // Closing write 
         close(fd_parent[1]); // Closing write 
         
         read(fd_child[0], &parent_value, sizeof(parent_value)); // Reading the value of the third matrix from child
         
         printf("Parent read value = %d from child.\n", parent_value);
         
         matrix3[row_num][col_num] = parent_value; // Adding the value to the matrix
         
         if(i == matrix3_size -1) // If this is the last loop we print the third matrix
         {
            printf("\n");
            for(j = 0; j < rows1; j++)
            {
               for(k = 0; k < cols2; k++)
               {
                  printf("Matrix3[%d][%d] = %d\n", j, k,matrix3[j][k]);
               }
            }
         }
         
         close(fd_parent[0]); // Closing read 
         close(fd_child[0]);  // Closing read 
         
         sleep(1);
         wait(NULL); // Waiting for the child.
      }
      
      printf("\n");

      if(row_num < rows1)  // If row_num is less than the sum of rows of matrix1 then
      {
         if(col_num < cols2 - 1) // If col_num is less than the sum of columns of matrix2 then
            col_num = col_num + 1;  
         else if(col_num == cols2 - 1)
         {
            col_num = 0;
            row_num = row_num + 1;
         }
         else
            row_num = row_num + 1;
      }      
   }   
   return 0;
}