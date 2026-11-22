//
//  otpad.h
//  VeridiumCore
//
//  Created by alexandru ilie on 07.04.2023.
//  Copyright © 2023 Veridium IP Ltd. All rights reserved.
//

#ifndef otpad_h
#define otpad_h

#import <Foundation/Foundation.h>

#ifdef __cplusplus
extern "C" {
#endif
    
NS_ASSUME_NONNULL_BEGIN

char* generateOTPAD(const char* seed, const unsigned char* salt);

NS_ASSUME_NONNULL_END

#ifdef __cplusplus
}
#endif

#endif
