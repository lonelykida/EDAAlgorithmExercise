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
#include <chrono>       //时间库
#include <filesystem>

using namespace std;
// using namespace std::chrono; //可以注释掉，主要是用来测试运行时间的

/*
    说明:
    1.包含算法:
        李氏、A*、两端开始的李氏和A*这四种算法。
    2.技术：
        分别对上述算法写出用多线程和不用多线程的代码,并分别进行比较。
    3.需要保存成文件的内容：
        1.ini配置文件：记录第几次生成矩阵
        2.初始化的搜索空间矩阵文件
        3.obsInfo文件：包含各个obs的x、y坐标、obs长度以及obs方向
        4.通过不同算法计算曼哈顿距离后的搜索空间矩阵文件;
        5.结果文件,包含：运行时间、路径若找到的话长度是多少、若有路径的话路径是什么,以及最后的结果比较
    4.结构功能重组分析：
        1.读取这是第几次生成结果,并根据结果初始化文件路径
        2.用户输入构造的随机搜索空间大小,包括：
            (1)行数,(2)列数,(3)obs最大长度,(4)obs个数
        要求用户必须指定上述参数,以保持结果的可比较性
        3.保存初始矩阵;
        4.对初始矩阵进行分块;
        5.根据分块结果,(不)使用多线程对初始块进行曼哈顿距离计算;生成中间结果,并记录运行时间time1
        6.中间结果存入文件
        7.根据中间结果回溯路径(记录运行时间time2),并记录路径位置;
            为了防止运行时爆栈,因此不采用递归方法,而采用手动模拟栈的方式来回溯路径;
        8.保存路径并生成结果报告文件,总运行时间time=time1+time2;
    5.x\y方向说明: x是行方向,y是列方向,但在坐标表示上略有差别,如下所示
                    0 1 2 3 4 5 6 7 8 9....
                    y1-----y2-----y3-----y4-----y5-----
        0    x1  -----------------------------------
        1    x2  -----------------------------------
        2    x3  -----------------------------------
        3    x4  -----------------------------------
        4    x5  -----------------------------------
        ...  ...  ...
    6.文中写法有些函数将tx写作ex,ty写作ey,含义相同
*/


//1.定义文件路径及所需变量、函数原型
int MatrixNum = 0;  //让用户决定一次要生成多少个矩阵
int times = 0;  //记录第几次生成矩阵,同时作为文件名的一部分
int isSameIni = 0; //所有矩阵配置是否都相同
#define MaxBlockSize 100 //每个矩阵块的最大大小
vector<vector<int>> MatrixInfo; //用户来决定每个矩阵的信息,格式如下:行数 列数 阻碍最大长度 阻碍个数,并在输入前让用户决定是否全部使用相同参数
vector<vector<vector<int>>> MATRIX; //保存最后生成的MatrixNum个矩形搜索空间
vector<vector<int>> SoE;    //保存每个矩阵的起终点信息,格式如下:sx sy ex ey
vector<vector<vector<int>>> BlockInfo;  //每个矩阵的分块信息
/*
    BlockInfo结构说明：
    1.内部存放的是二维数组;
    2.二维数组的第一行记录:x方向块数,x方向块大小,y方向块数,y方向块大小
        第二行记录：x方向每个块的起始x坐标
        第三行记录：y方向每个块的起始坐标
*/
vector<vector<int>> xySite;     //保存最后的路径x,y坐标

vector<int> infectionTime;  //感染时间
vector<int> FeedBackTime;   //回溯时间
vector<int> FullTime;       //总用时 = 感染时间+回溯时间

string tmpName; //临时文件名,用于作为文件名的一部分
string IniInfo = "./iniInfo.txt";   //包含第几次生成矩阵
string InitSpace = "./initSpace_" + tmpName + "_" + to_string(times) + ".txt";   //初始化的搜索空间矩阵
string obsInfo = "./obsInfo_" + tmpName + "_" + to_string(times) + ".txt";   //obs路径
string midRes  = "./midRes_" + tmpName + "_" + to_string(times) + ".txt";    //中间结果 - 每个算法都可能有几个中间结果文件,所以后边需要了再增加
string AlgoRes = "./AlgoRes_" + tmpName + "_" + to_string(times) + ".txt";   //每种算法的最终结果
string resultFile = "./res_"+to_string(times)+".txt";   //本次生成矩阵的最终结果文件
string SoEFile = "./SoE_"+tmpName+to_string(times)+".txt";      //本次生成矩阵的起终点文件
string BlockInfoFile = "./blockInfo_"+tmpName+to_string(times)+".txt";  //本次生成矩阵的块信息文件

