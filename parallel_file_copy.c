#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <aio.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define file_lenght 10537 // enter the length of file
int accrual; // amount of 
int condition; //amount of will read and write of thread
int remain; // remain amount
int opn=0;
int number_threads;
int file_lenght;
char source_path[50];
char destination_path[50];

int create_file();
int createFile();
void *read_file();
int write_file(int offset, char *ptr, int accrual,int num);



int main(int argc, char *argv[])
{

    if(file_lenght>104857600){   // can copy up to 100 mb
	printf("please enter a value less than 100 MB");
    }
      

    int i;
    
    do{
    printf("Please enter the number of threads(0-10):");  
    scanf("%d",&number_threads);    
    }while(number_threads>10 || number_threads<1);

    printf("Please enter the source path: ");
    scanf("%s",&source_path);
    printf("Please enter the destination path: ");
    scanf("%s",&destination_path);
    printf("Please enter length of file: ");
    scanf("%s",&file_lenght);
    printf("1)copy the contents of the entered source file \n2)create new content in to source file and copy this file \n");
    scanf("%d",&condition);
    if(condition==2)
    createFile();
    if(number_threads>file_lenght){
       number_threads=file_lenght;
    }
    pthread_t tid[number_threads];
    int Number[number_threads];
    accrual = file_lenght/ number_threads;
    remain=file_lenght % number_threads;

    int offset = 0 - accrual;

    for (i = 0; i < number_threads; i++)
    {
        Number[i] = i;
    }

    for (i = 0; i < number_threads; i++)
    {
       
        pthread_create(&tid[i], NULL, read_file, (void *)&Number[i]);
    }

    for (i = 0; i < number_threads; i++)
    {
        pthread_join(tid[i], NULL);
    }
    
}

int createFile(){  //  is produced random character with ascii and is writed source file in the source path.Briefly this function produce file that have random content   

 FILE *fp;
 int i,r;
     //srand(time(0));   // Initialization, should only be called once.
 fp = fopen(source_path,"w");

 for(i=0;i<file_lenght;i++){

     do{
     r = rand()%26+97;
     }while(r==31 || r==63 || r==95);

     char c = r;
     fprintf(fp,"%c",c);    
     
 }


fclose(fp);
}



void *read_file(void *number)// file is read with threads
{
    int n_byte;
    int num = *(int *)number;

    if(num==number_threads-1)
      n_byte=accrual+remain;
    else
      n_byte=accrual;

    
    int offset = num * accrual;
    int port_fd = 0, err, ret;
    char data[n_byte];
    struct aiocb aio;

    port_fd = open(source_path, O_RDONLY);
 
    if (port_fd == -1)
    {
        perror("open");
        return (void *)EXIT_FAILURE;
    }

    memset(&aio, 0, sizeof(aio));
    memset(&data, 0, sizeof(data));

    aio.aio_fildes = port_fd;
    aio.aio_buf = data;
    aio.aio_nbytes = n_byte;
    aio.aio_offset = offset;
    aio_read(&aio);

    while (aio_error(&aio) == EINPROGRESS)
    {
    }
    err = aio_error(&aio);
    ret = aio_return(&aio);

    if (err != 0)
    {
        printf("Eerror at aio_error() : %s\n", strerror(err));
        close(port_fd);
        exit(2);
    }

   if (ret != sizeof(data))
    {
        printf("Errorr at aio_return()\n");
        close(port_fd);
        exit(2);
    }

    fprintf(stdout, "%s\n", &data);
    char *ptr;
    ptr = data;
    close(port_fd);
    write_file(offset, ptr, n_byte,num);
    return (void *)EXIT_SUCCESS;
}

int write_file(int offset, char *ptr, int n_byte,int num) // data that is read is writed file in the destination path through threads
{
    char data[n_byte];
    strcpy(data, ptr);
     
    ssize_t bytes_read = 0;
    int port_fd = 0, err, ret;
    struct aiocb aio;


    port_fd = open(destination_path, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

    if (port_fd == -1)
    {
        perror("open");
        return EXIT_FAILURE;
    }
 

     memset(&aio, 0, sizeof(aio));
    aio.aio_fildes = port_fd;
    aio.aio_buf = data;
    aio.aio_nbytes = n_byte;
    aio.aio_offset=offset;
    aio_write(&aio);



   while (aio_error(&aio) == EINPROGRESS) { }
    err = aio_error(&aio);
    ret = aio_return(&aio);

    if (err != 0) {
        printf ("Error at aio_error() : %s\n", strerror (err));
        close (port_fd);
        exit(2);
    }

    if (ret != sizeof(data)) {
        printf("Error at aio_return()\n");
        close(port_fd);
        exit(2);
    }
  

    bytes_read = aio_return(&aio);

    close(port_fd);
  
    

}


