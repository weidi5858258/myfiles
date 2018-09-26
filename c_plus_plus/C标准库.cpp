/***
C标准库

*/
#include <string.h>
void 	*memchr(const void *str, int c, size_t n);
void 	*memcpy(void *dest, const void *src, size_t n);
void 	*memmove(void *dest, const void *src, size_t n);
void 	*memset(void *str, int c, size_t n);
int 	memcmp(const void *str1, const void *str2, size_t n);
int 	strcmp(const char *str1, const char *str2);
int 	strncmp(const char *str1, const char *str2, size_t n);
int 	strcoll(const char *str1, const char *str2);
char 	*strcat(char *dest, const char *src);
char 	*strncat(char *dest, const char *src, size_t n);
char 	*strchr(const char *str, int c);
char 	*strcpy(char *dest, const char *src);
char 	*strncpy(char *dest, const char *src, size_t n);
char 	*strerror(int errnum);
char 	*strpbrk(const char *str1, const char *str2);
char 	*strrchr(const char *str, int c);
char 	*strstr(const char *haystack, const char *needle);
char 	*strtok(char *str, const char *delim);
size_t 	strcspn(const char *str1, const char *str2);
size_t 	strlen(const char *str);
size_t 	strspn(const char *str1, const char *str2);
size_t 	strxfrm(char *dest, const char *src, size_t n);

#include <stdio.h>
FILE 	*fopen(const char *filename, const char *mode);
FILE 	*freopen(const char *filename, const char *mode, FILE *stream);
FILE 	*tmpfile(void);
size_t 	fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
size_t 	fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);
int 	fclose(FILE *stream);
int 	feof(FILE *stream);
int 	ferror(FILE *stream);
int 	fflush(FILE *stream);
int 	fgetpos(FILE *stream, fpos_t *pos);
int 	fseek(FILE *stream, long int offset, int whence);
int 	fsetpos(FILE *stream, const fpos_t *pos);
int 	remove(const char *filename);
int 	rename(const char *old_filename, const char *new_filename);
int 	setvbuf(FILE *stream, char *buffer, int mode, size_t size);
void 	perror(const char *str);
void 	setbuf(FILE *stream, char *buffer);
void 	rewind(FILE *stream);
void 	clearerr(FILE *stream);
char 	*tmpnam(char *str);
long int ftell(FILE *stream);

int 	printf(const char *format, ...);
int 	scanf(const char *format, ...);
int 	fprintf(FILE *stream, const char *format, ...);
int 	fscanf(FILE *stream, const char *format, ...);
int 	sprintf(char *str, const char *format, ...);
int 	sscanf(const char *str, const char *format, ...);
int 	vfprintf(FILE *stream, const char *format, va_list arg);
int 	vprintf(const char *format, va_list arg);
int 	vsprintf(char *str, const char *format, va_list arg);

int 	fgetc(FILE *stream);
int 	fputc(int char, FILE *stream);
int 	getc(FILE *stream);
int 	putc(int char, FILE *stream);
int 	getchar(void);
int 	putchar(int char);
char 	*gets(char *str);
int 	puts(const char *str);
char 	*fgets(char *str, int n, FILE *stream);
int 	fputs(const char *str, FILE *stream);
int 	ungetc(int char, FILE *stream);

#include <stdlib.h>
double 	atof(const char *str)
int 	atoi(const char *str)
long int atol(const char *str)
double 	strtod(const char *str, char **endptr)
long int strtol(const char *str, char **endptr, int base)
unsigned long int strtoul(const char *str, char **endptr, int base)
void 	*calloc(size_t nitems, size_t size)
void 	*malloc(size_t size)
void 	*realloc(void *ptr, size_t size)
void 	free(void *ptr)
void 	abort(void)
void 	*bsearch(const void *key, const void *base, size_t nitems, size_t size, int (*compar)(const void *, const void *))
void 	qsort(void *base, size_t nitems, size_t size, int (*compar)(const void *, const void*))
void 	exit(int status)
int 	atexit(void (*func)(void))
char 	*getenv(const char *name)
int 	system(const char *string)
int 	abs(int x)
long int labs(long int x)
div_t 	div(int numer, int denom)
ldiv_t 	ldiv(long int numer, long int denom)
int 	rand(void)
void 	srand(unsigned int seed)
int 	mblen(const char *str, size_t n)
int 	mbtowc(whcar_t *pwc, const char *str, size_t n)
size_t 	mbstowcs(schar_t *pwcs, const char *str, size_t n)
int 	wctomb(char *str, wchar_t wchar)
size_t 	wcstombs(char *str, const wchar_t *pwcs, size_t n)

