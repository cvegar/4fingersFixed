//
//  VeridiumLoggerProtocol.h
//  VeridiumCore
//
//  Created by Liviu Bosbiciu on 19.04.2023.
//  Copyright © 2023 Veridium IP Ltd. All rights reserved.
//

@protocol VeridiumLoggerProtocol <NSObject>

@required
- (NSString * _Nonnull)toLog;

@end
