如果需要编译的模块比较多，我们可能会将对应的模块放置在相应的目录中， 
这样，我们可以在每个目录中定义对应的Android.mk文件（类似于上面的写法）， 
最后，在根目录放置一个Android.mk文件，内容如下： 
include $(call all-subdir-makefiles) 
只需要这一行就可以了，它的作用就是包含所有子目录中的Android.mk文件。

多个模块共用一个Android.mk 
这个文件允许你将源文件组织成模块，这个模块中含有： 
  -静态库(.a文件) 
  -动态库(.so文件) 
只有共享库才能被安装/复制到您的应用软件（APK）包中 
include $(BUILD_STATIC_LIBRARY)，编译出的是静态库 
include $(BUILD_SHARED_LIBRARY)，编译出的是动态库 

#编译静态库  
LOCAL_PATH := $(call my-dir)  
include $(CLEAR_VARS)  

LOCAL_MODULE = libhellos  
LOCAL_CFLAGS = $(L_CFLAGS)  
LOCAL_SRC_FILES = hellos.c  
LOCAL_C_INCLUDES = $(INCLUDES)  
LOCAL_SHARED_LIBRARIES := libcutils  
LOCAL_COPY_HEADERS_TO := libhellos  
LOCAL_COPY_HEADERS := hellos.h  
include $(BUILD_STATIC_LIBRARY)  
  
#编译动态库  
LOCAL_PATH := $(call my-dir)  
include $(CLEAR_VARS)  

LOCAL_MODULE = libhellod  
LOCAL_CFLAGS = $(L_CFLAGS)  
LOCAL_SRC_FILES = hellod.c  
LOCAL_C_INCLUDES = $(INCLUDES)  
LOCAL_SHARED_LIBRARIES := libcutils  
LOCAL_COPY_HEADERS_TO := libhellod  
LOCAL_COPY_HEADERS := hellod.h  
include $(BUILD_SHARED_LIBRARY)  
  
#使用静态库  
LOCAL_PATH := $(call my-dir)  
include $(CLEAR_VARS)  

LOCAL_MODULE := hellos  
LOCAL_STATIC_LIBRARIES := libhellos  
LOCAL_SHARED_LIBRARIES :=  
LOCAL_LDLIBS += -ldl  
LOCAL_CFLAGS := $(L_CFLAGS)  
LOCAL_SRC_FILES := mains.c  
LOCAL_C_INCLUDES := $(INCLUDES)  
include $(BUILD_EXECUTABLE)  
  
#使用动态库  
LOCAL_PATH := $(call my-dir)  
include $(CLEAR_VARS)  

LOCAL_MODULE := hellod  
LOCAL_MODULE_TAGS := debug  
LOCAL_SHARED_LIBRARIES := libc libcutils libhellod  
LOCAL_LDLIBS += -ldl  
LOCAL_CFLAGS := $(L_CFLAGS)  
LOCAL_SRC_FILES := maind.c  
LOCAL_C_INCLUDES := $(INCLUDES)  
include $(BUILD_EXECUTABLE) 



一、编译一个静态库 libstatic_android.a 


LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE    := static_android  
LOCAL_SRC_FILES := libstatic_android.a   
include $(PREBUILT_STATIC_LIBRARY)  

include $(CLEAR_VARS)
LOCAL_MODULE    := fpdfembedsdk
LOCAL_SRC_FILES := xxxx.cpp
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../include/
LOCAL_LDLIBS := -llog -g -L. -ljnigraphics

#fill in the foxit library here
#LOCAL_LDLIBS += libstatic_android.a
LOCAL_STATIC_LIBRARIES := static_android
include $(BUILD_SHARED_LIBRARY) #$(BUILD_STATIC_LIBRARY)




二、编译多个so文件



(1)Android.mk文件首先需要指定LOCAL_PATH变量，用于查找源文件。由于一般情况下android.mk和需要编译的源文件在同一目录下，所以定义成如下形式：

LOCAL_PATH:=$(call my-dir)
上面的语句的意思是将LOCAL_PATH变量定义成本文件所在目录路径。
(2)Android.mk中可以定义多个编译模块，每个编译模块都是以include $(CLEAR_VARS)开始,以include $(BUILD_XXX)结束。
include $(CLEAR_VARS)
CLEAR_VARS由编译系统提供，指定让GNU MAKEFILE为你清除除LOCAL_PATH以外的所有LOCAL_XXX变量，
如LOCAL_MODULE，LOCAL_SRC_FILES，LOCAL_SHARED_LIBRARIES，LOCAL_STATIC_LIBRARIES等。

