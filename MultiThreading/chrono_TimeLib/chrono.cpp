#include <chrono>   //��Ҫ���ǽ���߳̿�����
#include <iostream>
#include <thread>


using namespace std;
using namespace std::chrono;

int main(){
    // chrono::seconds five_seconds = chrono::seconds(5);  //5���ӵ�ʱ��
    auto five_seconds = 5s; //������auto�Զ������Ƶ�
    chrono::seconds six = chrono::seconds(6);
    chrono::duration<double,ratio<5/1>> d = chrono::duration<double,ratio<5/1>>(1);
    cout<<"hello world"<<endl;
    this_thread::sleep_for(d);   //˯��5����ٽ�������
    auto time_point = chrono::system_clock::now();  //ȡ��ǰʱ���
    cout<<(time_point.time_since_epoch()).count()<<endl;
    
    return 0;

}