// Copyright © Veridium IP Ltd., 2018. All rights reserved
// This source is the sole property of Veridium IP Ltd and should not be copied
// in full or in part.

#ifndef IOSPlatformFourFCallbacks_h
#define IOSPlatformFourFCallbacks_h

#pragma clang diagnostic warning "-Wobjc-missing-property-synthesis"

@import Foundation;

typedef enum {
    FourFFailureReasonCancel = 0,
    FourFFailureReasonTimeout,
    FourFFailureReasonLiveness,
    FourFFailureReasonMismatch,
    FourFFailureReasonBadConfig,
    FourFFailureReasonException
} VeridiumFourFIOSFailureReason;

typedef void (^fourfDataBlock)(NSArray<NSData*>* _Nonnull data);

typedef void (^fourfFailureBlock)(VeridiumFourFIOSFailureReason reason, NSInteger fail_code);

#pragma GCC visibility push(default)

@protocol IOSPlatformFourFExecutionCallbackHolder

- (id _Nonnull)initWithOnSuccess:(fourfDataBlock _Nonnull)onSuccess
                   andOnFailed:(fourfFailureBlock _Nonnull)onFailed;

@property(copy, readonly, nonnull) fourfDataBlock onSuccess;
@property(copy, readonly, nonnull) fourfFailureBlock onFailed;

@end

__attribute__((visibility("default")))
@interface IOSPlatformFourFExecutionCallbackHolder : NSObject<IOSPlatformFourFExecutionCallbackHolder>
@end

#pragma GCC visibility pop

#endif