include $(BUILD_STATIC_LIBRARY)表示编译成静态库
include $(BUILD_SHARED_LIBRARY)表示编译成动态库。
include $(BUILD_EXECUTABLE)表示编译成可执行程序

(3)举例如下(frameworks/base/libs/audioflinger/Android.mk)：

LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)  模块一
ifeq ($(AUDIO_POLICY_TEST),true)
  ENABLE_AUDIO_DUMP := true
endif

LOCAL_SRC_FILES:= \
    AudioHardwareGeneric.cpp \
    AudioHardwareStub.cpp \
    AudioHardwareInterface.cpp
ifeq ($(ENABLE_AUDIO_DUMP),true)
  LOCAL_SRC_FILES += AudioDumpInterface.cpp
  LOCAL_CFLAGS += -DENABLE_AUDIO_DUMP
endif

LOCAL_SHARED_LIBRARIES := \
    libcutils \
    libutils \
    libbinder \
    libmedia \
    libhardware_legacy

ifeq ($(strip $(BOARD_USES_GENERIC_AUDIO)),true)
  LOCAL_CFLAGS += -DGENERIC_AUDIO
endif

LOCAL_MODULE:= libaudiointerface

ifeq ($(BOARD_HAVE_BLUETOOTH),true)
  LOCAL_SRC_FILES += A2dpAudioInterface.cpp
  LOCAL_SHARED_LIBRARIES += liba2dp
  LOCAL_CFLAGS += -DWITH_BLUETOOTH -DWITH_A2DP
  LOCAL_C_INCLUDES += $(call include-path-for, bluez)
endif

include $(BUILD_STATIC_LIBRARY)  模块一编译成静态库

include $(CLEAR_VARS)  模块二
LOCAL_SRC_FILES:=               \
    AudioPolicyManagerBase.cpp
LOCAL_SHARED_LIBRARIES := \
    libcutils \
    libutils \
    libmedia

ifeq ($(TARGET_SIMULATOR),true)
  LOCAL_LDLIBS += -ldl
else
  LOCAL_SHARED_LIBRARIES += libdl
endif

LOCAL_MODULE:= libaudiopolicybase
ifeq ($(BOARD_HAVE_BLUETOOTH),true)
  LOCAL_CFLAGS += -DWITH_A2DP
endif

ifeq ($(AUDIO_POLICY_TEST),true)
  LOCAL_CFLAGS += -DAUDIO_POLICY_TEST
endif

include $(BUILD_STATIC_LIBRARY) 模块二编译成静态库

include $(CLEAR_VARS) 模块三
LOCAL_SRC_FILES:=               \
    AudioFlinger.cpp            \
    AudioMixer.cpp.arm          \
    AudioResampler.cpp.arm      \
    AudioResamplerSinc.cpp.arm  \
    AudioResamplerCubic.cpp.arm \
    AudioPolicyService.cpp
LOCAL_SHARED_LIBRARIES := \
    libcutils \
    libutils \
    libbinder \
    libmedia \
    libhardware_legacy
ifeq ($(strip $(BOARD_USES_GENERIC_AUDIO)),true)
  LOCAL_STATIC_LIBRARIES += libaudiointerface libaudiopolicybase
  LOCAL_CFLAGS += -DGENERIC_AUDIO
else
  LOCAL_SHARED_LIBRARIES += libaudio libaudiopolicy
endif

ifeq ($(TARGET_SIMULATOR),true)
  LOCAL_LDLIBS += -ldl
else
  LOCAL_SHARED_LIBRARIES += libdl
endif

LOCAL_MODULE:= libaudioflinger
ifeq ($(BOARD_HAVE_BLUETOOTH),true)
  LOCAL_CFLAGS += -DWITH_BLUETOOTH -DWITH_A2DP
  LOCAL_SHARED_LIBRARIES += liba2dp
endif

ifeq ($(AUDIO_POLICY_TEST),true)
  LOCAL_CFLAGS += -DAUDIO_POLICY_TEST
endif

ifeq ($(TARGET_SIMULATOR),true)
    ifeq ($(HOST_OS),linux)
        LOCAL_LDLIBS += -lrt -lpthread
    endif
endif

ifeq ($(BOARD_USE_LVMX),true)
    LOCAL_CFLAGS += -DLVMX
    LOCAL_C_INCLUDES += vendor/nxp
    LOCAL_STATIC_LIBRARIES += liblifevibes
    LOCAL_SHARED_LIBRARIES += liblvmxservice
#    LOCAL_SHARED_LIBRARIES += liblvmxipc
endif

include $(BUILD_SHARED_LIBRARY) 模块三编译成动态库

