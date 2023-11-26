#pragma comment(linker,"/STACK:1073741824")
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <cmath>
#include <vector>
#include <queue>
#include <map>
#include <stack>
#include <list>
#include <limits>
#include <chrono>   //用于测试程序运行时间
#include <ctime>    //用于测试程序运行时间
#include <random>
#include <fstream>
#include <string>
using namespace std;

//保存每次的内容，需要做以下一些设置:
int TestNum = 0;    //测试次数
string timesFile = "C:\\WorkStation\\D盘\\05_研究生资料\\我的学习文档\\015_EDA方向代码练习\\Times.txt";  //记录测试次数的文件
string saveMatrixFile = "C:\\WorkStation\\D盘\\05_研究生资料\\我的学习文档\\015_EDA方向代码练习\\"+to_string(TestNum)+"_matrix.txt";//记录生成的矩阵
string MatrixMidRes1 = ".\\"+to_string(TestNum)+"_MatrixMidres1.txt";  //中间结果矩阵
string MatrixMidRes2 = ".\\"+to_string(TestNum)+"_MatrixMidres2.txt";  //中间结果矩阵
bool flag = true;   //用于控制使用哪个中间结果矩阵路径 - true:1,false:2

#define MaxSize 1000;
#define MAX 1000;
typedef int ElementType;

//坐标结构
typedef struct direc{
    int x;
    int y;
    struct direc*next;
}DirecNode;

vector<vector<int>>* testVectorCreation(int row,int col,int obsLen,int obsNum,string saveIniFile){
    ofstream IniFile(saveIniFile);
    //row:行数,col:列数,obsLen:障碍最大长度,obsNum:障碍个数
    //1.构造随机数环境
    //创建为随机数
    srand(time(nullptr));
    //创建真正的随机数引擎
    random_device rd;
    mt19937 gen(rd());  //mt19937是一个Mersenne Twister引擎
    //定义随机数的范围
    int lower_bound = 0;
    int upper_bound = max(row,col)-1; //可以用于指定放到哪一行/列
    //创建一个分布对象，指定范围
    uniform_int_distribution<int> dis(lower_bound,upper_bound);
    //后续只需要使用random_num = dis(gen)即可生成在lower_bound和upper_bound范围内的随机数
    //2.开始构造随机矩阵
    vector<vector<int>>*arr = new vector<vector<int>>(row,vector<int>(col,0));
    //随机构造阻碍
    for(int i = 0;i < obsNum;i++){  //放置obsNum个障碍
        int x = (dis(gen)+rand()+rand()%3)%row;   //1.生成要放置的x,y坐标
        int y = (dis(gen)+rand()-rand()%7)%col;
        // cout<<"genX,Y = "<<x<<","<<y<<endl;
        IniFile<<"genX,Y = "<<x<<","<<y<<endl;
        if(x>=row || y >= col){ //下标超限
            i--;
            continue;
        }
        //2.生成要横着还是竖着放
        int direction = (dis(gen)+rand()*rand())%2;  //0横着放，1竖着放
        // cout<<"Direction = "<<direction<<endl;
        IniFile<<"Direction = "<<direction<<endl;
        //3.选择障碍长度
        int len = (rand()%obsLen+dis(gen)+rand()*rand())%obsLen; //选择符合要求的障碍长度
        while(!len)len = (rand()%6+dis(gen)+rand()*rand())%obsLen;  //不能是0长度
        // cout<<"OBSLEN = "<<len<<endl;
        IniFile<<"OBSLEN = "<<len<<endl;
        //4.放置障碍
        if(direction){  //竖着
            for(int j = x;j<x+len && j < row;j++)
                (*arr)[j][y] = -1;
        }else{          //横着
            for(int j = y;j<y+len && j < col;j++)
                (*arr)[x][j] = -1;
        }
    }
    IniFile.close();
    return arr;
}

