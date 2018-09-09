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

    handleCountryMap();

//    map<string, string> localNameAndContentMap;
//    localNameAndContentMap.insert(map<string, string>::value_type("test", "hello"));
//    localNameAndContentMap.insert(map<string, string>::value_type("test2", "hello2"));
//    localNameAndContentMap.insert(map<string, string>::value_type("test3", "hello3"));
//    161022[64]
//    string value = localNameAndContentMap["test2"];
//    cout << value << endl;

    vector<string> testVector;
    testVector.push_back("test1");
    testVector.push_back("test2");
    testVector.push_back("test3");
    testVector.push_back("test4");
    testVector.push_back("test3");
    for (auto tempStr : testVector) {
        cout << tempStr << endl;
    }

    //根据元素查找索引
    std::vector<string>::iterator iter = std::find(std::begin(testVector),
                                                   std::end(testVector),
                                                   "test4");
    auto index = std::distance(std::begin(testVector), iter);
    cout << "index: " << index << endl;

    //根据索引修改元素
    testVector[index] = "test10";
    for (auto tempStr : testVector) {
        cout << tempStr << endl;
    }

    //根据索引删除元素
    testVector.erase(testVector.begin() + index);
    for (auto tempStr : testVector) {
        cout << tempStr << endl;
    }
    //根据索引插入元素(索引值不要超过vector的元素个数)
    testVector.insert(testVector.begin() + index, "test8");
    for (auto tempStr : testVector) {
        cout << tempStr << endl;
    }
}

void HandleAndroidString::recurseDir(const char *curDir) {
    if (NULL == curDir) {
        cout << "curDir is NULL" << endl;
        return;
    }

#ifdef linux
    struct stat s;
    lstat(curDir, &s);
    if (!S_ISDIR(s.st_mode)) {
        cout << "curDir is not a valid directory" << endl;
        return;
    }
#endif

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

//        cout << "正在处理: " << tempDir << endl;
        if (tempDir.length() == 13) {
            tempDir = tempDir.substr(0, 9);
        }
        //不包含
        if (countryMap.find(tempDir) == countryMap.end()) {
//            cout << "不包含: " << fileName->d_name << endl;
            vector<string> countryVector;
            countryVector.push_back(fileName->d_name);
            countryMap.insert(
                    map<string, vector<string>>::value_type(
                            tempDir, countryVector));
        } else {
//            cout << "包含: " << fileName->d_name << endl;
            map<string, vector<string>>::iterator iter;
            for (iter = countryMap.begin(); iter != countryMap.end(); ++iter) {
                if (tempDir.compare(iter->first) == 0) {
                    iter->second.push_back(fileName->d_name);
//                    cout << iter->first << endl;
//                    cout << &iter->second << endl;
//                    for (vector<string>::iterator it = iter->second.begin(); it != iter->second.end(); ++it) {
//                        cout << *it << "\t";
//                    }
                    break;
                }
            }
//            cout << endl;
        }
    }
}

/***
 * 字符串中只要有一个是数字,那么就返回0,其他情况返回1
 * @param srcStr
 * @return
 */
int HandleAndroidString::isContainNum(const string &srcStr) {
    if (srcStr.empty()) {
        return 1;
    }
    for (auto ch : srcStr) {
        if (ch >= 48 && ch <= 57) {
            return 0;
        }
    }
    return 1;
}

