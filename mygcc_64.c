//gcc command mini implementation
//author:- PKW

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<getopt.h>
#include<fcntl.h>
#include<time.h>

#define ASM 10
#define OBJ 20
#define EXE 30

#define TRUE   0
#define FALSE  1


typedef int    flag_t;
typedef int    STAGE;


flag_t S_used=FALSE;			//flag for -S switch
flag_t o_used=FALSE;			//flag for -o switch
flag_t c_used=FALSE;			//flag for -c switch

STAGE stage=EXE;
flag_t decision=TRUE;
int ret;				//getopt ret value
int err=0;				//errno
int ilen=0;				//length of custom name string
int fd;					//File descriptor for build.log file
char *src_file_name=NULL;		//src file name
char *op_file_name=NULL;		//op file name
char buff[26];


void build(STAGE,char *,char *);	//stage,src_file_name,op_file_name
char *asm_create(char *,char *);	//c_src_file_name,s_op_file_name
char *obj_create(char *,char *);	//s_src_file_name,o_op-file_name
char *exe_create(char *,char *);	//o_src_file_name,exe_op_file_name
char *print_time();			//returns date & time
void sys_err(char *);
int delete_file(char *);
flag_t decide(STAGE,char *);		//decides whether to build the processs again or skip


int main(int argc,char *argv[])
{
	while((ret=getopt(argc,argv,"Sco:"))!=-1)
	{
		switch(ret)
		{
		case 'S':
			S_used=TRUE;
			stage=ASM;
			break;
		case 'o':
			o_used=TRUE;
			op_file_name=optarg;
			ilen=strlen(op_file_name);
			ilen=ilen+2; //for extentions
			break;
		case 'c':
			c_used=TRUE;
			stage=OBJ;
			break;
		default:
			printf("\nDefault case error\n");
			break;
		}
	}
	src_file_name=argv[optind];
	if(o_used==FALSE)
	{
		ilen=strlen(src_file_name);
	}
	if(src_file_name==NULL)
	{
		fprintf(stderr,"\nError:Please give a source file\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		build(stage,src_file_name,op_file_name);
	}
	
return 0;
exit(EXIT_SUCCESS);
}

void build(STAGE stage,char *src_file_name,char *op_file_name)
{
char *obj_name,*exe_name,*s_name;

//creating new log file
fd=open("./build.log",O_RDWR|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR|S_IROTH|S_IRGRP);
if(fd==-1)
	sys_err("failed to create log file");
dup2(fd,STDOUT_FILENO);	//redirected stdout to build.log file

//checking for previous builds
	decision=decide(stage,src_file_name);	
	if(decision==FALSE)
	{
		exit(EXIT_SUCCESS);
	}
	switch(stage)
	{
	case ASM:
		s_name=  asm_create(src_file_name,op_file_name);
		break;
	case OBJ:
		s_name=  asm_create(src_file_name,op_file_name);
		obj_name=obj_create(s_name,op_file_name);
		delete_file(s_name);
		break;
	case EXE:
		s_name=  asm_create(src_file_name,op_file_name);
		obj_name=obj_create(s_name,op_file_name);
		exe_name=exe_create(obj_name,op_file_name);
		delete_file(s_name);
		delete_file(obj_name);
		break;
	}

}


void sys_err(char *str)
{
	fprintf(stderr,"%s:%s",str,strerror(errno));
	exit(EXIT_FAILURE);
}

int delete_file(char *file_name)
{
	int err;
	err=unlink(file_name);
	if(err==-1)
	{
		fprintf(stdout,"%s:ERROR Deleting file:%s\n",print_time(),file_name);
		exit(EXIT_FAILURE);
		return err;	
	}
	else
	{
		fprintf(stdout,"%s:Deleting file:%s\n",print_time(),file_name);	
		return 0;
	}
}


char *print_time()
{
	time_t timer;
	struct tm *tm_info;
	time(&timer);
	tm_info=localtime(&timer);
	strftime(buff,26,"%Y-%m-%d %H:%M:%S",tm_info);
return buff;
}



char *asm_create(char *src_file_name,char *s_op_file_name)
{
int iret;
//copying str into local variable
char *temp_s_op_file_name=calloc(ilen,sizeof(char));



	if(s_op_file_name==NULL)
	{
		// name will be hello.c we dont want .c part so using -2.
		strncpy(temp_s_op_file_name,src_file_name,ilen-2);
		strncat(temp_s_op_file_name,".s",ilen);
	}
	else
	{
		//name will be like -o hello so no -2
		strncpy(temp_s_op_file_name,s_op_file_name,ilen);
		strncat(temp_s_op_file_name,".s",ilen);
	}
	if(fork()==0)
	{
		err=execlp("/usr/bin/gcc","gcc","-S","-o",temp_s_op_file_name,src_file_name,(char *)0);
		if(err==-1)
		{
			printf("\nFAILED:exec:gcc\n");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		wait(&iret);
		fprintf(stdout,"%s:ASM file created:%s\n",print_time(),temp_s_op_file_name);
	}
return temp_s_op_file_name;
}

char *obj_create(char *s_src_file_name,char *o_op_file_name)
{
int iret;
//copying str into local variable
char *temp_o_op_file_name=calloc(ilen,sizeof(char));



	if(o_op_file_name==NULL)
	{
		// name will be hello.c we dont want .c part so using -2.
		strncpy(temp_o_op_file_name,s_src_file_name,ilen-2);
		strncat(temp_o_op_file_name,".o",ilen);
	}
	else
	{
		strncpy(temp_o_op_file_name,o_op_file_name,ilen);
		strncat(temp_o_op_file_name,".o",ilen);
	}
	if(fork()==0)
	{
		err=execlp("/usr/bin/as","as","-o",temp_o_op_file_name,s_src_file_name,(char *)0);
		if(err==-1)
		{
			printf("\nFAILED:exec:as\n");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		wait(&ret);
		fprintf(stdout,"%s:OBJ file created:%s\n",print_time(),temp_o_op_file_name);
	}
return temp_o_op_file_name;
}

char *exe_create(char * o_src_file_name,char * exe_op_file_name)
{
int iret;
//copying str into local variable
char *temp_exe_op_file_name=calloc(ilen,sizeof(char));


	if(exe_op_file_name==NULL)
	{
		strncpy(temp_exe_op_file_name,o_src_file_name,ilen-2);
		strncat(temp_exe_op_file_name,"",ilen);
	}
	else
	{
		strncpy(temp_exe_op_file_name,exe_op_file_name,ilen);
		strncat(temp_exe_op_file_name,"",ilen);
	}
	if(fork()==0)
	{
		err=execlp("ld","ld","-o",temp_exe_op_file_name,"-lc","-dynamic-linker","/lib64/ld-linux-x86-64.so.2",o_src_file_name,"-e","main",(char *)0);
		if(err==-1)
		{
			fprintf(stderr,"\nFAILED:exec:ld\n");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		wait(&iret);	
		fprintf(stdout,"%s:exe file created:%s\n",print_time(),temp_exe_op_file_name);
	}
return temp_exe_op_file_name;
}


flag_t decide(STAGE stage,char *src_file_name)
{
	int errval;
	double time_diff;
	struct stat stat_buff1,stat_buff2;
	time_t t1,t2;

	//creating a copy to perform string operations
	int str_len;
	str_len=strlen(src_file_name);
	
	char *copy=calloc(str_len,sizeof(char));
	strncpy(copy,src_file_name,str_len-2);
	
	//only default names are taken into consideration
	if(stage==ASM)
	{
		strncat(copy,".s",str_len);
	}
	if(stage==OBJ)
	{
		strncat(copy,".o",str_len);
	}
	if(stage==EXE)
	{
		strncat(copy,"",str_len);
	}
	
	fprintf(stdout,"%s:Checking previous builds:%s\n",print_time(),copy,strerror(errno));
	
	errval=stat(src_file_name,&stat_buff1);
		if(errval==-1)
		{
			fprintf(stdout,"%s:%s:%s\n",print_time(),src_file_name,strerror(errno));
			exit(EXIT_FAILURE);
		}

	errval=stat(copy,&stat_buff2);
		if(errval==-1)
		{
			fprintf(stdout,"%s:%s:%s\n",print_time(),copy,strerror(errno));
			fprintf(stdout,"%s:No previous build found,Build necessary:%s\n",print_time(),src_file_name);
			return TRUE;
		}	
		t1=stat_buff1.st_mtime;
		t2=stat_buff2.st_mtime;
	time_diff=difftime(t1,t2);//comapre .c with other file	for last modification stamp
	
	if(time_diff > 0)//if greater than 0 then file is updated
	{
		fprintf(stdout,"%s:Sorce file is updated,Build necessary:%s\n",print_time(),src_file_name);
		return TRUE;
	}
	else		//file is not updated
	{
		fprintf(stdout,"%s:Source file is not updated,No need to build:%s\n",print_time(),src_file_name);
		return FALSE;	
	}
}

