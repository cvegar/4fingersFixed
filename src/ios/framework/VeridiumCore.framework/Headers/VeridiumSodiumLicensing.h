//
//  VeridiumSodiumLicensing.h
//  VeridiumCore
//
//  Created by razvan on 2/26/18.
//  Copyright © 2019 Veridium IP Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef void(^licensingCompletionBlock)(NSString* _Nullable result, NSError* _Nullable error);

@interface PrivateLicenseStatus:NSObject

+(instancetype _Nonnull)newWithInnerLicense:(NSString* _Nullable)result
                        andOuterLicense:(NSString* _Nullable)outerLicense
                            andError:(NSError* _Nullable)error;

@property (nullable) NSString* innerLicenseJSON;
@property (nullable) NSString* outerLicenseJSON;
@property (nullable) NSError* error;

@end

@interface VeridiumSodiumLicensing : NSObject

+(PrivateLicenseStatus* _Nonnull) decryptServerPairingIdentity:(NSString* _Nonnull) license;
+(PrivateLicenseStatus* _Nonnull) validateLicense:(NSString* _Nonnull) license;
+(PrivateLicenseStatus* _Nonnull) getLicenseContents:(NSString* _Nonnull) license;
@end
