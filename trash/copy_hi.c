#include <stdio.h> //fileinput(), printf(), 
#include <stdlib.h> //exit()

#define FILE_EXIST 0
#define FILE_NOT_EXIST 1
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
int depth_int;
FILE *log_file;
struct task_struct *process_struct;
///////////////////////////////////
//PROTOTYPES
///////////////////////////////////
int check_num_arg(int);
void create_log_file();
void init(int, char **);
int get_depth(char *);
int read_input(char *);
void run();
///////////////////////////////////

////////////////////////////////////
//PSUEDOCODE MAIN()
//1.) BEGIN AN INIT() FUNCTION THAT WILL CREATE A LOG AND READ THE INPUT FILE AND 
//    STORE THE DATA IN A UNION DATA STRUCTURE.
//2.) THEN RUN() THE PROGRAM
///////////////////////////////////
void main(int argc, char *argv[])
{
	//1.)
	init(argc, argv);
	//2.)
	run();
}
int check_num_arg(int argc)
{
	if(argc == 3)
	{
		return 0;
	}
	else
	{
		fprintf(stderr, "Not enough arguments, please try again\n");
		return 2;
	}
}
void create_log_file()
{
	log_file = fopen("log.txt", "w+");
	fprintf(log_file, "Log created, Program running...\n");
}
int get_depth(char *num_args)
{
	return atoi(num_args); 
}
void init(int argc, char *argv[])
{
	create_log_file();
	
	int arg_test_int = check_num_arg(argc);
	if(arg_test_int == 0)
	{	
		fprintf(log_file, "Correct Number of arguments detected\n");
	}
	else if(arg_test_int == 2)
	{
		fprintf(stderr, "Please use the following format:\n./hit_or_miss <file.txt> <depth>\n");
		exit(2);
	}
	else if(arg_test_int == 3)
	{	
		fprintf(stderr, "Failed to create temp pointer to address_struct\n");
		exit(3);
	}
	

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
	fprintf(log_file, "Number of addresses to linearly travers:%d\n", depth_int);
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
	
}
