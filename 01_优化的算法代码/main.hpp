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
#include <chrono>       //ʱ���
#include <filesystem>

using namespace std;
// using namespace std::chrono; //����ע�͵�����Ҫ��������������ʱ���

/*
    ˵��:
    1.�����㷨:
        ���ϡ�A*�����˿�ʼ�����Ϻ�A*�������㷨��
    2.������
        �ֱ�������㷨д���ö��̺߳Ͳ��ö��̵߳Ĵ���,���ֱ���бȽϡ�
    3.��Ҫ������ļ������ݣ�
        1.ini�����ļ�����¼�ڼ������ɾ���
        2.��ʼ���������ռ�����ļ�
        3.obsInfo�ļ�����������obs��x��y���ꡢobs�����Լ�obs����
        4.ͨ����ͬ�㷨���������پ����������ռ�����ļ�;
        5.����ļ�,����������ʱ�䡢·�����ҵ��Ļ������Ƕ��١�����·���Ļ�·����ʲô,�Լ����Ľ���Ƚ�
    4.�ṹ�������������
        1.��ȡ���ǵڼ������ɽ��,�����ݽ����ʼ���ļ�·��
        2.�û����빹�����������ռ��С,������
            (1)����,(2)����,(3)obs��󳤶�,(4)obs����
        Ҫ���û�����ָ����������,�Ա��ֽ���ĿɱȽ���
        3.�����ʼ����;
        4.�Գ�ʼ������зֿ�;
        5.���ݷֿ���,(��)ʹ�ö��̶߳Գ�ʼ����������پ������;�����м���,����¼����ʱ��time1
        6.�м��������ļ�
        7.�����м�������·��(��¼����ʱ��time2),����¼·��λ��;
            Ϊ�˷�ֹ����ʱ��ջ,��˲����õݹ鷽��,�������ֶ�ģ��ջ�ķ�ʽ������·��;
        8.����·�������ɽ�������ļ�,������ʱ��time=time1+time2;
    5.x\y����˵��: x���з���,y���з���,���������ʾ�����в��,������ʾ
                    0 1 2 3 4 5 6 7 8 9....
                    y1-----y2-----y3-----y4-----y5-----
        0    x1  -----------------------------------
        1    x2  -----------------------------------
        2    x3  -----------------------------------
        3    x4  -----------------------------------
        4    x5  -----------------------------------
        ...  ...  ...
    6.����д����Щ������txд��ex,tyд��ey,������ͬ
*/


//1.�����ļ�·�����������������ԭ��
int MatrixNum = 0;  //���û�����һ��Ҫ���ɶ��ٸ�����
int times = 0;  //��¼�ڼ������ɾ���,ͬʱ��Ϊ�ļ�����һ����
int isSameIni = 0; //���о��������Ƿ���ͬ
#define MaxBlockSize 100 //ÿ������������С
vector<vector<int>> MatrixInfo; //�û�������ÿ���������Ϣ,��ʽ����:���� ���� �谭��󳤶� �谭����,��������ǰ���û������Ƿ�ȫ��ʹ����ͬ����
vector<vector<vector<int>>> MATRIX; //����������ɵ�MatrixNum�����������ռ�
vector<vector<int>> SoE;    //����ÿ����������յ���Ϣ,��ʽ����:sx sy ex ey
vector<vector<vector<int>>> BlockInfo;  //ÿ������ķֿ���Ϣ
/*
    BlockInfo�ṹ˵����
    1.�ڲ���ŵ��Ƕ�ά����;
    2.��ά����ĵ�һ�м�¼:x�������,x������С,y�������,y������С
        �ڶ��м�¼��x����ÿ�������ʼx����
        �����м�¼��y����ÿ�������ʼ����
*/
vector<vector<int>> xySite;     //��������·��x,y����

vector<int> infectionTime;  //��Ⱦʱ��
vector<int> FeedBackTime;   //����ʱ��
vector<int> FullTime;       //����ʱ = ��Ⱦʱ��+����ʱ��

