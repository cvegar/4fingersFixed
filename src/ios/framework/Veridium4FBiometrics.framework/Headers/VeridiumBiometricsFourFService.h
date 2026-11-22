//
//  VeridiumBiometrics4FExService.h
//  Veridium4FExportBiometrics
//
//  Copyright © 2018 Veridium IP Ltd. All rights reserved.
//

#ifndef VeridiumBiometricsFourFService_h
#define VeridiumBiometricsFourFService_h

#import <Foundation/Foundation.h>

#import "Constants.h"
#import "VeridiumBiometricsFourFCapturer.h"

@interface VeridiumBiometricsFourFService : NSObject

/*!/
 Ask the biometric engine to export a biometric in a configurable format.

 @param config          VeridiumBiometricExportConfig object containing configuration options
 @param onSuccessBlock  success callback VeridiumBiometricExternalHandlingBlock
 @param onFailureBlock  fail callback VeridiumBiometricFailureBlock; receives the failure reason and code
 @param onCancelBlock   cancel callback voidBlock
 @param onErrorBlock    error callback errorBlock; receives the error message
 */
+(void)exportTemplate:(id<VeridiumBiometricExportConfig> _Nonnull) config
            onSuccess:(VeridiumBiometricExternalHandlingBlock _Nonnull) onSuccessBlock
            onFailure:(VeridiumBiometricFailureBlock _Nonnull) onFailureBlock
             onCancel:(voidBlock _Nonnull) onCancelBlock
              onError:(errorBlock _Nonnull) onErrorBlock;


/*!/
 Ask the biometric engine to export a biometric in a configurable format and a Veridium
 enrolment template.

 @param config          VeridiumBiometricExportConfig object containing configuration options
 @param onSuccessBlock  success callback VeridiumBiometricFullVectorAndExternalHandlingBlock
 @param onFailureBlock  fail callback VeridiumBiometricFailureBlock; receives the failure reason and code
 @param onCancelBlock   cancel callback voidBlock
 @param onErrorBlock    error callback errorBlock; receives the error message
 */
+(void)exportAndEnrollTemplate:(VeridiumFourFCaptureConfig * _Nonnull)config
                     onSuccess:(VeridiumBiometricFullVectorAndExternalHandlingBlock _Nonnull) onSuccessBlock
                     onFailure:(VeridiumBiometricFailureBlock _Nonnull) onFailureBlock
                      onCancel:(voidBlock _Nonnull) onCancelBlock
                       onError:(errorBlock _Nonnull) onErrorBlock;

/*!
 DEPRECATED 5.17.0 Uses a void onFailBlock
 Exports data using the requested biometric export methods
 
 
 @param config         VeridiumBiometricExportConfig object containing configuration options
 @param onSuccessBlock success callback VeridiumBiometricExternalHandlingBlock
 @param onFailBlock    fail callback voidBlock
 @param onCancelBlock  cancel callback
 @param onErrorBlock   error callback errorBlock; receives the error as an NSError
 */
+(void)exportTemplate:(VeridiumFourFCaptureConfig * _Nonnull)config
            onSuccess:(VeridiumBiometricExternalHandlingBlock _Nonnull) onSuccessBlock
               onFail:(voidBlock _Nonnull) onFailBlock
             onCancel:(voidBlock _Nullable) onCancelBlock
              onError:(errorBlock _Nonnull) onErrorBlock DEPRECATED_MSG_ATTRIBUTE("Use onFailure with a VeridiumBiometricFailureBlock block");

/*!
 DEPRECATED 5.17.0 Uses a void onFailBlock
 Exports data using the requested biometric export methods
 */
+(void)exportAndEnrollTemplate:(VeridiumFourFCaptureConfig * _Nonnull)config
                     onSuccess:(VeridiumBiometricFullVectorAndExternalHandlingBlock _Nonnull) onSuccessBlock
                        onFail:(voidBlock _Nonnull) onFailBlock
                      onCancel:(voidBlock _Nullable) onCancelBlock
                       onError:(errorBlock _Nonnull) onErrorBlock DEPRECATED_MSG_ATTRIBUTE("Use onFailure with a VeridiumBiometricFailureBlock block");


@end


#endif /* VeridiumBiometricsFourFService_h */
