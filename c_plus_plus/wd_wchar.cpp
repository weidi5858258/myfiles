#include <stdio.h>
#include <wchar.h>

/***
 wint_t fgetwc(FILE* stream);
 */
void wd_fgetwc(void){
    FILE* pFile;
    wint_t wc;
    int n = 0;
    pFile = fopen("myfile.txt", "r");
    if(pFile != NULL){
        do{
            wc = fgetwc(pFile);
            wprintf(L"%d", wc);
            if(wc == L'$'){
                n++;
            }
        }while(wc != WEOF);
        fclose(pFile);
        wprintf(L"The file contains %d dollar sign characters ($).\n", n);
    }
}
