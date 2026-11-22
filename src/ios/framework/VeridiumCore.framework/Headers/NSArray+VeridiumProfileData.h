//
//  NSArray+VeridiumProfileData.h
//  VeridiumCore
//
//  Created by Liviu Bosbiciu on 13.04.2023.
//  Copyright © 2023 Veridium IP Ltd. All rights reserved.
//

#ifndef NSArray_VeridiumProfileData_h
#define NSArray_VeridiumProfileData_h

#import <Foundation/Foundation.h>
#import "VeridiumProfileData.h"

@interface NSArray (VeridiumProfileData)

- (NSArray<NSString*>*)mapToProfileNames;

@end


#endif /* NSArray_VeridiumProfileData_h */
