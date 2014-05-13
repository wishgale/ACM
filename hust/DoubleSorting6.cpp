#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<iostream>
#include<string>
#include<queue>
#include<map>
#include<set>
#include<cmath>
#include<stack>
#include<algorithm>
#include<functional>
using namespace std;
typedef long long LL;

struct T {
    LL s;
    int step;
    int left;
    int dis;
    int num;
    T() {}
    T(LL& s, int step, int num, int dis, int left):s(s),step(step),num(num),dis(dis),left(left) {
    }
    bool operator<(const T &t)const {
         return this->step + this->left > t.step + t.left;// < : 大堆;< : 小堆
    }

} tt;

set<LL>mhead;
priority_queue<T>qhead;
LL shead, stail;
int str[2][8];
int n;
int ans;
bool ansFind;
int nowMaxNum;
int nowStep;
int nowMinNum;

int getNum() {
    int ret = 0;
    int i,j,top,bottom;
    for(i=0; i<n; i++) {
        for(j=i+1; j<n; j++) {
            for(top=0; top<2; top++) {
                for(bottom=0; bottom<2; bottom++) {
                	ret += str[top][i] > str[bottom][j];
                }
            }
        }
    }
    return ret;
}

int changeNum(int num, int i, int j, int a, int b) {
    int ret = num;

    for(int top=0; top<2; top++) {
        for(int bottom=0; bottom<2; bottom++) {
            if(str[top][i] > str[bottom][j]) {
                ret++;
            }
        }
    }
    swap(str[a][i],str[b][j]);
    for(int top=0; top<2; top++) {
        for(int bottom=0; bottom<2; bottom++) {
            if(str[top][i] > str[bottom][j]) {
                ret--;
            }
        }
    }
    swap(str[a][i],str[b][j]);
    return ret;
}


int getDis(){
    int ret =0;
    int i,j;
    for(i=0; i<n; i++) {
        for(j=0; j<2; j++) {
            ret +=  abs(str[j][i] - i);
        }
    }
    return ret;
}

int changeDis(int Dis, int i, int j, int a, int b) {
    int ret = Dis;
	ret += (abs(str[a][i] - i) + abs(str[b][j] - j));
	ret -= (abs(str[a][i] - j) + abs(str[b][j] - i));
    return ret;
}

void toArray(LL& s) {
    for(int k=0; k<2; k++) {
        for(int i=0; i<n; i++) {
            str[k][i] = (s >> (i*6 + k*3)) & 7;
        }
    }
}

void toString(LL &s) {
    s  = 0;
    bool ok = false;
    for(int k=0; k<2; k++) {
        for(int i=0; i<n; i++) {
            if(str[0][i] > str[1][i]) {
                swap(str[0][i], str[1][i]);
                ok = true;
            }
            s |= (LL)(str[k][i]) << (i*6 + k*3);
            if(ok) {
                swap(str[0][i], str[1][i]);
                ok = false;
            }
        }
    }
}
void output() {
    for(int i=0; i<n; i++) {
        for(int j=0; j<2; j++) {
            printf("%d",str[j][i]);
        }
    }
    puts("");
    puts("__");
}
void init() {
    mhead.clear();
    while(!qhead.empty())qhead.pop();
    toString(shead);
    stail = 0;
    int num = getNum();
    int dis = getDis();
    qhead.push(T(shead, 0, num, dis, max(dis/2 + 1, num/3 + 1)));

    for(int k=0; k<2; k++) {
        for(int i=0; i<n; i++) {
            stail |= (LL)(i) << (i*6 + k*3);
        }
    }

    ans  = 0;
    if(stail == shead) {
        ansFind = true;
    } else {
        ansFind = false;
    }
}

void bfs() {
    T t;
    int i,j,k;
    while(!ansFind) {
        t = tt = qhead.top();
        qhead.pop();
        //printf("size=%d\n",qhead.size());
        if(tt.s == stail) {
            ansFind = true;
            ans = tt.step;
            return ;
        }

        if (mhead.count(tt.s)) {
            continue;
        }
        tt.step++;
        mhead.insert(tt.s);
        toArray(t.s);

        for(i=0; i<n-1; i++) {
			if(str[0][i] + 1== str[0][i+1] && str[0][i]== str[1][i] && str[0][i+1] == str[1][i+1]){
				continue;
			}

            for(j=0; j<2; j++) {

                if(str[1][i] == str[0][i] && j == 1)continue;
                for(k=0; k<2; k++) {
                    if(str[1][i+1] == str[0][i+1] && k == 1)continue;

                    if(str[j][i] <= str[k][i+1])continue;

                    swap(str[j][i],str[k][i+1]);

                    toString(tt.s);
                    tt.dis = changeDis(t.dis, i, i+1, j, k);
                    tt.num = changeNum(t.num, i, i+1, j, k);
                    if(tt.dis >= t.dis || tt.num >= t.num){
						continue;
                    }

                    tt.left = max((tt.num + 1 )/3 + 1, tt.dis/2 + 1);
                    qhead.push(tt);
                    swap(str[j][i],str[k][i+1]);
                }
            }
        }
    }

}

int main(int argc, char* argv[]) {
    while(~scanf("%d",&n),n) {
        for(int i=0; i<n; i++) {
            scanf("%d%d",&str[0][i],&str[1][i]);
            str[0][i]--;
            str[1][i]--;
            if(str[0][i]>str[1][i])swap(str[0][i],str[1][i]);
        }
        init();
        if(!ansFind) {
            bfs();
        }
        printf("%d\n",ans);
        break;
    }

    return 0;
}

/*
8
8 8
7 7
6 6
5 5
4 4
3 3
2 2
1 1

7
7 7
6 6
5 5
4 4
3 3
2 2
1 1

6
6 6
5 5
4 4
3 3
2 2
1 1

5
5 5
4 4
3 3
2 2
1 1

4
4 4
3 3
2 2
1 1

5
1 5
3 4
2 5
2 3
1 4

8
8 3
4 2
6 4
3 5
5 8
7 1
2 6
1 7


0:1534252314
1:1523452314
2:1523243514
3:1523241345
4:1523123445
5:1512233445
6:1125233445
7:1122353445
8:1122334545
9:1122334455

0:1534252314
1:1524352314
2:1245352314
3:1245233514
4:1245231345
5:1224351345
6:1224133545
7:1224133455
8:1212343455
9:1212334455
10:1122334455


*/
