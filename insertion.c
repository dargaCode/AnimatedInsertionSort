#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

// printf ANSI colors
#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_RESET   "\x1b[0m"

// constants
#define MAX_LEN 200
#define MAX_DELAY 1000
#define RANDOM_SIZE_FACTOR 10

// strings
#define VERBOSE_STRING "-v"

// globals with default values for user inputs
static int delay_milliseconds = 0;
static bool verbose = false;

// prototypes
bool is_valid(int argc, string argv[]);
void randomize_array(int len, int array[]);
void delay_ms(int duration);
void print_array(int len, int array[], int done, int source, int dest);
void sort_array(int len, int array[]);
void insert(int array[], int source, int dest);

/*
 * MAIN
 */
int main(int argc, string argv[])
{
    // bad input
    if (!is_valid(argc, argv))
    {
        printf(COLOR_RED);
        printf("FAILURE \n");
        printf(COLOR_RESET);

        // failure
        return 1;
    }

    // no buffer for printed text, allowing partial lines
    setbuf(stdout, NULL);

    // good input
    int len = atoi(argv[1]);
    int data[len];
    randomize_array(len, data);
    sort_array(len, data);

    // success
    return 0;
}

/*
 * Check command line inputs
 */
bool is_valid(int argc, string argv[])
{
    // allow 2-4 arguments only
    if (argc < 2 || argc > 4)
    {
        return false;
    }

    // require valid array length as second argument
    int len = atoi(argv[1]);
    // outside of the acceptible range
    if (len < 1 || len > MAX_LEN)
    {
        return false;
    }

    // third argument is present
    if (argc > 2)
    {
        // optional argument for delay
        int delay = atoi(argv[2]);
        // outside of the acceptible range
        if (delay < 1 || delay > MAX_DELAY)
        {
            return false;
        }
        // valid; override the global
        else
        {
            delay_milliseconds = delay;
        }
    }

    // fourth argument is present
    if (argc > 3)
    {
        // optional argument for verbose
        string output = argv[3];
        // doesn't match the expected input
        if (strcmp(output, VERBOSE_STRING) != 0)
        {
            return false;
        }
        // valid; override the global
        else
        {
            verbose = output;
        }
    }
    //success
    return true;
}

/*
 * Fill array with random numbers
 */
void randomize_array(int len, int array[])
{
    // random seed based on time
    srand(time(NULL));

    for (int i = 0; i < len; i++)
    {
        int value = rand() % (len * RANDOM_SIZE_FACTOR);
        array[i] = value;
    }
}

/*
 * Delay for a given number of milliseconds
 */
void delay_ms(int delay_duration)
{
    clock_t start = clock();
    clock_t now = clock();
    // convert to milliseconds
    clock_t wait = delay_duration * (CLOCKS_PER_SEC / 1000);

    while (now < start + wait)
    {
        // update time
        now = clock();
    }
}

/*
 * Print out the current state of the sort
 */
 void print_array(int len, int array[], int done, int source, int dest)
{
    // print one extra to enable last dest bracket
    for (int i = 0; i <= len; i++)
    {
        // brackets
        if (i == dest)
        {
            printf(COLOR_YELLOW);
            printf("[");
            printf(COLOR_RESET);
        }
        // hide brackets when dest starts as -1
        else if (i == dest + 1 && dest >= 0)
        {
            printf(COLOR_YELLOW);
            printf("]");
            printf(COLOR_RESET);
        }
        else
        {
            printf(" ");
        }    
        // colors for the element
        if (i < done)
        {
            printf(COLOR_GREEN);
        }
        else if (i == source)
        {
            printf(COLOR_RED);
        }
        // skip very last one; only included for brackets
        if (i < len)
        {
            // actually print the element
            printf("%i", array[i]);
        }
        printf(COLOR_RESET);        
    }
    // pause to display data
    delay_ms(delay_milliseconds);
    // print new line
    printf("\n");
}

/*
 * Insertion sort
 */
void sort_array(int len, int array[])
{
	// outer loop for sorting passes
	// for i = 0 i < len i++
	for (int i = 0; i < len; i++)
	{
		// show starting pass with done but no source/dest
		print_array(len, array, i, -10, -10);

		// initialize source to i
		int source = i;
		// initialize dest to source
		int dest = source - 1;

		// inner loop for each insertion
		// loop dest down to 0
		while (dest > 0)
		{
			// show done/source/dest
			print_array(len, array, i, source, dest);
			// check if dest value <= source value
				// if so insert source at dest + 1
				// and break
			// if not, decrement dest
			dest--;
		}
		// by now dest must be zero
		// show done/source/dest
		print_array(len, array, i, source, dest);
		// insert source at dest
		// break
	}
	// show all as done but no source/dest
	print_array(len, array, len, -10, -10);
	// show sorted array with no done/source/dest
	print_array(len, array, -10, -10, -10);
	// finished sorting
}

/*
 * Insert source at destination and shift bounded values
 */
void insert(int array[], int source, int dest)
{
	// TODO temp
    printf("insert %i at [%i]! \n", array[source], dest);

    // store source in temp
    int temp = array[source]; 
    
    // for values between source and dest
    for (int i = source; i > dest; i--)
    {
        // shift values one to the right
        array[i] = array[i - 1];
    }
    // once i == dest (default)
    // restore dest from temp
    array[dest] = temp;
}
