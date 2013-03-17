
//algorithms 6.6

// operation matrix
//
//   a b c
// a b b a
// b c b a
// c a c c
//
//
// input: a string of abc
// output: whether or not to parenthesize the string in a way that retrun a
//

#include <iostream>
#include <cstring>
using namespace std;

int const MAXN = 100;

//m[i,j,k]  stands s[i..j] can get k
//k = 0 ... a
//k = 1 ... b
//k = 2 ..  c
int m[MAXN][MAXN][3] = {0};

int p[MAXN][MAXN][3];
int l[MAXN][MAXN][3];
int r[MAXN][MAXN][3];

int opm[][3] = { {1,1,0} , {2,1,0}, {0,2,2}} ;

char *s = "bbbbbac";

bool check(int i,int j,int k, int c);
void print(int i,int j ,int k);
void printm();
int main(int argc, const char *argv[])
{

    int n = strlen(s);

    for(int i = 0; i < n; ++i)
    {
        m[i][i][ s[i] - 'a'] = 1;
    }

    for(int step = 2; step <= n; ++step) {

        for(int i = 0; i + step - 1 < n; ++i ) {

            //shoude compute m[i][i+step-1]
            int j = i + step -1;

            for(int k = i ; k < j; ++k) {

                // cout << i << " " << k << " " << k+1 << " " << j << " " << step << endl;

                for(int c = 0; c < 3; ++c)
                {
                    if( !m[i][j][c] )  {
                        check(i,j,k,c);
                    }
                }
            }
        }

    }

    cout << m[0][n-1][0] << endl;

    if( m[0][n-1][0] ) {

        print(0,n-1,0);
        cout << endl;
    }

    return 0;
}



//check s[i..j] 是否可以等于c
//k是分割的位置
bool check(int i,int j,int k, int c)
{
    for(int x = 0; x < 3; ++x)
    {
        for(int y = 0; y < 3; ++y)
        {
            if( (opm[x][y] == c) && m[i][k][x] && m[k+1][j][y])
            {

                m[i][j][c] = 1;

                p[i][j][c] = k;
                l[i][j][c] = x;
                r[i][j][c] = y;

                return true;
            }
        }
    }

    return false;
}

//print m[i][j] = k 的加括号情况
void print(int i,int j ,int k)
{
    if(i == j)
    {
        cout << s[i];
        return;
    }
    cout << "(";
    print(i,p[i][j][k],l[i][j][k]);
    print(p[i][j][k] +1, j,r[i][j][k]);
    cout << ")";
}

void printm()
{
    int n = strlen(s);
    cout << "a" << endl;
    for(int i = 0; i < n; ++i ) {

        for(int j = 0; j < n; ++j) {

            cout << m[i][j][0] << " ";
        }
        cout << endl;
    }

    cout << "b" << endl;
    for(int i = 0; i < n; ++i ) {

        for(int j = 0; j < n; ++j) {

            cout << m[i][j][1] << " ";
        }
        cout << endl;
    }

    cout << "c" << endl;
    for(int i = 0; i < n; ++i ) {

        for(int j = 0; j < n; ++j) {

            cout << m[i][j][2] << " ";
        }
        cout << endl;
    }
}
