#ifndef MYSTUDY_HANDLEANDROIDSTRING_H
#define MYSTUDY_HANDLEANDROIDSTRING_H

#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <string>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <algorithm>
#include <iterator>
#include <vector>
#include <map>

using namespace std;

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
    map<string, string> nameAndContentMap;
    map<string, vector<string>> countryMap;
};


#endif //MYSTUDY_HANDLEANDROIDSTRING_H
