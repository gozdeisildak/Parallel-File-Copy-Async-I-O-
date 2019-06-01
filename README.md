# Parallel-File-Copy-Async-I-O-
Parallel-File-Copy-Async-I-O-(Threads)

 The goal of the Parallel-File-Copy-Async-I-O is to copy a text file to a defined destination path using Asynchronous I/O operations like download manager .You will can copy the content of source file (source.txt) to a new destination file (destination.txt) by using threads.
 
 You will take 5 input parameters. 
 o Source path 
 o Destination path 
 o Number of threads
 o length of file
 o 1 or 2 (1)copy the contents of the entered source file 2)create new content in to source file and copy this file)
 
 Max size of file 100 MB
 
 For example; 
 let’s say that the user enters 8 threads and the size of the source file as 16 bytes. So each thread must copy 2 bytes for each. In order to be readable, your source file must be created randomly as “aabbccddeeffgghh”. 
 o 1st thread should copy from source.txt to destination.txt first 2 bytes > aa 
 o 2nd thread should copy from source.txt to destination.txt second 2 bytes > bb 
 o … 
 o 8th thread should copy from source.txt to destination.txt last 2 bytes > hh 
 
 COMPILE CODE 
 
 o gcc -lrt -c your_code_file.c 
 o gcc your_code_file.o -lrt -o your_code_file.out 
