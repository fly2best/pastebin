#include <stdio.h>
#include <iostream>
#include <algorithm>
using namespace std;

//
// 有A和B两路公交车, 平均发车间隔时间分别是5min和10min.
// 某乘客在站点S可以任意选择两者之一乘坐, 假设A和B到达S的时刻无法确定.
// 那么该乘客的平均等待时间约为:
// A. 1min20s  B. 3min20s  C.2min30s  D. 1min40s

// 5分钟一趟
// 10分钟一趟
// 假设单位是一秒
// 等5分钟一趟的车, 有300种情况
// 等10分钟一趟的车, 有600种情况
//
// 认为两者相互独立
// (x, y) 表示两辆车到达的时间, 单位为秒
// 则显然等待时间为z=min(x, y)
// 统计每个z出现的次数, 则可求出, p(Z=i)的概率 = count[i] / (300*600)
//
// 求平均等待时间, 就是求期望
// E(Z) = sum(z*p[z])
//      = sum(i*count[i]) / (300*600)
//      = 125.5
// 也就是说, 平均等待时间为2min5.5s
// 
// 题目种的答案只有: 1m20s, 1m40s, 2m30s, 3m20s
// 这个题目没有正确答案.
//
// 2013年 12月 22日 星期日 16:22:25 CST
//
// 连续的思路
//
// FZ(z)表示, Z < z的概率
// FX(x)表示, X < x的概率
// FX(x)表示, Y < y的概率
//
// FZ(z)表示的意思是在时间z内等到车的概率
// FZ(z) = p(Z < z)  // z的(0, 5]
//       = 1 - p(Z >= z)
//       = 1 - p(X >= z)p(Y >= z)  // Z = min(X, Y)
//       = 1 - (1- FX(z))(1- FY(z))
//       = 1 - (1 - z/5)(1- z/10)  
// 读FZ(z) 求积分,可得
// fZ(z) = (15- 2z)/50
// eZ = 定积分(z*fZ(z)) // z 的范围是0到5
// 计算的结果是, 25/12 min
// 2min5s
//
// 2013年 12月 23日 星期一 08:37:52 CST

int main(int argc, const char *argv[])
{
    int count[5*60+1] = {0};

    for(int i = 1; i <= 300; ++i) {
        for(int j = 1; j <= 600; ++j) {
            int z = min(i, j);
            count[z]++;
        }
    }

    double ez = 0;
    for(int i = 1; i <= 300; ++i) {
        ez += i * count[i];
    }

    ez /= 300;
    ez /= 600;

    cout << ez << endl;

    return 0;
}
/**
 * output: 125.5
 */