void HandleAndroidString::handleMerge(const string &srcFilePath,
                                      const string &destFilePath) {
    cout << "handleMerge()" << endl;
    cout << "srcFilePath: " << srcFilePath << endl;
    cout << "destFilePath: " << destFilePath << endl;
    vector<string> localSrcFileContentVector;
    vector<string> localDestFileContentVector;
    map<string, string> localDestFileNameContentMap;
    fstream file;
    file.open(srcFilePath);
    string alineString;
    string nameStr;
    string contentStr;
    while (getline(file, alineString)) {
        localSrcFileContentVector.push_back(alineString);
    }
    file.close();

    file.open(destFilePath);
    while (getline(file, alineString)) {
        localDestFileContentVector.push_back(alineString);
        nameStr.clear();
        contentStr.clear();
        getNameAndContent(alineString, nameStr, contentStr);
        if (!nameStr.empty() && !contentStr.empty()) {
            localDestFileNameContentMap.insert(
                    map<string, string>::value_type(nameStr, contentStr));
        }
    }
    file.close();

    /*for (auto str : localDestFileContentVector) {
        cout << str << endl;
    }
    if (true)return;*/

    file.open(srcFilePath);
    while (getline(file, alineString)) {
        nameStr.clear();
        contentStr.clear();
        getNameAndContent(alineString, nameStr, contentStr);
        if (!nameStr.empty() && !contentStr.empty()) {
            if (nameAndContentMap.find(nameStr) != nameAndContentMap.end()) {
                string valuesContentStr = nameAndContentMap[nameStr];
                //内容已经翻译过了
                if (contentStr.compare(valuesContentStr) != 0) {
                    if (localDestFileNameContentMap.find(nameStr)
                        == localDestFileNameContentMap.end()) {
                        //add
                        cout << "handleMerge() add "
                             << nameStr << ":" << valuesContentStr << endl;
                        handleMergeAddContent(localSrcFileContentVector,
                                              localDestFileContentVector,
                                              alineString);
                    } else {
                        //modify
                        cout << "handleMerge() modify "
                             << nameStr << ":" << valuesContentStr << endl;
                        handleMergeModifyContent(localDestFileContentVector,
                                                 alineString,
                                                 nameStr,
                                                 contentStr);
                    }
                }
            }
        }
    }
    file.close();

//    for (auto destContent : localDestFileContentVector) {
//        cout << destContent << endl;
//    }

    //把localDestFileContentVector内容写到文件中就行了
    FILE *destFile = fopen(destFilePath.c_str(), "w");
    for (auto destContent : localDestFileContentVector) {
        if (destContent.find("</resources>") == string::npos) {
            fputs((destContent + "\n").c_str(), destFile);
        } else {
            fputs((destContent).c_str(), destFile);
        }
    }
    fclose(destFile);
}

void HandleAndroidString::handleCopy(const string &srcFilePath,
                                     const string &destFilePath) {
    cout << "handleCopy()" << endl;
    cout << "srcFilePath: " << srcFilePath << endl;
    cout << "destFilePath: " << destFilePath << endl;
}

void HandleAndroidString::handleOneCountry(const string &country) {
    cout << "handleOneCountry()" << endl;
    //values-af-rZA
    cout << country << endl;
    string strFilePathSrc = srcDir + "/" + country + "/strings.xml";
    string strFilePathDest = destDir + "/" + country + "/strings.xml";
    int result = access(strFilePathDest.c_str(), F_OK);
    if (country.length() > 9) {
        if (result == -1) {
            fprintf(stdout, "%s文件不存在\n", strFilePathDest.c_str());
            strFilePathDest =
                    destDir + "/" + country.substr(0, 9) + "/strings.xml";
            result = access(strFilePathDest.c_str(), F_OK);
            if (result == -1) {
                fprintf(stdout, "%s文件不存在\n", strFilePathDest.c_str());
                handleCopy(strFilePathSrc, strFilePathDest);
            } else {
                fprintf(stdout,
                        "return:%d %s文件存在\n",
                        result, strFilePathDest.c_str());
                handleMerge(strFilePathSrc, strFilePathDest);
            }
        } else {
            fprintf(stdout,
                    "return:%d %s文件存在\n",
                    result, strFilePathDest.c_str());
            handleMerge(strFilePathSrc, strFilePathDest);
            strFilePathDest =
                    destDir + "/" + country.substr(0, 9) + "/strings.xml";
            result = access(strFilePathDest.c_str(), F_OK);
            if (result != -1) {
                handleMerge(strFilePathSrc, strFilePathDest);
            }
        }
    } else {
        if (result == -1) {
            handleCopy(strFilePathSrc, strFilePathDest);
        } else {
            handleMerge(strFilePathSrc, strFilePathDest);
        }
    }
}

