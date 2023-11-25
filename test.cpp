#include <iostream>
#include <vector>

using namespace std;

void test(vector<int> arr1,vector<int>&arr2){
    cout<<"test函数中arr1的地址为:"<<&arr1<<endl;
    cout<<"test函数中arr2的地址为:"<<&arr2<<endl;
}

int main(){
    vector<int> arr;
    cout<<"主函数中arr的地址为:"<<&arr<<endl;
    test(arr,arr);
    return 0;
}