//1.传统A-star算法
/*
    算法描述:
    1.给定的arr是路径矩阵，arr[i][j]表示一个路径点;
    2.路径点的规定如下:
        1.arr[i][j]==0 ,表示通路，无障碍物
        2.arr[i][j]==-1,表示障碍，无法从(i,j)点通过
        3.arr[i][j]>0时,表示从源点到目标点的曼哈顿距离
*/
bool infection(vector<vector<int>>&dis,int row,int col,int sx,int sy,int ex,int ey,int x,int y,int choose){
    //1.判断是否越界或是否为障碍或是否已经计算过曼哈顿距离
    if(x < 0 || x >= row || y < 0 || y >= col || dis[x][y] == -1 || dis[x][y] > 0) return false;
    //2.判断是否到达目标点，如果到达目标点，返回true，否则计算曼哈顿距离并继续感染
    if(x == ex && y == ey){ //到达目标点
        dis[x][y] = INT_MAX;
        return true;
    };
    dis[x][y] = abs(sx - x) + abs(sy - y);  //曼哈顿距离
    if(x == sx && y == sy)dis[x][y] = INT_MIN;

    /*
    //传统A*，不考虑曼哈顿距离的影响，类似广搜，运行时间结果如下:
    //测试环境:i5-13400F,4060ti
    //运行时间：6毫秒左右
    */
    if(!choose){
        //3.继续感染 - 只感染上下左右四个方向 - 传统的A*,不会考虑曼哈顿距离的影响
        if(infection(dis,row,col,sx,sy,ex,ey,x - 1,y,choose))return true; //上
        if(infection(dis,row,col,sx,sy,ex,ey,x + 1,y,choose))return true; //下
        if(infection(dis,row,col,sx,sy,ex,ey,x,y - 1,choose))return true; //左
        if(infection(dis,row,col,sx,sy,ex,ey,x,y + 1,choose))return true; //右
    }else{
    //扩散时考虑曼哈顿距离的方法 - 由于数据量太小，测试时间为0
    //1.往左:
    if(abs(ey-y)>abs(ey-y+1))  //若往左距离变小才继续往左
        if(infection(dis,row,col,sx,sy,ex,ey,x,y - 1,choose))return true; //左
    //2.往右:
    if(abs(ey-y)>abs(ey-y-1))  //若往右距离变小才继续往右
        if(infection(dis,row,col,sx,sy,ex,ey,x,y + 1,choose))return true; //右
    //3.往上:
    if(abs(ex-x)>abs(ex-x+1))  //若往上距离变小才继续往上
        if(infection(dis,row,col,sx,sy,ex,ey,x - 1,y,choose))return true; //上
    //4.往下:
    if(abs(ex-x)>abs(ex-x-1))  //若往下距离变小才继续往下
        if(infection(dis,row,col,sx,sy,ex,ey,x + 1,y,choose))return true; //下
    }
    return false;   //表示还未走到终点
}
void FeedBack(vector<vector<int>>&dis,int row,int col,int sx,int sy,int ex,int ey,DirecNode*head,int&len){
    //回溯算法，回溯路径
    int x = ex,y = ey;  //从终点开始
    while(x>=0 && y>=0 && x<row && y<col){
        if(x == sx && y == sy)break;
        //为防止路径出现重复的情况,在找到最小路径后，会将当前曼哈顿距离改为-2，表示已经计算走过
        int min = INT_MAX,direction = -1;//min表示最小曼哈顿距离，direction表示最小曼哈顿距离的方向，0123-左上右下
        if(y-1>=0 && dis[x][y-1]!=-1 && dis[x][y-1]!=0 && dis[x][y-1]!=-2){    //左
            if(x == sx && y-1 == sy){   //下一步能走到源点直接退出
                len++;
                break;
            }
            if(dis[x][y-1]<min){
                min = dis[x][y-1];
                dis[x][y] = -2;
                direction = 0;
            }
        }
        if(x-1>=0 && dis[x-1][y]!=-1 && dis[x-1][y]!=0 && dis[x-1][y]!=-2){    //上
            if(x-1 == sx && y == sy){   //下一步能走到源点直接退出
                len++;
                break;
            }if(dis[x-1][y]<min){
                min = dis[x-1][y];
                dis[x][y] = -2;
                direction = 1;
            }
        }
        if(y+1<col && dis[x][y+1]!=-1 && dis[x][y+1]!=0 && dis[x][y+1]!=-2){  //右
            if(x == sx && y+1 == sy){   //下一步能走到源点直接退出
                len++;
                break;
            }if(dis[x][y+1]<min){
                min = dis[x][y+1];
                dis[x][y] = -2;
                direction = 2;
            }
        }
        if(x+1<row && dis[x+1][y]!=-1 && dis[x+1][y]!=0 && dis[x+1][y]!=-2){  //下
            if(x+1 == sx && y == sy){   //下一步能走到源点直接退出
                len++;
                break;
            }if(dis[x+1][y]<min){
                min = dis[x+1][y];
                dis[x][y] = -2;
                direction = 3;
            }
        }
        DirecNode*cur = new DirecNode;
        switch(direction){  //改变下一步的走向
            case 0: //往左走
                cur->x = x;
                cur->y = y--;
                cur->next = head->next;
                head->next = cur;
                break;
            case 1: //往上走
                cur->x = x--;
                cur->y = y;
                cur->next = head->next;
                head->next = cur;
                break;
            case 2: //往右走
                cur->x = x;
                cur->y = y++;
                cur->next = head->next;
                head->next = cur;
                break;
            case 3: //往下走
                cur->x = x++;
                cur->y = y;
                cur->next = head->next;
                head->next = cur;
                break;
            default:
                cout<<"无路可走!\n";
                return ;

        }
        len++;
        if(x == sx && y == sy)break;
        // cout<<"("<<x<<","<<y<<")"<<endl;
        // cout<<len<<endl;
    }
    DirecNode*cur = new DirecNode;
    cur->x = sx;
    cur->y = sy;
    cur->next = head->next;
    head->next = cur;
}
bool Astar(const vector<vector<int>>&arr,int row,int col,int sx,int sy,int ex,int ey,DirecNode*head,int &len,int choose){
    //row:行数,col:列数,sx:起点x坐标,sy:起点y坐标,ex:终点x坐标,ey:终点y坐标,结果放入数组s中,len-路径长度
    //由于采用递归，且未做任何优化，因此最大只支持155*155的矩阵
    //1.初始化路径矩阵
    vector<vector<int>>dis(row,vector<int>(col,0));
    for(int i = 0;i < row;i++)
        for(int j = 0;j < col;j++)
            dis[i][j] = arr[i][j];
    //2.使用辅助遍历"感染"路径
    if(infection(dis,row,col,sx,sy,ex,ey,sx,sy,choose)){//最后的sx,sy表示当前走到的位置,返回true表示走到目标点
        //记录中间结果矩阵
        if(flag){   //向中间结果矩阵1写入数据
            flag = false;
            ofstream MidRes (MatrixMidRes1);
            for(int i = 0;i < row;i++){
                for(int j = 0;j < col;j++){
                    MidRes<<dis[i][j];
                    if(dis[i][j]<0) MidRes<<" ";
                    else MidRes<<"  ";
                }
                MidRes<<endl;
            }
        }else{
            flag = true;
            ofstream MidRes (MatrixMidRes2);
            for(int i = 0;i < row;i++){
                for(int j = 0;j < col;j++){
                    MidRes<<dis[i][j];
                    if(dis[i][j]<0) MidRes<<" ";
                    else MidRes<<"  ";
                }
                MidRes<<endl;
            }
        }
        //从目标点开始回溯距离最近的路径
        cout<<"开始回溯\n";
        FeedBack(dis,row,col,sx,sy,ex,ey,head,len);
        return true;
    }else return false; //表示没有走到目标点
}
