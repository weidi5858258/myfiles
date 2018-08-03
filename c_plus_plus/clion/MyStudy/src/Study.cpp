//
// Created by root on 18-7-12.
//
#ifndef MYSTUDY_STUDY
#define MYSTUDY_STUDY

#ifndef MYSTUDY_B0
#define MYSTUDY_B0
#include "../src/B0.cpp"
#endif //MYSTUDY_B0

#ifndef MYSTUDY_POINTER
#define MYSTUDY_POINTER
#include "../src/Pointer.cpp"
#endif //MYSTUDY_POINTER

// 先转化成(int *),然后取该地址处的值
int cmp(const void *a, const void *b) {
    return *(int *) a - *(int *) b;
}

/***
 要测试的代码放在main函数中,测试后放到这个函数中
 */
void test() {
    Test t;
    cout << t.getData() << endl;
    Test t2(10);
    Test t3(t2);
    Test t4 = t2;

    C co;
    // error
    // c.B0::b0 = 100;
    co.B1::b0 = 100;
    co.B2::b0 = 200;
    cout << co.B1::m << endl;
    cout << co.B2::m << endl;
    cout << co.c << endl;
    co.print();
    B1 *pb = &co;
    pb->print();

    // 指针
    Pointer pt;
    pt.point();

    char name[] = "ueventc";
    cout << strcmp(basename(name), "ueventd") << endl;
    size_t key_len = strlen("PATH");
    cout << "key_len = " << key_len << endl;// 4
    cout << "key_len = " << sizeof("PATH") << endl;// 5

    char ch = '!';
    printf("%d = %c.\n", ch, ch);
    int ich = 127;
    printf("%d = %c.\n", ich, ich);

    int tmp[5] = {1, 2, 3, 4, 5};
    int *p = tmp;
    cout << sizeof(tmp) << endl;// tmp数组占用空间大小
    cout << sizeof(tmp) / sizeof(int) << endl;// tmp数组元素个数

    cout << tmp << endl;
    cout << &tmp[0] << endl;
    cout << p << endl;

    cout << *tmp << endl;
    // cout << *(tmp++) << endl;// error
    cout << *(&tmp[0]) << endl;
    int j;
    for (j = 0; j < 5; ++j) {
        cout << *(p++) << endl;
    }

    cout << sizeof(char) << endl;// 1
    cout << sizeof(wchar_t) << endl;// 4
    cout << sizeof(int) << endl;// 4
    cout << sizeof(short) << endl;// 2
    cout << sizeof(float) << endl;// 4
    cout << sizeof(double) << endl;// 8
    cout << sizeof(long) << endl;// 8
    cout << sizeof(long long) << endl;// 8
    cout << sizeof(long long int) << endl;// 8
    cout << sizeof(long double) << endl;// 16

    cout << endl;

    cout << sizeof(signed char) << endl;// 1
    cout << sizeof(unsigned char) << endl;// 1
    cout << sizeof(unsigned wchar_t) << endl;// 4
    cout << sizeof(unsigned int) << endl;// 4
    cout << sizeof(unsigned short) << endl;// 2
    cout << sizeof(unsigned long) << endl;// 8
    cout << sizeof(unsigned long long) << endl;// 8
    cout << sizeof(unsigned long long int) << endl;// 8

    cout << endl;

    cout << CHAR_MIN << endl;
    cout << CHAR_MAX << endl;
    cout << INT_MIN << endl;
    cout << INT_MAX << endl;
    cout << LONG_MIN << endl;
    cout << LONG_MAX << endl;

    // char与signed char取值范围都是 -128 到 127(有符号位)
    // unsigned char取值范围是 0 到 255
    // 如果定义的变量的值在此范围内,那么输出的值就是当前值
    // 正常边界,正常输出
    char a = -128;
    signed char b = 127;
    unsigned char c = 0;
    unsigned char d = 255;
    printf("a=%d, b=%d, c=%d, d=%d\n", a, b, c, d);// -128 127 0
    printf("a=%c, b=%c, c=%c, d=%d\n", a, b, c, d);//
    // 超出边界,反向输出
    a = -129;
    b = 128;
    c = -1;
    d = 256;
    printf("a=%d, b=%d, c=%d, d=%d\n", a, b, c, d);// 127 -128 255 0

    cout << endl;

    FILE *fp;
    size_t size = 0;
    char *ar;
    // 二进制方式打开文件
    fp = fopen("src/B0.cpp", "rb");
    if (NULL == fp) {
        printf("Error:Open input.c file fail!\n");
        return;
    }
    fseek(fp, 0, SEEK_END);
    // 求得文件的大小
    size = ftell(fp);
    rewind(fp);
    // 申请一块能装下整个文件的空间
    ar = (char *) malloc(sizeof(char) * size);
    // 读文件
    // fread(ar, 1, all_pkts_size, fp);//每次读一个，共读size次
    fread(ar, size, 1, fp);
    // 内容的首地址
    printf("%p\n", ar);
    // 输出文件内容
    printf("%s\n", ar);
    fclose(fp);
    free(ar);
    ar = NULL;

    cout << endl;

    const char *path = "/data";
    printf("path = %s\n", path);

    const unsigned char image[] = "abcd\0efg";
    //  const unsigned char image[] = "";
    const unsigned char *pImage = image;
    size = sizeof(pImage);// 包括结尾隐藏的\0
    cout << size << endl;// 8 固定长度,用于存放指针地址的
    size = sizeof(image);
    cout << size << endl;// 9
    for (int i = 0; i < size; ++i) {
        // 这个例子说明了可以用pImage[i]这种方式一个字符一个字符的输出
        printf("%c\n", pImage[i]);
        if (pImage[i] == 0) {
            printf("遇到\\0换行\n");
        }
    }

    double tmpArray[] = {1, 2, 3, 4, 5};
    cout << sizeof(tmpArray) << endl;
    for (int i = 0; i < 5; i++) {
        printf("%f\t%f\t%f\t%p\t%p\n",
               tmpArray[i], *(&tmpArray[i]), *(tmpArray + i),
               &tmpArray[i], (tmpArray + i));
    }

    // 下面的代码在这个工具中无法编译通过
    /*// 在栈上开辟一维数组
    int *pOne = (int[]){0};
    // 在栈上开辟二维数组
    int ab[3][4];
    int (*pTwo)[4] = (int[][4]){0};
    // 在栈上开辟三维数组
    int (*pThree)[3][4] = (int[][3][4]){0};*/

    // 下面代码使用SDL2简单显示一个窗口
    /*SDL_Window *window = NULL;
    SDL_Surface *screenSurface = NULL;
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    } else {
        window = SDL_CreateWindow(
                "SDL Tutorial",
                SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED,
                640,
                480,
                SDL_WINDOW_SHOWN);
        screenSurface = SDL_GetWindowSurface(window);
        SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
        SDL_UpdateWindowSurface(window);
        SDL_Delay(5000);
    }
    SDL_DestroyWindow(window);
    SDL_Quit();*/

    /*SDL_Window *window = NULL;
    SDL_Renderer *renderer = 0;
    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow(
            "hello",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            640,
            480,
            SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    SDL_Delay(10000);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();*/

    /*glutInit(&argc,argv);
    //显示模式初始化
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB|GLUT_DEPTH);
    //定义窗口大小
    glutInitWindowSize(300,300);
    //定义窗口位置
    glutInitWindowPosition(100,100);
    //创建窗口
    glutCreateWindow("OpenGL Version");
    const GLubyte* name = glGetString(GL_VENDOR); //返回负责当前OpenGL实现厂商的名字
    const GLubyte* biaoshifu = glGetString(GL_RENDERER); //返回一个渲染器标识符，通常是个硬件平台
    const GLubyte* OpenGLVersion =glGetString(GL_VERSION); //返回当前OpenGL实现的版本号
    const GLubyte* gluVersion= gluGetString(GLU_VERSION); //返回当前GLU工具库版本
    printf("OpenGL实现厂商的名字：%s\n", name);
    printf("渲染器标识符：%s\n", biaoshifu);
    printf("OOpenGL实现的版本号：%s\n",OpenGLVersion );
    printf("OGLU工具库版本：%s\n", gluVersion);*/

}





#endif //MYSTUDY_STUDY