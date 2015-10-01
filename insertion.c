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
#define MAX_LEN 100
#define MAX_DELAY 1000
#define RANDOM_SIZE_FACTOR 10
#define HIDE -10 // for done/source/slot/dest

// strings
#define VERBOSE_STRING "-v"

// globals with default values for user inputs
static int delay_milliseconds = 0;
static bool verbose = false;

// prototypes
bool is_valid(int argc, string argv[]);
void randomize_array(int len, int array[]);
void delay_ms(int duration);
void print_array(int len, int array[], int done, int source, 
    int slot, int dest);
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
        printf("Usage: insertion <size 1-%i> ", MAX_LEN);
        printf("<delay ms 1-%i> \n", MAX_DELAY);
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
    // outside of the acceptable range
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
 void print_array(int len, int array[], int done, int source,
    int slot, int dest)
{
    // clear the screen if not verbose
    if (!verbose)
    {
        system("clear");
    }

    // print one extra to enable last slot bracket at the end
    for (int i = 0; i <= len; i++)
    {
        // brackets
        if (i == slot)
        {
            printf(COLOR_YELLOW);
            printf("[");
            printf(COLOR_RESET);
        }
        // hide brackets when slot starts as -1
        else if (i == slot + 1 && slot >= 0)
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
            // shifted value color supercedes done color
            if (i > slot && slot >= 0)
            {
                printf(COLOR_YELLOW);
            }
            if (i >= dest && dest >=0)
            {
                printf(COLOR_YELLOW);
            }
        }
        // has to be if instead of else if so source color
        // can supercede dest color after source is inserted
        if (i == source)
        {
            printf(COLOR_RED);
            // dest color supercedes done color
            if (i == dest)
            {
                printf(COLOR_YELLOW);
            }
        }
        // skip very last one; only included for brackets
        if (i < len)
        {
            // actually print the element
            printf("%i", array[i]);
        }
        printf(COLOR_RESET);        
    }
    // print new line
    printf("\n");
    // pause to display data
    delay_ms(delay_milliseconds);
}

/*
 * Insert source at destination and shift bounded values
 */
void insert(int array[], int source, int dest)
{
    // value not actually moving; do nothing
    if (source == dest)
    {
        return;
    }

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

/*
 * Insertion sort
 */
void sort_array(int len, int array[])
{
    // show unsorted array, no done/source/slot/dest
    print_array(len, array, HIDE, HIDE, HIDE, HIDE);

    // build the outer loop to check each source
    // first value is always done by default on first pass
    for (int source = 1; source < len; source++)
    {
        int done = source;
        int slot = source;

        // show start of pass with done, no source/slot/dest
        print_array(len, array, done, HIDE, HIDE, HIDE);

        // loop through slots to find the dest for this source
        while (slot >= 0)
        {
            // show done/source/slot
            print_array(len, array, done, source, slot, HIDE);

            // found a smaller value; can insert
            if (array[slot - 1] <= array[source])
            {
                // show done/source/slot/dest
                print_array(len, array, done, source, slot, 
                    slot);

                // insert at one slot to the right
                insert(array, source, slot);

                // don't redraw if source moved to same spot
                if (slot != source)
                {
                    // show finished pass with swapped values
                    // show done/source, no slot/dest
                    print_array(len, array, done + 1, slot, HIDE, 
                        HIDE);
                }
                // done!
                break;
            }
            // ran out of slots; insert 0
            else if (slot == 0)
            {
                // show done/source/slot/dest
                print_array(len, array, done, source, slot, 
                    slot);

                //insert at the beginning
                insert(array, source, 0);
                // done!
                break;
            }
            // keep looking by continuing the loop
            else
            {
                // move one more slot to the left
                slot--;
            }
        }
    }
    // show all values as done, no source/slot/dest
    print_array(len, array, len, HIDE, HIDE, HIDE);
    // show sorted array, no done/source/slot/dest
    print_array(len, array, HIDE, HIDE, HIDE, HIDE);
}

