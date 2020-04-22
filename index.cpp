#include <cstdio>
#include <cstring>
#include <string>
#include <iostream>
#include <dirent.h>
#include <errno.h>
using namespace std;

string path;
DIR* dirp;
struct dirent* dp;

string whatever = "草";
int zhCharLen = whatever.size();

void renameFile(string originalName, string modifyName) {
    if(rename((path + originalName).c_str(), (path + modifyName).c_str()) == -1) {
        cout << "Operation failed for directory " << dp -> d_name << endl;
        cout << "Error: " << strerror(errno) << endl;
    }
}

void getDirectory(string path) {
    if((dirp = opendir(path.c_str())) == NULL) {
       cout << "open directory " + path + " error" << endl;
       exit(1);
    }
}

int calcRealCount(string s, int from, int count) {
    int realCount = 0;
    while(count--) {
        if(s[from + realCount] < 0) {
            realCount += zhCharLen;
        } else {
            realCount++;
        }
    }
    return realCount;
}

void deleteString(int from, int count) {
    string fileName;
    while((dp = readdir(dirp)) != NULL){
        fileName = dp -> d_name;
        if(fileName[0] != '.') {
            int realFrom = calcRealCount(fileName, 0, from), realCount = calcRealCount(fileName, realFrom, count);
            fileName = fileName.substr(0, realFrom) + fileName.substr(realFrom + realCount);
            renameFile(dp -> d_name, fileName);
        }
    }
}

void insertString(int from, string insertStr) {
    string fileName;
    while((dp = readdir(dirp)) != NULL) {
        fileName = dp -> d_name;
        if(fileName[0] != '.') {
            int realFrom = calcRealCount(fileName, 0, from);
            fileName = fileName.substr(0, realFrom) + insertStr + fileName.substr(realFrom);
            renameFile(dp -> d_name, fileName);
        }
    }
}

void replaceString(string insertString, string replaced) {
    string fileName;
    while((dp = readdir(dirp)) != NULL) {
        fileName = dp -> d_name;
        if(fileName[0] != '.') {
            int replacePos = fileName.find(replaced);
            if(replacePos == string::npos) {
                cout << "Sorry, there is no " << replaced << " in directory " << fileName << endl;
            } else {
                fileName = fileName.replace(replacePos, replaced.size(), insertString);
                renameFile(dp -> d_name, fileName);
            }
        }
    }
}

int main() {
    // get directory name
    cout << "Please input your directory's path and press Enter to end your input: " << endl;
    // get directory&files' name in directory above
    cin >> path;
    getDirectory(path);
    // get user's command
    cout << "DELETE/INSERT/REPLACE are operations this program support currently." << endl;
    cout << "Please input the operation you want to run: " << endl;
    string operation;
    cin >> operation;

    if(operation == "DELETE") {
        int deleteCount, offset;
        cout << "Please input the number of character you want to delete: " << endl;
        cin >> deleteCount;
        cout << "You want to delete them from x-th character, and x is: " << endl;
        cin >> offset;
        deleteString(offset - 1, deleteCount);
    } else if(operation == "INSERT") {
        int insertPos;
        string insertStr;
        cout << "Please input the position you want to insert from: " << endl;
        cin >> insertPos;
        cout << "Input the string you want to insert: " << endl;
        cin >> insertStr;
        insertString(insertPos - 1, insertStr);
    } else if(operation == "REPLACE") {
        string insertStr, replaceStr;
        cout << "Please input the string you want to insert: " << endl;
        cin >> insertStr;
        cout << "Please input the string you want to replace: " << endl;
        cin >> replaceStr;
        replaceString(insertStr, replaceStr);
    } else {
        cout << "Sorry, '" << operation << "' is invalid command." << endl;
    }

    return 0;
}