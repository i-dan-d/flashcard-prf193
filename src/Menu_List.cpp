/*các chức năng chính toi sẽ code ở đây:
1. kiểm tra danh sách từ vựng có rỗng không bằng deck.empty().
2. cho người dùng chọn cách sắp xếp: 
- theo bảng chữ cái
- theo só lần ôn tập
3. Gọi listr.sortedBy() để lấy danh sách sắp xếp.
4. Duyệt danh sách và in từng bảng từ bằng WordLister::format(*word)*/

#include <algorithm>
#include "../include/Menu.h"
#include "../include/Word.h"
using namespace std;

void Menu::handleList() {
    if(deck.empty()){
        cout <<"Danh sach tu vung dang rong.\n";
        return;
    }

    cout <<"\n===== LIST ALL WORDS =====\n";
    cout <<"1. Sort by alphabet.\n";
    cout <<"2. Sort by review count\n";
    cout << "Choose: ";
    
    int choice;
    cin >> choice;

    WordLister::SortKey key;

    if(choice == 1){
        key == WordLister::SortKey::Alphabet;
    }
    else if(choice ==2) {
        key == WordLister::SortKey::ReviewCount;
    }
    else{
        cout << "Lua chon khong hop le.\n";
        return;
    }

    vector<const Word*> words = lister.sortedBy(key);

    cout <<"\n===== WORD LIST =====\n";

    for(const Word* word : words) {
        cout << word -> getId() << ". "
             << WordLister::format(*word);
        if(key == WordLister::SortKey::ReviewCount) {
            cout << " | Review count: "
                 << word -> getReviewCount();
        }   
        cout << '\n';
    }
}