void HandleAndroidString::handleMoreCountry(vector<string> &countryVector) {
    cout << "handleMoreCountry() " << endl;
    map<string, string> localNameAndContentMap;
    vector<string> localSaveDiffContentVector;
    bool isAllCountryContentSame = true;
    string preCountry;
    string alineString;
    string nameStr;
    string contentStr;
    for (vector<string>::iterator iter = countryVector.begin();
         iter != countryVector.end();
         ++iter) {
        cout << *iter << endl;
//        cout << *iter << "\t";
//        string srcFilePath = srcDir + "/" + *iter + "/strings.xml";
        fstream file(srcDir + "/" + *iter + "/strings.xml");
        while (getline(file, alineString)) {
            nameStr.clear();
            contentStr.clear();
            getNameAndContent(alineString, nameStr, contentStr);
            if (!nameStr.empty() && !contentStr.empty()) {
                if (localNameAndContentMap.find(nameStr) == localNameAndContentMap.end()) {
                    localNameAndContentMap.insert(map<string, string>::value_type(nameStr, contentStr));
                } else {
                    string localContentStr = localNameAndContentMap[nameStr];
                    if (contentStr.compare(localContentStr) != 0) {
                        isAllCountryContentSame = false;
                        localSaveDiffContentVector.push_back(
                                preCountry + ":" + nameStr + ":" + localContentStr);
                        localSaveDiffContentVector.push_back(
                                *iter + ":" + nameStr + ":" + contentStr);
                    }
                }
            }

//            if (!alineString.empty()
//                && alineString.find("sony_id=") != string::npos
//                && alineString.find("sony_id=\"M") == string::npos) {
//                int index1 = alineString.find("\"");
//                if (index1 != string::npos) {
//                    int index2 = alineString.find("\"", index1 + 1);
//                    if (index2 != string::npos) {
//                        string nameStr = alineString.substr(index1 + 1, index2 - index1 - 1);
//                        index1 = alineString.find(">");
//                        if (index1 != string::npos) {
//                            index2 = alineString.find("</", index1 + 1);
//                            if (index2 != string::npos) {
//                                string contentStr = alineString.substr(index1 + 1, index2 - index1 - 1);
//                                if (localNameAndContentMap.find(nameStr) == localNameAndContentMap.end()) {
//                                    localNameAndContentMap.insert(map<string, string>::value_type(nameStr, contentStr));
//                                } else {
//                                    string localContentStr = localNameAndContentMap[nameStr];
//                                    if (contentStr.compare(localContentStr) != 0) {
//                                        isAllCountryContentSame = false;
//                                        localSaveDiffContentVector.push_back(
//                                                preCountry + ":" + nameStr + ":" + localContentStr);
//                                        localSaveDiffContentVector.push_back(
//                                                *iter + ":" + nameStr + ":" + contentStr);
//                                    }
//                                }
//                            }
//                        }
//                    }
//                }
//            }
        }
        preCountry = *iter;
    }
    if (isAllCountryContentSame) {
        cout << "内容相同" << endl;
        handleSameContent(countryVector);
    } else {
        cout << "内容不同" << endl;
        for (auto nameAndContent : localSaveDiffContentVector) {
            cout << nameAndContent << endl;
        }

        handleDiffContent(countryVector);
    }
}

void HandleAndroidString::handleSameContent(vector<string> &countryVector) {
    string localCountry;
    for (vector<string>::iterator iter = countryVector.begin();
         iter != countryVector.end();
         ++iter) {
        localCountry = *iter;
        string srcFilePath = srcDir + "/" + *iter + "/strings.xml";
        string destFilePath = destDir + "/" + *iter + "/strings.xml";
        if (localCountry.length() > 9) {
            string destFilePath2 =
                    destDir + "/" + localCountry.substr(0, 9) + "/strings.xml";
            int result = access(destFilePath.c_str(), F_OK);
            if (result != -1) {
                handleMerge(srcFilePath, destFilePath);
            }
            result = access(destFilePath2.c_str(), F_OK);
            if (result != -1) {
                handleMerge(srcFilePath, destFilePath2);
            } else {
                handleCopy(srcFilePath, destFilePath2);
            }
        }
    }
}

