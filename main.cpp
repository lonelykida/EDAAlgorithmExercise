#include "main.hpp"
string saveIniFile = "C:\\WorkStation\\D��\\05_�о�������\\�ҵ�ѧϰ�ĵ�\\015_EDA���������ϰ\\"+to_string(TestNum)+"_ini.txt";  //�����ļ�
string resFile = "C:\\WorkStation\\D��\\05_�о�������\\�ҵ�ѧϰ�ĵ�\\015_EDA���������ϰ\\"+to_string(TestNum)+"_res.txt";  //����ļ�
int main(){
    //���ļ���ȡ���ɴ�������
    ifstream numFile (timesFile);
    if(!numFile){   //�ļ���ʧ�ܣ����½��ļ�
        ofstream numFile (timesFile);
        if(!numFile){
            cerr<<"��Times�ļ�:"<<timesFile<<"ʧ��..."<<endl;
            exit(-1);
        }
        numFile<<to_string(TestNum);
        numFile.close();
    }else{          //�����ļ����������ļ�
        numFile>>TestNum;
        numFile.close();
        TestNum++;
        saveMatrixFile = "C:\\WorkStation\\D��\\05_�о�������\\�ҵ�ѧϰ�ĵ�\\015_EDA���������ϰ\\"+to_string(TestNum)+"_matrix.txt";//��¼���ɵľ���
        saveIniFile = "C:\\WorkStation\\D��\\05_�о�������\\�ҵ�ѧϰ�ĵ�\\015_EDA���������ϰ\\"+to_string(TestNum)+"_ini.txt";  //�����ļ�
        resFile = "C:\\WorkStation\\D��\\05_�о�������\\�ҵ�ѧϰ�ĵ�\\015_EDA���������ϰ\\"+to_string(TestNum)+"_res.txt";  //����ļ�
        MatrixMidRes1 = ".\\"+to_string(TestNum)+"_MatrixMidres1.txt";  //�м�������
        MatrixMidRes2 = ".\\"+to_string(TestNum)+"_MatrixMidres2.txt";  //�м�������
        ofstream numFile (timesFile);//���ļ�������д������
        if(!numFile){
            cerr<<"��Times�ļ�:"<<timesFile<<"ʧ��..."<<endl;
            exit(-1);
        }
        numFile<<to_string(TestNum);
        numFile.close();
    }
    //�򿪾����ļ�������ʱ׼��д�����
    ofstream matrixFile (saveMatrixFile);
    ofstream resultFile(resFile);   //�򿪽����¼�ļ�
    if(!matrixFile){
        cerr<<"�򿪾����ļ�:"<<saveMatrixFile<<"ʧ��..."<<endl;
        exit(-1);
    }

    //1.����ͷ���
    DirecNode*head = new DirecNode;
    head->next = NULL;
    //2.������Ծ��� - ���±��Զ����ɵľ���������
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

    //���ɲ��Ծ���
    cout<<"������Ҫ���ɵľ����С��������ϰ���󳤶ȡ��ϰ�������ʹ�ÿո�ָ�.\n";
    int row = 200;  //��������
    int col = 200;  //��������
    int obsLen = 25;//�ϰ���󳤶�
    int obsNum = 40;//�ϰ�����
    cin>>row>>col>>obsLen>>obsNum;
    vector<vector<int>>arr = *testVectorCreation(row,col,obsLen,obsNum,saveIniFile);
    // vector<vector<int>>arr = *cur_arr;
    for(int i = 0;i < row;i++){ //����д�뵽�ļ�
        for(int j = 0;j < col;j++){
            matrixFile<<(arr)[i][j];
            if(arr[i][j]>=0)matrixFile<<"  ";
            else matrixFile<<" ";
        }
        matrixFile<<endl;
    }
    matrixFile.close();
    // if(resultFile.is_open())
    //     cout<<"�����¼�ļ��ɹ��򿪣�\n";
    // else cout<<"�����¼�ļ���ʧ�ܣ�\n";
    //3.����A-star�㷨
    int length = 0;
    //������ͨA*�㷨 - 0
    auto start_time1 = chrono::high_resolution_clock::now(); //A*�㷨��ʼʱ��
    bool isSuccessful = Astar(arr,row,col,0,0,row-1,col-1,head,length,0); //����0��ʾ������A*�㷨
    auto end_time1 = chrono::high_resolution_clock::now(); //A*�㷨����ʱ��
    auto duration = chrono::duration_cast<chrono::microseconds>(end_time1 - start_time1);//ʱ���-΢��
    cout<<"��ͨA*�㷨����ʱ�䣺"<<duration.count()<<"΢��"<<endl;
    resultFile<<"��ͨA*�㷨����ʱ�䣺"+to_string(duration.count())+"΢��"<<endl;
    if(isSuccessful){
        cout<<"�����Ǵ��۵�A*�㷨�ɹ��ҵ�·��!\n";
        cout<<"·������Ϊ:"<<length<<endl;
        resultFile<<"·������Ϊ:"<<to_string(length)<<endl;
        cout<<"·��Ϊ��"<<endl;
        resultFile<<"·��Ϊ��"<<endl;
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
        cout<<"δ�ɹ��ҵ�·����\n";
        resultFile<<"δ�ɹ��ҵ�·��!\n";
    }
    //���head
    DirecNode*cur = head->next,*tmp =cur->next;
    while(cur){
        delete(cur);
        cur = tmp;
        if (tmp)tmp = tmp->next;
    }
    head->next = NULL;
    length = 0; //��ճ���
    //���Կ��Ǵ��۵�A*�㷨 - 1
    auto start_time = chrono::high_resolution_clock::now(); //A*�㷨��ʼʱ��
    isSuccessful = Astar(arr,row,col,0,0,row-1,col-1,head,length,1); //����0��ʾ������A*�㷨
    auto end_time = chrono::high_resolution_clock::now(); //A*�㷨����ʱ��
    duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);//ʱ���-΢��
    cout<<"���Ǵ���A*�㷨����ʱ�䣺"<<duration.count()<<"΢��"<<endl;
    resultFile<<"���Ǵ���A*�㷨����ʱ�䣺"+to_string(duration.count())+"΢��"<<endl;
    // clock_t start_time = clock();
    // bool isSuccessful = Astar(arr,10,10,0,0,9,9,head,length,1);
    // clock_t end_time = clock();
    // double duration = static_cast<double>(end_time-start_time);
    // cout<<"���Ǵ��۵�A*�㷨����ʱ�䣺"<<duration<<"ʱ��"<<endl;
    // resultFile<<"���Ǵ��۵�A*�㷨����ʱ�䣺"<<duration<<"ʱ��"<<endl;
    if(isSuccessful){
        cout<<"���Ǵ��۵�A*�㷨�ɹ��ҵ�·��!\n";
        cout<<"·������Ϊ:"<<length<<endl;
        resultFile<<"·������Ϊ:"<<to_string(length)<<endl;
        cout<<"·��Ϊ��"<<endl;
        resultFile<<"·��Ϊ��"<<endl;
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
        cout<<"δ�ɹ��ҵ�·����\n";
        resultFile<<"δ�ɹ��ҵ�·��!\n";
    }
    resultFile.close();
    return 0;
}