// 上面各个函数的举例说明:

/***
void *memchr(const void *str, int c, size_t n);
在参数str所指向的字符串的前n个字节中搜索
第一次出现字符c(一个无符号字符)的位置.
*/
const char str[] = "http://www.runoob.com";
const char ch = ':';
char *ret;
ret = (char *) memchr(str, ch, strlen(str));
if (ret) {
    printf("|%c|之后的字符串是-|%s|\n", ch, ret + 1);
} else {
    printf("返回为NULL\n");
}
/***
输出:
|:|之后的字符串是-|//www.runoob.com|
*/

/***
int memcmp(const void *str1, const void *str2, size_t n);
把 str1 和 str2 的前 n 个字节进行比较。
*/
char str1[15];
char str2[15];
int ret;
memcpy(str1, "abcdef", 6);
memcpy(str2, "ABCDEF", 6);
ret = memcmp(str1, str2, 5);
if (ret > 0) {
    printf("str1大于str2\n");
} else if (ret < 0) {
    printf("str1小于str2\n");
} else {
    printf("str1等于str2\n");
}
/***
输出:
str1大于str2
*/

/***
void *memcpy(void *dest, const void *src, size_t n);
从 src 复制 n 个字符到 dest。
*/
const char src[50] = "http://www.runoob.com";
char dest[50];
memcpy(dest, src, strlen(src) + 1);
printf("dest = %s\n", dest);
/***
输出:
dest = http://www.runoob.com
*/

/***
void *memmove(void *dest, const void *src, size_t n);
另一个用于从str2复制n个字符到str1的函数。
从str2复制n个字符到str1，但是在重叠内存块这方面，
memmove()是比memcpy()更安全的方法。
如果目标区域和源区域有重叠的话，
memmove()能够保证源串在被覆盖之前将
重叠区域的字节拷贝到目标区域中，
复制后源区域的内容会被更改。
如果目标区域与源区域没有重叠，则和 memcpy() 函数功能相同。
*/
const char src[] = "newstringing";
char dest[] = "oldstring";
printf("Before memmove dest = %s, src = %s\n", dest, src);
memmove(dest, src, strlen(src));
printf("After  memmove dest = %s, src = %s\n", dest, src);
/***
输出:
Before memmove dest = oldstring, src = newstringing
After  memmove dest = newstringing, src = newstringing
*/

/***
void *memset(void *str, int c, size_t n);
复制字符 c（一个无符号字符）到参数str
所指向的字符串的前 n 个字符。
*/
char str[50];
strcpy(str, "This is string.h library function");
puts(str);
memset(str, '$', 7);
puts(str);
/***
输出:
This is string.h library function
$$$$$$$ string.h library function
*/

/***
char *strcat(char *dest, const char *src);
把src所指向的字符串追加到dest所指向的字符串的结尾。
该函数返回一个指向最终的目标字符串 dest 的指针。
*/
char src[50], dest[50];
strcpy(src, "This is source");
strcpy(dest, "This is destination");
strcat(dest, src);
printf("最终的目标字符串: |%s|\n", dest);
/***
输出:
鏈€缁堢殑鐩爣瀛楃涓? |This is destinationThis is source|
*/

/***
char *strncat(char *dest, const char *src, size_t n);
把src所指向的字符串追加到dest所指向
的字符串的结尾，直到 n 字符长度为止。
该函数返回一个指向最终的目标字符串 dest 的指针。
*/
char src[50], dest[50];
strcpy(src, "This is source");
strcpy(dest, "This is destination");
strncat(dest, src, 6);
printf("最终的目标字符串: |%s|\n", dest);
/***
输出:
鏈€缁堢殑鐩爣瀛楃涓? |This is destinationThis i|
*/

/***
char *strchr(const char *str, int c);
在参数 str 所指向的字符串中搜索第一次
出现字符c（一个无符号字符）的位置.
该函数返回在字符串 str 中第一次出现字符 c 的位置，如果未找到该字符则返回 NULL。
*/
const char str[] = "http://www.runoob.com";
const char ch = '.';
char *ret = NULL;
ret = strchr(str, ch);
printf("|%c|之后的字符串是 - |%s|\n", ch, ret);
/***
输出:
|.|涔嬪悗鐨勫瓧绗︿覆鏄?- |.runoob.com|
|#|涔嬪悗鐨勫瓧绗︿覆鏄?- |(null)|
*/