string tmpName; //��ʱ�ļ���,������Ϊ�ļ�����һ����
string IniInfo = "./iniInfo.txt";   //�����ڼ������ɾ���
string InitSpace = "./initSpace_" + tmpName + "_" + to_string(times) + ".txt";   //��ʼ���������ռ����
string obsInfo = "./obsInfo_" + tmpName + "_" + to_string(times) + ".txt";   //obs·��
string midRes  = "./midRes_" + tmpName + "_" + to_string(times) + ".txt";    //�м��� - ÿ���㷨�������м����м����ļ�,���Ժ����Ҫ��������
string AlgoRes = "./AlgoRes_" + tmpName + "_" + to_string(times) + ".txt";   //ÿ���㷨�����ս��
string resultFile = "./res_"+to_string(times)+".txt";   //�������ɾ�������ս���ļ�
string SoEFile = "./SoE_"+tmpName+to_string(times)+".txt";      //�������ɾ�������յ��ļ�
string BlockInfoFile = "./blockInfo_"+tmpName+to_string(times)+".txt";  //�������ɾ���Ŀ���Ϣ�ļ�

void FeedBack(vector<vector<int>>&arr,int sx,int sy,int tx,int ty,int xb,int yb,vector<vector<int>>&site);
void infection(vector<vector<int>>&arr,int left,int right,int up,int down,int x,int y,int sx,int sy,int tx,int ty);
void infection_seq(vector<vector<int>>&arr,int left,int right,int up,int down,int sx,int sy,int tx,int ty);
void IO_Matrix(vector<vector<int>>const &arr,int row ,int col,string fName);
void IO_Site(vector<int>const &xs,vector<int>const &ys,string fName);


//2.��ȡ�����ļ�,����times�������ļ���
void readIni(){
    //1.��iniInfo��ȡ�ڼ������ɾ���,����һ�����ɾ���,���ʼ����������
    ifstream iniFile_i(IniInfo);
    cout<<"�����ļ���ȡ��...\n";
    if(!iniFile_i){    //2.��ʧ��,��ʾ�ļ���δ����,�ǵ�һ�ζ�ȡ�ļ�,���г�ʼ��
        ofstream iniFile_o(IniInfo);    //���ļ�
        if(!iniFile_o.is_open()){//��ʧ��
            cout<<"�����ļ�д��ʧ��!����ֱ����ֹ\n";
            exit(-1);
        }
        iniFile_o<<times<<endl;   //д��ڼ������ɾ��������ļ�
        iniFile_o.close();
    }else
        iniFile_i>>times;   //3.��ȡ�ڼ������ɾ���,�˴��������ļ���,�����ȷ�����㷨����tmpName���ٸ����ļ���
    cout<<"�����ļ���ȡ�ɹ�...\n";
    iniFile_i.close();
}

