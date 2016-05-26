#include <stdio.h> //fileinput(), printf(), 
#include <stdlib.h> //exit()
#include <string.h> //memset()
#include <time.h> //clock()

#define EMPTY 0
#define ING_PROGRAM 1
#define INSTRUCTION_SIZE 32
#define SINGLE_HEX_BYTE 8
////////////////////////////////////
//CONVENTIONS
//1.)NO PLURAL NAMESPACES
//2.)ALL VARIABLES ARE APPENDED WITH DATA TYPE DESIGNATOR EXAMPLE) int example_int = 0;
//3.)CONTROL FLOW IF STATEMENTS IDEALLY CHECK FOR THE AFFIRMATIVE
//4.)VARIABLES AND FUNCTIONS ARE ALL ALPHEBETICAL
//5.)MAIN() IS AT THE TOP
////////////////////////////////////
///////////////////////////////////
///////////////////////////////////
//DATA
///////////////////////////////////
typedef struct infile_data
{
	int address_int;
	struct infile_data *next_address_ptr;		
}address_struct;
typedef struct cache_data
{
	int address_int;
	struct cache_data *head;					
}cache_struct;
///////////////////////////////////
//DATA VARIABLES
///////////////////////////////////
address_struct *address_head_node;
int bit_counter_int;
cache_struct *cache_head_node;
int cache_size_int;
int depth_int;
int hit_int;
FILE *log_file;
int miss_int;
int number_of_logical_bits;
int page_size_int;
int shift_amount;
///////////////////////////////////
//FUNCTION PROTOTYPES
///////////////////////////////////
int check_number_of_argument(int);
void create_a_logfile();
void create_cache(int);
void initialize_everything(int, char **);
int get_bit_count(int);
int get_cache_size(char *);
int get_depth(char *);
int get_page(char *);
//long double get_time();
void kill_error(char *);
int num_of_bits();
void read_input(char *);
void run();
///////////////////////////////////

////////////////////////////////////
//ORDER OF OPERATIONS
//1.) MAIN() 
//	A.)INIT()
//		I.) CREATE_LOG_FILE()
//		II.) CHECK_NUM_ARG()
//		ii 
//	B.)RUN()		
//   
////////////////////////////////////



