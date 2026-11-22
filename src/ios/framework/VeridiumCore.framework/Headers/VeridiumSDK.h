//
//  VeridiumSDK.h
//  VeridiumCore
//
//  Created by razvan on 11/25/15.
//  Copyright © 2015 HoyosLabs. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "VeridiumUtils.h"
#import "VeridiumBiometricsProtocols.h"
#import "VeridiumDiagnostics.h"
#import "VeridiumLicenseStatus.h"


typedef enum : NSUInteger {
  VeridiumTXSigningProtectionNone = 0,
  VeridiumTXSigningProtectionDevicePasscode,
  VeridiumTXSigningProtectionTouchIDOrDevicePasscode,
  VeridiumTXSigningProtectionTouchID,
  VeridiumTXSigningProtectionTouchIDCurrentSet,
} VeridiumTXSigningProtection;

/*!
 Completion block for the SDK license for online licensing
 */
typedef void(^licenseStatusBlock)(VeridiumLicenseStatus * _Nullable licenseStatus, NSError* _Nullable error);

/*!
 Completion block for the biometric library licenses for online licensing
 */
typedef void(^licenseStatusDictBlock)(NSDictionary<NSString*, VeridiumLicenseStatus*>* _Nullable licenseStatusDict, NSError* _Nullable error);

FOUNDATION_EXPORT NSString * _Nonnull const kBiometricEngineName4F;
FOUNDATION_EXPORT NSString * _Nonnull const kBiometricEngineNameVFace;

/*!
 This `Veridium SDK` class is the main entry point of any interraction with VeridiumSDK.
 
 It is basically a singleton that must be initialized using the `setup:` static method before use
 
 */
@interface VeridiumSDK : NSObject

///---------------------
/// @name Setting up
///---------------------

/*!
 This static initializer used to configure the SDK
 
 If the license provided checks out then all the other SDK functionality can be accessed via the static `sharedSDK` property.

 **IMPORTANT!** This method must be called prior to any call to `VeridiumSDK.sharedSDK`.
 Best place to put it would be in [AppDelegate application: didFinishLaunchingWithOptions:]
 */
+(void) setup;

+(VeridiumLicenseStatus* _Nonnull) loadLicense:(NSString* _Nonnull)license;

+(VeridiumLicenseStatus* _Nonnull) decodeLicense:(NSString* _Nonnull)licJSONString
                          outerLicenseJSONString:(NSString* _Nonnull)outerLicenseJSONString;

+(VeridiumLicenseStatus* _Nonnull) getLicenseStatus:(NSString* _Nonnull) license;

+(VeridiumLicenseStatus* _Nonnull) getLoadedLicenseInfo;


/*!
 This static initializer used to configure the SDK and biometric libraries with online licensing
 
 If the verification of the license provided by the licensing server is successful, then all the other SDK functionality can be accessed via the static `sharedSDK` property.
 
 The SDK verification result can be determined from the `onSdkLicenseCompletion` completion block. If verification is successful, biometric libraries
 can be registered in the same completion block.
 
 The `onBiolibsCompletion` block provides verification results for the biometric libraries that are registered in the `onSdkLicenseCompletion` completion block.
 
 **IMPORTANT!** This method must be called prior to any call to `VeridiumSDK.sharedSDK`.
 Best place to put it would be in [AppDelegate application: didFinishLaunchingWithOptions:]
 
 @param certificate The client certificate in p12 format as base64 string
 @param onSdkLicenseCompletion Completion block for the VeridiumSDK initialisation result
 @return onBiolibsCompletion Completion block for the license validation of registered biometric libraries
 */
+(void) setupWithOnlineLicensing:(NSString* _Nonnull)certificate
          onSdkLicenseCompletion:(licenseStatusBlock _Nonnull) sdkLicenseHandler
             onBiolibsCompletion:(licenseStatusDictBlock _Nonnull) biolibsLicenseHandler;

///---------------------
/// @name Main entry point after setup
///---------------------

/*!
 This is the entry point for all operations on the SDK.
 
 If this is called without proper setup it will throw an exception.
 
 @return the SDK shared instance
 
 @exception NSException with name `SDKNotInitialized` if the SDK hasn't been properly initialized
 */
@property (readonly, class, nonnull) VeridiumSDK* sharedSDK;

///---------------------
/// @name VeridiumSDK Version
///---------------------

/*!
 This method returns a string with the current version of the SDK.
 */
@property (class,readonly,nonnull) NSString* sdkVersion;

/*!
 This method returns a string with the current build version of the SDK.
 */
@property (class,readonly,nonnull) NSString* sdkBuildVersion;

/*!
 Returns whether the device is jailbroken or not.
 */
@property (readonly) BOOL isDeviceJailBroken;


///---------------------
/// @name Push notifications handling
///---------------------

/*!
 Registers the APNS token
 
 This method should be called from your app's [AppDelegate application: didRegisterForRemoteNotificationsWithDeviceToken:] method
 
 @param pushTokenData the push token data returned by the APNS server
 */
-(void) registerPushToken:(NSData* _Nullable) pushTokenData;

/*!
 Enable/disable native pushes
 
 @param enabled the push native mechanism is enabled
 */
-(void) enablePushNotifications:(Boolean) enabled;

