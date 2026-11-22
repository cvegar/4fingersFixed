// Copyright © Veridium IP Ltd., 2018. All rights reserved
// This source is the sole property of Veridium IP Ltd and should not be copied
// in full or in part.

#ifndef IOSPlatformCamera_h
#define IOSPlatformCamera_h

@import CoreMedia;
@import Foundation;

@protocol IOSPlatformFourFCamera

- (void)openCamera;
- (void)startPreviewFrames;
- (void)takePicture;
- (void)requestPreviewFrameWithFocusDistance:(float)distance;
- (void)requestPreviewAtDistance:(float)distance;
- (void)takePictureWithFocusDistance:(float)distance;
- (void)stopPreviewFrames;
- (void)closeCamera;

- (float)getFOV;
- (float)getMinimumFocusDistance; // Returns minimum distance, in cm, that this camera can focus on.

- (NSArray<NSNumber*>* _Nonnull)getPreviewResolutionList;
- (void)setPreviewResolutionToWidth:(int)width andHeight:(int)height;
- (NSArray<NSNumber*>* _Nonnull)getFullResolutionList;
- (void)setFullResolutionToWidth:(int)width andHeight:(int)height;
- (void)setExposureRegion:(CGRect)exposureRegion;

@end

// This is a promise that the FourF library will implement this function.
#ifdef __cplusplus
extern "C" {
#endif

#pragma GCC visibility push(default)

void libffid_ios_processPreviewFrame(CMSampleBufferRef _Nonnull frame);

void libffid_ios_processFullFrame(CVPixelBufferRef _Nonnull frame);

void libffid_ios_processFocusSweepPreviewFrame(CMSampleBufferRef _Nonnull frame, float focusDistance_cm);

void libffid_ios_processFullFrameWithDepth(CVPixelBufferRef _Nonnull frame, float focusDistance_cm);

void libffid_ios_fatalError(void);

#pragma GCC visibility pop

#ifdef __cplusplus
}
#endif

#endif /* IOSPlatformCamera_h */