void FeedBack(vector<vector<int>>&arr,int sx,int sy,int tx,int ty,int xb,int yb,vector<vector<int>>&site);
void infection(vector<vector<int>>&arr,int left,int right,int up,int down,int x,int y,int sx,int sy,int tx,int ty);
void infection_seq(vector<vector<int>>&arr,int left,int right,int up,int down,int sx,int sy,int tx,int ty);
void IO_Matrix(vector<vector<int>>const &arr,int row ,int col,string fName);
void IO_Site(vector<int>const &xs,vector<int>const &ys,string fName);


//2.读取配置文件,更新times变量及文件名
void readIni(){
    //1.打开iniInfo读取第几次生成矩阵,若第一次生成矩阵,则初始化其他变量
    ifstream iniFile_i(IniInfo);
    cout<<"配置文件读取中...\n";
    if(!iniFile_i){    //2.打开失败,表示文件还未创建,是第一次读取文件,进行初始化
        ofstream iniFile_o(IniInfo);    //打开文件
        if(!iniFile_o.is_open()){//打开失败
            cout<<"配置文件写入失败!程序直接中止\n";
            exit(-1);
        }
        iniFile_o<<times<<endl;   //写入第几次生成矩阵到配置文件
        iniFile_o.close();
    }else
        iniFile_i>>times;   //3.读取第几次生成矩阵,此处不更新文件名,待后边确定了算法名称tmpName后再更新文件名
    cout<<"配置文件读取成功...\n";
    iniFile_i.close();
}

//3.开始让用户输入最基本的生成信息 - 矩阵行数、列数、阻碍最大长度、阻碍个数 - 并使用isSameIni来决定是否使用相同的矩阵参数
void userInputInfo(){
    MatrixInfo.clear(); //开始前清空矩阵参数
    cout<<"开始输入搜索空间(矩阵)参数信息...\n";
    cout<<"请输入要一次生成多少个矩阵:";
    cin>>MatrixNum;
    cout<<"是否要使用相同的矩阵参数?若是,请输入1;若否,请输入0:";
    cin>>isSameIni;
    int row,col,obsLen,obsNum;
    if(isSameIni){  //使用相同参数
        vector<int> cur;
        cout<<"请输入矩阵的行数、列数、阻碍最大长度、阻碍个数,中间用空格分开:";
        cin>>row>>col>>obsLen>>obsNum;
        cur.push_back(row); //存入矩阵的行数、列数、阻碍最大长度、阻碍个数
        cur.push_back(col);
        cur.push_back(obsLen);
        cur.push_back(obsNum);
        for(int i = 0;i < MatrixNum;i++)
            MatrixInfo.push_back(cur);
    }else{  //分别输入每个矩阵的参数
        for(int i = 0;i < MatrixNum; i++){
            vector<int> cur;
            cout<<"请输入第"<<i+1<<"个矩阵的行数、列数、阻碍最大长度、阻碍个数,中间用空格分开:";
            cin>>row>>col>>obsLen>>obsNum;
            cur.push_back(row); //存入矩阵的行数、列数、阻碍最大长度、阻碍个数
            cur.push_back(col);
            cur.push_back(obsLen);
            cur.push_back(obsNum);
            MatrixInfo.push_back(cur);
        }
    }
    vector<string> SoEFilePath; //记录SoE信息的文件路径
    tmpName.clear();
    tmpName = "起终点";
    for(int i = 0;i < MatrixNum;i++){
        SoEFile = "./SoE_"+tmpName+to_string(times+i)+".txt";
        SoEFilePath.push_back(SoEFile);
    }
    //用户输入起终点信息:
    bool isSameSoE = false;
    cout<<"是否使用相同的起终点信息?(是-1,否-0):";
    cin>>isSameSoE;
    while(isSameSoE!=0 && isSameSoE!=1){
        cout<<"输入有误!请输入0(否)或1(是)来决定是否使用相同的起终点信息:";
        cin>>isSameSoE;
    }
    SoE.clear();    //清空数组
    if(isSameSoE){  //是相同起终点,只用输入一遍信息
        cout<<"请输入起终点的x、y坐标(sx sy ex ey形式):";
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
            fsoe<<"起点X Y :"<<sx<<" "<<sy<<endl;
            fsoe<<"终点X Y :"<<ex<<" "<<ey<<endl;
            fsoe.close();
        }
    }else{          //不同起终点
        for(int i = 0;i < MatrixNum;i++){   //输入每个矩阵的起终点
            cout<<"请输入第"<<i+1<<"个矩阵的起终点的x、y坐标(sx sy ex ey形式):";
            int sx = 0,sy = 0,ex = 0,ey = 0;
            cin>>sx>>sy>>ex>>ey;
            vector<int>st;
            st.push_back(sx);
            st.push_back(sy);
            st.push_back(ex);
            st.push_back(ey);
            SoE.push_back(st);
            ofstream fsoe(SoEFilePath[i]);
            fsoe<<"起点X Y :"<<sx<<" "<<sy<<endl;
            fsoe<<"终点X Y :"<<ex<<" "<<ey<<endl;
            fsoe.close();
        }
    }
    cout<<"搜索空间(矩阵)参数输入完成...\n";
}

