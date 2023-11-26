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
#include <chrono>   //���ڲ��Գ�������ʱ��
#include <ctime>    //���ڲ��Գ�������ʱ��
#include <random>
#include <fstream>
#include <string>
using namespace std;

//����ÿ�ε����ݣ���Ҫ������һЩ����:
int TestNum = 0;    //���Դ���
string timesFile = "C:\\WorkStation\\D��\\05_�о�������\\�ҵ�ѧϰ�ĵ�\\015_EDA���������ϰ\\Times.txt";  //��¼���Դ������ļ�
string saveMatrixFile = "C:\\WorkStation\\D��\\05_�о�������\\�ҵ�ѧϰ�ĵ�\\015_EDA���������ϰ\\"+to_string(TestNum)+"_matrix.txt";//��¼���ɵľ���
string MatrixMidRes1 = ".\\"+to_string(TestNum)+"_MatrixMidres1.txt";  //�м�������
string MatrixMidRes2 = ".\\"+to_string(TestNum)+"_MatrixMidres2.txt";  //�м�������
bool flag = true;   //���ڿ���ʹ���ĸ��м�������·�� - true:1,false:2

#define MaxSize 1000;
#define MAX 1000;
typedef int ElementType;

//����ṹ
typedef struct direc{
    int x;
    int y;
    struct direc*next;
}DirecNode;

vector<vector<int>>* testVectorCreation(int row,int col,int obsLen,int obsNum,string saveIniFile){
    ofstream IniFile(saveIniFile);
    //row:����,col:����,obsLen:�ϰ���󳤶�,obsNum:�ϰ�����
    //1.�������������
    //����Ϊ�����
    srand(time(nullptr));
    //�������������������
    random_device rd;
    mt19937 gen(rd());  //mt19937��һ��Mersenne Twister����
    //����������ķ�Χ
    int lower_bound = 0;
    int upper_bound = max(row,col)-1; //��������ָ���ŵ���һ��/��
    //����һ���ֲ�����ָ����Χ
    uniform_int_distribution<int> dis(lower_bound,upper_bound);
    //����ֻ��Ҫʹ��random_num = dis(gen)����������lower_bound��upper_bound��Χ�ڵ������
    //2.��ʼ�����������
    vector<vector<int>>*arr = new vector<vector<int>>(row,vector<int>(col,0));
    //��������谭
    for(int i = 0;i < obsNum;i++){  //����obsNum���ϰ�
        int x = (dis(gen)+rand()+rand()%3)%row;   //1.����Ҫ���õ�x,y����
        int y = (dis(gen)+rand()-rand()%7)%col;
        // cout<<"genX,Y = "<<x<<","<<y<<endl;
        IniFile<<"genX,Y = "<<x<<","<<y<<endl;
        if(x>=row || y >= col){ //�±곬��
            i--;
            continue;
        }
        //2.����Ҫ���Ż������ŷ�
        int direction = (dis(gen)+rand()*rand())%2;  //0���ŷţ�1���ŷ�
        // cout<<"Direction = "<<direction<<endl;
        IniFile<<"Direction = "<<direction<<endl;
        //3.ѡ���ϰ�����
        int len = (rand()%obsLen+dis(gen)+rand()*rand())%obsLen; //ѡ�����Ҫ����ϰ�����
        while(!len)len = (rand()%6+dis(gen)+rand()*rand())%obsLen;  //������0����
        // cout<<"OBSLEN = "<<len<<endl;
        IniFile<<"OBSLEN = "<<len<<endl;
        //4.�����ϰ�
        if(direction){  //����
            for(int j = x;j<x+len && j < row;j++)
                (*arr)[j][y] = -1;
        }else{          //����
            for(int j = y;j<y+len && j < col;j++)
                (*arr)[x][j] = -1;
        }
    }
    IniFile.close();
    return arr;
}

