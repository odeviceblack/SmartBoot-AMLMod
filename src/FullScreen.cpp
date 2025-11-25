#include "FullScreen.hpp"
#include "main.hpp"

void hideSystemUI()
{
	JNIEnv* env = aml->GetJNIEnvironment();
	if(!env) return logger->Error("invalid JNI environment!");

	jclass nv_util_class = env->FindClass("com/nvidia/devtech/NvUtil");
	if(!nv_util_class) return logger->Error("nv_util_class not found!");
	safeClear(env);

	jmethodID getInstance = env->GetStaticMethodID(nv_util_class, "getInstance", "()Lcom/nvidia/devtech/NvUtil;");
	if(!getInstance) return logger->Error("getInstance not found!");
	safeClear(env);

	jobject instance_obj = env->CallStaticObjectMethod(nv_util_class, getInstance);
	if(!instance_obj) return logger->Error("instance_obj not found!");
	safeClear(env);

	jfieldID activityField = env->GetFieldID(nv_util_class, "activity", "Landroid/app/Activity;");
	if(!activityField) return logger->Error("activityField not found!");
	safeClear(env);

	jobject activity_obj = env->GetObjectField(instance_obj, activityField);
	if(!activity_obj) return logger->Error("activity_obj not found!");
	safeClear(env);

	jclass activity_class = env->FindClass("com/nvidia/devtech/NvEventQueueActivity");
	if(!activity_class) return logger->Error("activity_class not found!");
	safeClear(env);

	jmethodID hideSystemUI = env->GetMethodID(activity_class, "hideSystemUI", "()V");
	if(!hideSystemUI) return logger->Error("hideSystemUI not found!");
	safeClear(env);

	env->CallVoidMethod(activity_obj, hideSystemUI);
	safeClear(env);

	env->DeleteLocalRef(nv_util_class);
	env->DeleteLocalRef(instance_obj);
	env->DeleteLocalRef(activity_obj);
	env->DeleteLocalRef(activity_class);
}
