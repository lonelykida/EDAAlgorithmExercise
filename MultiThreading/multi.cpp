#include <iostream>
#include <thread>

using namespace std;

void ThreadMain(){
    for(int i = 0;i < 10;i++){
        cout<<"thread main:"<<this_thread::get_id()<<endl;
        this_thread::sleep_for(1s);
    }
}

int main(int argc,char*argv[]){
    //��ȡ���������߳�ID���������ռ�this_thread���и�get_id()����
    cout<<"main thread id:"<<this_thread::get_id()<<endl;
    thread th(ThreadMain);  //�̴߳�������
    thread th2(ThreadMain);
    th.join();  //�����ȴ����߳��˳�
    th2.join();
    return 0;
}