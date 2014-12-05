LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := CrossApp_cpp_shared

LOCAL_MODULE_FILENAME := libCrossApp_cpp

#define all-cpp-files

#$(patsubst jni/%,%, $(shell find $(LOCAL_PATH)/../../Classes/ $(LOCAL_PATH) -name "*.cpp"))  
#endef

#LOCAL_SRC_FILES := $(call all-cpp-files) \
#hellocpp/main.cpp \

LOCAL_SRC_FILES := hellocpp/main.cpp \
hellocpp/com_dmzj_manhua_push_MessagePushTool.cpp \
hellocpp/com_dmzj_manhua_openapi_OpenApiHelper.cpp \
../../Classes/AppDelegate.cpp \
../../Classes/RootWindow.cpp \
../../Classes/CommonHttpManager.cpp \
../../Classes/DMTabBarController.cpp \
../../Classes/DMClassificationViewController.cpp \
../../Classes/DMClassifCatalogViewController.cpp \
../../Classes/DMRecommendController.cpp \
../../Classes/DMCatalogController.cpp \
../../Classes/DMCatalogCollectionCell.cpp \
../../Classes/DMBriefInfoNanager.cpp \
../../Classes/DMSearchController.cpp \
../../Classes/DMIntroductionController.cpp \
../../Classes/DMSubscribeController.cpp \
../../Classes/DMMoreController.cpp \
../../Classes/DMSettingController.cpp \
../../Classes/DMAboutUsController.cpp \
../../Classes/DMFeedBackController.cpp \
../../Classes/DMDownLoadMainController.cpp \
../../Classes/DMChartsController.cpp \
../../Classes/DMLoginCompleDetailController.cpp \
../../Classes/DMLoginController.cpp \
../../Classes/DMLoginRegistController.cpp \
../../Classes/DMBrowseController.cpp \
../../Classes/DMUserManager.cpp \
../../Classes/DMSettingManager.cpp \
../../Classes/DMSearchHistoryManager.cpp \
../../Classes/DMIntroductionInfoManager.cpp \
../../Classes/DMSelectionDownloadController.cpp \
../../Classes/DMDownloadController.cpp \
../../Classes/DMColllectionManager.cpp \
../../Classes/PushMessageReceiver.cpp \
../../Classes/plat_android/ThreePartyHelper.cpp \
../../Classes/NetWorkUtils/android/NetWorkUtils.cpp \
../../Classes/UrlBrowser/android/OpenBrowser.cpp \
../../Classes/DMDownloadInfoManager.cpp \
../../Classes/umSDK/android/umSDK.cpp \

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
$(LOCAL_PATH)/../../Classes/NetWorkUtils \
$(LOCAL_PATH)/../../Classes/NetWorkUtils/android \
$(LOCAL_PATH)/../../Classes/UrlBrowser \
$(LOCAL_PATH)/../../Classes/UrlBrowser/android \
$(LOCAL_PATH)/../../Classes/umSDK/ \
$(LOCAL_PATH)/../../Classes/umSDK/android/ \


LOCAL_WHOLE_STATIC_LIBRARIES += CrossApp_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += CrossApp_extension_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,CrossApp)
$(call import-module,CrossApp/platform/third_party/android/prebuilt/libcurl)
$(call import-module,CocosDenshion/android)
$(call import-module,extensions)
