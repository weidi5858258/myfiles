//
// Created by alexander on 2020/1/6.
//

#ifndef MYSTUDY_TESTSTL_HPP
#define MYSTUDY_TESTSTL_HPP

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <list>
#include <vector>
#include <queue>
#include <map>

namespace alexander {
    class TestSTL {
    public:
        TestSTL();

        virtual ~TestSTL();

        void testList();

        void testVector();
    };

    void TestSTL::testList() {
        struct Student {
            char name[50];
            int age;
        };
        std::list<struct Student> list1;
        std::list<struct Student> list2;
        struct Student student1;
        strcpy(student1.name, "123");
        student1.age = 30;
        list1.push_back(student1);
        struct Student student2;
        strcpy(student2.name, "456");
        student2.age = 35;
        list1.push_back(student2);
        struct Student student3;
        strcpy(student3.name, "789");
        student3.age = 40;
        list1.push_back(student3);

        std::list<struct Student>::iterator it; //声明一个迭代器
        for (auto it = list1.begin(); it != list1.end(); it++) {
            std::cout << (*it).name << " " << (*it).age << std::endl;
        }

        list2.clear();
        // ok
        // std::copy(list1.begin(), list1.end(), std::back_inserter(list2));
        // std::copy_n(list1.begin(), 2, std::back_inserter(list2));
        std::move(list1.begin(), list1.end(), std::back_inserter(list2));
        //list1.clear();
        std::cout << list1.size() << std::endl;
        std::cout << list2.size() << std::endl;
        for (auto it = list1.begin(); it != list1.end(); it++) {
            std::cout << (*it).name << " " << (*it).age << std::endl;
        }
        for (auto it = list2.begin(); it != list2.end(); ++it) {
            std::cout << (*it).name << " " << (*it).age << std::endl;
        }
    }

    void TestSTL::testVector() {
        std::vector<int> v1 = {1, 2, 3, 4, 5};
        std::vector<int> v2(5);
        copy(v1.begin(), v1.end(), v2.begin());
        for (auto it = v2.begin(); it != v2.end(); ++it) {
            std::cout << *it << std::endl;
        }
    }
}

#endif //MYSTUDY_TESTSTL_HPP
