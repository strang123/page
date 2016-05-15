#include <stdio.h> //fileinput(), printf(), 
#include <stdlib.h> //exit()
#include <string.h> //memset()

#define FILE_EXIST 0
#define FILE_NOT_EXIST 1
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
//RETURN VALUES
//1.) 0=NOMINAL
//2.) 1=NO_FILE_INPUT
//3.) 2=WRONG NUMBER OF ARGUMENTS
//4.) 3=FAILED TO CREATE A POINTER
///////////////////////////////////
///////////////////////////////////
//DATA
///////////////////////////////////
typedef struct infile_data
{
	int address_int;
	struct infile_data *next_address_ptr;		
}address_struct;
address_struct *address_head_node;
int bit_counter_int;
int depth_int;
FILE *log_file;
int num_entry_int;
struct task_struct *process_struct;
int page_size_int;
typedef struct page_table
{
					
}p_table;
///////////////////////////////////
//FUNCTION PROTOTYPES
///////////////////////////////////
int check_num_arg(int);
void create_log_file();
void init(int, char **);
int get_bit_count(int);
int get_depth(char *);
int get_page(char *);
int read_input(char *);
int hit, miss;
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
	init(argc, argv);
	//2.)
	run();
}
////////////////////////////////////
//PSUEDOCODE CHECK_NUM_ARG()
//1.) THERE SHOULD BE THREE ARGUMENTS
//2.) IF NOT WILL RETURN A 2.
/////////////////////////////////////
int check_num_arg(int argc)
{
	//1.)
	if(argc == 4)
	{
		return 0;
	}
	//2.)
	else
	{
		fprintf(stderr, "Not enough arguments, please try again\n");
		return 2;
	}
}
////////////////////////////////
//PSEUDOCODE CREATE_LOG_FILE()
//1.) OPEN A LOG CALLED LOG.TXT WITH WRITE PERMISSIONS
////////////////////////////////
void create_log_file()
{
	//1.)
	log_file = fopen("log.txt", "w+");
	fprintf(log_file, "Log created, Program running...\n");
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
/////////////////////////////////
//PSEUDOCODE GET_DEPTH()
//1.) GET THE DEPTH VALUE THAT WILL BE USED TO DETERMINE HOW MANY BYTES WILL BE SCANNED
//    
/////////////////////////////////
int get_depth(char *num_args)
{ 
	int return_int = atoi(num_args);
	if(return_int%INSTRUCTION_SIZE != 0)
	{
		fprintf(stderr, "Page depth is incompatible\n");
		exit(1);
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
		fprintf(stderr, "Page size is incompatible\n");
		exit(1);
	}
	else
	{
		return return_int; 
	}
}
////////////////////////////////
//PSUEDOCODE INIT()
//1.) INITIALIZE THE HIT AND MISS VARIABLES TO ZERO
//2.) EXECUTE CREATE_LOG_FILE() FUNCTION TO BEGIN LOGGING DEBUGGING AND EVENT INFORMATION
//3.) EXECUTE THE CHECK_NUM_ARG() FUNCTION THAT WILL ENSURE THE CORRECT NUMBER OF ARGUMENTS
//    ARE PRESENT.  IF NOT ENOUGH ARGUMENTS ARE INCLUDED THEN PROGRAM WILL TERMINATE WITH ERROR
//    CODE 2.
//4.) IF THE NUMBER OF ARGUMENTS CHECKS OUT, THEN GOO
////////////////////////////////
void init(int argc, char *argv[])
{
	//1.) 
	hit = 0;
	miss = 0;

	//2.) 
	create_log_file();

	//3.)	
	int arg_test_int = check_num_arg(argc);
	if(arg_test_int == 0)
	{	
		fprintf(log_file, "Correct Number of arguments detected\n");
	}
	else if(arg_test_int == 2)
	{
		fprintf(stderr, "Please use the following format:\n./hit_or_miss <file.txt> <depth> <page size>\n");
		exit(2);
	}
	else if(arg_test_int == 3)
	{	
		fprintf(stderr, "Failed to create temp pointer to address_struct\n");
		exit(3);
	}
	

	//4.)
	int infile_exist_int = read_input(argv[1]);
	if(infile_exist_int == FILE_EXIST)
	{
		fprintf(log_file, "Valid input file detected, parsing...\n");
	}
	else if(infile_exist_int == FILE_NOT_EXIST)
	{	
		fprintf(stderr, "No file provided\n");
		exit(1);
	}
	depth_int = get_depth(argv[2]);
	page_size_int = get_page(argv[3]);
	num_entry_int = INSTRUCTION_SIZE - get_bit_count(page_size_int) - 1;
	fprintf(log_file, "Number of addresses to linearly traverse: %d\n", depth_int);
}
int read_input(char* file)
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
				return 3;		
			}
			if(address_head_node == NULL)
			{
				address_head_node = (struct infile_data *)malloc(sizeof(struct infile_data));
				if(address_head_node == NULL)
				{
					return 3;		
				}
				fscanf(infile_fileptr, "%x", &address_head_node->address_int);
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
		address_struct *new_node = address_head_node;
		while(new_node)	
		{	
			printf("%#x\n", new_node->address_int);
			new_node = new_node->next_address_ptr;
		}
		#endif
	}
	else
	{
		return 1;
	}			
}
void run()
{
	
	int test_array[num_entry_int];
	memset(&test_array, 0, sizeof(test_array));
	address_struct *test_node = address_head_node;
	while(test_node)	
	{	
		int test_int = test_node->address_int;
		for(int i = 0; i < 5; i++)
		{
			for(int j = 0; j < depth_int; j = j + SINGLE_HEX_BYTE)
			{
				if(test_array[i] == test_int)
				{
					printf("yay\n");
				}
				else if(i == 4)
				{
					test_array[rand()%5] = test_int;	
				}
				test_int = test_int + SINGLE_HEX_BYTE;
				//printf("%#x is test_int and %#x is array\n", test_int, test_array[i]);
			}
		}
		test_node = test_node->next_address_ptr;
	}
}
