# Android.mk使用方式
1. 编译静态库
2. 编译动态库
3. 编译静态库和动态库
4. 使用已有的第三方静态库/动态库编译新的静态库/动态库

## 编译静态库
```
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE:=hello-jni
LOCAL_SRC_FILES:= hello-jni.c
```
## 编译动态库
```
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := hello-jni
LOCAL_SRC_FILES := hello-jni.c
include $(BUILD_SHARED_LIBRARY)
```
## 编译静态库和动态库
```
#静态库
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := `mylib_static`
LOCAL_SRC_FILES := src.c
include $(BUILD_STATIC_LIBRARY)
#动态库，使用前面构建的静态库
include $(CLEAR_VARS)
LOCAL_MODULE := mylib_shared
LOCAL_SRC_FILES := src2.c
LOCAL_STATIC_LIBRARIES := `mylib_static`
include $(BUILD_SHARED_LIBRARY)
```

## 使用已有的第三方静态库/动态库编译新的静态库/动态库
### 方式一
```
LOCAL_LDFLAGS := libcrypto-static.a（不推荐，有编译警告）
```
### 方式二
```
LOCAL_LDLIBS := libcrypto-static.a（不推荐，有编译警告）
```

### 推荐方式三
```
LOCAL_PATH := $(call my-dir)
include$(CLEAR_VARS)
LOCAL_MODULE := `third-lib`
LOCAL_SRC_FILES := libcrypto-static.a
#LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
include $(PREBUILT_STATIC_LIBRARY) #or PREBUILT_SHARED_LIBRARY

include $(CLEAR_VARS)
LOCAL_MODULE := mylib_use_thirdlib
LOCAL_SRC_FILES := src.c
LOCAL_STATIC_LIBRARIES := `third-lib`  #or LOCAL_SHARED_LIBRARY 
include $(BUILD_SHARED_LIBRARY)   #如果编译静态库，需要Application.mk
```

> android的mk使用静态库的顺序有要求，依赖的放在前面，比如libavformat必须放在libavcodec前面

##一些参数的含义
1. LOCAL_EXPORT_C_INCLUDES 
LOCAL_EXPORT_C_INCLUDES 定义确保了任何依赖这个预编译库的模块会自动在自己的 LOCAL_C_INCLUDES 变量中增加到这个预编译库的include目录的路径，从而能够找到其中的头文件。这样就不用再写一次LOCAL_C_INCLUDES 了。