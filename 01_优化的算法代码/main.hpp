#include <iostream>
#include <thread>   //�ṩ���߳�
#include <ctime>
#include <stdlib.h>
#include <vector>
#include <stack>
#include <queue>
#include <string>
#include <map>
#include <algorithm>
#include <cmath>
#include <list>
#include <limits>
#include <random>
#include <fstream>
#include <chrono>   //ʱ���

using namespace std;
using namespace std::chrono;

/*
    ˵��:
    1.�����㷨:
        ���ϡ�A*�����˿�ʼ�����Ϻ�A*�������㷨��
    2.������
        �ֱ�������㷨д���ö��̺߳Ͳ��ö��̵߳Ĵ��룬���ֱ���бȽϡ�
    3.��Ҫ������ļ������ݣ�
        1.ini�����ļ�����¼�ڼ������ɾ���
        2.��ʼ���������ռ�����ļ�
        3.obsInfo�ļ�����������obs��x��y���ꡢobs�����Լ�obs����
        4.ͨ����ͬ�㷨���������پ����������ռ�����ļ���
        5.����ļ�������������ʱ�䡢·�����ҵ��Ļ������Ƕ��١�����·���Ļ�·����ʲô���Լ����Ľ���Ƚ�
    4.�ṹ�������������
        1.��ȡ���ǵڼ������ɽ���������ݽ����ʼ���ļ�·��
        2.�û����빹�����������ռ��С��������
            (1)������(2)������(3)obs��󳤶ȣ�(4)obs����
        Ҫ���û�����ָ�������������Ա��ֽ���ĿɱȽ���
        3.�����ʼ����
        4.�Գ�ʼ������зֿ飻
        5.���ݷֿ�����(��)ʹ�ö��̶߳Գ�ʼ����������پ�����㣻�����м���������¼����ʱ��time1
        6.�м��������ļ�
        7.�����м�������·��(��¼����ʱ��time2)������¼·��λ�ã�
            Ϊ�˷�ֹ����ʱ��ջ����˲����õݹ鷽�����������ֶ�ģ��ջ�ķ�ʽ������·����
        8.����·�������ɽ�������ļ���������ʱ��time=time1+time2;

*/



//1.�����ļ�·�����������
int MatrixNum = 0;  //���û�����һ��Ҫ���ɶ��ٸ�����
vector<vector<int>> MatrixInfo; //�û�������ÿ���������Ϣ����ʽ����:���� ���� �谭��󳤶� �谭��������������ǰ���û������Ƿ�ȫ��ʹ����ͬ����
int times = 0;  //��¼�ڼ������ɾ���ͬʱ��Ϊ�ļ�����һ����
int isSameIni = 0; //���о��������Ƿ���ͬ

string tmpName; //��ʱ�ļ�����������Ϊ�ļ�����һ����
string IniInfo = "./iniInfo.txt";   //�����ڼ������ɾ���
string InitSpace = "./resFile/initSpace_" + tmpName + "_" + to_string(times) + ".txt";   //��ʼ���������ռ����
string obsInfo = "./resFile/obsInfo_" + tmpName + "_" + to_string(times) + ".txt";   //obs��Ϣ
string midRes  = "./resFile/midRes_" + tmpName + "_" + to_string(times) + ".txt";    //�м��� - ÿ���㷨�������м����м����ļ������Ժ����Ҫ��������
string AlgoRes = "./resFile/AlgoRes_" + tmpName + "_" + to_string(times) + ".txt";   //ÿ���㷨�����ս��
string resultFile = "./resFile/res_"+to_string(times)+".txt";           //�������ɾ�������ս���ļ�

//2.��ȡ�����ļ�������times�������ļ���
void readIni(){
    //1.��iniInfo��ȡ�ڼ������ɾ�������һ�����ɾ������ʼ����������
    ifstream iniFile_i(IniInfo);
    cout<<"�����ļ���ȡ��...\n";
    if(!iniFile_i){    //��ʧ�ܣ���ʾ�ļ���δ�������ǵ�һ�ζ�ȡ�ļ������г�ʼ��
        ofstream iniFile_o(IniInfo);    //���ļ�
        if(!iniFile_o.is_open()){//��ʧ��
            cout<<"�����ļ�д��ʧ�ܣ�����ֱ����ֹ\n";
            exit(-1);
        }
        iniFile_o<<times<<endl;   //д��ڼ������ɾ��������ļ�
        iniFile_o.close();
    }else
        iniFile_i>>times;   //��ȡ�ڼ������ɾ���,�˴��������ļ����������ȷ�����㷨����tmpName���ٸ����ļ���
    cout<<"�����ļ���ȡ�ɹ�...\n";
    iniFile_i.close();
}

//3.��ʼ���û������������������Ϣ - �����������������谭��󳤶ȡ��谭���� - ��ʹ��isSameIni�������Ƿ�ʹ����ͬ�ľ������
void userInputInfo(){
    MatrixInfo.clear(); //��ʼǰ��վ������
    cout<<"��ʼ���������ռ�(����)������Ϣ...\n";
    cout<<"������Ҫһ�����ɶ��ٸ�����:";
    cin>>MatrixNum;
    cout<<"�Ƿ�Ҫʹ����ͬ�ľ������?���ǣ�������1;����������0:";
    cin>>isSameIni;
    int row,col,obsLen,obsNum;
    if(isSameIni){  //ʹ����ͬ����
        vector<int> cur;
        cout<<"�����������������������谭��󳤶ȡ��谭�������м��ÿո�ֿ�:";
        cin>>row>>col>>obsLen>>obsNum;
        cur.push_back(row); //���������������������谭��󳤶ȡ��谭����
        cur.push_back(col);
        cur.push_back(obsLen);
        cur.push_back(obsNum);
        MatrixInfo.push_back(cur);
    }else{  //�ֱ�����ÿ������Ĳ���
        for(int i = 0;i < MatrixNum; i++){
            vector<int> cur;
            cout<<"�������"<<i+1<<"��������������������谭��󳤶ȡ��谭�������м��ÿո�ֿ�:";
            cin>>row>>col>>obsLen>>obsNum;
            cur.push_back(row); //���������������������谭��󳤶ȡ��谭����
            cur.push_back(col);
            cur.push_back(obsLen);
            cur.push_back(obsNum);
            MatrixInfo.push_back(cur);
        }
    }
    cout<<"�����ռ�(����)�����������...\n";
}