//4.生成矩阵
void generateMatrix(){
    //构造随机环境
    minstd_rand gen;    //构造随机数引擎
    //生成每个矩阵最后的存储路径
    vector<string>storePath;    //存储每个矩阵的初始保存路径
    vector<string>storeOBS;     //存储每个矩阵对应的OBS的保存路径
    for(int i = 0;i < MatrixNum;i++){
        tmpName.clear();
        tmpName = "初始矩阵";
        InitSpace = "./initSpace_" + tmpName + "_" + to_string(times) + ".txt";
        storePath.push_back(InitSpace);
        tmpName.clear();
        tmpName = "障碍";
        obsInfo = "./obsInfo_" + tmpName + "_" + to_string(times) + ".txt";
        storeOBS.push_back(obsInfo);
        times++;
    }
    times-=MatrixNum;   //times还原
    tmpName.clear();    //tmpName还原
    //开始构造MatrixNum个矩阵
    for(int i = 0;i < MatrixNum;i++){   //要生成MatrixNum个矩阵
        //读取构造信息并构建第i个初始矩阵并打开当前矩阵的初始保存路径
        cout<<"开始构造第"<<i+1<<"个矩阵...\n";
        vector<vector<int>>cur(MatrixInfo[i][0],vector<int>(MatrixInfo[i][1],0)); //最终的二维矩阵
        int row = MatrixInfo[i][0];     //读取矩阵行数
        int col = MatrixInfo[i][1];     //读取矩阵列数
        int obsLen = MatrixInfo[i][2];  //读取阻碍最大长度
        int obsNum = MatrixInfo[i][3];  //读取阻碍个数
        ofstream fobs(storeOBS[i]);     //打开第i个矩阵的阻碍信息保存路径
        ofstream fmtx(storePath[i]);    //打开第i个矩阵的初始保存路径
        fmtx<<row<<"行"<<col<<"列"<<endl;
        //1.生成随机阻碍位置及长度、方向
        //构造随机数生成器
        srand(time(0)); //置随机数种子
        uniform_int_distribution<int> obslength(1,obsLen);  //构造1-obsNum长度的阻碍
        uniform_int_distribution<int> ypos(0,col-1);        //构造0-col-1位置的阻碍
        uniform_int_distribution<int> xpos(0,row-1);        //构造0-row-1位置的随机数
        uniform_int_distribution<int> num(0,1);             //构造0-1的随机数 - 用于生成横(0)竖(1)方向的阻碍
        //构造随机参数
        int x = (xpos(gen)*rand()+rand()%rand())%row;   //随机生成x坐标
        int y = (ypos(gen)*rand()+rand()%rand())%col;   //随机生成y坐标
        int orient = num(gen)*rand()%2;   //选择随机方向 - 横(0)竖(1)
        //2.根据阻碍位置及长度、方向,开始生成obsNum个阻碍,并放置于二维矩阵中,同时要保存障碍信息
        for(int onum = 0;onum < obsNum;onum++){ //生成obsNum个阻碍
            srand(time(0));
            int curlen = (obslength(gen)*rand()+rand()%rand()%100)%obsLen;   //随机生成阻碍长度
            fobs<<"OBS X,Y "<<to_string(x)<<","<<y<<endl;
            fobs<<"OBS LEN "<<to_string(curlen)<<endl;
            fobs<<"OBS ORIENT "<<to_string(orient)<<" (横0竖1)"<<endl;
            if(orient == 0){    //横
                for(int cury = y;cury<y+curlen&&cury<col;cury++)    //横向生成
                    cur[x][cury] = -1;
            }else{              //竖
                for(int curx = x;curx<x+curlen && curx<row;curx++)  //纵向生成
                    cur[curx][y] = -1;
            }
            x = (xpos(gen)*rand()+rand()%rand())%row;
            y = (ypos(gen)*rand()+rand()%rand())%col;
            orient = num(gen)*rand()%2;
        }
        //读取起终点坐标,起点坐标初始化为INT_MIN,终点坐标初始化为INT_MAX
        int sx = SoE[i][0];
        int sy = SoE[i][1];
        int ex = SoE[i][2];
        int ey = SoE[i][3];
        cur[sx][sy] = INT_MIN;
        cur[ex][ey] = INT_MAX;
        //3.保存初始矩阵到MATRIX及文件
        MATRIX.push_back(cur);
        for(int curx = 0;curx<row;curx++){  //保存到文件
            for(int cury = 0;cury<col;cury++)
                fmtx<<cur[curx][cury]<<"\t";
            fmtx<<endl;            
        }
        fobs.close();
        fmtx.close();
        cout<<"第"<<i+1<<"个矩阵构造完毕...\n";
    }
}

