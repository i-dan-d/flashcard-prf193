#include <iostream>
#include <cstring>
using namespace std;

int main(){
    char input[500];
    cout << "Enter vocab or phrase: ";
    cin >> ws;
    cin.getline(input, sizeof(input));
    char* token = strtok(input, " ");
    char* term[3];
    while (token != NULL){
        cout << token<<' '<<*token<<' '<<&token <<endl;
        token = strtok(NULL, " ");
    }
    int cout=0;

    return 0;
}