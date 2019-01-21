#ifndef MYSTUDY_LEAK_DETECTOR_C_HPP
#define MYSTUDY_LEAK_DETECTOR_C_HPP

#include <stdio.h>
#include <malloc.h>
#include <string.h>

#define  FILE_NAME_LENGTH               256
//#define  OUTPUT_FILE                    "/root/temp_dir/leak_info.txt"
/***
 加上ptr的目的就是为了防止内存泄漏.
 1.
 当使用同一个指针变量多次申请新的内存时,
 如果不把之前申请的给free掉,那么就会发生内存泄漏,
 因为之前的内存地址已经找不到了.
 2.
 这样做就不用担心同一个指针变量在申请新的内存时有没有把原来的
 内存给free掉了.
 */
#define  new_malloc(ptr, size)           xmalloc(ptr, size, __FILE__, __LINE__)
#define  new_calloc(ptr, elements, size) xcalloc(ptr, elements, size, __FILE__, __LINE__)
#define  new_free(mem_ref)               xfree(mem_ref)

struct _MEM_INFO {
    void *address;
    size_t size;
    char file_name[FILE_NAME_LENGTH];
    size_t line;
};
typedef struct _MEM_INFO MEM_INFO;

struct _MEM_LEAK {
    MEM_INFO mem_info;
    struct _MEM_LEAK *next;
};
typedef struct _MEM_LEAK MEM_LEAK;

void *xmalloc(void *ptr, size_t size, const char *file, size_t line);

void *xcalloc(void *ptr, size_t elements, size_t size, const char *file, size_t line);

void xfree(void *mem_ref);

void add(MEM_INFO alloc_info);

void erase(size_t pos);

void clear(void);

void add_mem_info(void *mem_ref, size_t size, const char *file, size_t line);

void remove_mem_info(void *mem_ref);

void report_mem_leak(void);

//#undef        malloc
//#undef        calloc
//#undef        free

static MEM_LEAK *ptr_start = NULL;
static MEM_LEAK *ptr_next = NULL;

/***
如何使用:
char *ptr1 = (char *) new_malloc(ptr1, 10);
int *ptr2 = (int *) new_calloc(ptr2, 10, sizeof(int));
float *ptr3 = (float *) new_calloc(ptr3, 15, sizeof(float));
ptr1 = (char *) new_malloc(ptr1, 10);
//    new_free(ptr1);
//    new_free(ptr2);
new_free(ptr3);
atexit(report_mem_leak);
 */



/***
 * replacement of malloc
 */
void *xmalloc(void *ptr, size_t size, const char *file, size_t line) {
    // fprintf(stdout, "xmalloc() size: %d, file: %s, line: %d\n", size, file, line);
    new_free(ptr);
    void *ptr_ = malloc(size);
    if (ptr_) {
        // fprintf(stdout, "xmalloc() ptr: %p\n", ptr);
        add_mem_info(ptr_, size, file, line);
    }
    return ptr_;
}

/***
 * replacement of calloc
 */
void *xcalloc(void *ptr, size_t elements, size_t size, const char *file, size_t line) {
    // fprintf(stdout, "xcalloc() elements: %d, size: %d, file: %s, line: %d\n", elements, size, file, line);
    new_free(ptr);
    size_t total_size;
    void *ptr_ = calloc(elements, size);
    if (ptr_) {
        // fprintf(stdout, "xcalloc() ptr: %p\n", ptr);
        total_size = elements * size;
        add_mem_info(ptr_, total_size, file, line);
    }
    return ptr_;
}


/***
 * replacement of free
 */
void xfree(void *mem_ref) {
    if (!mem_ref) {
        return;
    }
    /*if (mem_ref) {
        fprintf(stdout, "xfree() mem_ref: %p\n", mem_ref);
    }*/

    remove_mem_info(mem_ref);
}

/***
 * gets the allocated memory info and adds it to a list
 *
 */
void add_mem_info(void *mem_ref, size_t size, const char *file, size_t line) {
    if (!mem_ref) {
        return;
    }

    MEM_INFO mem_alloc_info;
    /* fill up the structure with all info */
    memset(&mem_alloc_info, 0, sizeof(mem_alloc_info));
    mem_alloc_info.address = mem_ref;
    mem_alloc_info.size = size;
    strncpy(mem_alloc_info.file_name, file, FILE_NAME_LENGTH);
    mem_alloc_info.line = line;

    /* add the above info to a list */
    add(mem_alloc_info);
}

/***
 * if the allocated memory info is part of the list, removes it
 *
 */