/***
int strcmp(const char *str1, const char *str2);
把str1所指向的字符串和str2所指向的字符串进行比较.
该函数返回值如下：
	如果返回值 < 0，则表示 str1 小于 str2。
	如果返回值 > 0，则表示 str2 小于 str1。
	如果返回值 = 0，则表示 str1 等于 str2。
*/
char str1[15];
char str2[15];
int ret;
strcpy(str1, "abcdef");
strcpy(str2, "ABCDEF");
ret = strcmp(str1, str2);
if (ret > 0) {
    printf("str1 大于 str2\n");
} else if (ret < 0) {
    printf("str1 小于 str2\n");
} else {
    printf("str1 等于 str2\n");
}
/***
输出:
str1 大于 str2
*/

/***
int strncmp(const char *str1, const char *str2, size_t n);
把str1和str2进行比较，最多比较前n个字节.
*/
char str1[15];
char str2[15];
int ret;
strcpy(str1, "abcdef");
strcpy(str2, "ABCDEF");
ret = strncmp(str1, str2, 4);
if (ret > 0) {
    printf("str1 大于 str2\n");
} else if (ret < 0) {
    printf("str1 小于 str2\n");
} else {
    printf("str1 等于 str2\n");
}
/***
输出:
str1 大于 str2
*/

/***
int strcoll(const char *str1, const char *str2);
把str1和str2进行比较，结果取决于LC_COLLATE的位置设置.
该函数返回值如下：
	如果返回值 < 0，则表示 str1 小于 str2。
	如果返回值 > 0，则表示 str2 小于 str1。
	如果返回值 = 0，则表示 str1 等于 str2。
*/
char str1[15];
char str2[15];
int ret;
strcpy(str1, "abc");
strcpy(str2, "ABC");
ret = strcoll(str1, str2);
if (ret > 0) {
    printf("str1 大于 str2\n");
} else if (ret < 0) {
    printf("str1 小于 str2\n");
} else {
    printf("str1 等于 str2\n");
}
/***
输出:
str1 大于 str2
*/

/***
char *strcpy(char *dest, const char *src);
把src所指向的字符串复制到dest.
该函数返回一个指向最终的目标字符串 dest 的指针。
*/
char src[40];
char dest[100];
memset(dest, '\0', sizeof(dest));
strcpy(src, "This is runoob.com");
strcpy(dest, src);
printf("最终的目标字符串: %s\n", dest);
/***
输出:
最终的目标字符串: This is runoob.com
*/

/***
char *strncpy(char *dest, const char *src, size_t n);
把src所指向的字符串复制到dest，最多复制n个字符.
*/
char src[40];
char dest[100];
memset(dest, '\0', sizeof(dest));
strcpy(src, "This is runoob.com");
strncpy(dest, src, 10);
printf("最终的目标字符串: %s\n", dest);
/***
输出:
最终的目标字符串: This is ru
*/

/***
size_t strcspn(const char *str1, const char *str2);
检索字符串str1开头连续有几个字符
都不含字符串 str2 中的字符.
该函数返回 str1 开头连续都不含字符串 str2 中字符的字符数。
*/
const char str1[] = "ABCDEF4960910";
const char str2[] = "013";
int len;
len = strcspn(str1, str2);
printf("第一个匹配的字符是在第 %d 个位置\n", len + 1);
/***
输出:
第一个匹配的字符是在第 10 个位置
*/

/***
char *strerror(int errnum);
从内部数组中搜索错误号 errnum，
并返回一个指向错误消息字符串的指针.
该函数返回一个指向错误字符串的指针，该错误字符串描述了错误 errnum。
*/
FILE *fp = NULL;
fp = fopen("file.txt", "r");
if (!fp) {
    printf("Error: %s\n", strerror(errno));
}
/***
输出:
Error: No such file or directory
*/

/***
size_t strlen(const char *str);
计算字符串 str 的长度，直到空结束字符，但不包括空结束字符.
*/
char str[50];
int len;
strcpy(str, "This is runoob.com");
len = strlen(str);
printf("|%s| 的长度是 |%d|\n", str, len);
/***
输出:
|This is runoob.com| 的长度是 |18|
*/

