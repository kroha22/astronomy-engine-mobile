#include <jni.h>

#include "astronomy_engine_mobile.h"

static jdoubleArray longitude_array(JNIEnv *env, ame_longitude_result_t result)
{
    jdouble values[3];
    values[0] = (jdouble) result.status;
    values[1] = result.degrees;
    values[2] = result.maximum_error_degrees;
    jdoubleArray array = (*env)->NewDoubleArray(env, 3);
    if (array != NULL) {
        (*env)->SetDoubleArrayRegion(env, array, 0, 3, values);
    }
    return array;
}

JNIEXPORT jstring JNICALL
Java_com_nyra_astronomy_mobile_NativeBridge_wrapperVersion(JNIEnv *env, jobject receiver)
{
    (void) receiver;
    return (*env)->NewStringUTF(env, ame_wrapper_version());
}

JNIEXPORT jstring JNICALL
Java_com_nyra_astronomy_mobile_NativeBridge_upstreamVersion(JNIEnv *env, jobject receiver)
{
    (void) receiver;
    return (*env)->NewStringUTF(env, ame_upstream_version());
}

JNIEXPORT jstring JNICALL
Java_com_nyra_astronomy_mobile_NativeBridge_coordinateContract(JNIEnv *env, jobject receiver)
{
    (void) receiver;
    return (*env)->NewStringUTF(env, ame_coordinate_contract());
}

JNIEXPORT jstring JNICALL
Java_com_nyra_astronomy_mobile_NativeBridge_trueNodeContract(JNIEnv *env, jobject receiver)
{
    (void) receiver;
    return (*env)->NewStringUTF(env, ame_true_node_contract());
}

JNIEXPORT jdoubleArray JNICALL
Java_com_nyra_astronomy_mobile_NativeBridge_longitude(
    JNIEnv *env,
    jobject receiver,
    jint body_code,
    jlong epoch_milliseconds
)
{
    (void) receiver;
    return longitude_array(
        env,
        ame_ecliptic_longitude((ame_body_t) body_code, (long long) epoch_milliseconds)
    );
}

JNIEXPORT jdoubleArray JNICALL
Java_com_nyra_astronomy_mobile_NativeBridge_lunarNode(
    JNIEnv *env,
    jobject receiver,
    jint convention_code,
    jlong epoch_milliseconds
)
{
    (void) receiver;
    return longitude_array(
        env,
        ame_lunar_ascending_node(
            (ame_node_convention_t) convention_code,
            (long long) epoch_milliseconds
        )
    );
}
