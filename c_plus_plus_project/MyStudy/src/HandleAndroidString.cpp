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

//    string test("0123abc4567abc879");
//    const char *c_str = test.c_str();
//    cout << c_str << endl;
//    int index = test.find("abc");
//    cout << index << endl;
//    cout << test.substr(0, index) << endl;// 0 ~ index-1
//    cout << test.substr(index + 3) << endl;// index ~
//
//    cout << test << endl;
//    test.clear();
//    cout << test << endl;
}

void HandleAndroidString::recurseDir(const char *curDir) {
    if (NULL == curDir) {
        cout << "curDir is NULL" << endl;
        return;
    }

    struct stat s;
    lstat(curDir, &s);
    if (!S_ISDIR(s.st_mode)) {
        cout << "curDir is not a valid directory" << endl;
        return;
    }

    struct dirent *fileName = NULL;    // return value for readdir()
    DIR *dir = NULL;                   // return value for opendir()
    dir = opendir(curDir);
    if (NULL == dir) {
        cout << "Can not open dir " << curDir << endl;
        return;
    }

    string tempDir;
    while ((fileName = readdir(dir)) != NULL) {
        // get rid of "." and ".."
        tempDir = fileName->d_name;
        if (strcmp(fileName->d_name, ".") == 0
            || strcmp(fileName->d_name, "..") == 0
            || tempDir.find("dp") != string::npos
            || tempDir.find("land") != string::npos
            || tempDir.find("values-") == string::npos
            || isContainNum(tempDir) == 0) {
            continue;
        }

        cout << tempDir << endl;
        if (tempDir.length() == 13) {
            tempDir = tempDir.substr(0, 9);
        }
        //不包含
        if (countryMap.find(tempDir) == countryMap.end()) {
            vector<string> countryVector;
            countryVector.push_back(fileName->d_name);
            countryMap.insert(map<string, vector<string>>::value_type(tempDir, countryVector));
        } else {
//            vector<string> countryVector = countryMap[tempDir];
//            countryVector.push_back(fileName->d_name);
//            countryMap.insert(map<string, vector<string>>::value_type(tempDir, countryVector));

            map<string, vector<string>>::iterator iter;
            for (iter = countryMap.begin(); iter != countryMap.end(); ++iter) {
                if (tempDir.compare(0, tempDir.length(), fileName->d_name, 9) == 0) {
//                    vector<string> countryVector = iter->second;
                    vector<string> countryVector = countryMap[tempDir];
                    cout << &countryVector << endl;
                    countryVector.push_back(fileName->d_name);
                    break;
                }
            }
        }
//        string tempFileName(curDir);
//        tempFileName = tempFileName + "/" + fileName->d_name;
//        cout << tempFileName << endl;
    }
}

int HandleAndroidString::isContainNum(const string &srcStr) {
    if (srcStr.empty()) {
        return 1;
    }
    int length = srcStr.length();
    for (int i = 0; i < length; i++) {
        if ((srcStr[i] >= 48 && srcStr[i] <= 57)) {
            return 0;
        }
    }
    return 1;
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

    fstream file(srcDir + "/values/strings.xml");
    string alineString;
    while (getline(file, alineString)) {
        if (!alineString.empty()
            && alineString.find("sony_id=") != string::npos
            && alineString.find("sony_id=\"M") == string::npos) {
            int index1 = alineString.find("\"");
            if (index1 != string::npos) {
                int index2 = alineString.find("\"", index1 + 1);
                if (index2 != string::npos) {
                    string nameStr = alineString.substr(index1 + 1, index2 - index1 - 1);
                    index1 = alineString.find(">");
                    if (index1 != string::npos) {
                        index2 = alineString.find("</", index1 + 1);
                        if (index2 != string::npos) {
                            string contentStr = alineString.substr(index1 + 1, index2 - index1 - 1);
                            nameAndContentMap.insert(map<string, string>::value_type(nameStr, contentStr));
                        }
                    }
                }
            }
        }
    }

//    map<string, string>::iterator iter;
//    for (iter = nameAndContentMap.begin(); iter != nameAndContentMap.end(); iter++) {
//        cout << iter->first << " = " << iter->second << endl;
//    }

    recurseDir(srcDir.c_str());
    cout << "----------------------------------------" << endl;
    map<string, vector<string>>::iterator iter;
    for (iter = countryMap.begin(); iter != countryMap.end(); ++iter) {
        cout << iter->first << endl;
        vector<string> countryVector = iter->second;
        cout << &countryVector << endl;
        for (vector<string>::iterator it = countryVector.begin(); it != countryVector.end(); ++it) {
            cout << *it << "\t";
        }
    }
    cout << endl;

    string temp = "values-vi";
    char *temp2 = "values-vi-rHE";
    if (temp.compare(0, temp.length(), temp2, 9) == 0) {
        cout << "adfasdf" << endl;
    }

//    vector<string> countryVector;
//    countryVector.push_back("hello");
//    countryMap.insert(map<string, vector<string>>::value_type("test", countryVector));
//
//    map<string, vector<string>>::iterator iter;
//    for (iter = countryMap.begin(); iter != countryMap.end(); iter++) {
//        cout << iter->first << endl;
//        vector<string> countryVector = iter->second;
//        cout << &countryVector << endl;
//        for (vector<string>::iterator it = countryVector.begin(); it != countryVector.end(); ++it) {
//            cout << *it << "\t";
//        }
//    }
//    cout << endl;
//
//    for (iter = countryMap.begin(); iter != countryMap.end(); iter++) {
//        cout << iter->first << endl;
//        vector<string> countryVector = iter->second;
//        countryVector.push_back("world");
//        cout << &countryVector << endl;
//        for (vector<string>::iterator it = countryVector.begin(); it != countryVector.end(); ++it) {
//            cout << *it << "\t";
//        }
//    }
//    cout << endl;






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
