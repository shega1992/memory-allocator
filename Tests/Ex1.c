#include <stdio.h>
#include <wallocator.h>

int * make_array(int elem, int val);
void show_array(const int ar[], int n);

int main(void)
{
    int * pa;
    int size;
    int value;
    printf("Enter the number of elements: ");
    while (scanf("%d", &size) == 1 && size > 0)
    {
        printf("Enter the initialization value: ");
        scanf("%d", &value);
        pa = make_array(size, value);
        if (pa)
        {
            show_array(pa, size);
            wfree(pa);
        }
        printf("Enter the number of elements (<1 to quit): ");
    }
    printf("Done.\n");
    return 0;
}

int * make_array(int elem, int val)
{
    int *arr;
    arr = (int *) wmalloc(elem * sizeof(int));

    for(int i = 0; i < elem; i++)
        arr[i] = val;

    return arr;
}

void show_array(const int ar[], int n)
{
    for(int i = 0; i < n; i++)
    {
        if(i % 8 == 0)
            putchar('\n');

        printf("%d ", ar[i]);
    }

    putchar('\n');
}
