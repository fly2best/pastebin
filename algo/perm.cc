/**
 * 总结了几种排列, 组合的生成方法
 */

#include <stdio.h>
#include <algorithm>
using std::swap;


void test_p_rb();
void test_print_perm_with_repeat();
void test_print_p();
void test_print_c();

int main(int argc, const char *argv[])
{
    // test_p_rb();
    // test_print_perm_with_repeat();
    // test_print_p();
    test_print_c();
    return 0;
}

void print_p_rb(int *p, int n, int k);
void test_p_rb()
{
    const int n = 5;
    int *p = new int[n];
    for(int i =0; i < n; ++i) {
        p[i] = i;
    }
    print_p_rb(p, n, 0);
}

/**
 * 递归+回溯的方法搞定, 
 * 思路非常清晰
 */
void print_p_rb(int *p, int n, int k)
{
    if (k == n) {
        for(int i = 0;i < n; ++i) {
            printf("%d ", p[i]);
        }
        printf("\n");
        return;
    }

    for(int i = k; i < n; ++i) {
        swap(p[i], p[k]);
        print_p_rb(p, n, k + 1);
        swap(p[i], p[k]);
    }
}


void print_perm_with_repeat(int *p, int n, int *po, int k);

void test_print_perm_with_repeat()
{
    int n = 5;
    int *p = new int[n];

    p[0] = 0;
    p[1] = 0;
    p[2] = 2;
    p[3] = 2;
    p[4] = 2;

    int *po = new int[n];

    print_perm_with_repeat(p, n, po, 0);

}

/**
 * 字典序, 可以处理p中有重复的元素
 */
void print_perm_with_repeat(int *p, int n, int *po, int k)
{

    if (k == n) {
        for(int i = 0;i < n; ++i) {
            printf("%d ", po[i]);
        }
        printf("\n");
        return;
    }

    for(int i = 0; i < n; ++i) {

        //这个已经重复了
        if ( i != 0 && p[i] == p[i-1])
            continue;

        int count = 0;

        for (int j = 0; j < n; ++j) {
            if (p[j] == p[i])
                ++count;
        }

        for (int j = 0; j < k; ++j) {
            if (po[j] == p[i]) {
                --count;
            }
        }

        if (count > 0) {
            po[k] = p[i];
            print_perm_with_repeat(p, n, po, k+1);
        }
    }
}


void print_p(int *p, int n, int k);

void test_print_p()
{
    int n = 5;
    int *p = new int[5];

    print_p(p, n, 0);
}

/**
 * 输入1, n上的全排列, 字典序
 */
void print_p(int *p, int n, int k)
{

    if (k == n) {
        for(int i = 0;i < n; ++i) {
            printf("%d ", p[i]);
        }
        printf("\n");
        return;
    }

    for(int i = 0; i < n; ++i) {

        bool isFound = false;

        for (int j = 0; j < k; ++j) {
            if (p[j] == i) {
                isFound = true;
                break;
            }
        }

        if (!isFound) {
            p[k] = i;
            print_p(p, n, k+1);
        }
    }
}

void print_c(int *p, int n, int k);

void test_print_c()
{
    int n = 5;
    int *p = new int[5];

    print_c(p, n, 0);

}

/**
 * 输入1, n的所有组合
 * 字典序
 * 和排列不同的地方是确定当前位置上数字的方法和什么时候输出.
 */
void print_c(int *p, int n, int k)
{

    if (k == n)
        return;

    for(int i = (k == 0 ? 0 : p[k-1] + 1); i < n; ++i) {

        //枚举第k个数, 然后递归呗
        p[k] = i;

        for(int j = 0; j < k + 1; ++j) {
            printf("%d ", p[j]);
        }
        printf("\n");

        print_c(p, n, k+1);
    }
}
