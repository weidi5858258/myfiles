//
// Created by root on 18-9-3.
//

#ifndef MYSTUDY_HANDLEANDROIDSTRING_H
#define MYSTUDY_HANDLEANDROIDSTRING_H

#include "MyHead.h"

class HandleAndroidString {
public:
    void init();
private:
    string curDir;
    string srcDir;
    string destDir;
    string cacheDir;
    map<string, string> englishStringMap;


};


#endif //MYSTUDY_HANDLEANDROIDSTRING_H
