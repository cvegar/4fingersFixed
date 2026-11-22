//
//  VeridiumFourFCameraImpl.h
//  VeridiumFourFCamera
//
//  Created by Lewis Carney on 19/07/2018.
//  Copyright © 2018 veridium. All rights reserved.
//

#ifndef VeridiumFourFCameraImpl_h
#define VeridiumFourFCameraImpl_h

#import <UIKit/UIKit.h>
#import <CoreMotion/CoreMotion.h>
#import <AVFoundation/AVFoundation.h>
#import <Veridium4FLib/Veridium4FLib.h>

@interface VeridiumFourFCameraImpl : NSObject<IOSPlatformFourFCamera, AVCapturePhotoCaptureDelegate>

@property (nonatomic) float zDirectionGravityForCapture;
@property CMMotionManager* motionManager;

-(void) setPreviewHolder:(UIView *) previewHolder;

struct LensFit
{
    float gravityConstant;
    float focal_length;
    float focal_offset;
    bool use_ultrawide_cam;
};

@end


#endif /* VeridiumFourFCameraImpl_h */
