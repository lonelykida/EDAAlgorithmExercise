#include "main.hpp"
// #include <iostream>
// using namespace std;



int main(){
    

    bool isContinue = false;    //ѯ���û��Ƿ�������ɾ���
    //1.���²��� - ��Ҫ�Ƕ�ȡtimes
    readIni();
    //2.��ʼ���û�������Ҫ���������Լ���Ӧ����ϸ�������
    userInputInfo();
    do{ //��������
        //1.�����û�����ľ�����Ϣ�����ɾ��󣬲�ִ�������㷨
        generateMatrix();   //���ɾ���
        for(int t = 0;t < MatrixNum;t++){
            //��ȡ���ɵľ���
            vector<vector<int>> matrix = MATRIX[t];
            //ִ��Lee���㷨
            // Lee();  
            Lee(t,matrix);
            tmpName.clear();    //������ʱ������
        }
        //2.�����㷨ִ����Ϻ�ĺ������:
        tmpName.clear();    //������ʱ������
        MATRIX.clear();     //��վ���
        //3.ѯ���û��Ƿ�������ɾ���
        cout<<"�Ƿ�������ɾ���?(��-1/��-0) :";
        cin>>isContinue;
        if(isContinue)userInputInfo();  //�������ɾ�����������������Ϣ
    }while(isContinue);
    //3.����׶�
    times+=MatrixNum;
    ofstream f(IniInfo);
    f<<times<<endl;
    f.close();
    cout<<"Done.\n";
    return 0;
}