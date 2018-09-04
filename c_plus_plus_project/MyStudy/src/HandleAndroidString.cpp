//
// Created by root on 18-9-3.
//
#ifdef WIN32
#include "HandleAndroidString.h"
#endif
#ifdef linux

#include "../include/HandleAndroidString.h"

#endif

void HandleAndroidString::init() {
    curDir = "/root/下载/translation_list";
    srcDir = curDir + "/src/res";
    destDir = curDir + "dest/res";
    cacheDir = curDir + "/cache";

    int result = access(cacheDir.c_str(), F_OK);
    if (result == -1) {
        fprintf(stdout, "return:%d %s目录不存在\n", result, cacheDir.c_str());
    } else {
        fprintf(stdout, "return:%d %s目录存在\n", result, cacheDir.c_str());
    }

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
