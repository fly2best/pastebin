#include <algorithm>
#include <ctime>
#include <iostream>
#include <cstring>
#include <cstdio>
#include <cassert>
using namespace std;

const int m = 2000;
const int n  = 100000;


//这个题目
//最早是在算法概论的课后习题中看到的
//当时我以为我懂了, 其实没有
//今天又好好琢磨了, 写了写代码
//如果, 你觉得代码不好写, 八成是没想清楚.
//加油
//汇聚涓涓细流
// Mon Sep  9 20:06:09 CST 2013

int *pA = new int[m];
int *pB = new int[n];
int *pC = new int[m+n];

void generate(int *p, int n);
void print(int *p, int n);

int find_k(int *p1, int m, int *p2, int n, int k);

int main(int argc, const char *argv[])
{
    time_t t; 
    srand((unsigned) time(&t));

    generate(pA, m);
    generate(pB, n);

    // print(pA, m);
    // print(pB, n);

    memcpy(pC, pA, m * sizeof(int));
    memcpy(pC + m, pB, n * sizeof(int));

    sort(pA, pA + m);
    sort(pB, pB + n);

    // print(pC, m + n);
    sort(pC, pC+ m + n);
    // print(pC, m + n);

    // int i = 14;
    for(int i = 0; i < m + n; ++i) {
        int ret = find_k(pA, m, pB, n, i);
        // assert(pC[i] == ret);
        if (pC[i] != ret)
            printf("%d %d %d\n", i , pC[i], ret);
    }

    return 0;
}


int find_k(int *p1, int m, int *p2, int n, int k)
{

    if (m == 0) {
        return p2[k];
    }

    if (n == 0) {
        return p1[k];
    }

    int mid1 = p1[m/2];
    int mid2 = p2[n/2];

    // 这个可以合并到其他的情况里去
    // 加上也是不错的
    // if (mid1 == mid2) {

        // int s = m/2 + n/2;

        // if (k == s || k == s + 1) {
            // return mid1;
        // }
        // else {

            // if (k < s) {
                // return find_k(p1, m/2, p2, n/2, k);
            // }
            // //k > s + 1
            // else {
                // return find_k(p1 + m/2 + 1, m - m/2 - 1,  p2 + n/2 + 1, n - n/2 - 1, k - (s + 2));
            // }
        // }
    // }


    if (mid2 < mid1) {
        swap(p1, p2);
        swap(m, n);
        swap(mid1, mid2);
    }


    //mid2 >= mid1

    //s1是mid2最小的可能下标
    int s1 = m/2 + n/2 + 1;
    //如果比k < s1, 那么B[n/2, n) 可以删除
    if ( k < s1) {
        return find_k(p1, m, p2, n/2, k);
    }

    //s2是mid1最大的可能下标
    //mid1, 可能的最大位置是仅次于A(m/2, m) U B[n/2, n)的位置
    //pos = m + n - (m - (m/2 + 1) + n - n/2) - 1
    //    = m/2 + n/ 2
    int s2 = m/2 + n/2;

    //如果k > s2则A[0, mid1] 可以删除
    if (k > s2) {
        return find_k(p1 + m/2 + 1, m - m/2 -1, p2, n, k - m/2 -1);
    }

    // s1 = s2 + 1, 我们的判断条件是完备的
    // 永远不会执行到这个地方
    assert(0);

    return -1;
}


void generate(int *p, int n)
{
    for(int i = 0;i < n; ++i) {
        p[i] = rand() % n;
    }
}

void print(int *p, int n)
{
    for(int i = 0;i < n; ++i) {
        printf("%d ", p[i]);
    }
    printf("\n");
}