(4)编译一个应用程序(APK)

  LOCAL_PATH := $(call my-dir)
  include $(CLEAR_VARS)
  # Build all java files in the java subdirectory-->直译（建立在java子目录中的所有Java文件）
  LOCAL_SRC_FILES := $(call all-subdir-java-files)
  # Name of the APK to build-->直译（创建APK的名称）
  LOCAL_PACKAGE_NAME := LocalPackage
  # Tell it to build an APK-->直译（告诉它来建立一个APK）
  include $(BUILD_PACKAGE)

(5)编译一个依赖于静态Java库(static.jar)的应用程序

  LOCAL_PATH := $(call my-dir)
  include $(CLEAR_VARS)
  # List of static libraries to include in the package
  LOCAL_STATIC_JAVA_LIBRARIES := static-library
  # Build all java files in the java subdirectory
  LOCAL_SRC_FILES := $(call all-subdir-java-files)
  # Name of the APK to build
  LOCAL_PACKAGE_NAME := LocalPackage
  # Tell it to build an APK
  include $(BUILD_PACKAGE)

(6)编译一个需要用平台的key签名的应用程序

  LOCAL_PATH := $(call my-dir)
  include $(CLEAR_VARS)
  # Build all java files in the java subdirectory
  LOCAL_SRC_FILES := $(call all-subdir-java-files)
  # Name of the APK to build
  LOCAL_PACKAGE_NAME := LocalPackage
  LOCAL_CERTIFICATE := platform
  # Tell it to build an APK
  include $(BUILD_PACKAGE)

(7)编译一个需要用特定key前面的应用程序

  LOCAL_PATH := $(call my-dir)
  include $(CLEAR_VARS)
  # Build all java files in the java subdirectory
  LOCAL_SRC_FILES := $(call all-subdir-java-files)
  # Name of the APK to build
  LOCAL_PACKAGE_NAME := LocalPackage
  LOCAL_CERTIFICATE := vendor/example/certs/app
  # Tell it to build an APK
  include $(BUILD_PACKAGE)

(8)添加一个预编译应用程序

  LOCAL_PATH := $(call my-dir)
  include $(CLEAR_VARS)
  # Module name should match apk name to be installed.
  LOCAL_MODULE := LocalModuleName
  LOCAL_SRC_FILES := $(LOCAL_MODULE).apk
  LOCAL_MODULE_CLASS := APPS
  LOCAL_MODULE_SUFFIX := $(COMMON_ANDROID_PACKAGE_SUFFIX)
  include $(BUILD_PREBUILT)

(9)添加一个静态JAVA库

  LOCAL_PATH := $(call my-dir)
  include $(CLEAR_VARS)
  # Build all java files in the java subdirectory
  LOCAL_SRC_FILES := $(call all-subdir-java-files)
  # Any libraries that this library depends on
  LOCAL_JAVA_LIBRARIES := android.test.runner
  # The name of the jar file to create
  LOCAL_MODULE := sample
  # Build a static jar file.
  include $(BUILD_STATIC_JAVA_LIBRARY)
(10)Android.mk的编译模块中间可以定义相关的编译内容，也就是指定相关的变量如下：

LOCAL_AAPT_FLAGS
LOCAL_ACP_UNAVAILABLE 
LOCAL_ADDITIONAL_JAVA_DIR 
LOCAL_AIDL_INCLUDES 
LOCAL_ALLOW_UNDEFINED_SYMBOLS 
LOCAL_ARM_MODE 
LOCAL_ASFLAGS 
LOCAL_ASSET_DIR 
LOCAL_ASSET_FILES 在Android.mk文件中编译应用程序(BUILD_PACKAGE)时设置此变量，表示资源文件，
                  通常会定义成LOCAL_ASSET_FILES += $(call find-subdir-assets)
LOCAL_BUILT_MODULE_STEM  
LOCAL_C_INCLUDES 额外的C/C++编译头文件路径，用LOCAL_PATH表示本文件所在目录
                 举例如下：
                 LOCAL_C_INCLUDES += extlibs/zlib-1.2.3
                 LOCAL_C_INCLUDES += $(LOCAL_PATH)/src 
LOCAL_CC 指定C编译器
LOCAL_CERTIFICATE  签名认证
LOCAL_CFLAGS 为C/C++编译器定义额外的标志(如宏定义)，举例：LOCAL_CFLAGS += -DLIBUTILS_NATIVE=1
LOCAL_CLASSPATH 
LOCAL_COMPRESS_MODULE_SYMBOLS 
LOCAL_COPY_HEADERS install应用程序时需要复制的头文件，必须同时定义LOCAL_COPY_HEADERS_TO
LOCAL_COPY_HEADERS_TO install应用程序时复制头文件的目的路径
LOCAL_CPP_EXTENSION 如果你的C++文件不是以cpp为文件后缀，你可以通过LOCAL_CPP_EXTENSION指定C++文件后缀名 
                    如：LOCAL_CPP_EXTENSION := .cc
                    注意统一模块中C++文件后缀必须保持一致。
