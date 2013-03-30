
#include <stdio.h>
#include <assert.h>

int bin_search_first(int key,int *p, int n);
int bin_search(int key,int *p, int n);
int bin_search_last(int key,int *p, int n);
int bin_search_just_small(int key, int *p, int n);

int main(int argc, const char *argv[])
{
    int a[] = { 1, 1, 1 , 3, 4, 8, 8, 8, 8, 8};

    for(unsigned int i = 0; i < sizeof(a) / sizeof(a[0]); ++i) {
        printf("%d ", a[i]);
    }
    printf("\n");

    printf("first:\n");
    for(unsigned int i = 0; i < sizeof(a) / sizeof(a[0]); ++i) {
        printf("%d ", bin_search_first(a[i], a, sizeof(a) / sizeof(a[0])));
    }
    printf("\n");

    printf("%d \n", bin_search_first(2, a, sizeof(a) / sizeof(a[0])));

    printf("last:\n");
    for(unsigned int i = 0; i < sizeof(a) / sizeof(a[0]); ++i) {
        printf("%d ", bin_search_last(a[i], a, sizeof(a) / sizeof(a[0])));
    }
    printf("\n");

    return 0;
}


// [0,n), 查找key
int bin_search(int key,int *p, int n)
{
    if (p == NULL || n <= 0)
        return -1;

    int l = 0;
    int r = n;

    while (l < r) {
        int m = l + (r - l) / 2;

        if (p[m] == key)
            return m;
        else if (p[m] < key) {
            l = m + 1;
        } else {
            r = m;
        }
    }
    assert(l == r);
    return -1;
}

int bin_search_first(int key,int *p, int n)
{
    if (p == NULL || n <= 0)
        return -1;

    int l = 0;
    int r = n;

    int first = -1;
    while (l < r) {
        int m = l + (r - l) / 2;

        if (p[m] == key) {
            first = m;
            r = m;
        }
        else if (p[m] < key) {
            l = m + 1;
        } else {
            r = m;
        }
    }

    assert(l == r);

    return first;
}


int bin_search_last(int key,int *p, int n)
{

    if (p == NULL || n <= 0)
        return -1;

    int l = 0;
    int r = n;
    int last = -1;

    while (l < r) {
        int m = l + (r - l) / 2;

        if (p[m] == key) {
            l = m + 1;
            last = m;
        }
        else if (p[m] < key) {
            l = m + 1;
        } else {
            r = m;
        }
    }

    assert(l == r);

    return last;
}