void HandleAndroidString::handleDiffContent(vector<string> &countryVector) {
    string localCountry;
    for (vector<string>::iterator iter = countryVector.begin();
         iter != countryVector.end();
         ++iter) {
        localCountry = *iter;
        string srcFilePath = srcDir + "/" + *iter + "/strings.xml";
        string destFilePath = destDir + "/" + *iter + "/strings.xml";
        if (localCountry.length() > 9) {
            string destFilePath2 =
                    destDir + "/" + localCountry.substr(0, 9) + "/strings.xml";
            int result = access(destFilePath.c_str(), F_OK);
            if (result != -1) {
                handleMerge(srcFilePath, destFilePath);
            } else {
                handleCopy(srcFilePath, destFilePath);
            }
            result = access(destFilePath2.c_str(), F_OK);
            if (result != -1) {
                handleMerge(srcFilePath, destFilePath2);
            }
        }
    }
}

void HandleAndroidString::handleCountryMap() {
    for (map<string, vector<string>>::iterator iter = countryMap.begin();
         iter != countryMap.end();
         ++iter) {
//        cout << iter->first << " " << &iter->second << endl;
        if (iter->second.size() == 1) {
            handleOneCountry(iter->second[0]);
        } else {
            handleMoreCountry(iter->second);
        }
    }
}

void HandleAndroidString::getNameAndContent(
        const string &content, string &nameStr, string &contentStr) {
    if (!content.empty()
        && content.find("sony_id=") != string::npos
        && content.find("sony_id=\"M") == string::npos) {
        int index1 = content.find("\"");
        if (index1 != string::npos) {
            int index2 = content.find("\"", index1 + 1);
            if (index2 != string::npos) {
                nameStr = content.substr(index1 + 1, index2 - index1 - 1);
                index1 = content.find(">");
                if (index1 != string::npos) {
                    index2 = content.find("</", index1 + 1);
                    if (index2 != string::npos) {
                        contentStr = content.substr(
                                index1 + 1, index2 - index1 - 1);
                    }
                }
            }
        }
    }
}

