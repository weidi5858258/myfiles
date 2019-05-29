#ifndef __MY_JNI_CONST_H_
#define __MY_JNI_CONST_H_

typedef unsigned char BYTE;

/** Public error cord **/
// Result OK.
#define RESULT_OK 0x00000000
#define RESULT_ERR_INTERNAL 0x00030005
#define RESULT_ERR_BINDER_ERROR 0x00030006

/** Private error cord **/
// Do nothing.
#define RESULT_DO_NOTHING 0x00031000

#endif  /* __MY_JNI_CONST_H_ */
