//
// Created by alexander on 2020/1/6.
//

#ifndef MYSTUDY_TESTMEMORY_HPP
#define MYSTUDY_TESTMEMORY_HPP

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace alexander {

    class TestMemory {
    public:
        TestMemory();

        virtual ~TestMemory();

        void testMalloc();

        void testCalloc();

        void testRealloc();
    };

    TestMemory::TestMemory() {}

    TestMemory::~TestMemory() {}

    void TestMemory::testMalloc() {
        int n = 5;
        int *ptr = NULL;
        /***
         malloc(size): 在内存的动态存储区中分配一块长度为“size”字节的连续区域，然后返回该区域的首地址.
         sizeof(int)为4 bytes
         开辟1个空间,这个空间的大小为20 bytes
         空间没有被初始化,需要调用memset函数进行初始化
         */
        ptr = (int *) malloc(n * sizeof(int));
        printf("before *ptr: %d\n", *ptr);
        printf("ptr[0]: %d\n", ptr[0]);
        printf("ptr[1]: %d\n", ptr[1]);
        printf("ptr[2]: %d\n", ptr[2]);
        printf("ptr[3]: %d\n", ptr[3]);
        printf("ptr[4]: %d\n", ptr[4]);
        printf("ptr[5]: %d\n", ptr[5]);
        memset(ptr, 0, n * sizeof(int));
        printf("after  *ptr: %d\n", *ptr);
        printf("ptr[0]: %d\n", ptr[0]);
        printf("ptr[1]: %d\n", ptr[1]);
        printf("ptr[2]: %d\n", ptr[2]);
        printf("ptr[3]: %d\n", ptr[3]);
        printf("ptr[4]: %d\n", ptr[4]);
        printf("ptr[5]: %d\n", ptr[5]);

        for (int i = 0; i < n; ++i) {
            ptr[i] = i + 1;
        }
        printf("The elements of the array are: ");
        for (int i = 0; i < n; ++i) {
            printf("%d ", ptr[i]);
        }
        printf("\n");

        xfree(ptr);
    }

    void TestMemory::testCalloc() {
        int n = 5;
        int *ptr = NULL;
        /***
         calloc(n，size): 在内存的动态存储区中分配n块长度为“size”字节的连续区域，然后返回首地址
         开辟5个空间,每个空间的大小为4 bytes
         空间已经被初始化,不需要再调用memset函数进行初始化
         多用于数组
         */
        ptr = (int *) calloc(n, sizeof(int));

        for (int i = 0; i < n; ++i) {
            ptr[i] = i + 10;
        }
        printf("The elements of the array are: ");
        for (int i = 0; i < n; ++i) {
            printf("%d ", ptr[i]);
        }
        printf("\n");

        xfree(ptr);
    }

    void TestMemory::testRealloc() {
        int n = 5;
        int *ptr = NULL;
        /***
         先开辟1个空间,这个空间的大小为20 bytes
         后开辟1个空间,这个空间的大小为40 bytes
         空间没有被初始化,需要调用memset函数进行初始化
         */
        ptr = (int *) malloc(n * sizeof(int));
        memset(ptr, 0, n * sizeof(int));
        ptr = (int *) realloc(ptr, 10 * sizeof(int));
        memset(ptr, 0, 10 * sizeof(int));

        for (int i = 0; i < n; ++i) {
            ptr[i] = i + 100;
        }
        printf("The elements of the array are: ");
        for (int i = 0; i < n; ++i) {
            printf("%d ", ptr[i]);
        }
        printf("\n");

        xfree(ptr);
    }

}

#endif //MYSTUDY_TESTMEMORY_HPP
