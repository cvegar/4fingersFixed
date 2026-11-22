//
//  Analytics.h
//  VeridiumAnalytics
//
//  Created by Lewis Carney on 08/08/2017.
//  Copyright © 2017 veridium. All rights reserved.
//


#import <UIKit/UIKit.h>

// Levels of verbosity. Set a level using "analyticsCurrentVerbosity"
// Messages wiuth Levels below this are not tracked
typedef enum analyticsVerbosity
{
    VERBOSE,
    DDEBUG,
    INFO,
    ERROR,
    OFF
} Verbosity;

/* Label messages with components. You can add to the bottom of this list
   Converted to a string in Analytics.m
 */
typedef enum analyticsComponent {
  FOURF,
  VFACE,
  UBA,
  ANALYTICS,
  NATIVE_BIO,
  SERVER,
  CORE
} Component;

extern NSString * const ComponentType_toString[]; // convert component enum to string ID

/*
 Categories of message
 */
typedef enum analyticsCategory
{
    C_INFO,
    C_CAMERA,
    C_CONFIG,
    C_ENROL,
    C_AUTH,
    C_EXPORT,
    C_IMPROT,
    C_ENROL_EXPORT,
    C_LICENCE,
    C_LIVENESS,
    C_SEQUENCE,
    C_ERROR,
    C_UEXCEPTION,  // unhandled
    C_EXCEPTION
} Category;

typedef void(^voidBlock)(void);

extern NSString * const CategoryType_toString[];  // Convert category enum to string

@interface Analytics : NSObject

+siAnalytics;

+(NSString *) NSStringFromVerbosity:(Verbosity) verbosity;

+(void)setLogVerbosity:(Verbosity) v;

+(void)send:(NSString*)event withComponent:(Component)component andCategory:(Category)category;

+(void)logData:(NSData*)data withVerbosity:(Verbosity) verbosity andComponent:(Component)component andTag:(NSString*) tag andMessage:(NSString*)message;

+(void)logString:(NSString*) message withVerbosity:(Verbosity) verbosity
                                     andComponent:(Component)component andTag:(NSString*) tag;

+(void)logString:(NSString*) message withVerbosity:(Verbosity) verbosity
                                     andComponent:(Component)component andCategory:(Category) category;

+(void)provideCredentials:(NSString*) serverID andPassword:(NSString*) password;

+(void)sendDeviceInfo:(Component)component;

+(void)setLogLabel:(NSString*) label;

+(void)splitLog; // indicate a separation. eg. to split files.

@end

