#include <iostream>
#include <sstream>
#include <set>
#include "avl.h"
#include <vector>
#include <regex>

using namespace std;

// FROM ANDREW PENTON'S SLIDES
string verifyName(vector<string>& vect) {
    string word = "";
    for (int i = 1; i < vect.size() - 1; i++) {
        word += vect[i];
    }
    word = word.substr(1, word.length() - 2);

    for (char c : word) {
        bool valid = (bool)isalpha(c) || (bool)isspace(c);
        if (!valid) {
            return "";
        }
    }

    return word;
}

bool isNumeric(const string& str) {
    regex numericPattern("^[0-9]+$");
    return regex_match(str, numericPattern);
}

string verifyID(string id) {
    if (id.length() != 8) {
        return "";
    }
    if (!isNumeric(id)) {
        return "";
    }
    return id;
}

int main()
{       
    int n;
    cin >> n;

    string command;
    string name;
    string id;

    vector<string> s_input;
    AVL_Tree avl;
    cin >> ws;

    for (int i = 0; i < n; i++) {
        // take in each line and enter inputs into an array, clearing after each loop
        s_input.clear();
        string line;
        getline(cin, line);
        istringstream in(line);
        string word;
        while (in >> word) {
            s_input.push_back(word);
        }

        // get the first input from line, the command
        command = s_input[0];
        
        if (command == "insert") {
            // check that the name and id fit correct convention
            name = verifyName(s_input);
            id = verifyID(s_input[s_input.size()-1]);

            if (name == "" || id == "") {
                cout << "unsuccessful" << endl;
                continue;
            }
            if (avl.IDverify(id)) {
                avl.insert(name, id);
                cout << "successful" << endl;
            }
            else {
                cout << "unsuccessful" << endl;
            }
        }
        else if (command == "remove") {
            id = s_input[1];
            avl.remove(id);
        }

        // traversals
        else if (command == "printInorder") {
            avl.inorder();
        }
        else if (command == "printPreorder") {
            avl.preorder();
        }
        else if (command == "printPostorder") {
            avl.postorder();
        }
        else if (command == "removeInorder") {
            int m = stoi(s_input[1]);
            avl.rmInorder(m);
        }

        else if (command == "search") {
            // check if the search is given a name or id(numeric)
            if (isNumeric(s_input[1])) {
                avl.searchID(s_input[1]);
            }
            else {
                s_input.push_back("filler");
                name = verifyName(s_input);
                avl.searchName(name);
            }
        }
        else if (command == "printLevelCount") {
            avl.printLevelCount();
        }
    }
}

