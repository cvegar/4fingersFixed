//
//  SigningHelper.h
//  VeridiumCore
//
//  Created by alexandru ilie on 23.02.2024.
//  Copyright © 2024 Veridium IP Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface VeridiumCertSigningHelper : NSObject

+ (NSString *)signString:(NSString *)stringToSign withCertificateBase64:(NSString *)certificateBase64 password:(NSString *)password;

+ (NSString *)extractPublicCertificateBase64FromBase64P12:(NSString *)base64P12 password:(NSString *)password;
    
@end
