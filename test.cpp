#include <iostream>
#include <vector>

using namespace std;

void test(vector<int> arr1,vector<int>&arr2){
    cout<<"test������arr1�ĵ�ַΪ:"<<&arr1<<endl;
    cout<<"test������arr2�ĵ�ַΪ:"<<&arr2<<endl;
}

int main(){
    vector<int> arr;
    cout<<"��������arr�ĵ�ַΪ:"<<&arr<<endl;
    test(arr,arr);
    return 0;
}