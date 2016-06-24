#include <stdio.h> 	//open(),close(), printf()
#include <stdlib.h> 	//exit()
#include <string.h> 	//memset()
#include <stdbool.h>

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
pagetable_data *pagetable_head, *pagetable_tail; 
int hit_int = 0;
int miss_int = 0;
FILE *logfile_ptr;
char *instruction_message = "Not enough arguments, please try again\nPlease use the following format:\n./hit_or_miss <file.txt> <depth in power of 2> <page size in power of 2> <cach size in integer>\nExample) ./hit_or_miss wr128_random-1.txt 256 4096 128\n";
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
void run();

void main(int argc, char *argv[])
{
	initialize(argc, argv);
	run();
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
		fscanf(tempfile_ptr, "%x\n",&temp_int);
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
	for(int i = 1; i < pagetable_int; i++)
	{
		new_node = (struct pagetable_struct *)malloc(sizeof(struct pagetable_struct));
		new_node->address_int = 0;
		placeholder_node->pagetable_ptr = new_node;
		new_node->pagetable_ptr = NULL;	
		placeholder_node = new_node;	
	}
	pagetable_tail = placeholder_node;
	//use "make debug" if you to print inputfile 
	//-------------------------------------------------------
	#if DEBUG==1
		fprintf(logfile_ptr, "\nInitial Stack\n----------------\n");
		pagetable_data *print_node = pagetable_head;
		while(print_node)
		{
			fprintf(logfile_ptr, "[address=%#x]\n",print_node->address_int);
			print_node = print_node->pagetable_ptr;
		}
		fprintf(logfile_ptr,"\n");
	#endif
}
void run()
{
	inputfile_data *input_ptr = inputfile_head;	
	pagetable_data *current_node, *prev_node, *temp_node, *top_node, *last_node;
	bool found = false;
	int test_address_int = 0;
	int cache_address_int = 0;
	
	while(input_ptr)
	{
		current_node = (struct pagetable_struct *)malloc(sizeof(struct pagetable_struct));
		temp_node = (struct pagetable_struct *)malloc(sizeof(struct pagetable_struct));
		top_node = pagetable_head;
		current_node = top_node;
		test_address_int = input_ptr->address_int;
		cache_address_int = current_node->address_int;
		//LOOP TO FIND MATCHING ADDRESS IN TLB
		//Test the first element in the TLB
		//-------------------------------------------------------
		if(test_address_int == cache_address_int)
		{
			hit_int++;
			found = true;
			fprintf(logfile_ptr, "\n!HIT!");
		}
		else
		{
			//Test the n+1 elements
			//-----------------------------------------------
			while(current_node->pagetable_ptr)
			{
				//cache traversal
				//---------------------------------------
				prev_node = current_node;
				current_node = current_node->pagetable_ptr;
				cache_address_int = current_node->address_int;
				if(test_address_int == cache_address_int)
				{
					hit_int++;
					found = true;
					fprintf(logfile_ptr, "\n!HIT!");
					temp_node->address_int = cache_address_int;
					temp_node->pagetable_ptr = top_node;
					pagetable_head = temp_node;
					prev_node->pagetable_ptr = current_node->pagetable_ptr;
					free(current_node);
					break;
				}
			}
		}
		//IF NO MATCHING ADDRESS IN TLB, THEN LOOP TO ADD THE ADDRESS
		//------------------------------------------------------------
		if(found == false)
		{
			miss_int++;

			//[n]->[n-1]->...->[n-k]->NULL
			// ^_________________|
			last_node = current_node;

			//[n]->[n-k]->NULL  [X_node]->???
			// ^__________________|
			current_node = top_node;

			if(current_node->address_int == 0)
			{
				current_node->address_int = test_address_int;
			}
			else
			{
				temp_node->address_int = test_address_int;
				temp_node->pagetable_ptr = top_node;
				pagetable_head = temp_node;
				prev_node->pagetable_ptr = NULL;
			}
			free(last_node);	
		}
		found = false;
		input_ptr = input_ptr->address_ptr;
	//Print stack after every iteration
	//---------------------------------------------------------------------
	#if DEBUG==1
		fprintf(logfile_ptr, "\nStack after %#x\n----------------\n",test_address_int);
		pagetable_data *print_node = pagetable_head;
		while(print_node)
		{
			fprintf(logfile_ptr, "[address=%#x]\n",print_node->address_int);
			print_node = print_node->pagetable_ptr;
		}
		fprintf(logfile_ptr,"\n");
	#endif
	}
	//Print the result
	//----------------------------------------------------------------------	
	printf("Number of hits %d and number of misses %d\n",hit_int, miss_int); 
}
