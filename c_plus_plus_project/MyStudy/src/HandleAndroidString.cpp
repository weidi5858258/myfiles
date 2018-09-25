
#ifdef WIN32

#include "HandleAndroidString.h"

#else

#include "../include/HandleAndroidString.h"

#endif

void HandleAndroidString::doSomething(string tempCache) {
    this->TEMP_CACHE = tempCache;

    init();

    handleCountryMap();
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

//        cout << "It's handling: " << tempDir << endl;
        if (tempDir.length() == 13) {
            tempDir = tempDir.substr(0, 9);
        }
        if (countryMap.find(tempDir) == countryMap.end()) {
//            cout << "Do not contain: " << fileName->d_name << endl;
            vector<string> countryVector;
            countryVector.push_back(fileName->d_name);
            countryMap.insert(
                    map<string, vector<string>>::value_type(
                            tempDir, countryVector));
        } else {
//            cout << "Contain: " << fileName->d_name << endl;
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

    file.open(srcFilePath);
    while (getline(file, alineString)) {
        nameStr.clear();
        contentStr.clear();
        getNameAndContent(alineString, nameStr, contentStr);
        if (!nameStr.empty() && !contentStr.empty()) {
            if (nameAndContentMap.find(nameStr) != nameAndContentMap.end()) {
                string valuesContentStr = nameAndContentMap[nameStr];
                //The content has been translated.
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

    //put localDestFileContentVector's contents write to file
    FILE *destFile = fopen(destFilePath.c_str(), "w");
    for (auto destContent : localDestFileContentVector) {
        if (destContent.find("</resources>") == string::npos) {
            fputs((destContent + "\n").c_str(), destFile);
        } else {
            fputs((destContent).c_str(), destFile);
        }
    }
    fclose(destFile);

//    for (auto destContent : localDestFileContentVector) {
//        cout << destContent << endl;
//    }
}

void HandleAndroidString::handleCopy(const string &srcFilePath,
                                     const string &destFilePath) {
    cout << "handleCopy()" << endl;
    cout << "srcFilePath: " << srcFilePath << endl;
    cout << "destFilePath: " << destFilePath << endl;
}

void HandleAndroidString::handleOneCountry(const string &country) {
    cout << "handleOneCountry()" << endl;
    //values-af
    //values-af-rZA
    cout << country << endl;
    string strFilePathSrc = srcDir + "/" + country + "/strings.xml";
    string strFilePathDest = destDir + "/" + country + "/strings.xml";
    int result = access(strFilePathDest.c_str(), F_OK);
    if (country.length() > 9) {
        if (result == -1) {
            fprintf(stdout, "%s file not exist\n", strFilePathDest.c_str());
            strFilePathDest =
                    destDir + "/" + country.substr(0, 9) + "/strings.xml";
            result = access(strFilePathDest.c_str(), F_OK);
            if (result == -1) {
                fprintf(stdout, "%s file not exist\n", strFilePathDest.c_str());
                handleCopy(strFilePathSrc, strFilePathDest);
            } else {
                fprintf(stdout,
                        "return:%d %s file exist\n",
                        result, strFilePathDest.c_str());
                handleMerge(strFilePathSrc, strFilePathDest);
            }
        } else {
            fprintf(stdout,
                    "return:%d %s file exist\n",
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
        }
        preCountry = *iter;
    }

    if (isAllCountryContentSame) {
        cout << "contents are same" << endl;
        handleSameContent(countryVector);
    } else {
        cout << "contents are different" << endl;
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
    srcDir = TEMP_CACHE + "/src/res";
    destDir = TEMP_CACHE + "/dest/res";

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
}

