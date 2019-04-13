//
// Created by root on 19-4-13.
//

#ifndef MYSTUDY_TESTREGULAREXPRESSION_HPP
#define MYSTUDY_TESTREGULAREXPRESSION_HPP

#include "MyHead.h"

/***
 gcc要求: 版本>=4.9
 regex_match全部匹配上返回才是true
 regex_search搜索字符串中存在符合规则的子字符串
 regex_replace将符合匹配规则的子字符串替换为其他字符串
 匹配某个特征时,最好把这个匹配规则用括号括起来
 ^表示字符串的开始
 $表示字符串的结尾
 \d：匹配一个数字字符
 + ：匹配一次或多次
 \d+:表示多个数字,但是由于"\"需要转义,因此最后的写法是"\\d+"
 \\d{4}:表示四个数字
 (.{5}):表示5个任意单字符
 (.{3})(.{2})_(\\d{4}):表示3个任意字符+2个任意字符+下划线+4个数字(有三个括号,匹配三组数据)
*/
class TestRegularExpression {
private:
public:

public:
    TestRegularExpression();

    ~TestRegularExpression();

    void test();

private:
};

TestRegularExpression::TestRegularExpression() {

}

TestRegularExpression::~TestRegularExpression() {

}

void TestRegularExpression::test() {
    smatch testSmatch;
    string testContent = "subject";
    string testPattern = "(sub)(.*)";
    testPattern.clear();
    testPattern = "ject$";
    std::regex testRegex(testPattern);
    if (std::regex_match(testContent, testRegex)) {
        std::cout << "string literal matched\n";//字符串匹配
    } else {
        std::cout << "string literal not matched\n";//字符串匹配
    }

    testContent.clear();
    testContent = "12345";
    testPattern.clear();
    testPattern = "\\d+";
    testRegex.assign(testPattern);
    if (std::regex_match(testContent, testRegex)) {
        std::cout << "string literal matched\n";//字符串匹配
    } else {
        std::cout << "string literal not matched\n";//字符串匹配
    }

    //结果为0，需要转义字符'\'
    cout << std::regex_match("123", std::regex("\d+")) << endl;
    cout << std::regex_match("123", std::regex("\\d+")) << endl;
    cout << std::regex_match("123", std::regex("^1")) << endl;
    cout << std::regex_search("123", std::regex("^1")) << endl;

    cout << std::regex_match("123", std::regex("\\d")) << endl;
    cout << std::regex_match("abcd123", std::regex("^abcd\\d+")) << endl;

    cout << std::regex_match("123", std::regex("\\d")) << endl;
    cout << std::regex_search("123", std::regex("\\d")) << endl;
    // 忽略大小写
    cout << std::regex_match("aaaAAA", std::regex("a*", regex::icase)) << endl;
    cout << std::regex_match("aaaAAA", std::regex("a*")) << endl;

    //输出：_2018,将Hello替换为""
    cout << std::regex_replace("Hello_2018!", std::regex("Hello"), "") << endl;
    //输出：Hi_2018,将Hello替换为Hi
    cout << std::regex_replace("Hello_2018!", std::regex("Hello"), "Hi") << endl;
}

#endif //MYSTUDY_TESTREGULAREXPRESSION_HPP