void HandleAndroidString::handleMergeAddContent(vector<string> &localSrcFileContentVector,
                                                vector<string> &localDestFileContentVector,
                                                const string &alineString) {
    //1.在src中先找到要add的那行的行号
    //2.查看有没有注释.如果有,那么需要复制过去,如果没有,则不做.
    //3.在src中先往上找邻居位置,这个邻居位置在dest中是否存在.
    std::vector<string>::iterator iter =
            std::find(std::begin(localSrcFileContentVector),
                      std::end(localSrcFileContentVector),
                      alineString);
    auto index = std::distance(
            std::begin(localSrcFileContentVector), iter);
    cout << "index: " << index << endl;
    int index2 = index;
    bool isFindAtDestFilePath = false;
    string tempContentStr;
    string notesStr;
    while (1) {
        index--;
        tempContentStr = localSrcFileContentVector[index];
        if (tempContentStr.empty()) {
            continue;
        }
        if (tempContentStr.find("<!--") != string::npos
            && tempContentStr.find("-->") != string::npos
            && index == index2 - 1) {
            notesStr = tempContentStr;
        }
        if (tempContentStr.find("<string") != string::npos
            && tempContentStr.find("</string>") != string::npos) {
            string tempName1;
            string tempContent1;
            string tempName2;
            string tempContent2;
            tempName1.clear();
            tempContent1.clear();
            getNameAndContent(tempContentStr, tempName1, tempContent1);
            for (auto tempContent : localDestFileContentVector) {
                if (tempContent.find("<string") != string::npos
                    && tempContent.find("</string>") != string::npos) {
                    tempName2.clear();
                    tempContent2.clear();
                    getNameAndContent(tempContent, tempName2, tempContent2);
                    if (tempName1.compare(tempName2) == 0) {
                        isFindAtDestFilePath = true;
                        std::vector<string>::iterator iter_ =
                                std::find(std::begin(localDestFileContentVector),
                                          std::end(localDestFileContentVector),
                                          tempContent);
                        auto index_ = std::distance(
                                std::begin(localDestFileContentVector), iter_);
//                        cout << "index_ up: " << index_ << endl;
                        if (notesStr.empty()) {
                            localDestFileContentVector.insert(
                                    localDestFileContentVector.begin() + index_ + 1, alineString);
                        } else {
                            localDestFileContentVector.insert(
                                    localDestFileContentVector.begin() + index_ + 1, notesStr);
                            localDestFileContentVector.insert(
                                    localDestFileContentVector.begin() + index_ + 2, alineString);
                        }
                        return;
                    }
                }
            }
        }
        if (tempContentStr.find("<resources") != string::npos) {
            break;
        }
    }
    if (isFindAtDestFilePath) {
        return;
    }
    index = index2;
    while (1) {
        index++;
        tempContentStr = localSrcFileContentVector[index];
        if (tempContentStr.empty()) {
            continue;
        }
        if (tempContentStr.find("<!--") != string::npos
            && tempContentStr.find("-->") != string::npos
            && index == index2 + 1) {
            notesStr = tempContentStr;
        }
        if (tempContentStr.find("<string") != string::npos
            && tempContentStr.find("</string>") != string::npos) {
            string tempName1;
            string tempContent1;
            string tempName2;
            string tempContent2;
            tempName1.clear();
            tempContent1.clear();
            getNameAndContent(tempContentStr, tempName1, tempContent1);
            for (auto tempContent : localDestFileContentVector) {
                if (tempContent.find("<string") != string::npos
                    && tempContent.find("</string>") != string::npos) {
                    tempName2.clear();
                    tempContent2.clear();
                    getNameAndContent(tempContent, tempName2, tempContent2);
                    if (tempName1.compare(tempName2) == 0) {
                        std::vector<string>::iterator iter_ =
                                std::find(std::begin(localDestFileContentVector),
                                          std::end(localDestFileContentVector),
                                          tempContent);
                        auto index_ = std::distance(
                                std::begin(localDestFileContentVector), iter_);
//                        cout << "index_ down: " << index_ << endl;
                        if (notesStr.empty()) {
                            localDestFileContentVector.insert(
                                    localDestFileContentVector.begin() + index_ - 1, alineString);
                        } else {
                            localDestFileContentVector.insert(
                                    localDestFileContentVector.begin() + index_ - 1, notesStr);
                            localDestFileContentVector.insert(
                                    localDestFileContentVector.begin() + index_ - 2, alineString);
                        }
                        return;
                    }
                }
            }
        }
        if (tempContentStr.find("</resources>") != string::npos) {
            break;
        }
    }
}

void HandleAndroidString::handleMergeModifyContent(vector<string> &localDestFileContentVector,
                                                   const string &alineString,
                                                   const string &nameStr,
                                                   const string &contentStr) {
    string tempNameStr;
    string tempContentStr;
    for (auto tempContent : localDestFileContentVector) {
        tempNameStr.clear();
        tempContentStr.clear();
        getNameAndContent(tempContent, tempNameStr, tempContentStr);
        if (nameStr.compare(tempNameStr) == 0) {
            std::vector<string>::iterator iter =
                    std::find(std::begin(localDestFileContentVector),
                              std::end(localDestFileContentVector),
                              tempContent);
            auto index = std::distance(
                    std::begin(localDestFileContentVector), iter);
            cout << "index: " << index << endl;
            localDestFileContentVector[index] = alineString;
//            cout << localDestFileContentVector[index] << endl;
            return;
        }
    }
}

