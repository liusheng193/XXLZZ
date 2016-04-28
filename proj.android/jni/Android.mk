LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp


FILE_LIST := hellocpp/main.cpp	
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/*.cpp) 
LOCAL_SRC_FILES :=  $(FILE_LIST:$(LOCAL_PATH)/%=%)
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
									$(LOCAL_PATH)/../../cocos2d/libiconv/include \
									$(LOCAL_PATH)/../../cocos2d/libiconv/libcharset \
									$(LOCAL_PATH)/../../cocos2d/libiconv/lib \
									$(LOCAL_PATH)/../../cocos2d/libiconv/libcharset/include \
									$(LOCAL_PATH)/../../cocos2d/libiconv/srclib \
									$(LOCAL_PATH)/../../cocos2d/libiconv \

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static
LOCAL_STATIC_LIBRARIES += libiconv_static
# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)
$(call import-module,libiconv)
# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