/***
char *strpbrk(const char *str1, const char *str2);
检索字符串str1中第一个匹配字符串str2中字符的字符，
不包含空结束字符.也就是说，依次检验字符串str1中的字符，
当被检验字符在字符串str2中也包含时，则停止检验，并返回该字符位置.
该函数返回 str1 中第一个匹配字符串 str2 中字符的字符数，如果未找到字符则返回 NULL。
*/
const char str1[] = "abcde2fghi3jk4l";
const char str2[] = "34";
char *ret = NULL;
ret = strpbrk(str1, str2);
if (ret) {
    printf("第一个匹配的字符是: %c\n", *ret);
} else {
    printf("未找到字符");
}
/***
输出:
第一个匹配的字符是: 3
*/

/***
char *strrchr(const char *str, int c);
在参数 str 所指向的字符串中搜索最后一次
出现字符 c（一个无符号字符,以 int 形式传递，但是最终会转换回 char 形式。）的位置.
该函数返回 str 中最后一次出现字符 c 的位置。如果未找到该值，则函数返回一个空指针。
*/
const char str[] = "http://www.w3cschool.cc";
const char ch = '.';
char *ret = NULL;
ret = strrchr(str, ch);
printf("|%c| 之后的字符串是 - |%s|\n", ch, ret);
/***
输出:
|.| 之后的字符串是 - |.cc|
*/

/***
size_t strspn(const char *str1, const char *str2);
检索字符串 str1 中第一个不在字符串 str2 中出现的字符下标.
该函数返回 str1 中第一个不在字符串 str2 中出现的字符下标。
*/
const char str1[] = "ABCDEFG019874";
const char str2[] = "ABCD";
int len;
len = strspn(str1, str2);
printf("初始段匹配长度 %d\n", len);
/***
输出:
初始段匹配长度 4
*/

/***
char *strstr(const char *haystack, const char *needle);
在字符串 haystack 中查找第一次出现字符串 needle（不包含空结束字符）的位置.
该函数返回在 haystack 中第一次出现 needle 字符串的位置，如果未找到则返回 null。
*/
const char haystack[20] = "RUNOOB";
const char needle[10] = "NOOB";
char *ret = NULL;
ret = strstr(haystack, needle);
printf("子字符串是: %s\n", ret);
/***
输出:
子字符串是: NOOB
*/

/***
char *strtok(char *str, const char *delim);
分解字符串 str 为一组字符串，delim 为分隔符.
该函数返回被分解的第一个子字符串，如果没有可检索的字符串，则返回一个空指针。
*/
char str[80] = "This is - www.runoob.com - website";
const char s[2] = "-";
char *token = NULL;
token = strtok(str, s);
while (token != NULL) {
    printf("%s\n", token);
    token = strtok(NULL, s);
}
/***
输出:
This is
 www.runoob.com(前面有空格的)
 website
*/

/***
size_t strxfrm(char *dest, const char *src, size_t n);
根据程序当前的区域选项中的 LC_COLLATE 来转换字符串 src 的前 n 个字符，
并把它们放置在字符串 dest 中.
该函数返回被转换字符串的长度，不包括空结束字符。
*/
char src[20];
char dest[20];
int len;
strcpy(src, "W3C School");
len = strxfrm(dest, src, 20);
printf("字符串 |%s| 的长度是: |%d|\n", dest, len);
/***
输出:
字符串 |W3C School| 的长度是: |10|
*/

/***
int fclose(FILE *stream)
关闭流 stream。刷新所有的缓冲区。
如果流成功关闭，则该方法返回零。如果失败，则返回 EOF。
*/
FILE *fp = nullptr;
fp = fopen("file.txt", "w");
fprintf(fp, "%s", "这里是 runoob.com");
fclose(fp);
/***
输出:
*/

/***
void clearerr(FILE *stream)
清除给定流 stream 的文件结束和错误标识符。
这不会失败，且不会设置外部变量 errno，
但是如果它检测到它的参数不是一个有效的流，
则返回 -1，并设置 errno 为 EBADF。
*/
/***
输出:
*/

/***
int feof(FILE *stream)
测试给定流 stream 的文件结束标识符。
*/
/***
输出:
*/

/***
int ferror(FILE *stream)
测试给定流 stream 的错误标识符。
*/
/***
输出:
*/

/***
int fflush(FILE *stream)
刷新流 stream 的输出缓冲区。
*/
/***
输出:
*/

