#include <stdio.h>
#include <string.h>

void get_next(const char *p, int *next);
int kmp(const char *t,const char *p, int *next);

int main(int argc, const char *argv[])
{
    char *t = "google";
    char *p = "gl";

    int *next = new int(strlen(p));

    printf("%d\n", kmp(t, p, next));

    return 0;
}

int kmp(const char *t,const char *p, int *next)
{
    int n = strlen(t);
    int m = strlen(p);

    int i = 0;
    int j = 0;

    get_next(p, next);

    while (i < n) {
        while (j >= 0 && p[j] != t[i])
            j = next[j];

        ++i;
        ++j;

        if ( j == m )
            return i - j;
    }

    return -1;
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

        i++; 
        j++;
        //把next[0] 设置为-1,不是没道理的
        //当next[0]=-1的时候, 可以把代码写的更精简.
        next[i] = j;
    }
}

// http://chaoswork.com/blog/2011/06/14/kmp%E7%AE%97%E6%B3%95%E5%B0%8F%E7%BB%93/
// 记得当时参考了这篇文章, 是从wangcong的博客链过去的
// 当时我对一些细节搞得不是太清楚
// 今天, 又理了理思路,
// 
// 在kmp,算法里重要一点就是next数组的求取
// next[i] = j 的含义
// str[0, j) 和 str[i - j, i) 是相等的
//
// next[1] = 0 是有意义的意思是说
// str[0, 0) == str[1, 1]
// 理解就是 null 等于 null
// 如果第一个字符不相等就从第0个字符开始比较
//
// next[0] = -1 纯粹是个标记, 为-1 是为了编程实现上的简洁
//
//
// 求next数组的过程是一个递归的过程
// next[i] = j
// 那么next[i+1] 呢
// 如果p[i] == p[j]  
// 则有 str[0, j+1) 等于 str[i-j, i+1)
// 则next[i+1]=j+1
// 如果p[i] != p[j], 要递归的走下去
// 判断p[i] == p[next[j]]
//
// 匹配的时候, 如果 j = 0, 还不相等, 那么只加i, j就不动了
//
// 时间复杂度分析
// 这个比较纠结的是
//
// 这个循环的执行次数
//      while (j >= 0 && p[j] != t[i])
//              j = next[j];
//
// 可以从这么分析, j = next[j], j是减小的
// 但是增加j的位置只有
//      ++j;
// 最多增加n次
// 所以, j = next[j], 最多执行n次
// 那个while循环的最多执行n次
// 所以kmp的时间复杂度是O(n)
