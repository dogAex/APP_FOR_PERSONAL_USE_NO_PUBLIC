#include <jni.h>
#include <string>
#include <android/log.h>
#include <dlfcn.h>
#include <vulkan/vulkan.h>
#include <cstring>

#define LOG_TAG "QG_Ghost_Core"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

extern "C" JNIEXPORT jstring JNICALL
Java_com_qg_ghost_MainActivity_probeVulkan(JNIEnv* env, jobject) {
    void* vk_lib = dlopen("libvulkan.so", RTLD_NOW);
    if (!vk_lib) return env->NewStringUTF("Failed to load libvulkan.so");

    PFN_vkCreateInstance p_vkCreateInstance = (PFN_vkCreateInstance)dlsym(vk_lib, "vkCreateInstance");
    if (!p_vkCreateInstance) return env->NewStringUTF("Failed to find vkCreateInstance");

    // 【核心修复】使用 C++ 标准的空初始化，避免类型不匹配报错
    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    VkInstance instance;

    VkResult res = p_vkCreateInstance(&createInfo, nullptr, &instance);
    
    if (res == VK_SUCCESS) {
        LOGI("Vulkan Instance Created Successfully!");
        return env->NewStringUTF("Vulkan 底层接管成功！幽灵穿透已就绪。");
    }
    
    // 打印具体的错误码
    char errBuf[64];
    snprintf(errBuf, sizeof(errBuf), "Vulkan Failed: %d", res);
    return env->NewStringUTF(errBuf);
}