//3.��ʼ���û������������������Ϣ - �����������������谭��󳤶ȡ��谭���� - ��ʹ��isSameIni�������Ƿ�ʹ����ͬ�ľ������
void userInputInfo(){
    MatrixInfo.clear(); //��ʼǰ��վ������
    cout<<"��ʼ���������ռ�(����)������Ϣ...\n";
    cout<<"������Ҫһ�����ɶ��ٸ�����:";
    cin>>MatrixNum;
    cout<<"�Ƿ�Ҫʹ����ͬ�ľ������?����,������1;����,������0:";
    cin>>isSameIni;
    int row,col,obsLen,obsNum;
    if(isSameIni){  //ʹ����ͬ����
        vector<int> cur;
        cout<<"�����������������������谭��󳤶ȡ��谭����,�м��ÿո�ֿ�:";
        cin>>row>>col>>obsLen>>obsNum;
        cur.push_back(row); //���������������������谭��󳤶ȡ��谭����
        cur.push_back(col);
        cur.push_back(obsLen);
        cur.push_back(obsNum);
        for(int i = 0;i < MatrixNum;i++)
            MatrixInfo.push_back(cur);
    }else{  //�ֱ�����ÿ������Ĳ���
        for(int i = 0;i < MatrixNum; i++){
            vector<int> cur;
            cout<<"�������"<<i+1<<"��������������������谭��󳤶ȡ��谭����,�м��ÿո�ֿ�:";
            cin>>row>>col>>obsLen>>obsNum;
            cur.push_back(row); //���������������������谭��󳤶ȡ��谭����
            cur.push_back(col);
            cur.push_back(obsLen);
            cur.push_back(obsNum);
            MatrixInfo.push_back(cur);
        }
    }
    vector<string> SoEFilePath; //��¼SoE��Ϣ���ļ�·��
    tmpName.clear();
    tmpName = "���յ�";
    for(int i = 0;i < MatrixNum;i++){
        SoEFile = "./SoE_"+tmpName+to_string(times+i)+".txt";
        SoEFilePath.push_back(SoEFile);
    }
    //�û��������յ���Ϣ:
    bool isSameSoE = false;
    cout<<"�Ƿ�ʹ����ͬ�����յ���Ϣ?(��-1,��-0):";
    cin>>isSameSoE;
    while(isSameSoE!=0 && isSameSoE!=1){
        cout<<"��������!������0(��)��1(��)�������Ƿ�ʹ����ͬ�����յ���Ϣ:";
        cin>>isSameSoE;
    }
    SoE.clear();    //�������
    if(isSameSoE){  //����ͬ���յ�,ֻ������һ����Ϣ
        cout<<"���������յ��x��y����(sx sy ex ey��ʽ):";
        int sx = 0,sy = 0,ex = 0,ey = 0;
        cin>>sx>>sy>>ex>>ey;
        vector<int>st;
        st.push_back(sx);
        st.push_back(sy);
        st.push_back(ex);
        st.push_back(ey);
        for(int i = 0;i < MatrixNum;i++){
            SoE.push_back(st);
            ofstream fsoe(SoEFilePath[i]);
            fsoe<<"���X Y :"<<sx<<" "<<sy<<endl;
            fsoe<<"�յ�X Y :"<<ex<<" "<<ey<<endl;
            fsoe.close();
        }
    }else{          //��ͬ���յ�
        for(int i = 0;i < MatrixNum;i++){   //����ÿ����������յ�
            cout<<"�������"<<i+1<<"����������յ��x��y����(sx sy ex ey��ʽ):";
            int sx = 0,sy = 0,ex = 0,ey = 0;
            cin>>sx>>sy>>ex>>ey;
            vector<int>st;
            st.push_back(sx);
            st.push_back(sy);
            st.push_back(ex);
            st.push_back(ey);
            SoE.push_back(st);
            ofstream fsoe(SoEFilePath[i]);
            fsoe<<"���X Y :"<<sx<<" "<<sy<<endl;
            fsoe<<"�յ�X Y :"<<ex<<" "<<ey<<endl;
            fsoe.close();
        }
    }
    cout<<"�����ռ�(����)�����������...\n";
}

