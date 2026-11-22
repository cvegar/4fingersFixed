//
//  VeridiumBiometricsFourFAuthenticator.h
//  VeridiumFourFBiometrics
//
//  Created by Josiah Kane on 09/08/2018.
//  Copyright © 2018 veridium. All rights reserved.
//

#ifndef VeridiumBiometricsFourFCapturer_h
#define VeridiumBiometricsFourFCapturer_h

@import VeridiumCore;
#import "VeridiumBiometricsFourFUI.h"
#import "Constants.h"

typedef short int NumberOfRetries NS_TYPED_ENUM;
static NumberOfRetries const infinite = -1;
static NumberOfRetries const none = 0;

/*!
 Data class for holding the FourF config params
 */
@interface VeridiumFourFCaptureConfig : NSObject<VeridiumBiometricExportConfig>

/// True if we should create a 4F enrollment template.
@property BOOL do_enroll;
/// True if we should create a 4F authentication template.
@property BOOL do_auth;
/// True if we should capture a 4F image and export it in a standard fingerprint format.
@property BOOL do_export;

/// True if, as well as any of the above, we should package together and record the captured images.
@property BOOL do_debug DEPRECATED_MSG_ATTRIBUTE("Use packDevOutput instead.");


/// Callback which will be used if set during enroll or auth.
/// This will be passed the exported data for external checking, such as against an external database. If it returns true, the enroll or auth will proceed as usual. If not it will fail.
@property VeridiumBiometricExternalValidationBlock _Nullable validate_export;

/// Callback which will be used if set during enroll or auth.
/// This will be passed the exported data, and could be used for tasks such as storing the export in an external database. It should not return any value.
@property VeridiumBiometricExternalHandlingBlock _Nullable process_export;

/// Callback which will be used if set during enroll or auth.
/// This will be passed the dev data. It should be used to store it for later analysis or reprocessing
/// as needed. It should not return any value.
@property VeridiumBiometricExternalHandlingBlock _Nullable process_dev_output;

/// The order of execution of these callbacks for enroll is defined as follows:
//process_dev_output()
//validate_export(), fail if fail
//process_export()
//complete enroll

/// The order of execution of these callbacks for auth is defined as follows:
//process_dev_output()
//validate_export(), fail if fail
//run authentication match, fail if mismatch
//process_export()
//complete authentication success

/// True if we should show the instruction screen
@property BOOL show_instruction_screen;

/*!
 Whether that the engine is using 4F Liveness (defaults to FALSE) and difficulty factor (default 80)
 */
@property BOOL useLiveness;
@property int liveness_factor;

/*!
 Whether to show hand selector screen on UI or not (defaults to FALSE)
 */
@property BOOL showHandSelectorOnEnrolment;

/*!
 Whether to use the left or right hand (defaults to Veridium4FExExportHandDefaultLeft)
 */

/// Create a new config object, which will require further property changes to set up.
-(instancetype _Nonnull ) initForAdvancedConfiguration;
/// Convenience method for creating an enrollment configuration with sane defaults.
-(instancetype _Nonnull ) initForEnrollWithHand:(VeridiumFourFHandChoice)hand andLiveness:(BOOL)liveness;
/// Convenience method for creating an authentication configuration with sane defaults.
-(instancetype _Nonnull ) initForAuthWithHand:(VeridiumFourFHandChoice)hand andLiveness:(BOOL)liveness;

/// Convenience method for setting standard sets of fingers to capture, such as all four fingers on the left hand.
-(void) setHand:(VeridiumFourFHandChoice)chosenHand;
/// Advanced method for setting precise sets of fingers to capture, such as index and middle fingers on both hands. Parameter is a set of numbers between 1 and 10, where 1 is the right thumb and 10 is the left little finger.
-(void) setFingers:(NSSet<NSNumber*>*_Nonnull)chosenFingers;

/// This should not be set directly, use setFingers or setHand.
@property NSSet<NSNumber*>* _Nonnull chosenFingers;
/// Advanced flag indicating whether the user may change which hand is being captured at run time. This should almost always be false.
@property BOOL canSwitch;

-(void) configureTimeoutEnabled:(BOOL)new_timeout_enabled withTimeoutCanRestart:(BOOL)new_timeout_can_restart
               andTimoutSeconds:(int)new_timeout_time_seconds andAllowedRetries:(NumberOfRetries)new_allowed_retries
               andAllowSkipping:(BOOL)new_allow_skipping __deprecated_msg("TimeoutCanRestart is always True and cannot be disabled. Timeout is per capture.");

-(void) configureTimeoutEnabled:(BOOL)new_timeout_enabled andTimoutSeconds:(int)new_timeout_time_seconds
              andAllowedRetries:(NumberOfRetries)new_allowed_retries andAllowSkipping:(BOOL)new_allow_skipping;

@property BOOL timeout_enabled;
@property int timeout_time_seconds;
@property NumberOfRetries allowed_retries;
@property BOOL allow_skipping;

@property VeridiumFourFExportTemplateFormat exportFormat;

/// True if, as well as biometric templates, we should package together and return detailed data
/// Note, this will only function with 4F library debug builds or logging builds.
@property BOOL packDevOutput;
@property BOOL agentMode;
@property BOOL legacyExport;    // revert to the legacy V5 export pipeline
@property BOOL nnExport;        // Neural network ridge extraction export pipeline
@property int qualityThreshold;

@property BOOL pack_wsq;
@property WSQCompressRatio wsq_compression_ratio;
@property BOOL pack_png;
@property BOOL pack_raw;
@property BOOL pack_bmp;
@property BOOL calculate_nfiq;
@property BOOL extra_scaled_image;
@property int fixed_print_width;
@property int fixed_print_height;
@property BOOL pack_audit_image;
@property BOOL pack_fmr_iso;  // Finger Minutiae records
@property BOOL pack_fmr_ansi;
@property NistType nist_type; // choose to output either Type-14, 4 or 9 records
@property BOOL targetIndexFinger;
@property BOOL targetLittleFinger;
@property BOOL allowPartialEnrolment;
@property BOOL allowForceCapture;  // UI can request to bypass quality checks (logging builds only)

// About info for entry into templates
@property NSString* _Nonnull deviceModel;
@property NSString* _Nonnull deviceManufacturer;
@property NSString* _Nonnull participantID;
@property NSString* _Nonnull supplierID;
@property NSString* _Nonnull softwareInfo;
@property NSString* _Nonnull transactionControlID;

// NIST / EBTS customisable fields
@property NSString* _Nullable originating_agency_identifier;
@property NSString* _Nonnull destination_agency_identifier;
@property NSString* _Nonnull attention_indicator;
@property NSString* _Nonnull contributor_case_prefix;
@property NSString* _Nonnull contributor_case_identifier;
@property NSString* _Nonnull contributor_case_identifier_extension;

@end

/*!
 The FourF biometric authenticator.
 */
@interface VeridiumBiometricsFourFCapturer : NSObject<VeridiumBioAuthenticator, VeridiumBioEnroller, VeridiumBioExporter>
-(instancetype _Nonnull )configure:(VeridiumFourFCaptureConfig* _Nonnull)config;
@property id<VeridiumFourFUIDelegate> _Nonnull uiDelegate;
@property (readonly) VeridiumFourFCaptureConfig* _Nonnull config;
@end


#endif /* VeridiumBiometricsFourFAuthenticator_h */