void HandleAndroidString::init() {
    srcDir = string(CUR_DIR) + "/src/res";
    destDir = string(CUR_DIR) + "/dest/res";
    cacheDir = string(CUR_DIR) + "/cache";

    int result = access(cacheDir.c_str(), F_OK);
    if (result == -1) {
        fprintf(stdout, "return:%d %s目录不存在\n", result, cacheDir.c_str());
    } else {
        fprintf(stdout, "return:%d %s目录存在\n", result, cacheDir.c_str());
    }

    fstream file(srcDir + "/values/strings.xml");
    string alineString;
    string nameStr;
    string contentStr;
    while (getline(file, alineString)) {
        nameStr.clear();
        contentStr.clear();
        getNameAndContent(alineString, nameStr, contentStr);
        if (!nameStr.empty() && !contentStr.empty()) {
            nameAndContentMap.insert(
                    map<string, string>::value_type(nameStr, contentStr));
        }
    }
    file.close();

//    for (map<string, string>::iterator iter = nameAndContentMap.begin();
//         iter != nameAndContentMap.end();
//         ++iter) {
//        cout << iter->first << " = " << iter->second << endl;
//    }

    //把有关数据保存到countryMap中去
    recurseDir(srcDir.c_str());

//    for (map<string, vector<string>>::iterator iter = countryMap.begin();
//         iter != countryMap.end();
//         ++iter) {
//        cout << iter->first << " " << &iter->second << endl;
//        for (vector<string>::iterator it = iter->second.begin(); it != iter->second.end(); ++it) {
//            cout << *it << "\t";
//        }
//        cout << endl;
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

//求子串
char *substr(const char *str, unsigned long start, unsigned long end);

void replace(const char *src,
             const char *dst,
             int fd,
             unsigned char *buffer,
             unsigned long size);

//替换文本. ori:将要替换的字符串、dst:替换的目的字符串, path:文件名
void replace(const char *src, const char *dest, const char *path);

//求子串
char *substr(const char *str, unsigned long start, unsigned long end) {
    unsigned long n = end - start;
    static char stbuf[1024 * 512];
    strncpy(stbuf, str + start, n);
    stbuf[n] = 0;                           //字串最后加上0
    return stbuf;
}


void replace(const char *src,
             const char *dst,
             int fd,
             unsigned char *buffer,
             unsigned long size) {
    unsigned char *buf = new unsigned char[size];
    unsigned long i = 0;

    unsigned long *dSize = new unsigned long;
    *dSize = size;  //动态文件大小，由于替换数据造成文件加大

    unsigned char *buffer2 = new unsigned char[size];
    memccpy(buffer2, buffer, sizeof(unsigned char), size);

    long dt = 0;
    while (i < *dSize) {
        if (0 == strcmp(src, substr((char *) buffer2, i, i + strlen(src)))) {
            dt += strlen(dst) - strlen(src);

            //定位到号+ori长的位置读取数据
            lseek(fd, i + strlen(src), SEEK_SET);
            read(fd, buf, *dSize - i - strlen(src));


            //定位到#号+1+length的位置
            lseek(fd, i + strlen(dst), SEEK_SET);
            write(fd, buf, *dSize - i - strlen(src));


            //回到#号位置并写入数据
            lseek(fd, i, SEEK_SET);
            write(fd, dst, strlen(dst));

            //由于size变了,需要修改dSize,将数据存储在临时指针中
            unsigned char *t = buffer2;

            *dSize += strlen(dst) - strlen(src);
            buffer2 = new unsigned char[*dSize];
            lseek(fd, 0, SEEK_SET);
            read(fd, buffer2, *dSize);

            delete[]t;
        }
        i++;
    }

    if (dt < 0) {
        for (long i = *dSize; i < *dSize - dt; i++) {
            *(buffer2 + i) = 32;
        }
        lseek(fd, 0, SEEK_SET);
        write(fd, buffer2, *dSize - dt);
    }

    delete[] buf;
    buf = NULL;
    delete dSize;
    dSize = NULL;
}


//删除注释.path:文件名
/*void replace(const char *src, const char *dest, const char *path) {
    int fd = open(path, O_RDWR);

    //获取文件大小
    struct stat sb;
    fstat(fd, &sb);

    unsigned char *buffer = (unsigned char *) mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    *//* 判断是否映射成功 *//*
    if (buffer == MAP_FAILED) {
        printf("映射失败，文件过大或者没有权限");
        return;
    }

    replace(src, dest, fd, buffer, sb.st_size);

    close(fd);
    munmap(buffer, sb.st_size); *//* 解除映射 *//*
}*/
