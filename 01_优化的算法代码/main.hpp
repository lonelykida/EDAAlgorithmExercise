#include <iostream>
#include <thread>   //提供多线程
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
#include <chrono>   //时间库

using namespace std;
using namespace std::chrono;

/*
    说明:
    1.包含算法:
        李氏、A*、两端开始的李氏和A*这四种算法。
    2.技术：
        分别对上述算法写出用多线程和不用多线程的代码，并分别进行比较。
    3.需要保存成文件的内容：
        1.ini配置文件：记录第几次生成矩阵
        2.初始化的搜索空间矩阵文件
        3.obsInfo文件：包含各个obs的x、y坐标、obs长度以及obs方向
        4.通过不同算法计算曼哈顿距离后的搜索空间矩阵文件；
        5.结果文件，包含：运行时间、路径若找到的话长度是多少、若有路径的话路径是什么，以及最后的结果比较
    4.结构功能重组分析：
        1.读取这是第几次生成结果，并根据结果初始化文件路径
        2.用户输入构造的随机搜索空间大小，包括：
            (1)行数，(2)列数，(3)obs最大长度，(4)obs个数
        要求用户必须指定上述参数，以保持结果的可比较性
        3.保存初始矩阵；
        4.对初始矩阵进行分块；
        5.根据分块结果，(不)使用多线程对初始块进行曼哈顿距离计算；生成中间结果，并记录运行时间time1
        6.中间结果存入文件
        7.根据中间结果回溯路径(记录运行时间time2)，并记录路径位置；
            为了防止运行时爆栈，因此不采用递归方法，而采用手动模拟栈的方式来回溯路径；
        8.保存路径并生成结果报告文件，总运行时间time=time1+time2;

*/



//1.定义文件路径及所需变量
int MatrixNum = 0;  //让用户决定一次要生成多少个矩阵
vector<vector<int>> MatrixInfo; //用户来决定每个矩阵的信息，格式如下:行数 列数 阻碍最大长度 阻碍个数，并在输入前让用户决定是否全部使用相同参数
int times = 0;  //记录第几次生成矩阵，同时作为文件名的一部分
int isSameIni = 0; //所有矩阵配置是否都相同

string tmpName; //临时文件名，用于作为文件名的一部分
string IniInfo = "./iniInfo.txt";   //包含第几次生成矩阵
string InitSpace = "./resFile/initSpace_" + tmpName + "_" + to_string(times) + ".txt";   //初始化的搜索空间矩阵
string obsInfo = "./resFile/obsInfo_" + tmpName + "_" + to_string(times) + ".txt";   //obs信息
string midRes  = "./resFile/midRes_" + tmpName + "_" + to_string(times) + ".txt";    //中间结果 - 每个算法都可能有几个中间结果文件，所以后边需要了再增加
string AlgoRes = "./resFile/AlgoRes_" + tmpName + "_" + to_string(times) + ".txt";   //每种算法的最终结果
string resultFile = "./resFile/res_"+to_string(times)+".txt";           //本次生成矩阵的最终结果文件

//2.读取配置文件，更新times变量及文件名
void readIni(){
    //1.打开iniInfo读取第几次生成矩阵，若第一次生成矩阵，则初始化其他变量
    ifstream iniFile_i(IniInfo);
    cout<<"配置文件读取中...\n";
    if(!iniFile_i){    //打开失败，表示文件还未创建，是第一次读取文件，进行初始化
        ofstream iniFile_o(IniInfo);    //打开文件
        if(!iniFile_o.is_open()){//打开失败
            cout<<"配置文件写入失败！程序直接中止\n";
            exit(-1);
        }
        iniFile_o<<times<<endl;   //写入第几次生成矩阵到配置文件
        iniFile_o.close();
    }else
        iniFile_i>>times;   //读取第几次生成矩阵,此处不更新文件名，待后边确定了算法名称tmpName后再更新文件名
    cout<<"配置文件读取成功...\n";
    iniFile_i.close();
}

//3.开始让用户输入最基本的生成信息 - 矩阵行数、列数、阻碍最大长度、阻碍个数 - 并使用isSameIni来决定是否使用相同的矩阵参数
void userInputInfo(){
    MatrixInfo.clear(); //开始前清空矩阵参数
    cout<<"开始输入搜索空间(矩阵)参数信息...\n";
    cout<<"请输入要一次生成多少个矩阵:";
    cin>>MatrixNum;
    cout<<"是否要使用相同的矩阵参数?若是，请输入1;若否，请输入0:";
    cin>>isSameIni;
    int row,col,obsLen,obsNum;
    if(isSameIni){  //使用相同参数
        vector<int> cur;
        cout<<"请输入矩阵的行数、列数、阻碍最大长度、阻碍个数，中间用空格分开:";
        cin>>row>>col>>obsLen>>obsNum;
        cur.push_back(row); //存入矩阵的行数、列数、阻碍最大长度、阻碍个数
        cur.push_back(col);
        cur.push_back(obsLen);
        cur.push_back(obsNum);
        MatrixInfo.push_back(cur);
    }else{  //分别输入每个矩阵的参数
        for(int i = 0;i < MatrixNum; i++){
            vector<int> cur;
            cout<<"请输入第"<<i+1<<"个矩阵的行数、列数、阻碍最大长度、阻碍个数，中间用空格分开:";
            cin>>row>>col>>obsLen>>obsNum;
            cur.push_back(row); //存入矩阵的行数、列数、阻碍最大长度、阻碍个数
            cur.push_back(col);
            cur.push_back(obsLen);
            cur.push_back(obsNum);
            MatrixInfo.push_back(cur);
        }
    }
    cout<<"搜索空间(矩阵)参数输入完成...\n";
}


