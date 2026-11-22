//
//  VeridiumLocalizationService.h
//  VeridiumCore
//
//  Created by alexandru ilie on 18.03.2024.
//  Copyright © 2024 Veridium IP Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface VeridiumLocalizationService : NSObject

// Singleton instance
+ (nonnull instancetype)sharedInstance;

// Set translations for a specific domain and locale
- (void)setTranslations:(NSDictionary * _Nonnull)translations forDomain:(NSString * _Nonnull)domain locale:(NSString * _Nonnull)locale;

// Get translation for a specific key, domain, and locale
- (nonnull NSString *)translationForKey:(NSString * _Nonnull)key domain:(NSString * _Nonnull)domain;

// Set translations etag for a specific domain
- (void)setTranslationEtag:(NSString * _Nullable)etag forDomain:(NSString * _Nonnull)domain;

// Get translation etag for a specific domain
- (nonnull NSString *)getEtagForDomain:(NSString * _Nonnull)domain;

@end