//4.���ɾ���
void generateMatrix(){
    //�����������
    minstd_rand gen;    //�������������
    //����ÿ���������Ĵ洢·��
    vector<string>storePath;    //�洢ÿ������ĳ�ʼ����·��
    vector<string>storeOBS;     //�洢ÿ�������Ӧ��OBS�ı���·��
    for(int i = 0;i < MatrixNum;i++){
        tmpName.clear();
        tmpName = "��ʼ����";
        InitSpace = "./initSpace_" + tmpName + "_" + to_string(times) + ".txt";
        storePath.push_back(InitSpace);
        tmpName.clear();
        tmpName = "�ϰ�";
        obsInfo = "./obsInfo_" + tmpName + "_" + to_string(times) + ".txt";
        storeOBS.push_back(obsInfo);
        times++;
    }
    times-=MatrixNum;   //times��ԭ
    tmpName.clear();    //tmpName��ԭ
    //��ʼ����MatrixNum������
    for(int i = 0;i < MatrixNum;i++){   //Ҫ����MatrixNum������
        //��ȡ������Ϣ��������i����ʼ���󲢴򿪵�ǰ����ĳ�ʼ����·��
        cout<<"��ʼ�����"<<i+1<<"������...\n";
        vector<vector<int>>cur(MatrixInfo[i][0],vector<int>(MatrixInfo[i][1],0)); //���յĶ�ά����
        int row = MatrixInfo[i][0];     //��ȡ��������
        int col = MatrixInfo[i][1];     //��ȡ��������
        int obsLen = MatrixInfo[i][2];  //��ȡ�谭��󳤶�
        int obsNum = MatrixInfo[i][3];  //��ȡ�谭����
        ofstream fobs(storeOBS[i]);     //�򿪵�i��������谭��Ϣ����·��
        ofstream fmtx(storePath[i]);    //�򿪵�i������ĳ�ʼ����·��
        fmtx<<row<<"��"<<col<<"��"<<endl;
        //1.��������谭λ�ü����ȡ�����
        //���������������
        srand(time(0)); //�����������
        uniform_int_distribution<int> obslength(1,obsLen);  //����1-obsNum���ȵ��谭
        uniform_int_distribution<int> ypos(0,col-1);        //����0-col-1λ�õ��谭
        uniform_int_distribution<int> xpos(0,row-1);        //����0-row-1λ�õ������
        uniform_int_distribution<int> num(0,1);             //����0-1������� - �������ɺ�(0)��(1)������谭
        //�����������
        int x = (xpos(gen)*rand()+rand()%rand())%row;   //�������x����
        int y = (ypos(gen)*rand()+rand()%rand())%col;   //�������y����
        int orient = num(gen)*rand()%2;   //ѡ��������� - ��(0)��(1)
        //2.�����谭λ�ü����ȡ�����,��ʼ����obsNum���谭,�������ڶ�ά������,ͬʱҪ�����ϰ���Ϣ
        for(int onum = 0;onum < obsNum;onum++){ //����obsNum���谭
            srand(time(0));
            int curlen = (obslength(gen)*rand()+rand()%rand()%100)%obsLen;   //��������谭����
            fobs<<"OBS X,Y "<<to_string(x)<<","<<y<<endl;
            fobs<<"OBS LEN "<<to_string(curlen)<<endl;
            fobs<<"OBS ORIENT "<<to_string(orient)<<" (��0��1)"<<endl;
            if(orient == 0){    //��
                for(int cury = y;cury<y+curlen&&cury<col;cury++)    //��������
                    cur[x][cury] = -1;
            }else{              //��
                for(int curx = x;curx<x+curlen && curx<row;curx++)  //��������
                    cur[curx][y] = -1;
            }
            x = (xpos(gen)*rand()+rand()%rand())%row;
            y = (ypos(gen)*rand()+rand()%rand())%col;
            orient = num(gen)*rand()%2;
        }
        //��ȡ���յ�����,��������ʼ��ΪINT_MIN,�յ������ʼ��ΪINT_MAX
        int sx = SoE[i][0];
        int sy = SoE[i][1];
        int ex = SoE[i][2];
        int ey = SoE[i][3];
        cur[sx][sy] = INT_MIN;
        cur[ex][ey] = INT_MAX;
        //3.�����ʼ����MATRIX���ļ�
        MATRIX.push_back(cur);
        for(int curx = 0;curx<row;curx++){  //���浽�ļ�
            for(int cury = 0;cury<col;cury++)
                fmtx<<cur[curx][cury]<<"\t";
            fmtx<<endl;            
        }
        fobs.close();
        fmtx.close();
        cout<<"��"<<i+1<<"�����������...\n";
    }
}