/***
int fgetpos(FILE *stream, fpos_t *pos)
获取流 stream 的当前文件位置，并把它写入到 pos。
*/
/***
输出:
*/

/***
FILE *fopen(const char *filename, const char *mode)
使用给定的模式 mode 打开 filename 所指向的文件。
*/
/***
输出:
*/

/***
size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
从给定流 stream 读取数据到 ptr 所指向的数组中。
*/
/***
输出:
*/

/***
FILE *freopen(const char *filename, const char *mode, FILE *stream)
把一个新的文件名 filename 与给定的打开的流 stream 关联，同时关闭流中的旧文件。
*/
/***
输出:
*/

/***
int fseek(FILE *stream, long int offset, int whence)
设置流 stream 的文件位置为给定的偏移 offset，参数 offset 意味着从给定的 whence 位置查找的字节数。
*/
/***
输出:
*/

/***
int fsetpos(FILE *stream, const fpos_t *pos)
设置给定流 stream 的文件位置为给定的位置。
参数 pos 是由函数 fgetpos 给定的位置。
*/
/***
输出:
*/

/***
long int ftell(FILE *stream)
返回给定流 stream 的当前文件位置。
*/
/***
输出:
*/

/***
size_t fwrite(const void *ptr, 
                size_t size, 
                size_t nmemb, 
                FILE *stream)
把 ptr 所指向的数组中的数据写入到给定流 stream 中。
*/
/***
输出:
*/

/***
int remove(const char *filename)
删除给定的文件名 filename，以便它不再被访问。
*/
/***
输出:
*/

/***
int rename(const char *old_filename,
            const char *new_filename)
把 old_filename 所指向的文件名改为 new_filename。
*/
/***
输出:
*/

/***
void rewind(FILE *stream)
设置文件位置为给定流 stream 的文件的开头。
*/
/***
输出:
*/

/***
void setbuf(FILE *stream, char *buffer)
定义流 stream 应如何缓冲。
*/
/***
输出:
*/

/***
int setvbuf(FILE *stream, 
            char *buffer, 
            int mode, 
            size_t size)
另一个定义流 stream 应如何缓冲的函数。
*/
/***
输出:
*/

/***
FILE *tmpfile(void)
以二进制更新模式(wb+)创建临时文件。
*/
/***
输出:
*/

/***
char *tmpnam(char *str)
生成并返回一个有效的临时文件名，该文件名之前是不存在的。
*/
/***
输出:
*/

/***
int fprintf(FILE *stream, const char *format, ...)
发送格式化输出到流 stream 中。
*/
/***
输出:
*/

/***
int printf(const char *format, ...)
发送格式化输出到标准输出 stdout。
*/
/***
输出:
*/

/***
int sprintf(char *str, const char *format, ...)
发送格式化输出到字符串。
*/
/***
输出:
*/

/***
int vfprintf(FILE *stream, const char *format, va_list arg)
使用参数列表发送格式化输出到流 stream 中。
*/
/***
输出:
*/

/***
int vprintf(const char *format, va_list arg)
使用参数列表发送格式化输出到标准输出 stdout。
*/
/***
输出:
*/

/***
int vsprintf(char *str, const char *format, va_list arg)
使用参数列表发送格式化输出到字符串。
*/
/***
输出:
*/

/***
int fscanf(FILE *stream, const char *format, ...)
从流 stream 读取格式化输入。
*/
/***
输出:
*/

/***
int scanf(const char *format, ...)
从标准输入 stdin 读取格式化输入。
*/
/***
输出:
*/

/***
int sscanf(const char *str, const char *format, ...)
从字符串读取格式化输入。
*/
/***
输出:
*/

/***
int fgetc(FILE *stream)
从指定的流 stream 获取下一个字符（一个无符号字符），并把位置标识符往前移动。
*/
/***
输出:
*/

/***
char *fgets(char *str, int n, FILE *stream)
从指定的流 stream 读取一行，并把它存储在 str 所指向的字符串内。当读取 (n-1) 个字符时，或者读取到换行符时，或者到达文件末尾时，它会停止，具体视情况而定。
*/
/***
输出:
*/

/***
int fputc(int char, FILE *stream)
把参数 char 指定的字符（一个无符号字符）写入到
指定的流 stream 中，并把位置标识符往前移动。
*/
/***
输出:
*/

/***
int fputs(const char *str, FILE *stream)
把字符串写入到指定的流 stream 中，但不包括空字符。
*/
/***
输出:
*/

