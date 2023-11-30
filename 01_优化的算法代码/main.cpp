#include "main.hpp"
// #include <iostream>
// using namespace std;



int main(){
    

    bool isContinue = false;    //询问用户是否继续生成矩阵
    //1.更新参数 - 主要是读取times
    readIni();
    //2.开始让用户输入需要几个矩阵，以及对应的详细矩阵参数
    userInputInfo();
    do{ //函数主体
        //1.根据用户输入的矩阵信息，生成矩阵，并执行李氏算法
        generateMatrix();   //生成矩阵
        for(int t = 0;t < MatrixNum;t++){
            //获取生成的矩阵
            vector<vector<int>> matrix = MATRIX[t];
            //执行Lee氏算法
            // Lee();  
            Lee(t,matrix);
            tmpName.clear();    //重置临时矩阵名
        }
        //2.李氏算法执行完毕后的后处理过程:
        tmpName.clear();    //重置临时矩阵名
        MATRIX.clear();     //清空矩阵
        //3.询问用户是否继续生成矩阵
        cout<<"是否继续生成矩阵?(是-1/否-0) :";
        cin>>isContinue;
        if(isContinue)userInputInfo();  //继续生成矩阵则继续输入矩阵信息
    }while(isContinue);
    //3.后处理阶段
    times+=MatrixNum;
    ofstream f(IniInfo);
    f<<times<<endl;
    f.close();
    cout<<"Done.\n";
    return 0;
}