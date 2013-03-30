
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//poj 2388
//G++ runtiime error  //srand(time(NULL)),悠着点
//C++ AC
//不只是要理解思路, 还要攒一些好的代码
//partition用的,用的是算法导论上的代码，思路很清新
//findk, 写了两个版本 递归的和非递归的
//个人喜欢递归的，思路相当清晰, 不过非递归的也不错，类似二分查找的写法.


void swap(int &l, int &r);
int partition(int *p, int l, int r);
int findk(int* p, int l,  int r, int k);
int findk(int* p, int n, int k);
int findknr(int* p, int n, int k);

const int N = 10100;
int a[N];

int main(int argc, const char *argv[])
{
    int n, i, k;

    srand(time(NULL));

    scanf("%d", &n);

    i = 0;
    while(i < n) {
        scanf("%d", a + i);
        ++i;
    }
    k = n / 2;

    printf("%d\n", findknr(a, n, k));
    return 0;
}


int findk(int* p, int n, int k)
{
    int rnum = rand() % n;
    // printf("%d\n", rnum);
    swap(p[rnum], p[n-1]);

    int m = partition(p, 0, n);

    if (m == k)
        return p[k];

    if (m > k)
        return findk(p, m, k);

    return findk(p + m + 1, n - m -1 , k - m - 1);
}


int findknr(int* p, int n, int k)
{
    int rnum = rand() % n;
    // printf("%d\n", rnum);
    int l = 0;
    int r = n;
    int m = partition(p, l, r);

    while (m != k) {

        if (m > k) {
            r = m;
        } else {
            l = m + 1;
        }
        rnum = rand() % (r -l);
        swap(p[l+rnum], p [r-1]);
        m = partition(p, l, r);
    }

    return p[m];
}

/**
 * 读数组进行划分
 * [l, r)
 *
 * return m
 * 满足 [l, m + 1) <= x, [m+1, r) > x
 *
 * 算法导论上的代码, 思路相当清晰
 * 当循环开始的时候, 满足循环不变式
 * [l, i] <= x
 * [i + 1, j) > x
 */
int partition(int *p, int l, int r)
{
    int x = p[r-1];
    int i = l - 1;
    for (int j = l; j < r - 1; ++j) {
        if (p[j] < x) {
            ++i;
            swap(p[i], p[j]);
        }
    }
    swap(p[i+1], p[r-1]);
    return i + 1;
}

void swap(int &l, int &r)
{
    int tmp = l;
    l = r;
    r = tmp;
}