//5.�����㷨����
/*
    ����˼·��
    1.�����л��д���100,������зֿ��Ա��ڽ���"��Ⱦ";
    2.�ֿ�����ÿ��Ĵ�С,ͬʱע�����һ��ı߽�;
    3.�õݹ��ǵݹ����"��Ⱦ"�������������پ���; - ��Ⱦ������Ҫ��ʱ
    4.�����м�����a.�ȸ����ļ�����·��;b.�����м������ļ�
    5.��ʼ����,����¼����ʱ��;
    6.�����ݽ�����浽ȫ�ֱ���,Ϊ�����ȽϽ����׼��;
*/
void Lee(int num,vector<vector<int>>&arr){  //num��ʾ���ǵڼ������������
    //�������ݵĶ�ȡ
    int row = MatrixInfo[num][0];   //��ȡ����
    int col = MatrixInfo[num][1];   //��ȡ����
    int xnum,ynum;  //x��y����ķֿ���
    int xsize,ysize;//x��y����ķֿ��С
    //1.��ʼ�����С��еķֿ���
    cout<<"��һ������ʼ�����"<<times+num+1<<"���������ռ���С��еķֿ��������С...\n";
    if(row < MaxBlockSize) xnum = 1;
    else{
        int tmp = row;  //��2,ֱ�����СС��MaxBlockSize
        xnum = 1;
        while(tmp/2>MaxBlockSize){
            tmp/=2;
            xnum++;
        }
    }
    if(col < MaxBlockSize) ynum = 1;
    else{
        int tmp = col;  //��2,ֱ�����СС��MaxBlockSize
        ynum = 1;
        while(tmp/2>MaxBlockSize){
            tmp/=2;
            ynum++;
        }
    }
    //2.���ݷֿ���������С
    xsize = row/xnum;
    ysize = col/ynum;
    //3.����ÿ���ֿ������յ����Ϣ,�����浽���鼰�ļ�
    cout<<"�ڶ�������ʼ�����"<<times+num+1<<"���������ռ�ķֿ����յ���Ϣ������...\n";
    vector<vector<int>> BInfo;  //�洢block����Ϣ,��Ҳ��BlockInfo�����һ����
    vector<int>info_xySizeInfo; //�洢xnum,xsize,ynum,ysize
    vector<int>info_xSite;      //�洢x����ÿ��������
    vector<int>info_ySite;      //�洢y����ÿ��������
    info_xySizeInfo.push_back(xnum);    //�洢�п���
    info_xySizeInfo.push_back(xsize);   //�洢�п��С
    info_xySizeInfo.push_back(ynum);    //�洢�п���
    info_xySizeInfo.push_back(ysize);   //�洢�п��С
    BInfo.push_back(info_xySizeInfo);
    tmpName.clear();
    tmpName = "�ֿ�";
    BlockInfoFile = "./blockInfo_"+tmpName+to_string(times+num)+".txt"; //�����ļ�����·��
    ofstream fblk(BlockInfoFile);   //���ļ�·��,�����ļ�
    fblk<<"x�������:"<<xnum<<endl;
    fblk<<"x������С:"<<xsize<<endl;
    fblk<<"y�������:"<<ynum<<endl;
    fblk<<"y������С:"<<ysize<<endl;
    for(int i = 0;i <xnum-1;i++){   //д��x����ÿ��������,�����浽info_xSite
        info_xSite.push_back(i*xsize);
        fblk<<i*xsize<<"\t";
    }
    fblk<<endl;
    for(int i = 0;i <ynum-1;i++){   //д��y����ÿ��������,�����浽info_ySite
        info_ySite.push_back(i*ysize);
        fblk<<i*ysize<<"\t";
    }
    fblk<<endl;
    BInfo.push_back(info_xSite);
    BInfo.push_back(info_ySite);
    fblk.close();
    BlockInfo.push_back(BInfo);
    //4.��ʼinfection("��Ⱦ") - Ҫ��ʱ
    cout<<"����������ʼ��"<<times+num+1<<"�������ռ�ĸ�Ⱦ����...\n";
    auto startTime = chrono::high_resolution_clock::now();  //ʹ��high_resolution_clock��ø߾���ʱ���
    for(int i = 0;i < xnum;i++){
        for(int j = 0;j < ynum;j++){
            int left = i*xsize; //��߽�
            int right = i+1==i?col:(i+1)*xsize-1; //�ұ߽�
            int up = j*ysize; //�ϱ߽�
            int down = j+1==j?row:(j+1)*ysize-1; //�±߽�
            infection_seq(arr,left,right,up,down,SoE[num][0],SoE[num][1],SoE[num][2],SoE[num][3]);
        }
    }
    auto endTime = chrono::high_resolution_clock::now();    //ʹ��high_resolution_clock��ø߾���ʱ���
    infectionTime.push_back(static_cast<int>(chrono::duration_cast<chrono::microseconds>(endTime-startTime).count()));
    //5.�м�������д�뵽�ļ�
    cout<<"���Ĳ��������"<<times+num+1<<"�������ռ���м����...\n";
    tmpName.clear();
    tmpName = "�м����";
    midRes  = "./midRes_" + tmpName + "_" + to_string(times+num) + ".txt";
    IO_Matrix(arr,row,col,midRes);
    //6.���� - Ҫ��¼ʱ��
    cout<<"���岽����ʼ��"<<times+num+1<<"�������ռ�Ļ��ݹ���...\n";
    xySite.clear();
    startTime = chrono::high_resolution_clock::now();       //ʹ��high_resolution_clock��ø߾���ʱ���
    FeedBack(arr,SoE[num][0],SoE[num][1],SoE[num][2],SoE[num][3],row-1,col-1,xySite);
    endTime = chrono::high_resolution_clock::now();         //ʹ��high_resolution_clock��ø߾���ʱ���
    FeedBackTime.push_back(static_cast<int>((chrono::duration_cast<chrono::microseconds>(endTime-startTime)).count()));
    FullTime.push_back(infectionTime[num]+FeedBackTime[num]);
    //7.���ݽ��д���ļ�
    cout<<"����������"<<times+num+1<<"�������ռ�Ļ��ݽ�������ļ�...\n";
    tmpName = "����·��";
    midRes  = "./midRes_" + tmpName + "_" + to_string(times+num) + ".txt";  //·���洢·��
    tmpName = "���ݾ���";
    AlgoRes = "./AlgoRes_" + tmpName + "_" + to_string(times+num) + ".txt"; //���ľ���洢·��
    IO_Site(xySite[0],xySite[1],midRes);
    IO_Matrix(arr,row,col,AlgoRes);
    //8.��������ʱ�����ļ�:
    cout<<"���߲������ɵ�"<<times+num+1<<"�������ռ������ʱ�����ļ�...\n";
    tmpName = "����ʱ��";
    resultFile = "./res_"+to_string(times+num)+".txt";
    ofstream res(resultFile);
    res<<"��Ⱦʱ��:"<<infectionTime[num]<<endl;
    res<<"����ʱ��:"<<FeedBackTime[num]<<endl;
    res<<"��ʱ��:"<<FullTime[num]<<endl;
}

