#include "main.hpp"
string saveIniFile = "C:\\WorkStation\\D盘\\05_研究生资料\\我的学习文档\\015_EDA方向代码练习\\"+to_string(TestNum)+"_ini.txt";  //配置文件
string resFile = "C:\\WorkStation\\D盘\\05_研究生资料\\我的学习文档\\015_EDA方向代码练习\\"+to_string(TestNum)+"_res.txt";  //结果文件
int main(){
    //打开文件读取生成次数数据
    ifstream numFile (timesFile);
    if(!numFile){   //文件打开失败，则新建文件
        ofstream numFile (timesFile);
        if(!numFile){
            cerr<<"打开Times文件:"<<timesFile<<"失败..."<<endl;
            exit(-1);
        }
        numFile<<to_string(TestNum);
        numFile.close();
    }else{          //读入文件，并更新文件
        numFile>>TestNum;
        numFile.close();
        TestNum++;
        cout<<"Hello World!\n";
        saveMatrixFile = "C:\\WorkStation\\D盘\\05_研究生资料\\我的学习文档\\015_EDA方向代码练习\\"+to_string(TestNum)+"_matrix.txt";//记录生成的矩阵
        saveIniFile = "C:\\WorkStation\\D盘\\05_研究生资料\\我的学习文档\\015_EDA方向代码练习\\"+to_string(TestNum)+"_ini.txt";  //配置文件
        resFile = "C:\\WorkStation\\D盘\\05_研究生资料\\我的学习文档\\015_EDA方向代码练习\\"+to_string(TestNum)+"_res.txt";  //结果文件
        MatrixMidRes1 = ".\\"+to_string(TestNum)+"_MatrixMidres1.txt";  //中间结果矩阵1
        MatrixMidRes2 = ".\\"+to_string(TestNum)+"_MatrixMidres2.txt";  //中间结果矩阵2
        cout<<"存储TestNum的文件路径:"<<timesFile<<endl;
        cout<<"存储生成的矩阵的文件路径:"<<saveMatrixFile<<endl;
        cout<<"存储配置文件的文件路径:"<<saveIniFile<<endl;
        cout<<"存储结果文件的文件路径:"<<resFile<<endl;
        cout<<"存储中间结果矩阵1的文件路径:"<<MatrixMidRes1<<endl;
        cout<<"存储中间结果矩阵2的文件路径:"<<MatrixMidRes2<<endl;
        ofstream numFile (timesFile);//打开文件并重新写入数据
        if(!numFile){
            cerr<<"打开Times文件:"<<timesFile<<"失败..."<<endl;
            exit(-1);
        }
        numFile<<to_string(TestNum);
        numFile.close();
    }
    //打开矩阵文件，并随时准备写入矩阵
    ofstream matrixFile (saveMatrixFile);
    ofstream resultFile(resFile);   //打开结果记录文件
    if(!matrixFile){
        cerr<<"打开矩阵文件:"<<saveMatrixFile<<"失败..."<<endl;
        exit(-1);
    }

    //1.坐标头结点
    DirecNode*head = new DirecNode;
    head->next = NULL;
    //2.构造测试矩阵 - 用下边自动生成的矩阵来测试
    // vector<vector<int>>arr = {
    //     {0,-1, 0, 0, 0, 0, 0, 0, 0, 0},
    //     {0,-1, 0, 0, 0, 0, 0, 0, 0, 0},
    //     {0,-1, 0, 0,-1,-1,-1,-1, 0, 0},
    //     {0,-1, 0, 0, 0, 0, 0, 0, 0, 0},
    //     {0, 0, 0,-1,-1,-1,-1, 0,-1, 0},
    //     {0, 0, 0, 0, 0, 0, 0, 0,-1, 0},
    //     {0, 0,-1,-1,-1,-1,-1, 0,-1, 0},
    //     {0, 0, 0, 0, 0, 0, 0, 0,-1, 0},
    //     {0,-1,-1,-1, 0, 0,-1,-1,-1,-1},
    //     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    // };

    //生成测试矩阵
    cout<<"请输入要生成的矩阵行、列数及障碍最大长度、障碍个数，使用空格分隔.\n";
    int row = 200;  //矩阵行数
    int col = 200;  //矩阵列数
    int obsLen = 25;//障碍最大长度
    int obsNum = 40;//障碍个数
    cin>>row>>col>>obsLen>>obsNum;
    vector<vector<int>>arr = *testVectorCreation(row,col,obsLen,obsNum,saveIniFile);
    vector<vector<int>>arr1 = arr;  //拷贝，因为arr会被改变
    // vector<vector<int>>arr = *cur_arr;
    for(int i = 0;i < row;i++){ //矩阵写入到文件
        for(int j = 0;j < col;j++)
            matrixFile<<(arr)[i][j]<<"\t";
        matrixFile<<endl;
    }
    matrixFile.close();
    // if(resultFile.is_open())
    //     cout<<"结果记录文件成功打开！\n";
    // else cout<<"结果记录文件打开失败！\n";
    
    //3.测试A-star算法
    int length = 0;

    //测试普通A*算法 - 0
    auto start_time1 = chrono::high_resolution_clock::now(); //A*算法开始时间
    bool isSuccessful = Astar(arr,row,col,0,0,row-1,col-1,head,length,0); //最后的0表示用哪种A*算法
    auto end_time1 = chrono::high_resolution_clock::now(); //A*算法结束时间
    auto duration = chrono::duration_cast<chrono::microseconds>(end_time1 - start_time1);//时间差-微秒
    cout<<"普通A*算法运行时间："<<duration.count()<<"微秒"<<endl;
    resultFile<<"普通A*算法运行时间："+to_string(duration.count())+"微秒"<<endl;
    if(isSuccessful){
        cout<<"不考虑代价的A*算法成功找到路径!\n";
        cout<<"路径长度为:"<<length<<endl;
        resultFile<<"路径长度为:"<<to_string(length)<<endl;
        cout<<"路径为："<<endl;
        resultFile<<"路径为："<<endl;
        DirecNode*ptr = head->next;
        while(ptr){
            cout<<"("<<ptr->x<<","<<ptr->y<<")";
            resultFile<<"("+to_string(ptr->x)+","+to_string(ptr->y)+")";
            if(ptr->next){
                cout<<"->";
                resultFile<<"->";
            }
            ptr = ptr->next;
        }
        cout<<endl;
        resultFile<<endl;
    }else{
        cout<<"未成功找到路径！\n";
        resultFile<<"未成功找到路径!\n";
    }
    int ComparedRunTime_Astar = duration.count();   //普通A*的运行时间 - 用于生成后续结果
    int ComparedLength_Astar = length;              //普通A*的路径长度 - 用于生成后续结果
    //清空head
    DirecNode*cur = head->next,*tmp =cur->next;
    while(cur){
        delete(cur);
        cur = tmp;
        if (tmp)tmp = tmp->next;
    }
    head->next = NULL;
    length = 0; //清空长度

    //测试考虑代价的A*算法 - 1
    auto start_time = chrono::high_resolution_clock::now(); //A*算法开始时间
    isSuccessful = Astar(arr1,row,col,0,0,row-1,col-1,head,length,1); //最后的0表示用哪种A*算法
    auto end_time = chrono::high_resolution_clock::now(); //A*算法结束时间
    duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);//时间差-微秒
    cout<<"考虑代价A*算法运行时间："<<duration.count()<<"微秒"<<endl;
    resultFile<<"考虑代价A*算法运行时间："+to_string(duration.count())+"微秒"<<endl;
    // clock_t start_time = clock();
    // bool isSuccessful = Astar(arr,10,10,0,0,9,9,head,length,1);
    // clock_t end_time = clock();
    // double duration = static_cast<double>(end_time-start_time);
    // cout<<"考虑代价的A*算法运行时间："<<duration<<"时刻"<<endl;
    // resultFile<<"考虑代价的A*算法运行时间："<<duration<<"时刻"<<endl;
    if(isSuccessful){
        cout<<"考虑代价的A*算法成功找到路径!\n";
        cout<<"路径长度为:"<<length<<endl;
        resultFile<<"路径长度为:"<<to_string(length)<<endl;
        cout<<"路径为："<<endl;
        resultFile<<"路径为："<<endl;
        DirecNode*ptr = head->next;
        while(ptr){
            cout<<"("<<ptr->x<<","<<ptr->y<<")";
            resultFile<<"("+to_string(ptr->x)+","+to_string(ptr->y)+")";
            if(ptr->next){
                cout<<"->";
                resultFile<<"->";
            }
            ptr = ptr->next;
        }
        cout<<endl;
        resultFile<<endl;
        resultFile<<"普通A*算法的运行时长与考虑代价的A*算法运行时长之比 = "<<((double)(ComparedRunTime_Astar)/(double)(duration.count()))<<endl;
        resultFile<<"普通A*算法路径长度与考虑代价的A*算法路径长度是否相等："<<(ComparedLength_Astar==length?"是":"否")<<endl;
    }else{
        cout<<"未成功找到路径！\n";
        resultFile<<"未成功找到路径!\n";
    }
    resultFile.close();
    return 0;
}