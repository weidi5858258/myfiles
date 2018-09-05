//
// Created by root on 18-9-3.
//
#ifdef WIN32
#include "HandleAndroidString.h"
#endif
#ifdef linux

#include "../include/HandleAndroidString.h"

#endif

/***
 1.
 把values/strings.xml中的name和content保存到变量中
 2.
 把src/res下的有关文件保存到变量中
 3.

 */

/***
<string name="cancel_edit_permission" sony_id="4">Cancel</string>
<string name="mv_toast_unsupported_operation_txt" sony_id="40112" xliff:restype="x-GENERIC/TOAST/TEXT">
 Feature not available with this content
 </string>
*/
void split(vector<string> &vecStr, const string &srcStr, const string &delim) {
    if (!vecStr.empty()) {
        vecStr.clear();
    }
    string tempSrcStr(srcStr);
    while (1) {
        int index = tempSrcStr.find(delim);
        if (index == -1) {
            return;
        }
        int delimLen = delim.length();
        vecStr.push_back(srcStr.substr(0, index));
        string tempStr = srcStr.substr(index + delimLen);

    }

}

void HandleAndroidString::doSomething() {
    init();

    string test("0123abc4567abc879");
    const char *c_str = test.c_str();
    cout << c_str << endl;
    int index = test.find("abc");
    cout << index << endl;
    cout << test.substr(0, index) << endl;// 0 ~ index-1
    cout << test.substr(index + 3) << endl;// index ~

    cout << test << endl;
    test.clear();
    cout << test << endl;
}

void HandleAndroidString::init() {
    srcDir = string(CUR_DIR) + "/src/res";
    destDir = string(CUR_DIR) + "dest/res";
    cacheDir = string(CUR_DIR) + "/cache";

    int result = access(cacheDir.c_str(), F_OK);
    if (result == -1) {
        fprintf(stdout, "return:%d %s目录不存在\n", result, cacheDir.c_str());
    } else {
        fprintf(stdout, "return:%d %s目录存在\n", result, cacheDir.c_str());
    }

//    fstream file(srcDir + "/values/strings.xml");
//    string alineString;
//    while (getline(file, alineString)) {
//        if (!alineString.empty()
//            && alineString.find("sony_id=") != string::npos
//            && alineString.find("sony_id=\"M") == string::npos) {
//            cout << alineString << endl;
//        }
//    }

//    FILE *cacheFile = fopen(cacheDir.c_str(), "w");
//    if (cacheFile == NULL) {
//        printf("%s", "can not open the file");
//    } else {
//        fclose(cacheFile);
//    }

//    char *fileName = "/root/temp_dir/a.txt", *tag;
//    for (tag = fileName; *tag; tag++) {
//        if (*tag == '\\') {
//            char buf[1000], path[1000];
//            strcpy(buf, fileName);
//            buf[strlen(fileName) - strlen(tag) + 1] = NULL;
//            strcpy(path, buf);
//            if (access(path, 6) == -1) {
//                mkdir(path, 0);
//            }
//        }
//    }

//    string dir = "./hello";
//    if (access(dir.c_str(), 0) == -1) {
//        cout << dir << " is not existing" << endl;
//        cout << "now make it" << endl;
//#ifdef WIN32
//        int flag=mkdir(dir.c_str());
//#endif
//#ifdef linux
//        int flag = mkdir(dir.c_str(), 0777);
//#endif
//        if (flag == 0) {
//            cout << "make successfully" << endl;
//        } else {
//            cout << "make errorly" << endl;
//        }
//    }
//
//    if (access(dir.c_str(), 0) == 0) {
//        cout << dir << " exists" << endl;
//        cout << "now delete it" << endl;
//        int flag = rmdir(dir.c_str());
//        if (flag == 0) {
//            cout << "delete it successfully" << endl;
//        } else {
//            cout << "delete it errorly" << endl;
//        }
//    }
//
//    //cout<<"Hello World"<<endl;
//
//    cout << "end..." << endl;
//    cin.get();



}
