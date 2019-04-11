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
    vector<string> *contents;
public:

public:
    FormatCode();

    ~FormatCode();

    void start(int argc, char **argv);

private:
    string printBlankSpace(int blankSpaceCount);

    void qDebug(int argc, char **argv);
};

FormatCode::FormatCode() {
    std::cout << "start Create FormatCode() Object: " << this << std::endl;
    contents = new vector<string>;
    std::cout << "end   Create FormatCode() Object: " << this << std::endl;
}

FormatCode::~FormatCode() {
    std::cout << "start Destroy FormatCode  Object: " << this << std::endl;
    if (contents) {
        delete contents;
    }
    std::cout << "end   Destroy FormatCode  Object: " << this << std::endl;
}

void FormatCode::start(int argc, char **argv) {
    std::cout << "start() Object: " << this << std::endl;
    srcFilePath = argv[1];
    desFilePath = argv[2];
    std::cout << "start() Object: " << this <<
              " srcFilePath: " << srcFilePath <<
              " desFilePath: " << desFilePath << std::endl;
    int result = access(srcFilePath.c_str(), F_OK);
    if (result == -1) {
        fprintf(stdout, "return: %d %s文件不存在\n", result, srcFilePath.c_str());
        return;
    } else {
        fprintf(stdout, "return: %d %s文件存在\n", result, srcFilePath.c_str());
    }
    result = access(desFilePath.c_str(), F_OK);
    if (result == -1) {
        fprintf(stdout, "return: %d %s文件不存在\n", result, desFilePath.c_str());
    } else {
        fprintf(stdout, "return: %d %s文件存在\n", result, desFilePath.c_str());
    }

    // 文件不存在就创建,存在就清空
    FILE *fp = fopen(desFilePath.c_str(), "wt");
    fclose(fp);


    std::cout << "start() Object: " << this << "@" << printBlankSpace(1) << "@" << std::endl;
    char **msg;
    msg[0] = "start()";
    msg[1] = "#";
    msg[2] = const_cast<char *>(printBlankSpace(5).c_str());
    msg[3] = "#";
    qDebug(4, msg);
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

void FormatCode::qDebug(int argc, char **argv) {
    if (argc < 0) {
        return;
    }
    if (argc == 1) {
        std::cout << argv[0] << " Object: " << this << std::endl;
    } else {
        string msg;
        for (int i = 1; i < argc; i++) {
            msg.append(argv[i]);
        }
        std::cout << argv[0] << " Object: " << this << msg << std::endl;
    }
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
