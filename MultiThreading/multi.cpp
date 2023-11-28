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
    //获取主函数的线程ID，在命名空间this_thread中有个get_id()函数
    cout<<"main thread id:"<<this_thread::get_id()<<endl;
    thread th(ThreadMain);  //线程创建启动
    thread th2(ThreadMain);
    th.join();  //阻塞等待子线程退出
    th2.join();
    return 0;
}