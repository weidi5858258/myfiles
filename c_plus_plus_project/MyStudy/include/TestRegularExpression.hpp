//
// Created by root on 19-4-13.
//

#ifndef MYSTUDY_TESTREGULAREXPRESSION_HPP
#define MYSTUDY_TESTREGULAREXPRESSION_HPP

#include "MyHead.h"

/***
 gcc要求: 版本>=4.9
 \d："\"需要转义,因此最后的写法是"\\d"
 * 匹配前面的子表达式零次或多次
 + 匹配前面的子表达式一次或多次
 ? 匹配前面的子表达式零次或一次
 {n}   n是一个非负整数,匹配确定的n次
 {n,}  n是一个非负整数,至少匹配n次
 {n,m} m和n均为非负整数,其中n <= m.最少匹配n次且最多匹配m次
 [0-9] 0到9的数字(包括0和9)

 如果要匹配下面这些特殊字符本身时,其写法为(都需要在前面加\):
 \& |( \) \* \+ \. \[ \? \\ \^ \{ \|

 regex_match全部匹配上返回才是true
 regex_search搜索字符串中存在符合规则的子字符串
 regex_replace将符合匹配规则的子字符串替换为其他字符串
 匹配某个特征时,最好把这个匹配规则用括号括起来
 下面需要分情况而论,即是否全匹配.
 用regex_match函数时如果全匹配就返回true
 用regex_match函数时如果局部匹配就返回false
 用regex_search函数时如果全匹配就返回true
 用regex_search函数时如果局部匹配也返回true
 std::regex("(^abc)"):匹配的字符串是否以abc开头
 std::regex("([^abc])"):首先匹配单个字符,该字符不能是a和b和c
 std::regex("([abc])"):首先匹配单个字符,该字符是a或者b或者c
 std::regex("([a-z])"):首先匹配单个字符,该字符是任何小写字母
 std::regex("(abc$)"):匹配的字符串是否以abc结尾
 std::regex("(\\d)"):匹配的字符串是否是一个数字
 std::regex("(\\d+)"):匹配的字符串是否是一个或者多个数字
 std::regex("(\\d{4})"):匹配的字符串是否是4个数字
 std::regex("(.{5})"):匹配的字符串是否是5个任意单字符
 std::regex("(.{3})(.{2})_(\\d{4})"):匹配的字符串是否是
 3个任意字符+2个任意字符+下划线+4个数字(有三个括号,匹配三组数据)
 std::regex("(a*)", regex::icase):
 先忽略大小写,然后匹配的字符串是否是以a开头,后面跟零个或者多个a
 std::regex("(zo*)")等价于std::regex("(zo{0,})"): z zo zoo zooo...
 std::regex("(zo+)")等价于std::regex("(zo{1,})"):   zo zoo zooo...
 std::regex("(do(es)?)")等价于std::regex("(do(es){0,1})"): do does
 std::regex("(o{2})"): food
 std::regex("([1-9][0-9]*)"):匹配的字符串是否是任意的两位数,三位数,四位数...
 std::regex("((\s)*)"): 匹配的字符串是否是一个或者多个空格

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
    cout << std::regex_match(" ", std::regex("((\\s)*)")) << endl;
    cout << std::regex_search("a b", std::regex("(^a[:blank:]b$)")) << endl;


    string str = "Hello_2018";
    smatch result;
    regex pattern("(.{5})_(\\d{4})");    //匹配5个任意单字符 + 下划线 + 4个数字

    if (regex_match(str, result, pattern)) {
        cout << result[0] << endl;        //完整匹配结果，Hello_2018
        cout << result[1] << endl;        //第一组匹配的数据，Hello
        cout << result[2] << endl;        //第二组匹配的数据，2018
        cout << "结果显示形式2" << endl;
        cout << result.str() << endl;    //完整结果，Hello_2018
        cout << result.str(1) << endl;    //第一组匹配的数据，Hello
        cout << result.str(2) << endl;    //第二组匹配的数据，2018
    }

    //遍历结果
    for (int i = 0; i < result.size(); ++i) {
        cout << result[i] << endl;
    }

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
    testContent = "abcdefg";
    testPattern.clear();
    testPattern = "\\d+";
    testRegex.assign(testPattern);
    if (std::regex_search(testContent, testRegex)) {
        std::cout << "string literal matched\n";//字符串匹配
    } else {
        std::cout << "string literal not matched\n";//字符串匹配
    }

    cout << std::regex_search("abcdefg", std::regex("(^abc)")) << endl;

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
//    cout << std::regex_replace("Hello_2018!", std::regex("Hello"), "") << endl;
    //输出：Hi_2018,将Hello替换为Hi
//    cout << std::regex_replace("Hello_2018!", std::regex("Hello"), "Hi") << endl;
}

#endif //MYSTUDY_TESTREGULAREXPRESSION_HPP