////////////////////////////////////
//PSUEDOCODE MAIN()
//1.) EXECUTE INIT() FUNCTION THAT WILL CREATE A LOG AND READ THE INPUT FILE AND 
//    STORE THE HEX ADDRESS DATA IN A STRUCT OF TYPE INFILE_DATA 
//    WITH NAMESPACE ADDRESS_STRUCT.
//2.) THEN EXECUTES RUN() THE PROGRAM
///////////////////////////////////
void main(int argc, char *argv[])
{
	//1.)
	initialize_everything(argc, argv);
	//2.)
	run();
	
}
////////////////////////////////////
//PSUEDOCODE GET_NUMBER_OF_ARGUMENT()
//1.) THERE SHOULD BE THREE ARGUMENTS
//2.) IF NOT WILL RETURN A 2.
/////////////////////////////////////
int check_number_of_argument(int argc)
{
	//1.)
	if(argc == 5)
	{
		return 0;
	}
	//2.)
	else
	{
		kill_error("Not enough arguments, please try again\nPlease use the following format:\n./hit_or_miss <file.txt> <depth in power of 2> <page size in power of 2> <cach size in integer>\n");
	}
}
////////////////////////////////
//PSEUDOCODE CREATE_LOG_FILE()
//1.) OPEN A LOG CALLED LOG.TXT WITH WRITE PERMISSIONS
////////////////////////////////
void create_a_logfile()
{
	//1.)
	log_file = fopen("log.txt", "w+");
	fprintf(log_file, "Log created, Program running...\n");
}
void create_cache(int size_arg)
{
	for(int i=0; i < cache_size_int; i++)
	{
		if(cache_head_node == NULL)
		{
			cache_head_node = (struct cache_data *)malloc(sizeof(struct cache_data));
			cache_head_node->address_int = 0;
			cache_head_node->head = NULL;
		}
		else
		{
			cache_struct *new_node = (struct cache_data *)malloc(sizeof(struct cache_data));
			cache_struct *current_node = (struct cache_data *)malloc(sizeof(struct cache_data));
			current_node = cache_head_node;
			while(current_node->head != NULL)
			{
				current_node = current_node->head;
			}
			new_node->address_int = 0;
			current_node->head = new_node;
			new_node->head = NULL;
		}
	}
	#if DEBUG==1
	cache_struct *print_node = cache_head_node;
	fprintf(log_file,"\nInitial Stack\n-------------\n");
	while(print_node)
	{
		fprintf(log_file, "[address=%#x]\n", print_node->address_int);
		print_node = print_node->head;
	}
	fprintf(log_file,"\n");
	#endif
}
int get_bit_count(int page_arg)
{	
	if(page_arg == 2)
	{
		return bit_counter_int;
	}
	else
	{
		bit_counter_int++;
		return get_bit_count(page_arg/2);
	}
}
int get_cache_size(char *cache_arg)
{
	int return_int = atoi(cache_arg);
	return return_int;	
}
/////////////////////////////////
//PSEUDOCODE GET_DEPTH()
//1.) GET THE DEPTH VALUE THAT WILL BE USED TO DETERMINE HOW MANY BYTES WILL BE SCANNED
//    
/////////////////////////////////
int get_depth(char *num_arg)
{ 
	int return_int = atoi(num_arg);
	if(return_int%INSTRUCTION_SIZE != 0)
	{
		kill_error("Page depth is incompatible\n");
	}
	else
	{
		return return_int; 
	}
}
int get_page(char *num_args)
{
	int return_int = atoi(num_args);
	if(return_int%INSTRUCTION_SIZE != 0)
	{
		kill_error("Page size is incompatible\n");
	}
	else
	{
		return return_int; 
	}
}
//long double get_time()
//{
//	return clock();
//}
////////////////////////////////
//PSUEDOCODE INIT()
//1.) INITIALIZE THE HIT AND MISS VARIABLES TO ZERO
//2.) EXECUTE CREATE_LOG_FILE() FUNCTION TO BEGIN LOGGING DEBUGGING AND EVENT INFORMATION
//3.) EXECUTE THE CHECK_NUM_ARG() FUNCTION THAT WILL ENSURE THE CORRECT NUMBER OF ARGUMENTS
//    ARE PRESENT.  IF NOT ENOUGH ARGUMENTS ARE INCLUDED THEN PROGRAM WILL TERMINATE WITH ERROR
//    CODE 2.
//4.) IF THE NUMBER OF ARGUMENTS CHECKS OUT, THEN GOO
////////////////////////////////
void initialize_everything(int argc, char *argv[])
{
	//1.) 
	hit_int = 0;
	miss_int = 0;

	//2.) 
	create_a_logfile();

	//3.)	
	check_number_of_argument(argc);

	//4.)
	read_input(argv[1]);
	
	depth_int = get_depth(argv[2]);
	page_size_int = get_page(argv[3]);
	cache_size_int = get_cache_size(argv[4]);
	number_of_logical_bits = num_of_bits();
	shift_amount = INSTRUCTION_SIZE-number_of_logical_bits;
	create_cache(cache_size_int);
		
	#if DEBUG==1
	fprintf(log_file, "The number of logical address bits is %d\nThe number to shift is %d\n", number_of_logical_bits,shift_amount);
	#endif
}
void kill_error(char *message)
{
	fprintf(stderr,"%s",message);
	exit(ING_PROGRAM);
}
int num_of_bits()
{
	return INSTRUCTION_SIZE - get_bit_count(page_size_int) - 1;
}
void read_input(char* file)
{	
	FILE *infile_fileptr;
	if(file != NULL)
	{	
		infile_fileptr = fopen(file, "r+");
		while(!feof(infile_fileptr))
		{
			address_struct *new_node = (struct infile_data *)malloc(sizeof(struct infile_data));
			address_struct *current_node = (struct infile_data *)malloc(sizeof(struct infile_data));
			if(new_node == NULL || current_node == NULL)
			{	
				kill_error("Could not create nodes from the address struct.\n");		
			}
			if(address_head_node == NULL)
			{
				address_head_node = (struct infile_data *)malloc(sizeof(struct infile_data));
				if(address_head_node == NULL)
				{
					kill_error("Could not create nodes from the address head node");
				}
				fscanf(infile_fileptr, "%x\n", &address_head_node->address_int);
				address_head_node->next_address_ptr = NULL;
			}
			else
			{
				current_node = address_head_node;
				while(current_node->next_address_ptr != NULL)
				{
					current_node = current_node->next_address_ptr;
				}
				fscanf(infile_fileptr, "%x\n", &new_node->address_int);
				current_node->next_address_ptr = new_node;
				new_node->next_address_ptr = NULL;
			}
		}
		fclose(infile_fileptr);

		#if DEBUG==1
		fprintf(log_file, "The input file consists of the following:\n");
		address_struct *new_node = address_head_node;
		while(new_node)	
		{	
			fprintf(log_file, "%#x\n", new_node->address_int);
			new_node = new_node->next_address_ptr;
		}
		#endif
	}
	else
	{
		kill_error("Failed to create a file pointer.\n");
	}			
}
void run()
{
	cache_struct *cache_node, *insert_at_zero_node, *last_node_for_deletion, *second_to_last_for_deletion, *second_node, *prev_node;
	address_struct *test_node = address_head_node;
	int found = 0;
	while(test_node)	
	{	
		cache_node = cache_head_node;
		second_node = cache_head_node;
		prev_node = (struct cache_data *)malloc(sizeof(struct cache_data));
		insert_at_zero_node = (struct cache_data *)malloc(sizeof(struct cache_data));
		last_node_for_deletion = cache_head_node;
		second_to_last_for_deletion = cache_head_node;
		while(last_node_for_deletion)
		{
			last_node_for_deletion = last_node_for_deletion->head;
		}
		while(second_to_last_for_deletion->head->head)
		{
			second_to_last_for_deletion = second_to_last_for_deletion->head;
		}
		int test_int = test_node->address_int>>shift_amount;		
		while(cache_node)
		{
			if(test_int == cache_node->address_int && prev_node->head == NULL)
			{
				hit_int++;
				fprintf(log_file, "!HIT!\n");
				found = 1;
				break;
			}
			else if(test_int == cache_node->address_int)
			{
				hit_int++;
				found = 1;
				insert_at_zero_node->address_int = test_int;
				insert_at_zero_node->head = second_node;
				prev_node->head = cache_node->head;
				cache_head_node = insert_at_zero_node;
				fprintf(log_file, "!HIT!\n");
				break;
			}
			prev_node = cache_node;
			cache_node = cache_node->head;
		}
		if(found == 0)
		{
			cache_node = cache_head_node;
		
			while(cache_node)
			{	
				if(cache_node->address_int == EMPTY)
				{
					cache_node->address_int = test_int; 
					miss_int++;			
					break;		
				}
				else
				{
					insert_at_zero_node->address_int = test_int;
					insert_at_zero_node->head = second_node;
					cache_head_node = insert_at_zero_node;
					second_to_last_for_deletion->head = NULL;
					free(last_node_for_deletion);
					miss_int++;
					break;			
				}
				cache_node = cache_node->head;
			}
		}
		test_node = test_node->next_address_ptr;
		found = 0;
		
		#if DEBUG==1
		int counter = 0;
		cache_struct *print_node = cache_head_node;
		fprintf(log_file,"Stack after %#x\n-------------\n",test_int);
		while(print_node)
		{
			fprintf(log_file, "[address=%#x]\n", print_node->address_int);
			print_node = print_node->head;
			counter++;
		}
		fprintf(log_file, "TOTAL =%d\n",counter);
		fprintf(log_file,"\n");
		#endif
	}
	printf("Number of hits %d and number of misses %d\n",hit_int, miss_int); 
}