/*!
 Returns the registered push token (if registered previously using [VeridiumSDK registerPushToken:]
 */
@property (readonly, nullable) NSString* pushToken;

/*!
 Detection mechanism whether the application can register for pushes or not. The mechanism is based on what push settings user has chosen.
 Note: if the application registeres for pushes, register the push token to the SDK via `registerPushToken:` method.

 @param callBack the callback contianing `canRegister` which indicates if app can register for remote notifications and `canAskForAuthorization` indicating whether if the request for push notification was made or not.
 */
-(void)canRegisterForPushes:(void(^ _Nonnull)(BOOL canRegister, BOOL canAskForAuthorization))callBack;


///---------------------
/// @name Biometrics
///---------------------

/*!
   Registers a biometric authenticator (VeridiumBioAuthenticator implementation) with the SDK
   (_Note:_ the VeridiumTouchIDAuthenticator is automatically registered by the SDK if available on the device)
 
   @param authenticator the VeridiumBioAuthenticator instance
 */
-(void)registerAuthenticator:(id<VeridiumBioAuthenticator> _Nonnull)authenticator;

/*!
 Unregisters a biometric authenticator (VeridiumBioAuthenticator implementation) with the SDK
 
 @param authenticator the VeridiumBioAuthenticator instance
 */
-(void)unregisterAuthenticator:(id<VeridiumBioAuthenticator> _Nonnull)authenticator;


/*!
   Registerd a biometric enroller (VeridiumBioEnroller implementation) with the SDK
 
   @param enroller the VeridiumBioEnroller instance
 */
-(void)registerEnroller:(id<VeridiumBioEnroller> _Nonnull)enroller;

/*!
 Unregisterd a biometric enroller (VeridiumBioEnroller implementation) with the SDK
 
 @param enroller the VeridiumBioEnroller instance
 */
-(void)unregisterEnroller:(id<VeridiumBioEnroller> _Nonnull)enroller;

/*!
 Registers a biometric exporter (VeridiumBioExporter implementation) with the SDK
 
 @param exporter the VeridiumBioExporter instance
 */

-(void)registerExporter:(id<VeridiumBioExporter> _Nonnull)exporter;

/*!
  Registers a biometric library license loader (VeridiumBioLibbraryLoader implementation) with the SDK
 
  @param loader the VeridiumBioLibraryLoader instance
 */
- (void)registerLicenseLoader:(nonnull id<VeridiumBioLibraryLoader>)loader NS_SWIFT_NAME(registerLicenseLoader(_:));

/*!
  Sets up biometric library with given name with license
 
  @param license license string
  @param engineName the biometric library name
    
  @return the VeridiumLicenseStatus instance containing status of the license loading. If there is no VeridiumBioLibraryLoader implementation registered for this name, returns nil
 */
- (nullable VeridiumLicenseStatus *)setupBioLicense:(nonnull NSString *)license forName:(nonnull NSString *)engineName;

/*!
  Updates configuration for all registered biometric authenticators
 
  @param configDict dictionary containing configuration for all registered authenticators
 */
- (void) updateConfigForAuthenticators:(nonnull NSDictionary *)configDict NS_SWIFT_NAME(updateConfigForAuthenticators(configDict:));
- (void) updateConfigForAuthenticators:(nonnull NSArray<id<VeridiumBioAuthenticator>>*) authenticators config: (nonnull NSDictionary *)configDict;

/*!
  Clears configuration for all registered biometric authenticators
 */
- (void) clearConfigForAuthenticators;

/*!
  Deinitialize all registered biometric authenticators
 */
- (void) deinitializeAuthenticators;

/*!
  Initialize a biometric authenticator
 */
- (void)initializeAuthenticator:engineName
                        license:(nonnull NSString*)license;

/*!
 Retuns the registered authenticator for the sepcified method (if any)
 
 @param method the method (aka engine name)
 
 @return the registered authenticator or `nil` if not found
 */
-(id<VeridiumBioAuthenticator> _Nullable) authenticatorForMethod:(NSString * _Nonnull)method;

/*!
 Retuns the registered enroller for the sepcified method (if any)
 
 @param method the method (aka engine name)
 
 @return the registered enroller or `nil` if not found
 */
-(id<VeridiumBioEnroller> _Nullable) enrollerForMethod:(NSString * _Nonnull)method;

/*!
Retuns the registered exporter for the sepcified method (if any)

@param method the method (aka engine name)

@return the registered exporter or `nil` if not found
*/
-(id<VeridiumBioExporter> _Nullable) exporterForMethod:(NSString * _Nonnull)method;

/*!
 The list with engines names of all registered biometric authenticators
 */
@property (readonly,nonatomic,nonnull) NSArray<NSString*>* registeredAuthenticationBiometricMethods;

/*!
 The list with engines names of all registered biometric enrollers
 */
@property (readonly,nonatomic,nonnull) NSArray<NSString*>* registeredEnrollBiometricMethods;

/*!
 The list with engines names of all biometric methods that the device is capable of
 */
@property (readonly,nonatomic,nonnull) NSArray<NSString*>* deviceBiometricsCapabilities;

@property (nonnull) id<VeridiumDiagnosticStorage> diagnostics;

@end
