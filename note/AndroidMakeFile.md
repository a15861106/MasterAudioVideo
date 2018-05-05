#Android.mk使用方式
1. 编译静态库
2. 编译动态库
3. 编译静态库和动态库
4. 使用已有的第三方静态库/动态库编译新的静态库/动态库

##编译静态库
LOCAL\_PATH := $(call my-dir) include $(CLEAR\_VARS) LOCAL\_MODULE  := hello-jni LOCAL_\SRC_FILES := hello-jni.c
##编译动态库
LOCAL\_PATH := $(call my-dir) include $(CLEAR\_VARS) LOCAL\_MODULE := hello-jni LOCAL\_SRC\_FILES := hello-jni.c include $(BUILD\_SHARED\_LIBRARY)
## 编译静态库和动态库
\#静态库
LOCAL\_PATH := $(call my-dir)
include $(CLEAR\_VARS) LOCAL\_MODULE := `mylib_static` LOCAL\_SRC\_FILES := src.c include $(BUILD\_STATIC\_LIBRARY)
\#动态库，使用前面构建的静态库
include $(CLEAR\_VARS) LOCAL\_MODULE := mylib\_shared LOCAL\_SRC\_FILES := src2.c
LOCAL\_STATIC\_LIBRARIES := `mylib_static`
include $(BUILD\_SHARED\_LIBRARY)
##使用已有的第三方静态库/动态库编译新的静态库/动态库
###方式一
LOCAL\_LDFLAGS := libcrypto-static.a（不推荐，有编译警告）
###方式二
LOCAL\_LDLIBS := libcrypto-static.a（不推荐，有编译警告）
###推荐方式三
LOCAL\_PATH := $(call my-dir)
include$(CLEAR\_VARS) LOCAL\_MODULE := `third-lib` LOCAL\_SRC\_FILES := libcrypto-static.a #LOCAL\_EXPORT\_C\_INCLUDES := $(LOCAL\_PATH)/include
include $(PREBUILT\_STATIC\_LIBRARY) #or PREBUILT\_SHARED\_LIBRARY

include $(CLEAR\_VARS) LOCAL\_MODULE := mylib\_use\_thirdlib LOCAL\_SRC\_FILES := src.c
LOCAL\_STATIC\_LIBRARIES := `third-lib`  #or LOCAL\_SHARED\_LIBRARY 
include $(BUILD\_SHARED\_LIBRARY)   #如果编译静态库，需要Application.mk


##一些参数的含义
1. LOCAL\_EXPORT\_C\_INCLUDES 
LOCAL\_EXPORT\_C\_INCLUDES 定义确保了任何依赖这个预编译库的模块会自动在自己的 LOCAL\_C\_INCLUDES 变量中增加到这个预编译库的include目录的路径，从而能够找到其中的头文件。这样就不用再写一次LOCAL\_C\_INCLUDES 了。