//5.李氏算法主体
/*
    基本思路：
    1.将若行或列大于100,则将其进行分块以便于进行"感染";
    2.分块后计算每块的大小,同时注意最后一块的边界;
    3.用递归或非递归进行"感染"——计算曼哈顿距离; - 感染过程需要计时
    4.保存中间结果：a.先更新文件保存路径;b.保存中间结果到文件
    5.开始回溯,并记录回溯时间;
    6.将回溯结果保存到全局变量,为后续比较结果做准备;
*/
void Lee(int num,vector<vector<int>>&arr){  //num表示这是第几个矩阵的数据
    //基础数据的读取
    int row = MatrixInfo[num][0];   //读取行数
    int col = MatrixInfo[num][1];   //读取列数
    int xnum,ynum;  //x、y方向的分块数
    int xsize,ysize;//x、y方向的分块大小
    //1.开始计算行、列的分块数
    cout<<"第一步：开始计算第"<<times+num+1<<"个的搜索空间的行、列的分块数及块大小...\n";
    if(row < MaxBlockSize) xnum = 1;
    else{
        int tmp = row;  //除2,直到块大小小于MaxBlockSize
        xnum = 1;
        while(tmp/2>MaxBlockSize){
            tmp/=2;
            xnum++;
        }
    }
    if(col < MaxBlockSize) ynum = 1;
    else{
        int tmp = col;  //除2,直到块大小小于MaxBlockSize
        ynum = 1;
        while(tmp/2>MaxBlockSize){
            tmp/=2;
            ynum++;
        }
    }
    //2.根据分块数计算块大小
    xsize = row/xnum;
    ysize = col/ynum;
    //3.计算每个分块的起点终点等信息,并保存到数组及文件
    cout<<"第二步：开始计算第"<<times+num+1<<"个的搜索空间的分块起终点信息并保存...\n";
    vector<vector<int>> BInfo;  //存储block的信息,这也是BlockInfo数组的一部分
    vector<int>info_xySizeInfo; //存储xnum,xsize,ynum,ysize
    vector<int>info_xSite;      //存储x方向每个块的起点
    vector<int>info_ySite;      //存储y方向每个块的起点
    info_xySizeInfo.push_back(xnum);    //存储行块数
    info_xySizeInfo.push_back(xsize);   //存储行块大小
    info_xySizeInfo.push_back(ynum);    //存储列块数
    info_xySizeInfo.push_back(ysize);   //存储列块大小
    BInfo.push_back(info_xySizeInfo);
    tmpName.clear();
    tmpName = "分块";
    BlockInfoFile = "./blockInfo_"+tmpName+to_string(times+num)+".txt"; //更新文件保存路径
    ofstream fblk(BlockInfoFile);   //打开文件路径,保存文件
    fblk<<"x方向块数:"<<xnum<<endl;
    fblk<<"x方向块大小:"<<xsize<<endl;
    fblk<<"y方向块数:"<<ynum<<endl;
    fblk<<"y方向块大小:"<<ysize<<endl;
    for(int i = 0;i <xnum-1;i++){   //写入x方向每个块的起点,并保存到info_xSite
        info_xSite.push_back(i*xsize);
        fblk<<i*xsize<<"\t";
    }
    fblk<<endl;
    for(int i = 0;i <ynum-1;i++){   //写入y方向每个块的起点,并保存到info_ySite
        info_ySite.push_back(i*ysize);
        fblk<<i*ysize<<"\t";
    }
    fblk<<endl;
    BInfo.push_back(info_xSite);
    BInfo.push_back(info_ySite);
    fblk.close();
    BlockInfo.push_back(BInfo);
    //4.开始infection("感染") - 要计时
    cout<<"第三步：开始第"<<times+num+1<<"个搜索空间的感染过程...\n";
    auto startTime = chrono::high_resolution_clock::now();  //使用high_resolution_clock获得高精度时间点
    for(int i = 0;i < xnum;i++){
        for(int j = 0;j < ynum;j++){
            int left = i*xsize; //左边界
            int right = i+1==i?col:(i+1)*xsize-1; //右边界
            int up = j*ysize; //上边界
            int down = j+1==j?row:(j+1)*ysize-1; //下边界
            infection_seq(arr,left,right,up,down,SoE[num][0],SoE[num][1],SoE[num][2],SoE[num][3]);
        }
    }
    auto endTime = chrono::high_resolution_clock::now();    //使用high_resolution_clock获得高精度时间点
    infectionTime.push_back(static_cast<int>(chrono::duration_cast<chrono::microseconds>(endTime-startTime).count()));
    //5.中间结果矩阵写入到文件
    cout<<"第四步：保存第"<<times+num+1<<"个搜索空间的中间矩阵...\n";
    tmpName.clear();
    tmpName = "中间矩阵";
    midRes  = "./midRes_" + tmpName + "_" + to_string(times+num) + ".txt";
    IO_Matrix(arr,row,col,midRes);
    //6.回溯 - 要记录时间
    cout<<"第五步：开始第"<<times+num+1<<"个搜索空间的回溯过程...\n";
    xySite.clear();
    startTime = chrono::high_resolution_clock::now();       //使用high_resolution_clock获得高精度时间点
    FeedBack(arr,SoE[num][0],SoE[num][1],SoE[num][2],SoE[num][3],row-1,col-1,xySite);
    endTime = chrono::high_resolution_clock::now();         //使用high_resolution_clock获得高精度时间点
    FeedBackTime.push_back(static_cast<int>((chrono::duration_cast<chrono::microseconds>(endTime-startTime)).count()));
    FullTime.push_back(infectionTime[num]+FeedBackTime[num]);
    //7.回溯结果写入文件
    cout<<"第六步：第"<<times+num+1<<"个搜索空间的回溯结果存入文件...\n";
    tmpName = "回溯路径";
    midRes  = "./midRes_" + tmpName + "_" + to_string(times+num) + ".txt";  //路径存储路径
    tmpName = "回溯矩阵";
    AlgoRes = "./AlgoRes_" + tmpName + "_" + to_string(times+num) + ".txt"; //最后的矩阵存储路径
    IO_Site(xySite[0],xySite[1],midRes);
    IO_Matrix(arr,row,col,AlgoRes);
    //8.生成运行时间结果文件:
    cout<<"第七步：生成第"<<times+num+1<<"个搜索空间的运行时间结果文件...\n";
    tmpName = "运行时间";
    resultFile = "./res_"+to_string(times+num)+".txt";
    ofstream res(resultFile);
    res<<"感染时间:"<<infectionTime[num]<<endl;
    res<<"回溯时间:"<<FeedBackTime[num]<<endl;
    res<<"总时间:"<<FullTime[num]<<endl;
}