LOCAL_CPPFLAGS 传递额外的标志给C++编译器，如：LOCAL_CPPFLAGS += -ffriend-injection
LOCAL_CXX 指定C++编译器
LOCAL_DX_FLAGS
LOCAL_EXPORT_PACKAGE_RESOURCES
LOCAL_FORCE_STATIC_EXECUTABLE 如果编译的可执行程序要进行静态链接(执行时不依赖于任何动态库)，则设置LOCAL_FORCE_STATIC_EXECUTABLE:=true
                              目前只有libc有静态库形式，这个只有文件系统中/sbin目录下的应用程序会用到，这个目录下的应用程序在运行时通常
                              文件系统的其它部分还没有加载，所以必须进行静态链接。
LOCAL_GENERATED_SOURCES
LOCAL_INSTRUMENTATION_FOR
LOCAL_INSTRUMENTATION_FOR_PACKAGE_NAME
LOCAL_INTERMEDIATE_SOURCES
LOCAL_INTERMEDIATE_TARGETS
LOCAL_IS_HOST_MODULE
LOCAL_JAR_MANIFEST
LOCAL_JARJAR_RULES
LOCAL_JAVA_LIBRARIES 编译Java应用程序和库的时候指定包含的java类库，目前有core和framework两种
                     多数情况下定义成：LOCAL_JAVA_LIBRARIES := core framework
                     注意LOCAL_JAVA_LIBRARIES不是必须的，而且编译APK时不允许定义(系统会自动添加)
LOCAL_JAVA_RESOURCE_DIRS 
LOCAL_JAVA_RESOURCE_FILES 
LOCAL_JNI_SHARED_LIBRARIES 
LOCAL_LDFLAGS 传递额外的参数给连接器(务必注意参数的顺序)
LOCAL_LDLIBS 为可执行程序或者库的编译指定额外的库，指定库以"-lxxx"格式，举例：
             LOCAL_LDLIBS += -lcurses -lpthread
             LOCAL_LDLIBS += -Wl,-z,origin 
LOCAL_MODULE 生成的模块的名称(注意应用程序名称用LOCAL_PACKAGE_NAME而不是LOCAL_MODULE)
LOCAL_MODULE_PATH 生成模块的路径
LOCAL_MODULE_STEM 
LOCAL_MODULE_TAGS 生成模块的标记 
LOCAL_NO_DEFAULT_COMPILER_FLAGS 
LOCAL_NO_EMMA_COMPILE 
LOCAL_NO_EMMA_INSTRUMENT 
LOCAL_NO_STANDARD_LIBRARIES 
LOCAL_OVERRIDES_PACKAGES 
LOCAL_PACKAGE_NAME APK应用程序的名称 
LOCAL_POST_PROCESS_COMMAND
LOCAL_PREBUILT_EXECUTABLES 预编译including $(BUILD_PREBUILT)或者$(BUILD_HOST_PREBUILT)时所用,指定需要复制的可执行文件
LOCAL_PREBUILT_JAVA_LIBRARIES 
LOCAL_PREBUILT_LIBS 预编译including $(BUILD_PREBUILT)或者$(BUILD_HOST_PREBUILT)时所用, 指定需要复制的库.
LOCAL_PREBUILT_OBJ_FILES 
LOCAL_PREBUILT_STATIC_JAVA_LIBRARIES  
LOCAL_PRELINK_MODULE 是否需要预连接处理(默认需要，用来做动态库优化)
LOCAL_REQUIRED_MODULES 指定模块运行所依赖的模块(模块安装时将会同步安装它所依赖的模块)
LOCAL_RESOURCE_DIR
LOCAL_SDK_VERSION
LOCAL_SHARED_LIBRARIES 可链接动态库
LOCAL_SRC_FILES 编译源文件
LOCAL_STATIC_JAVA_LIBRARIES 
LOCAL_STATIC_LIBRARIES 可链接静态库 
LOCAL_UNINSTALLABLE_MODULE 
LOCAL_UNSTRIPPED_PATH
LOCAL_WHOLE_STATIC_LIBRARIES 指定模块所需要载入的完整静态库(这些精通库在链接是不允许链接器删除其中无用的代码)
LOCAL_YACCFLAGS
OVERRIDE_BUILT_MODULE_PATH