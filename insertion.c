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

    int dest = 0;
    int done = len - 1;
    int source = len - 1;
    int hide = -10;
    
    // show starting pass
    print_array(len, array, done, hide, hide); 

    // show source and destination 
    print_array(len, array, done, source, dest); 

    printf("source: %i dest: %i \n", array[source], array[dest]);

    // if source >= dest, already sorted; do nothing
    if (array[source] >= array[dest])
    {
        printf("Already sorted! No insert\n");
    }
    
    // if source < dest, insert before dest
    else if (array[source] < array[dest])
    {
            insert(array, source, dest);    
    }
    
    // show all done
    print_array(len, array, len, hide, hide);
    // show result
    print_array(len, array, hide, hide, hide);
    
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
