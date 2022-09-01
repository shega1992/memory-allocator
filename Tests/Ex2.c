#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wallocator.h>
#define SIZE 41

int get_number_of_words(void);
void read_the_words(char *arr_of_ptr[],int number_of_words);
void print_the_words(char *arr_of_ptr[], int number_of_words);

int main(void)
{
    int number_of_words;
    char **arr_of_ptr;

    number_of_words = get_number_of_words();

    arr_of_ptr = (char **) wmalloc(number_of_words * sizeof(char *));

    if(arr_of_ptr == NULL)
    {
        puts("Memory allocation failed. Goodbye.");
        exit(EXIT_FAILURE);
    }

    read_the_words(arr_of_ptr, number_of_words);
    print_the_words(arr_of_ptr, number_of_words);

    for(int i = 0; i < number_of_words; i++)
        wfree(arr_of_ptr[i]);

    wfree(arr_of_ptr);

    return 0;
}

int get_number_of_words(void)
{
    int number_of_words;

    fputs("Please enter the number of words: ", stdout);

    while(scanf("%d", &number_of_words) != 1 || number_of_words <= 0)
    {
        fputs("Please enter the number of words greater than 0: ", stdout);

        while(getchar() != '\n')
            continue;
    }

    return number_of_words;

}

void read_the_words(char *arr_of_ptr[],int number_of_words)
{
    int i;
    char temp[SIZE];
    char *ptr_to_word;

    printf("Enter %d words now:\n", number_of_words);

    for(i = 0; i < number_of_words; i++)
    {
        scanf("%40s", temp);
        ptr_to_word = (char *) wmalloc((strlen(temp) + 1) * sizeof(char));

        if(ptr_to_word == NULL)
        {
            puts("Memory allocation failed. Goodbye.");
            exit(EXIT_FAILURE);
        }

        arr_of_ptr[i] = ptr_to_word;

        strcpy(arr_of_ptr[i], temp);

    }

}

void print_the_words(char *arr_of_ptr[], int number_of_words)
{
    int i;
    puts("Here are your words:");

    for(i = 0; i < number_of_words; i++)
        printf("%s\n", arr_of_ptr[i]);


}


