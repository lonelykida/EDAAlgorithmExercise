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

//1.A-star�㷨
/*
    �㷨����:
    1.������arr��·������arr[i][j]��ʾһ��·����;
    2.·����Ĺ涨����:
        1.arr[i][j]==0 ,��ʾͨ·�����ϰ���
        2.arr[i][j]==-1,��ʾ�ϰ����޷���(i,j)��ͨ��
        3.arr[i][j]>0ʱ,��ʾ��Դ�㵽Ŀ���ľ���
*/
bool infection
bool Astar(const vector<vector<int>>&arr,int row,int col,int sx,int sy,int ex,int ey,vector<int>&s){
    //row:����,col:����,sx:���x����,sy:���y����,ex:�յ�x����,ey:�յ�y����,�����������s��
    vector<vector<int>>dis(row,vector<int>(col,0));
    
    //1.ʹ�ø�������"��Ⱦ"·��


}