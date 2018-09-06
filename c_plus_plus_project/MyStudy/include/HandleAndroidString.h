//
// Created by root on 18-9-3.
//

#ifndef MYSTUDY_HANDLEANDROIDSTRING_H
#define MYSTUDY_HANDLEANDROIDSTRING_H

#include "MyHead.h"

#define CUR_DIR "/media/root/Weidi/Users/weidi5858258/Documents/temp_dir"

class HandleAndroidString {
public:
    void doSomething();

private:
    void init();

    void recurseDir(const char *curDir);

    int isContainNum(const string &srcStr);

private:
    string srcDir;
    string destDir;
    string cacheDir;
    string srcStringFilePath;
    string destStringFilePath;
    map<string, string> nameAndContentMap;
    map<string, vector<string>> countryMap;


};


#endif //MYSTUDY_HANDLEANDROIDSTRING_H
