//
// Created by root on 18-9-3.
//

#ifndef MYSTUDY_HANDLEANDROIDSTRING_H
#define MYSTUDY_HANDLEANDROIDSTRING_H

#include "MyHead.h"

//#define TEMP_CACHE "/root/temp_dir/merge_string"
//#define TEMP_CACHE "/media/root/Weidi/Users/weidi5858258/Documents/temp_dir"
//#define TEMP_CACHE "C:\\Users\\weidi5858258\\Documents\\temp_dir"
//#define TEMP_CACHE "/mnt/c/Users/weidi5858258/Documents/temp_dir"

class HandleAndroidString {
public:
    void doSomething(string TEMP_CACHE);

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

    void handleMergeAddContent(vector<string> &,
                               vector<string> &,
                               const string &);

    void handleMergeModifyContent(vector<string> &,
                                  const string &,
                                  const string &,
                                  const string &);

private:
    string TEMP_CACHE;
    string srcDir;
    string destDir;
    string cacheDir;
    map<string, string> nameAndContentMap;
    map<string, vector<string>> countryMap;
};


#endif //MYSTUDY_HANDLEANDROIDSTRING_H
