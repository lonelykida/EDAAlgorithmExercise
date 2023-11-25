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

using namespace std;

#define MaxSize 1000;
#define MAX 1000;
typedef int ElementType;

//1.A-star算法
/*
    算法描述:
    1.给定的arr是路径矩阵，arr[i][j]表示一个路径点;
    2.路径点的规定如下:
        1.arr[i][j]==0 ,表示通路，无障碍物
        2.arr[i][j]==-1,表示障碍，无法从(i,j)点通过
        3.arr[i][j]>0时,表示从源点到目标点的距离
*/
bool infection
bool Astar(const vector<vector<int>>&arr,int row,int col,int sx,int sy,int ex,int ey,vector<int>&s){
    //row:行数,col:列数,sx:起点x坐标,sy:起点y坐标,ex:终点x坐标,ey:终点y坐标,结果放入数组s中
    vector<vector<int>>dis(row,vector<int>(col,0));
    
    //1.使用辅助遍历"感染"路径


}