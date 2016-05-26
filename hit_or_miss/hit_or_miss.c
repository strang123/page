#include <stdio.h> //fileinput(), printf(), 
#include <stdlib.h> //exit()
#include <string.h> //memset()
#include <time.h> //clock()

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
address_struct *address_head_node;
int bit_counter_int;
int depth_int;
int hit;
FILE *log_file;
int miss;
int num_entry_int;
struct task_struct *process_struct;
int page_size_int;
typedef struct lru_data
{
	int address_int;
	long double time_long_double;
	struct lru_data *head, *tail;					
}lru_struct;
///////////////////////////////////
//FUNCTION PROTOTYPES
///////////////////////////////////
int check_number_of_argument(int);
void create_a_logfile();
void initialize_everything(int, char **);
int get_bit_count(int);
int get_depth(char *);
int get_page(char *);
long double get_time();
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
	//run();
	get_time();
	
}
////////////////////////////////////
//PSUEDOCODE GET_NUMBER_OF_ARGUMENT()
//1.) THERE SHOULD BE THREE ARGUMENTS
//2.) IF NOT WILL RETURN A 2.
/////////////////////////////////////
int check_number_of_argument(int argc)
{
	//1.)
	if(argc == 4)
	{
		return 0;
	}
	//2.)
	else
	{
		kill_error("Not enough arguments, please try again\nPlease use the following format:\n./hit_or_miss <file.txt> <depth> <page size>\n");
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
long double get_time()
{
	return clock();
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
void initialize_everything(int argc, char *argv[])
{
	//1.) 
	hit = 0;
	miss = 0;

	//2.) 
	create_a_logfile();

	//3.)	
	check_number_of_argument(argc);

	//4.)
	read_input(argv[1]);
	
	depth_int = get_depth(argv[2]);
	page_size_int = get_page(argv[3]);
	num_entry_int = num_of_bits();
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
		kill_error("Failed to create a file pointer.\n");
	}			
}
//void run()
//{
//	
//	int test_array[num_entry_int];
//	int test_array[10];
//	memset(&test_array, 0, sizeof(test_array));
//	address_struct *test_node = address_head_node;
//	while(test_node)	
//	{	
//		int test_int = test_node->address_int;
//		for(int i = 0; i < num_entry_int; i++)
//		{
//			for(int j = 0; j < depth_int; j = j + SINGLE_HEX_BYTE)
//			{
//				if(test_array[i] == test_int)
//				{
//					printf("yay\n");
//				}
//				else if(i == num_entry_int-1)
//				{
//					test_array[rand()%5] = test_int;	
//				}
//				test_int = test_int + SINGLE_HEX_BYTE;
//				//printf("%#x is test_int and %#x is array\n", test_int, test_array[i]);
//			}
//		}
//		test_node = test_node->next_address_ptr;
//	}

void run()
{
	
	lru_struct *tlb;
	address_struct *test_node = address_head_node;
	while(test_node)	
	{	
		int test_int = test_node->address_int;
		for(int i = 0; i < 10; i++)
		{
			for(int j = 0; j < depth_int; j = j + SINGLE_HEX_BYTE)
			{
				/*f(test_array[i] == test_int)
				{
					printf("yay\n");
				}
				else if(i == num_entry_int-1)
				{
					test_array[rand()%5] = test_int;	
				}*/
				test_int = test_int + SINGLE_HEX_BYTE;
				//printf("%#x is test_int and %#x is array\n", test_int, test_array[i]);
			}
		}
		test_node = test_node->next_address_ptr;
	}
}