//6.递归感染过程
void infection(vector<vector<int>>&arr,int left,int right,int up,int down,int x,int y,int sx,int sy,int tx,int ty){
    //x和y是感染的起点坐标,left等是传入的这个感染块的上下左右的边界
    if(x < left || x > right || y < up || y > down || arr[x][y] != 0)return;    //范围错误或者已经感染过了
    if((x == tx && y == ty) || (sx == x && sy == y)) return ;  //到达终点或者从起点到达终点
    arr[x][y] = abs(x-sx)+abs(y-sy);    //求其曼哈顿距离
    //开始递归往上下左右四个方向感染
    infection(arr,left,right,up,down,x-1,y,sx,sy,tx,ty);    //上
    infection(arr,left,right,up,down,x+1,y,sx,sy,tx,ty);    //下
    infection(arr,left,right,up,down,x,y-1,sx,sy,tx,ty);    //左
    infection(arr,left,right,up,down,x,y+1,sx,sy,tx,ty);    //右
    /*
        分析：
            递归感染,爆栈主要也就是这个过程会爆栈,因此尽可能不要使用递归调用,另外,多线程必须使用非递归,因为可能爆栈
            时间复杂度：O(n^2),因为会对几乎所有的位置都遍历一次
            空间复杂度：O(n^2),因为递归会自动压栈,几乎每个位置都会被递归调用
    */
}