/***
int getc(FILE *stream)
从指定的流 stream 获取下一个字符（一个无符号字符），
并把位置标识符往前移动。
*/
/***
输出:
*/

/***
int getchar(void)
从标准输入 stdin 获取一个字符（一个无符号字符）。
*/
/***
输出:
*/

/***
char *gets(char *str)
从标准输入 stdin 读取一行，并把它存储在 str 所指向
的字符串中。当读取到换行符时，或者到达文件末尾时，
它会停止，具体视情况而定。
*/
/***
输出:
*/

/***
int putc(int char, FILE *stream)
把参数 char 指定的字符（一个无符号字符）写入到
指定的流 stream 中，并把位置标识符往前移动。
*/
/***
输出:
*/

/***
int putchar(int char)
把参数 char 指定的字符（一个无符号字符）写入到
标准输出 stdout 中。
*/
/***
输出:
*/

/***
int puts(const char *str)
把一个字符串写入到标准输出 stdout，直到空字符，
但不包括空字符。换行符会被追加到输出中。
*/
/***
输出:
*/

/***
int ungetc(int char, FILE *stream)
把字符 char（一个无符号字符）推入到指定的流 stream 中，
以便它是下一个被读取到的字符。
*/
/***
输出:
*/

/***
void perror(const char *str)
把一个描述性错误消息输出到标准错误 stderr。
首先输出字符串 str，后跟一个冒号，然后是一个空格。
*/
/***
输出:
*/

double atof(const char *str)
把参数 str 所指向的字符串转换为一个浮点数（类型为 double 型）。
int atoi(const char *str)
把参数 str 所指向的字符串转换为一个整数（类型为 int 型）。
long int atol(const char *str)
把参数 str 所指向的字符串转换为一个长整数（类型为 long int 型）。
double strtod(const char *str, char **endptr)
把参数 str 所指向的字符串转换为一个浮点数（类型为 double 型）。
long int strtol(const char *str, char **endptr, int base)
把参数 str 所指向的字符串转换为一个长整数（类型为 long int 型）。
unsigned long int strtoul(const char *str, char **endptr, int base)
把参数 str 所指向的字符串转换为一个无符号长整数（类型为 unsigned long int 型）。
void *calloc(size_t nitems, size_t size)
分配所需的内存空间，并返回一个指向它的指针。
void free(void *ptr)
释放之前调用 calloc、malloc 或 realloc 所分配的内存空间。
void *malloc(size_t size)
分配所需的内存空间，并返回一个指向它的指针。
void *realloc(void *ptr, size_t size)
尝试重新调整之前调用 malloc 或 calloc 所分配的 ptr 所指向的内存块的大小。
void abort(void)
使一个异常程序终止。
int atexit(void (*func)(void))
当程序正常终止时，调用指定的函数 func。
void exit(int status)
使程序正常终止。
char *getenv(const char *name)
搜索 name 所指向的环境字符串，并返回相关的值给字符串。
int system(const char *string)
由 string 指定的命令传给要被命令处理器执行的主机环境。
void *bsearch(const void *key, const void *base, size_t nitems, size_t size, int (*compar)(const void *, const void *))
执行二分查找。
void qsort(void *base, size_t nitems, size_t size, int (*compar)(const void *, const void*))
数组排序。
int abs(int x)
返回 x 的绝对值。
div_t div(int numer, int denom)
分子除以分母。
long int labs(long int x)
返回 x 的绝对值。
ldiv_t ldiv(long int numer, long int denom)
分子除以分母。
int rand(void)
返回一个范围在 0 到 RAND_MAX 之间的伪随机数。
void srand(unsigned int seed)
该函数播种由函数 rand 使用的随机数发生器。
int mblen(const char *str, size_t n)
返回参数 str 所指向的多字节字符的长度。
size_t mbstowcs(schar_t *pwcs, const char *str, size_t n)
把参数 str 所指向的多字节字符的字符串转换为参数 pwcs 所指向的数组。
int mbtowc(whcar_t *pwc, const char *str, size_t n)
检查参数 str 所指向的多字节字符。
size_t wcstombs(char *str, const wchar_t *pwcs, size_t n)
把数组 pwcs 中存储的编码转换为多字节字符，并把它们存储在字符串 str 中。
int wctomb(char *str, wchar_t wchar)
检查对应于参数 wchar 所给出的多字节字符的编码。