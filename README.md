# Mygcc_mini
Basic implementation of gcc command

eg.   mygcc hello.c
The command is just like the gcc command given.

it takes the options such as 
  1) -S : for assembly file
          gcc -S -o file_name.s file_name.c
  2) -c  : for generating Object file
          as -o  file_name.o file_name.s
  3) -o  : for custom output file name
  
  4) To link a file using ld
      ld -o file_name.exe -lc -dynamic-linker /lib/i386-linux-gnu/ld-linux.so.2 file_name.o -e main
      
 we used getopt() function for getopt.h file
    the loop should be like ---  while((ret=getopt())!=-1)  {...}
    optarg-- gives the string next theto the option given
    optind-- points to the last string , so argv[optind] will give the last string
    
 create the logic to activate the proper chaining and serial processings of the statements. (see the source code)
  we have to create the log file. create the file using the open() system call.
  eg.  fd=open("./build.log",O_RDWR|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR|S_IROTH|S_IRGRP);
  
  to print the log file redirect the STDOUT to file descriptor of the build.log .
     eg.   dup2(fd,STDOUT_FILENO);
  
  To print the time:--->
   use time_t type & a struct tm.
   time(&time_t)--> will give the time in seconds from 1 january 1970
   struct tm=localtime(&time_t);--> will give the ans in year-month-date-hour-min-sec. struct tm is 26 byte structure.
   use strftime() to convert time into string format. ie.  strftime(buff,26,"%Y-%m-%d %H:%M:%S",tm_info);

To delete file--> use unlink() system call
To create a Child process--> use fork() & execlp(loc_of_file,file_name,list_of_args..);
use wait(&ret) system call to achieve syncronous behaviour.

To create custom name:--
                  copy the string after the switch -o which is pointed by optarg into a local buffer.
                  copy the optarg into local buffer using strncpy()
                  append the suffix using strncat(). eg strncat(str,".obj",str_len);
                  
To print errno :---
                use strerror() function to print errno from file errno.h
                eg. fprintf(stderr,"%s:%s","Failed\n",strerror(errno));

The decide() function:--
			the function checks whether source file is modifided or not. acc to that it build the program or skips building process.
			it checks only for custom name. eg. for hello.c , according to STAGE it creates a name hello.s/hello.o/hello
			stat() both the files & compare the last modification time & returns TRUE or FALSE

NOTE: to create custom output name; first copy the argument into local buffer & use that buffer.
      Do NOT change original argv[]...if you do that then the pointer arragement done by getopt() will be tempered.
 
   
  
