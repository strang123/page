#include <stdio.h> 	//open(),close(), printf()
#include <stdlib.h> 	//exit()
#include <string.h> 	//memset()

#define EMPTY 0
#define PROGRAM 1
#define INSTRUCTION_SIZE32 32
#define VALID_NUMBER_ARGUMENTS 5
#define DEPTH_ARGUMENT 2
#define PAGESIZE_ARGUMENT 3
#define PAGETABLESIZE_ARGUMENT 4
#define MODULO_RETURNS_ZERO 0

typedef struct inputfile_struct
{
	int address_int;
	struct inputfile_struct *address_ptr;
}inputfile_data;
typedef struct pagetable_struct
{
	int address_int;
	struct pagetable_struct *pagetable_ptr;
}pagetable_data;
inputfile_data *inputfile_head;
pagetable_data *pagetable_head; 
int hit_int = 0;
int miss_int = 0;
FILE *logfile_ptr;
char *instruction_message = "Not enough arguments, please try again\nPlease use the following format:\n./hit_or_miss <file.txt> <depth in power of 2> <page size in power of 2> <cach size in integer>\nExample) ./hit_or_miss 256 4096 128\n";
int bytedepth_int;
int pagesize_int;


void initialize(int, char**);
void create_log();
void process_arguments(int, char**);
void kill_error(char *);


void main(int argc, char *argv[])
{
	initialize(argc, argv);
}
void initialize(int argc_proxy, char *argv_proxy[])
{
	create_log();
	process_arguments(argc_proxy, argv_proxy);

}
void create_log()
{
	logfile_ptr = fopen("log.txt","w+");
	fprintf(logfile_ptr,"Log created, Program initiating...\n");
}
void process_arguments(int argc_proxy,char *argv_proxy[])
{
	//-----------------------------------------------------
	if(argc_proxy == VALID_NUMBER_ARGUMENTS)
	{
		fprintf(logfile_ptr, "Correct number of arguments detected\n");
	}
	else
	{
		kill_error("Incorrect Number of Arugments!\n");
	}
	//------------------------------------------------------
	bytedepth_int = atoi(argv_proxy[DEPTH_ARGUMENT]);
	if(bytedepth_int%INSTRUCTION_SIZE32 == MODULO_RETURNS_ZERO)
	{
		fprintf(logfile_ptr, "Depth = %d\n",bytedepth_int);
	}
	else
	{
		kill_error("Faulty depth value!\n");
	}
	//-------------------------------------------------------
	pagesize_int = atoi(argv_proxy[PAGESIZE_ARGUMENT]);
	if(pagesize_int%INSTRUCTION_SIZE32 == MODULO_RETURNS_ZERO)
	{
		fprintf(logfile_ptr, "Pagesize = %d\n", pagesize_int); 
	}
	else
	{
		kill_error("Faulty pagesize value!\n");
	}
}
void kill_error(char *error_message)
{
	fprintf(stderr,"\n%s",error_message);
	fprintf(stderr,"%s\n",instruction_message);
	exit(PROGRAM);
}
