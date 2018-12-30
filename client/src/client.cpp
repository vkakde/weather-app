#include<iostream>
#include<string>

using namespace std;

int main(){

    string input = "";

    while(input!="q"){
        cout<<"\nEnter city name (\'q\' to quit): ";
        cin>>input;
    }

    return 0;
}