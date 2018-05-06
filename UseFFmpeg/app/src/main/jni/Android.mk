PATH_TO_THIRDPARTY = $(LOCAL_PATH)/thirdparty
PATH_TO_FFMPEG_PRE_BUILT = $(PATH_TO_THIRDPARTY)/ffmpeg/prebuilt/armeabi-v7a
PATH_TO_AVILIB_PRE_BUILT = $(PATH_TO_THIRDPARTY)/avilib/prebuilt

LOCAL_PATH := $(call my-dir)

#ffmpeg lib
include $(CLEAR_VARS)
LOCAL_MODULE := libavcodec
LOCAL_SRC_FILES := $(PATH_TO_FFMPEG_PRE_BUILT)/libavcodec.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libavfilter
LOCAL_SRC_FILES := $(PATH_TO_FFMPEG_PRE_BUILT)/libavfilter.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libavformat
LOCAL_SRC_FILES := $(PATH_TO_FFMPEG_PRE_BUILT)/libavformat.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libavutil
LOCAL_SRC_FILES := $(PATH_TO_FFMPEG_PRE_BUILT)/libavutil.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libfdk-aac
LOCAL_SRC_FILES := $(PATH_TO_FFMPEG_PRE_BUILT)/libfdk-aac.a
include $(PREBUILT_STATIC_LIBRARY)


include $(CLEAR_VARS)
LOCAL_MODULE := libpostproc
LOCAL_SRC_FILES := $(PATH_TO_FFMPEG_PRE_BUILT)/libpostproc.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libswresample
LOCAL_SRC_FILES := $(PATH_TO_FFMPEG_PRE_BUILT)/libswresample.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libswscale
LOCAL_SRC_FILES := $(PATH_TO_FFMPEG_PRE_BUILT)/libswscale.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libvo-aacenc
LOCAL_SRC_FILES := $(PATH_TO_FFMPEG_PRE_BUILT)/libvo-aacenc.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libx264
LOCAL_SRC_FILES := $(PATH_TO_FFMPEG_PRE_BUILT)/libx264.a
include $(PREBUILT_STATIC_LIBRARY)

#avilib，这个再本地通过ndk编译好了，这里直接使用
include $(CLEAR_VARS)
LOCAL_MODULE := libavi
LOCAL_SRC_FILES := $(PATH_TO_AVILIB_PRE_BUILT)/libavilib.so
include $(PREBUILT_SHARED_LIBRARY)

#所有ffmpeg库
include $(CLEAR_VARS)
LOCAL_MODULE := avlib
LOCAL_CPP_EXTENSION += .cpp .cc


LOCAL_C_INCLUDES += $(LOCAL_PATH)/thirdparty/ffmpeg/include
LOCAL_C_INCLUDES += $(LOCAL_PATH)/thirdparty/avilib/include
LOCAL_C_INCLUDES += $(LOCAL_PATH)/libyuv/include

LOCAL_SRC_FILES += main.cpp AviRender.cpp common/common.cpp FFmpegGraphrcs.cpp
#查找所有目录下面的文
ALL_LIBYUV_SOURCE_FILE := $(wildcard $(LOCAL_PATH)/libyuv/source/*.cc)
#找到的文件名中的$(LOCLA_PATH)路径给去掉
ALL_LIBYUV_SOURCE_FILE := $(ALL_LIBYUV_SOURCE_FILE:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES += $(ALL_LIBYUV_SOURCE_FILE)

#-landroid参数 for native windows
LOCAL_LDLIBS := -llog -landroid -lz
LOCAL_LDLIBS += -ljnigraphics #bitmap头文件
# 这里太坑了，android的mk使用静态库的顺序有要求，依赖的放在前面，这里libavformat必须放在libavcodec前面
LOCAL_STATIC_LIBRARIES := libavformat libavcodec libavfilter libavutil libfdk-aac libpostproc libswresample libswscale libvo-aacenc libx264
LOCAL_SHARED_LIBRARIES := libavi
include $(BUILD_SHARED_LIBRARY)
