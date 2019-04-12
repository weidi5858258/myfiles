//
// Created by root on 19-4-10.
//

#ifndef MYSTUDY_FORMATCODE_HPP
#define MYSTUDY_FORMATCODE_HPP

#include "MyHead.h"

class FormatCode {
private:
    string srcFilePath;
    string desFilePath;
    string blankSpace;
    vector<string> contents;
public:

public:
    FormatCode();

    ~FormatCode();

    void start(int argc, char **argv);

    void release();

private:
    string printBlankSpace(int blankSpaceCount);

    /***
     输出不确定个数的字符串内容
     现在不好的地方:
     最后必须再加上""(没有没有空格),不然程序要崩
     */
    void qDebug(string msg, ...);
};

FormatCode::FormatCode() {
    qDebug("start Create FormatCode() Object", "");
    qDebug("end   Create FormatCode() Object", "");
}

FormatCode::~FormatCode() {
    qDebug("start Destroy FormatCode  Object", "");
    qDebug("end   Destroy FormatCode  Object", "");
}

void FormatCode::start(int argc, char **argv) {
    qDebug("start()", "");
    srcFilePath = argv[1];
    desFilePath = argv[2];
    qDebug("start()",
           "srcFilePath:",
           srcFilePath.c_str(),
           "desFilePath:",
           desFilePath.c_str(),
           "");
    // 为0时存在
    int result = access(srcFilePath.c_str(), F_OK);
    if (result == -1) {
        qDebug("start()",
               "result:",
               std::to_string(result).c_str(),
               srcFilePath.c_str(),
               "文件不存在",
               "");
        return;
    } else {
        qDebug("start()",
               "result:",
               std::to_string(result).c_str(),
               srcFilePath.c_str(),
               "文件存在",
               "");
    }
    result = access(desFilePath.c_str(), F_OK);
    if (result == -1) {
        qDebug("start()",
               "result:",
               std::to_string(result).c_str(),
               desFilePath.c_str(),
               "文件不存在",
               "");
    } else {
        qDebug("start()",
               "result:",
               std::to_string(result).c_str(),
               desFilePath.c_str(),
               "文件存在",
               "");
    }

    // 文件不存在就创建,存在就清空
    FILE *fp = fopen(desFilePath.c_str(), "wt");
    fclose(fp);

    qDebug("start()", "hello", "world", "");
}

void FormatCode::release() {
//    srcFilePath.clear();
//    desFilePath.clear();
//    blankSpace.clear();
//    contents.clear();
}

string FormatCode::printBlankSpace(int blankSpaceCount) {
    if (blankSpaceCount < 0) {
        blankSpaceCount = 1;
    }
    blankSpace.clear();
    for (int i = 0; i < blankSpaceCount; i++) {
        blankSpace.append(" ");
    }
    return blankSpace;
}

void FormatCode::qDebug(string msg, ...) {
    string tempMsg;
    va_list msgPtr;
    va_start(msgPtr, msg);
    while (true) {
        tempMsg = va_arg(msgPtr, char*);
        if (tempMsg.empty()) {
            break;
        }
        msg.append(" ");
        msg.append(tempMsg);
    }
    va_end(msgPtr);
    std::cout << "qDebug() Object: " << this << " " << msg << std::endl;
    //fprintf(stdout, "return: %d %s文件不存在\n", result, srcFilePath.c_str());
}

/*void CombineSpace(UINT8 *pszOldStr, UINT8 *pszNewStr) {
    UINT32 iOldIndex = 0;
    UINT32 iNewIndex = 0;
    UINT8 chSingleChar = '\0';
    UINT8 iIsSpace = 0;
    UINT8 szTmpNewStr[100] = {0};

    if ((NULL == pszOldStr) || (NULL == pszNewStr)) {
        printf("CombineSpace: input parameter(s) is NULL!");
        return;
    }

    while ('\0' != (chSingleChar = pszOldStr[iOldIndex++])) {
        if (chSingleChar == ' ')   // 多个连续空格只保留一个
        {
            if (iIsSpace == 0) {
                iIsSpace = 1;
                szTmpNewStr[iNewIndex++] = chSingleChar;
            }
        } else {
            iIsSpace = 0;
            szTmpNewStr[iNewIndex++] = chSingleChar;
        }
    }

    // 如果最后一个字符是空格, 则将其去掉
    if (szTmpNewStr[strlen(szTmpNewStr) - 1] == ' ') {
        szTmpNewStr[strlen(szTmpNewStr) - 1] = '\0';
    }

    // 如果第一个字符是空格, 则将其去掉
    if (szTmpNewStr[0] == ' ') {
        memcpy(pszNewStr, szTmpNewStr + 1, strlen(szTmpNewStr) - 1);
    } else {
        memcpy(pszNewStr, szTmpNewStr, strlen(szTmpNewStr));
    }

    return;
}*/

#endif //MYSTUDY_FORMATCODE_HPP
