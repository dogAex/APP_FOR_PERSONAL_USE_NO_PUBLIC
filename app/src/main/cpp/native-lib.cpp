#include <jni.h>
#include <string>
#include <android/log.h>
#include <dlfcn.h>
#include <vulkan/vulkan.h>

#define LOG_TAG "QG_Ghost_Core"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

extern "C" JNIEXPORT jstring JNICALL
Java_com_qg_ghost_MainActivity_probeVulkan(JNIEnv* env, jobject) {
    void* vk_lib = dlopen("libvulkan.so", RTLD_NOW);
    if (!vk_lib) return env->NewStringUTF("Failed to load libvulkan.so");

    PFN_vkCreateInstance p_vkCreateInstance = (PFN_vkCreateInstance)dlsym(vk_lib, "vkCreateInstance");
    if (!p_vkCreateInstance) return env->NewStringUTF("Failed to find vkCreateInstance");

    VkInstanceCreateInfo createInfo = {0};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    VkInstance instance;

    if (p_vkCreateInstance(&createInfo, NULL, &instance) == VK_SUCCESS) {
        LOGI("Vulkan Instance Created!");
        return env->NewStringUTF("Vulkan 底层接管成功！幽灵穿透已就绪。");
    }
    return env->NewStringUTF("Vulkan Instance Creation Failed");
}