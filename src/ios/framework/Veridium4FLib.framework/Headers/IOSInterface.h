// Copyright © Veridium IP Ltd., 2018. All rights reserved
// This source is the sole property of Veridium IP Ltd and should not be copied
// in full or in part.

#ifndef IOSFourFInterface_h
#define IOSFourFInterface_h

#include "IOSPlatformFourFCallbacks.h"
#include "IOSPlatformFourFCamera.h"
#include "IOSPlatformFourFExternalLogger.h"
#include "IOSPlatformFourFGui.h"

@import Foundation;

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    // Keep in sync with the main InitResult enum
    FourFInitResultFailInitThrewException = -8,
    FourFInitResultFailLibraryInconsistentState,
    FourFInitResultFailDeviceFingerprint,
    FourFInitResultFailLicenceDecode,
    FourFInitResultFailLicenceExpired,
    FourFInitResultFailLicenceVersionMissmatch,
    FourFInitResultFailLicenceBiometricDisabled,
    FourFInitResultFailUnspecified,
    FourFInitResultSuccess = 0,
    FourFInitResultSuccessWithGrace,
    FourFInitResultSuccessAlreadyInitted
} VeridiumFourFInitResult;

#pragma GCC visibility push(default)

VeridiumFourFInitResult libffid_ios_init(NSString* _Nonnull license_string);
void libffid_ios_shutdown(void);

void libffid_ios_register_camera(id<IOSPlatformFourFCamera> _Nonnull camera);
void libffid_ios_register_gui(id<IOSPlatformFourFGui> _Nonnull gui);
void libffid_ios_register_external_logger(id<IOSPlatformFourFExternalLogger> _Nonnull logger);

void libffid_ios_enroll(NSString* _Nonnull config,
                        IOSPlatformFourFExecutionCallbackHolder* _Nonnull callbacks);

void libffid_ios_do_export(NSString* _Nonnull config,
                           IOSPlatformFourFExecutionCallbackHolder* _Nonnull callbacks);

void libffid_ios_authenticate(NSString* _Nonnull config,
                              IOSPlatformFourFExecutionCallbackHolder* _Nonnull callbacks);

typedef struct libffid_ios_match_res
{
    int authorized;  ///< 1 - true, 0 - false
    int error_no;    ///< for audit history
    float score;     ///< for audit history
} IOSPlatformFourFBioMatchRes;

IOSPlatformFourFBioMatchRes libffid_ios_bio_match(NSData* _Nonnull probe, NSData* _Nonnull gallery, NSMutableData* _Nonnull augmented);

NSString* _Nonnull libffid_ios_get_vector_metadata(NSData* _Nonnull gallery);

NSString* _Nonnull libffid_ios_version(void);

#pragma GCC visibility pop

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // IOSFourFInterface_h