//6.�ݹ��Ⱦ����
void infection(vector<vector<int>>&arr,int left,int right,int up,int down,int x,int y,int sx,int sy,int tx,int ty){
    //x��y�Ǹ�Ⱦ���������,left���Ǵ���������Ⱦ����������ҵı߽�
    if(x < left || x > right || y < up || y > down || arr[x][y] != 0)return;    //��Χ��������Ѿ���Ⱦ����
    if((x == tx && y == ty) || (sx == x && sy == y)) return ;  //�����յ���ߴ���㵽���յ�
    arr[x][y] = abs(x-sx)+abs(y-sy);    //���������پ���
    //��ʼ�ݹ������������ĸ������Ⱦ
    infection(arr,left,right,up,down,x-1,y,sx,sy,tx,ty);    //��
    infection(arr,left,right,up,down,x+1,y,sx,sy,tx,ty);    //��
    infection(arr,left,right,up,down,x,y-1,sx,sy,tx,ty);    //��
    infection(arr,left,right,up,down,x,y+1,sx,sy,tx,ty);    //��
    /*
        ������
            �ݹ��Ⱦ,��ջ��ҪҲ����������̻ᱬջ,��˾����ܲ�Ҫʹ�õݹ����,����,���̱߳���ʹ�÷ǵݹ�,��Ϊ���ܱ�ջ
            ʱ�临�Ӷȣ�O(n^2),��Ϊ��Լ������е�λ�ö�����һ��
            �ռ临�Ӷȣ�O(n^2),��Ϊ�ݹ���Զ�ѹջ,����ÿ��λ�ö��ᱻ�ݹ����
    */
}