//1.��ͳA-star�㷨
/*
    �㷨����:
    1.������arr��·������arr[i][j]��ʾһ��·����;
    2.·����Ĺ涨����:
        1.arr[i][j]==0 ,��ʾͨ·�����ϰ���
        2.arr[i][j]==-1,��ʾ�ϰ����޷���(i,j)��ͨ��
        3.arr[i][j]>0ʱ,��ʾ��Դ�㵽Ŀ���������پ���
*/
bool infection(vector<vector<int>>&dis,int row,int col,int sx,int sy,int ex,int ey,int x,int y,int choose){
    //1.�ж��Ƿ�Խ����Ƿ�Ϊ�ϰ����Ƿ��Ѿ�����������پ���
    if(x < 0 || x >= row || y < 0 || y >= col || dis[x][y] == -1 || dis[x][y] > 0) return false;
    //2.�ж��Ƿ񵽴�Ŀ��㣬�������Ŀ��㣬����true��������������پ��벢������Ⱦ
    if(x == ex && y == ey){ //����Ŀ���
        dis[x][y] = INT_MAX;
        return true;
    };
    dis[x][y] = abs(sx - x) + abs(sy - y);  //�����پ���
    if(x == sx && y == sy)dis[x][y] = INT_MIN;

    /*
    //��ͳA*�������������پ����Ӱ�죬���ƹ��ѣ�����ʱ��������:
    //���Ի���:i5-13400F,4060ti
    //����ʱ�䣺6��������
    */
    if(!choose){
        //3.������Ⱦ - ֻ��Ⱦ���������ĸ����� - ��ͳ��A*,���ῼ�������پ����Ӱ��
        if(infection(dis,row,col,sx,sy,ex,ey,x - 1,y,choose))return true; //��
        if(infection(dis,row,col,sx,sy,ex,ey,x + 1,y,choose))return true; //��
        if(infection(dis,row,col,sx,sy,ex,ey,x,y - 1,choose))return true; //��
        if(infection(dis,row,col,sx,sy,ex,ey,x,y + 1,choose))return true; //��
    }else{
    //��ɢʱ���������پ���ķ��� - ����������̫С������ʱ��Ϊ0
    //1.����:
    if(abs(ey-y)>abs(ey-y+1))  //����������С�ż�������
        if(infection(dis,row,col,sx,sy,ex,ey,x,y - 1,choose))return true; //��
    //2.����:
    if(abs(ey-y)>abs(ey-y-1))  //�����Ҿ����С�ż�������
        if(infection(dis,row,col,sx,sy,ex,ey,x,y + 1,choose))return true; //��
    //3.����:
    if(abs(ex-x)>abs(ex-x+1))  //�����Ͼ����С�ż�������
        if(infection(dis,row,col,sx,sy,ex,ey,x - 1,y,choose))return true; //��
    //4.����:
    if(abs(ex-x)>abs(ex-x-1))  //�����¾����С�ż�������
        if(infection(dis,row,col,sx,sy,ex,ey,x + 1,y,choose))return true; //��
    }
    return false;   //��ʾ��δ�ߵ��յ�
}
void FeedBack(vector<vector<int>>&dis,int row,int col,int sx,int sy,int ex,int ey,DirecNode*head,int&len){
    //�����㷨������·��
    int x = ex,y = ey;  //���յ㿪ʼ
    while(x>=0 && y>=0 && x<row && y<col){
        if(x == sx && y == sy)break;
        //Ϊ��ֹ·�������ظ������,���ҵ���С·���󣬻Ὣ��ǰ�����پ����Ϊ-2����ʾ�Ѿ������߹�
        int min = INT_MAX,direction = -1;//min��ʾ��С�����پ��룬direction��ʾ��С�����پ���ķ���0123-��������
        if(y-1>=0 && dis[x][y-1]!=-1 && dis[x][y-1]!=0 && dis[x][y-1]!=-2){    //��
            if(x == sx && y-1 == sy){   //��һ�����ߵ�Դ��ֱ���˳�
                len++;
                break;
            }
            if(dis[x][y-1]<min){
                min = dis[x][y-1];
                dis[x][y] = -2;
                direction = 0;
            }
        }
        if(x-1>=0 && dis[x-1][y]!=-1 && dis[x-1][y]!=0 && dis[x-1][y]!=-2){    //��
            if(x-1 == sx && y == sy){   //��һ�����ߵ�Դ��ֱ���˳�
                len++;
                break;
            }if(dis[x-1][y]<min){
                min = dis[x-1][y];
                dis[x][y] = -2;
                direction = 1;
            }
        }
        if(y+1<col && dis[x][y+1]!=-1 && dis[x][y+1]!=0 && dis[x][y+1]!=-2){  //��
            if(x == sx && y+1 == sy){   //��һ�����ߵ�Դ��ֱ���˳�
                len++;
                break;
            }if(dis[x][y+1]<min){
                min = dis[x][y+1];
                dis[x][y] = -2;
                direction = 2;
            }
        }
        if(x+1<row && dis[x+1][y]!=-1 && dis[x+1][y]!=0 && dis[x+1][y]!=-2){  //��
            if(x+1 == sx && y == sy){   //��һ�����ߵ�Դ��ֱ���˳�
                len++;
                break;
            }if(dis[x+1][y]<min){
                min = dis[x+1][y];
                dis[x][y] = -2;
                direction = 3;
            }
        }
        DirecNode*cur = new DirecNode;
        switch(direction){  //�ı���һ��������
            case 0: //������
                cur->x = x;
                cur->y = y--;
                cur->next = head->next;
                head->next = cur;
                break;
            case 1: //������
                cur->x = x--;
                cur->y = y;
                cur->next = head->next;
                head->next = cur;
                break;
            case 2: //������
                cur->x = x;
                cur->y = y++;
                cur->next = head->next;
                head->next = cur;
                break;
            case 3: //������
                cur->x = x++;
                cur->y = y;
                cur->next = head->next;
                head->next = cur;
                break;
            default:
                cout<<"��·����!\n";
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
    //row:����,col:����,sx:���x����,sy:���y����,ex:�յ�x����,ey:�յ�y����,�����������s��,len-·������
    //���ڲ��õݹ飬��δ���κ��Ż���������ֻ֧��155*155�ľ���
    //1.��ʼ��·������
    vector<vector<int>>dis(row,vector<int>(col,0));
    for(int i = 0;i < row;i++)
        for(int j = 0;j < col;j++)
            dis[i][j] = arr[i][j];
    //2.ʹ�ø�������"��Ⱦ"·��
    if(infection(dis,row,col,sx,sy,ex,ey,sx,sy,choose)){//����sx,sy��ʾ��ǰ�ߵ���λ��,����true��ʾ�ߵ�Ŀ���
        //��¼�м�������
        if(flag){   //���м�������1д������
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
        //��Ŀ��㿪ʼ���ݾ��������·��
        cout<<"��ʼ����\n";
        FeedBack(dis,row,col,sx,sy,ex,ey,head,len);
        return true;
    }else return false; //��ʾû���ߵ�Ŀ���
}