void remove_mem_info(void *mem_ref) {
    if (!mem_ref) {
        return;
    }

    size_t index;
    MEM_LEAK *leak_info = ptr_start;

    /* check if allocate memory is in our list */
    for (index = 0;
         leak_info != NULL;
         leak_info = leak_info->next, ++index) {
        if (leak_info->mem_info.address == mem_ref) {
            erase(index);
            break;
        }
    }
}

/***
 * writes all info of the unallocated memory into a file
 */
void report_mem_leak(void) {
    MEM_LEAK *leak_info = NULL;

    leak_info = ptr_start;
    if (leak_info) {
        fprintf(stdout, "%s\n", "Memory Leak Summary");
        fprintf(stdout, "%s\n", "-----------------------------------");
        for (; leak_info != NULL; leak_info = leak_info->next) {
            fprintf(stdout, "address : %d\n", leak_info->mem_info.address);
            fprintf(stdout, "size    : %d bytes\n", leak_info->mem_info.size);
            fprintf(stdout, "file    : %s\n", leak_info->mem_info.file_name);
            fprintf(stdout, "line    : %d\n", leak_info->mem_info.line);
        }
        fprintf(stdout, "%s\n", "-----------------------------------");
    }

#ifdef OUTPUT_FILE
    leak_info = ptr_start;
    if (leak_info) {
        FILE *fp_write = fopen(OUTPUT_FILE, "wt");
        char info[1024];

        if (fp_write) {
            sprintf(info, "%s\n", "Memory Leak Summary");
            fwrite(info, (strlen(info) + 1), 1, fp_write);
            sprintf(info, "%s\n", "-----------------------------------");
            fwrite(info, (strlen(info) + 1), 1, fp_write);

            for (; leak_info != NULL; leak_info = leak_info->next) {
                sprintf(info, "address : %d\n", leak_info->mem_info.address);
                fwrite(info, (strlen(info) + 1), 1, fp_write);
                sprintf(info, "size    : %d bytes\n", leak_info->mem_info.size);
                fwrite(info, (strlen(info) + 1), 1, fp_write);
                sprintf(info, "file    : %s\n", leak_info->mem_info.file_name);
                fwrite(info, (strlen(info) + 1), 1, fp_write);
                sprintf(info, "line    : %d\n", leak_info->mem_info.line);
                fwrite(info, (strlen(info) + 1), 1, fp_write);
                sprintf(info, "%s\n", "-----------------------------------");
                fwrite(info, (strlen(info) + 1), 1, fp_write);
            }
        }
        fclose(fp_write);
    } else {
        int result = access(OUTPUT_FILE, F_OK);
        if (result != -1) {
            remove(OUTPUT_FILE);
        }
    }
#endif

    clear();
}

/***
 * adds allocated memory info. into the list
 *
 */
void add(MEM_INFO alloc_info) {
    MEM_LEAK *mem_leak_info = NULL;
    mem_leak_info = (MEM_LEAK *) malloc(sizeof(MEM_LEAK));
    mem_leak_info->mem_info.address = alloc_info.address;
    mem_leak_info->mem_info.size = alloc_info.size;
    strcpy(mem_leak_info->mem_info.file_name, alloc_info.file_name);
    mem_leak_info->mem_info.line = alloc_info.line;
    mem_leak_info->next = NULL;

    if (!ptr_start) {
        ptr_start = mem_leak_info;
        ptr_next = ptr_start;
    } else {
        ptr_next->next = mem_leak_info;
        ptr_next = ptr_next->next;
    }
}

/***
 * erases memory info. from the list
 *
 */
void erase(size_t pos) {
    if (pos < 0) {
        return;
    }
    size_t index = 0;
    MEM_LEAK *alloc_info = NULL, *temp = NULL;

    if (pos == 0) {
        MEM_LEAK *temp = ptr_start;
        ptr_start = ptr_start->next;
        if (temp) {
            free(temp);
        }
    } else {
        for (index = 0, alloc_info = ptr_start;
             index < pos;
             alloc_info = alloc_info->next, ++index) {
            if (pos == index + 1) {
                temp = alloc_info->next;
                alloc_info->next = temp->next;
                if (temp) {
                    free(temp);
                }
                break;
            }
        }
    }
}

/***
 * deletes all the elements from the list
 */
void clear() {
    MEM_LEAK *alloc_info = ptr_start;
    MEM_LEAK *temp = ptr_start;

    while (alloc_info) {
        alloc_info = alloc_info->next;
        if (temp) {
            free(temp);
        }
        temp = alloc_info;
    }
}

#endif //MYSTUDY_LEAK_DETECTOR_C_HPP