//7.非递归感染过程 - 多线程必须使用该种方式
void infection_seq(vector<vector<int>>&arr,int left,int right,int up,int down,int sx,int sy,int tx,int ty){
    //用不着x和y,因为会遍历这个块内的每一个点,left等是传入的这个感染块的上下左右的边界
    for(int j = left;j<=right;j++){ //列
        for(int i = up;i <= down;i++){
            if(arr[i][j]!=0)continue;   //说明该点不可达
            arr[i][j] = abs(i-sx)+abs(j-sy);    //求曼哈顿距离
        }
    }
    /*
        分析：
            时间复杂度:O(n^2),因为每个位置都会被访问到
            空间复杂度:O(1),因为只使用了有限的几个变量
            不会有爆栈等问题
    */
}

//8.回溯过程——因递归会爆栈,因此使用非递归方式来回溯(左边界和上边界是0)
void FeedBack(vector<vector<int>>&arr,int sx,int sy,int tx,int ty,int xb,int yb,vector<vector<int>>&site){
    //xb和yb是下边界和右边界,site用于记录x和y点的坐标
    int x = sx,y = sy;      //起始点
    vector<int>s_x,s_y;     //模拟坐标栈
    do{
        s_x.push_back(x);   //x、y坐标入栈
        s_y.push_back(y);
        if(x==tx && y==ty)break;    //已经到达目标点,退出循环
        if(x-1>=0 && arr[x-1][y]>arr[x][y]){    //往上走,由于有arr[x-1][y]>arr[x][y],因此能保证不会走回头路
            x--;
            continue;
        }
        else if(x+1>=0 && arr[x+1][y]>arr[x][y]){    //往下走
            x--;
            continue;
        }
        else if(y-1>=0 && arr[x][y-1]>arr[x][y]){    //往左走
            x--;
            continue;
        }
        else if(y+1>=0 && arr[x][y+1]>arr[x][y]){    //往右走
            x--;
            continue;
        }else { //此路不通,标记已访问标志-2
            arr[x][y] = -2;
            x = s_x[s_x.size()-1];
            s_x.pop_back();
            y = s_y[s_y.size()-1];
            s_y.pop_back();
        }
    }while(!s_x.empty());   //当栈为空时退出循环
    site.push_back(s_x);    //x坐标记录
    site.push_back(s_y);    //y坐标记录
}

//将数组写入到给定文件
void IO_Matrix(vector<vector<int>>const &arr,int row ,int col,string fName){
    ofstream f(fName);  //打开文件
    for(int i = 0;i < row;i++){
        for (int j = 0;j < col;j++) f<<arr[i][j]<<"\t";
        f<<endl;
    }
    f.close();
}

//将坐标写入到结果文件
void IO_Site(vector<int>const &xs,vector<int>const &ys,string fName){
    ofstream f(fName,ios::app); //以追加方式打开文件
    if(xs.empty() || ys.empty())
        f<<"未找到路径!\n";
    else
        for(int i = 0,j = xs.size();i < j;i++){ //将xs.size()固定到j中加速程序执行过程
            f<<"("<<xs[i]<<","<<ys[i]<<")";
            if(i+1 == xs.size())f<<"->";
        }
    f.close();
}



