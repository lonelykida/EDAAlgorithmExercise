#include <chrono>   //主要还是结合线程库来用
#include <iostream>
#include <thread>


using namespace std;
using namespace std::chrono;

int main(){
    // chrono::seconds five_seconds = chrono::seconds(5);  //5秒钟的时钟
    auto five_seconds = 5s; //可以用auto自动类型推导
    chrono::seconds six = chrono::seconds(6);
    chrono::duration<double,ratio<5/1>> d = chrono::duration<double,ratio<5/1>>(1);
    cout<<"hello world"<<endl;
    this_thread::sleep_for(d);   //睡眠5秒后再结束运行
    auto time_point = chrono::system_clock::now();  //取当前时间点
    cout<<(time_point.time_since_epoch()).count()<<endl;
    
    return 0;

}