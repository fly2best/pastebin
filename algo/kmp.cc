#include <stdio.h>
#include <string.h>

void kmpGetNext(const char* p, int * b);
void getFail(const char *P, int *f);
void get_next(const char *p, int *next);
int main(int argc, const char *argv[])
{
    int next[256] = {0};

    const char * str = argv[1];
    int len = strlen(str);

    kmpGetNext(str, next);

    for (int i = 0; i < len; ++i)
        printf("%d ", next[i]);
    printf("\n");

    get_next(str, next);

    for (int i = 0; i < len; ++i)
        printf("%d ", next[i]);
    printf("\n");

    return 0;
}

int kmp(const char *T,const char *P, int *f)
{
    int n = strlen(T);
    int m = strlen(P);

    getFail(P,f);
    int j = 0;

    for (int i = 0; i < n; ++i) {
        while (j != 0 && P[j] != T[i])
            j = f[j];

        if (P[j] == T[i])
            j++;

        if ( j == m)
            return i - m + 1;
    }

    return -1;
}


void getFail(const char *P, int *f)
{
    int m = strlen(P);
    f[0] = f[1] = 0;

    for(int i = 1; i < m; ++i) {
        int j = f[i];
        while (j && P[i] != P[j])
            j = f[j];

        f[i + 1] = P[i] == P[j] ? j + 1 : 0;
    }
}


void kmpGetNext(const char* p, int * b)
{
    int i=0, j=-1;
    b[i]=j;
    int m = strlen(p);
    while (i<m)
    {
        while (j>=0 && p[i]!=p[j]) j=b[j];
        i++; j++;
        b[i]=j;
    }
}




void get_next(const char *p, int *next)
{
    int m = strlen(p);

    int i = 0;
    int j = -1;

    next[i] = j;

    while (i < m) {
        while(j >= 0 && p[i] != p[j])
            j = next[j];

        i++; j++;
        //把next[0] 设置为-1,不是没道理的
        //当next[0]=-1的时候, 可以把代码写的更精简.
        next[i] = j;
    }
}
