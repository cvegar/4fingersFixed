// Copyright © Veridium IP Ltd., 2018. All rights reserved
// This source is the sole property of Veridium IP Ltd and should not be copied
// in full or in part.

#ifndef IOSPlatformFourFExternalLogger_h
#define IOSPlatformFourFExternalLogger_h

@import Foundation;

@protocol IOSPlatformFourFExternalLogger

- (void)sendEvent:(NSString*)event withCategory:(NSString*)category;

- (void)logData:(NSData*)data
  withVerbosity:(int)verbosity
         andTag:(NSString*)tag
     andMessage:(NSString*)message;

- (void)logString:(NSString*)message
    withVerbosity:(int)verbosity
          andTag:(NSString*)tag;

- (void)uploadData:(NSData*)data
     withVerbosity:(int)verbosity
            andTag:(NSString*)tag
        andMessage:(NSString*)message;

@end

#endif /* IOSPlatformFourFExternalLogger_h */
