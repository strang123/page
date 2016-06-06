#include <stdio.h> 	//open(),close(), printf()
#include <stdlib.h> 	//exit()
#include <string.h> 	//memset()

#define EMPTY 0
#define PROGRAM 1
#define INSTRUCTION_SIZE32 32
#define VALID_NUMBER_ARGUMENTS 5
#define INPUTFILE_NAME 1
#define DEPTH_ARGUMENT 2
#define PAGESIZE_ARGUMENT 3
#define PAGETABLE_ARGUMENT 4
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
int shiftamount_int;
int pagetable_int;
int addresscount_int;

void initialize(int, char**);
void create_log();
void process_arguments(int, char**);
void kill_error(char *);
void get_offset(int);
void input_file(char *);
void cache_initialize();

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
	//check number of args
	//-----------------------------------------------------
	if(argc_proxy == VALID_NUMBER_ARGUMENTS)
	{
		fprintf(logfile_ptr, "Correct number of arguments detected\n");
	}
	else
	{
		kill_error("Incorrect Number of Arugments!\n");
	}
	//check valid byte depth
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
	//check valid page size
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
	//get the number of offset bits
	//-------------------------------------------------------
	int pagetable_temp_int = atoi(argv_proxy[PAGESIZE_ARGUMENT]);
	get_offset(pagetable_temp_int);
	//check valid page table entry size
	//-------------------------------------------------------
	pagetable_int = atoi(argv_proxy[PAGETABLE_ARGUMENT]);
	if(pagetable_int > 1)
	{
		fprintf(logfile_ptr, "Pagetable Size = %d\n", pagetable_int);
	}
	else
	{
		kill_error("Faulty Pagetable entry size!\n");
	}
	//take the input file and place in array
	//--------------------------------------------------------
	input_file(argv_proxy[INPUTFILE_NAME]);
	//initialize the TLB cache
	//--------------------------------------------------------
	cache_initialize();
}
void kill_error(char *error_message)
{
	fprintf(stderr,"\n%s",error_message);
	fprintf(stderr,"%s\n",instruction_message);
	exit(PROGRAM);
}
void get_offset(int pagesize_proxy)
{
	if(pagesize_proxy == 2)
	{
		shiftamount_int++;
		fprintf(logfile_ptr, "Shiftamount = %d\n",shiftamount_int);
		return;
	}
	else
	{
		shiftamount_int++;
		return get_offset(pagesize_proxy/2);
	}
}
void input_file(char *inputfile_name)
{
	FILE *tempfile_ptr = fopen(inputfile_name, "r+");
	int temp_int = 0;
	if(tempfile_ptr == NULL)
	{
		kill_error("Faulty input file name in commandline!\n");
	}
	else
	{
		//get first element in input file
		//----------------------------------------------
		inputfile_data *placeholder_node, *new_node;
		inputfile_head = (struct inputfile_struct *)malloc(sizeof(struct inputfile_struct));
		//fscanf(tempfile_ptr, "%x\n",&inputfile_head->address_int);
		fscanf(tempfile_ptr, "%x\n",&temp_int);
		//inputfile_head->address_int = inputfile_head->address_int>>shiftamount_int;
		inputfile_head->address_int = temp_int>>shiftamount_int;
		inputfile_head->address_ptr = NULL;
		placeholder_node = inputfile_head;
		//get the rest of the elements
		//-----------------------------------------------
		while(!feof(tempfile_ptr))
		{
			new_node = (struct inputfile_struct *)malloc(sizeof(struct inputfile_struct));
			fscanf(tempfile_ptr, "%x\n",&temp_int);
			new_node->address_int = temp_int>>shiftamount_int;
			new_node->address_ptr = NULL;
			placeholder_node->address_ptr = new_node;
			placeholder_node = new_node;
			addresscount_int++;
		}
	}
	//use "make debug" if you to print inputfile 
	//-------------------------------------------------------
	#if DEBUG==1
		fprintf(logfile_ptr, "The input file consists of the following\n");
		inputfile_data *print_node = inputfile_head;
		while(print_node)
		{
			fprintf(logfile_ptr, "%#x\n",print_node->address_int);
			print_node = print_node->address_ptr;
		}
	#endif	
}
void cache_initialize()
{
	pagetable_data *new_node, *placeholder_node;
	//create head node for cache
	//-------------------------------------------------------	
	pagetable_head = (struct pagetable_struct *)malloc(sizeof(struct pagetable_struct));
	pagetable_head->address_int = 0;
	pagetable_head->pagetable_ptr = NULL;
	placeholder_node = pagetable_head;
	//create the rest of the TLB
	//-------------------------------------------------------
	for(int i = 0; i < pagetable_int; i++)
	{
		new_node = (struct pagetable_struct *)malloc(sizeof(struct pagetable_struct));
		new_node->address_int = 0;
		placeholder_node->pagetable_ptr = new_node;
		new_node->pagetable_ptr = NULL;	
		placeholder_node = new_node;	
	}
	#if DEBUG==1
		fprintf(logfile_ptr, "\nInitial Stack\n----------------\n");
		pagetable_data *print_node;
		while(print_node)
		{
			fprintf(logfile_ptr, "[address=%#x]\n",print_node->address_int);
			print_node = print_node->pagetable_ptr;
		}
		fprintf(logfile_ptr,"\n")
	#endif
}
