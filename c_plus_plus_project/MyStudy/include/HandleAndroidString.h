//
// Created by root on 18-9-3.
//

#ifndef MYSTUDY_HANDLEANDROIDSTRING_H
#define MYSTUDY_HANDLEANDROIDSTRING_H

#include "MyHead.h"

//#define CUR_DIR "/media/root/Weidi/Users/weidi5858258/Documents/temp_dir"
//#define CUR_DIR "/root/temp_dir/merge_string"
#define CUR_DIR "C:\\Users\\weidi5858258\\Documents\\temp_dir"

class HandleAndroidString {
public:
    void doSomething();

private:
    void init();

    void recurseDir(const char *curDir);

    int isContainNum(const string &srcStr);

    void handleCountryMap();

    void handleOneCountry(const string &);

    void handleMoreCountry(vector<string> &);

    void handleMerge(const string &, const string &);

    void handleCopy(const string &, const string &);

    void handleSameContent(vector<string> &);

    void handleDiffContent(vector<string> &);

    void getNameAndContent(const string &content,
            string &nameStr,
            string &contentStr);

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
