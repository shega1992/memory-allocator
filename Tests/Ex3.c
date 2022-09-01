/* films2.c -- using a linked list of structures */
#include <stdio.h>
#include <string.h>
#include <wallocator.h>
#define TSIZE 45

struct film {
    char title[TSIZE];
    int rating;
    struct film * next; /* points to next struct in list */
};

char * s_gets(char * st, int n);
void print_list_in_reverse_order(struct film *current);

int main(void)
{
    struct film * head = NULL;
    struct film * prev, * current;
    char input[TSIZE];


    puts("Enter first movie title:");
    while (s_gets(input, TSIZE) != NULL && input[0] != '\0')
    {
        current = (struct film *) wmalloc(sizeof(struct film));
        if (head == NULL) /* first structure */
            head = current;
        else /* subsequent structures */
            prev->next = current;

        current->next = NULL;
        strcpy(current->title, input);
        puts("Enter your rating <0-10>:");
        scanf("%d", &current->rating);

        while(getchar() != '\n')
            continue;

        puts("Enter next movie title (empty line to stop):");
        prev = current;
    }

    /* Show list of movies */
    if (head == NULL)
        printf("No data entered. ");
    else
        printf ("Here is the movie list:\n");

    current = head;
    while (current != NULL)
    {
        printf("Movie: %s Rating: %d\n",
            current->title, current->rating);
        current = current->next;
    }

    puts("**********************************************");
    printf ("Here is the movie list in reverse order:\n");

    current = head;
    print_list_in_reverse_order(current);


/* Program done, so free allocated memory */
    current = head;
    while (current != NULL)
    {
        wfree(current);
        current = current->next;
    }
    printf("Bye!\n");
    return 0;
}
char * s_gets(char * st, int n)
{
    char * ret_val;
    char * find;
    ret_val = fgets(st, n, stdin);
    if (ret_val)
    {
        find = strchr(st, '\n');
        if (find)
            *find = '\0';
        else
            while (getchar() != '\n')
                continue;
    }
    return ret_val;
}

void print_list_in_reverse_order(struct film *current)
{
     if(current == NULL)
        return;
     else
        print_list_in_reverse_order(current->next);


    printf("Movie: %s Rating: %d\n",
            current->title, current->rating);
}