//7.�ǵݹ��Ⱦ���� - ���̱߳���ʹ�ø��ַ�ʽ
void infection_seq(vector<vector<int>>&arr,int left,int right,int up,int down,int sx,int sy,int tx,int ty){
    //�ò���x��y,��Ϊ�����������ڵ�ÿһ����,left���Ǵ���������Ⱦ����������ҵı߽�
    for(int j = left;j<=right;j++){ //��
        for(int i = up;i <= down;i++){
            if(arr[i][j]!=0)continue;   //˵���õ㲻�ɴ�
            arr[i][j] = abs(i-sx)+abs(j-sy);    //�������پ���
        }
    }
    /*
        ������
            ʱ�临�Ӷ�:O(n^2),��Ϊÿ��λ�ö��ᱻ���ʵ�
            �ռ临�Ӷ�:O(1),��Ϊֻʹ�������޵ļ�������
            �����б�ջ������
    */
}

//8.���ݹ��̡�����ݹ�ᱬջ,���ʹ�÷ǵݹ鷽ʽ������(��߽���ϱ߽���0)
void FeedBack(vector<vector<int>>&arr,int sx,int sy,int tx,int ty,int xb,int yb,vector<vector<int>>&site){
    //xb��yb���±߽���ұ߽�,site���ڼ�¼x��y�������
    int x = sx,y = sy;      //��ʼ��
    vector<int>s_x,s_y;     //ģ������ջ
    do{
        s_x.push_back(x);   //x��y������ջ
        s_y.push_back(y);
        if(x==tx && y==ty)break;    //�Ѿ�����Ŀ���,�˳�ѭ��
        if(x-1>=0 && arr[x-1][y]>arr[x][y]){    //������,������arr[x-1][y]>arr[x][y],����ܱ�֤�����߻�ͷ·
            x--;
            continue;
        }
        else if(x+1>=0 && arr[x+1][y]>arr[x][y]){    //������
            x--;
            continue;
        }
        else if(y-1>=0 && arr[x][y-1]>arr[x][y]){    //������
            x--;
            continue;
        }
        else if(y+1>=0 && arr[x][y+1]>arr[x][y]){    //������
            x--;
            continue;
        }else { //��·��ͨ,����ѷ��ʱ�־-2
            arr[x][y] = -2;
            x = s_x[s_x.size()-1];
            s_x.pop_back();
            y = s_y[s_y.size()-1];
            s_y.pop_back();
        }
    }while(!s_x.empty());   //��ջΪ��ʱ�˳�ѭ��
    site.push_back(s_x);    //x�����¼
    site.push_back(s_y);    //y�����¼
}

//������д�뵽�����ļ�
void IO_Matrix(vector<vector<int>>const &arr,int row ,int col,string fName){
    ofstream f(fName);  //���ļ�
    for(int i = 0;i < row;i++){
        for (int j = 0;j < col;j++) f<<arr[i][j]<<"\t";
        f<<endl;
    }
    f.close();
}

//������д�뵽����ļ�
void IO_Site(vector<int>const &xs,vector<int>const &ys,string fName){
    ofstream f(fName,ios::app); //��׷�ӷ�ʽ���ļ�
    if(xs.empty() || ys.empty())
        f<<"δ�ҵ�·��!\n";
    else
        for(int i = 0,j = xs.size();i < j;i++){ //��xs.size()�̶���j�м��ٳ���ִ�й���
            f<<"("<<xs[i]<<","<<ys[i]<<")";
            if(i+1 == xs.size())f<<"->";
        }
    f.close();
}



