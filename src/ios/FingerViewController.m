#import "FingerViewController.h"
@import Veridium4FLib;

@interface FingerViewController () {
    int saveCount;
}

@property (nonatomic, assign) NSInteger fingerValue;
@property (nonatomic, assign) BOOL livenessValue;


@end

@implementation FingerViewController
@synthesize callbackId = _callbackId;


+ (instancetype)sharedHelper:(NSString *)callbackid {
    
    static FingerViewController *sharedClass = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        sharedClass = [[self alloc] init];
        sharedClass.callbackId = callbackid;
    });

    return sharedClass;
}


-(void)startConfig{

     [VeridiumSDK setup];
     
    VeridiumLicenseStatus *sdkStatus = [VeridiumSDK loadLicense:@"Rvmuiz9qjUE+2SgoTkpNLyA0xvyY9OKun/aBXnkkVoQRtuUmKCJJczc/9pvmtAcyLOrOGa3y0cLVlx2TDB6hCXsiZGV2aWNlRmluZ2VycHJpbnQiOiJKdGllNEtFMENVbDhWQnpVdWwva3VZcUdiTW5rMllPb2g0d2tLT09nV0JVPSIsImxpY2Vuc2UiOiJCZkNiS0Nqa1NYTUNraEdXL0pHb1ZNcGk4R0gvOEN6Yk9sK280aTVCMHRtb3M5MkFpTXB6NUp4Rkh2bVBJVjdNZkplaGtKMlE3bnpKZHVzcE9JdENDM3NpZEhsd1pTSTZJbE5FU3lJc0ltNWhiV1VpT2lKVFJFc2lMQ0pzWVhOMFRXOWthV1pwWldRaU9qRTNOemcyT1RnNE16RXNJbU52YlhCaGJubE9ZVzFsSWpvaVNXNXpiMngxZEdsdmJuTWdMU0JGYm5SbGJDQXlNREkzSWl3aVkyOXVkR0ZqZEVsdVptOGlPaUpCYm01MVlXd2diR2xqWlc1elpTQm1iM0lnUlc1MFpXd2daVzVrYVc1bklFcDFibVVnTXpBc0lESXdNamNnWEc1Y2JtTnZiUzVsYm5SbGJDNXRiM1pwYkM1aGRYUnZZV04wYVhaaFkybHZiaXhjYm1OdmJTNWxiblJsYkM1dGIzWnBiQzVoY0hCa1pXeHBkbVZ5ZVN4Y2JtTnZiUzV2ZFhSemVYTjBaVzF6Wlc1MFpYSndjbWx6WlM1bGJuUmxiQzVGYm5SbGJGUmxjbkpsYm04c1hHNWpiMjB1YjNWMGMzbHpkR1Z0YzJWdWRHVnljSEpwYzJVdVpXNTBaV3d1VUVWTllYbHZjbWx6ZEdFZ0lpd2lZMjl1ZEdGamRFVnRZV2xzSWpvaWFtOXViV0Z5TG5CbGNtVjZRR2x1YzI5c2RYUnBiMjV6TG5CbElpd2ljM1ZpVEdsalpXNXphVzVuVUhWaWJHbGpTMlY1SWpvaVdGcDZNbGc1TDB4YUwwczNURGR6TVRjd1ptY3paVmRDU1hsWGQybDBWR1p2UzBSaGRXdDZlVlJJVlQwaUxDSnpkR0Z5ZEVSaGRHVWlPakUzTnpnMk5EUTRNREFzSW1WNGNHbHlZWFJwYjI1RVlYUmxJam94T0RFME16STRNREF3TENKbmNtRmpaVVZ1WkVSaGRHVWlPakU0TVRVeE9USXdNREFzSW5WemFXNW5VMEZOVEZSdmEyVnVJanBtWVd4elpTd2lkWE5wYm1kR2NtVmxVa0ZFU1ZWVElqcG1ZV3h6WlN3aWRYTnBibWRCWTNScGRtVkVhWEpsWTNSdmNua2lPbVpoYkhObExDSmlhVzlzYVdKR1lXTmxSWGh3YjNKMFJXNWhZbXhsWkNJNlptRnNjMlVzSW5KMWJuUnBiV1ZGYm5acGNtOXViV1Z1ZENJNmV5SnpaWEoyWlhJaU9tWmhiSE5sTENKa1pYWnBZMlZVYVdWa0lqcG1ZV3h6Wlgwc0ltVnVabTl5WTJVaU9uc2ljR0ZqYTJGblpVNWhiV1Z6SWpwYkltTnZiUzVsYm5SbGJDNXRiM1pwYkM1aGRYUnZZV04wYVhaaFkybHZiaUlzSW1OdmJTNWxiblJsYkM1dGIzWnBiQzVoY0hCa1pXeHBkbVZ5ZVNJc0ltTnZiUzV2ZFhSemVYTjBaVzF6Wlc1MFpYSndjbWx6WlM1bGJuUmxiQzVGYm5SbGJGUmxjbkpsYm04aUxDSmpiMjB1YjNWMGMzbHpkR1Z0YzJWdWRHVnljSEpwYzJVdVpXNTBaV3d1VUVWTllYbHZjbWx6ZEdFaVhTd2ljMlZ5ZG1WeVEyVnlkRWhoYzJobGN5STZXMTE5ZlE9PSJ9"];

    if(!sdkStatus.initSuccess){
        return;
    }

    VeridiumLicenseStatus *status4F = [VeridiumSDK.sharedSDK setupLib4FWithLicense:@"C4WRPtrFB80QYIDMxffib+W2uHHaPCkzdV9ue2qqKRdzxinYxBspK4IYLsLw7qashM3nlpBAbPySdQLj7XY0AHsiZGV2aWNlRmluZ2VycHJpbnQiOiJKdGllNEtFMENVbDhWQnpVdWwva3VZcUdiTW5rMllPb2g0d2tLT09nV0JVPSIsImxpY2Vuc2UiOiJ4RSt1YnVrRk1aMVJFY0g1anRLTk1maWVBaGQyVHN5YXJVbTFFZlhlVUhGcFBQRjJsdVBvejJJM1VhUHpXcTFxS2dDTXFEbXN0L3NPVlM2MnJwMjlBbnNpZEhsd1pTSTZJa0pKVDB4SlFsTWlMQ0p1WVcxbElqb2lORVlpTENKc1lYTjBUVzlrYVdacFpXUWlPakUzTnpnMk9UZzRNekUxTkRVc0ltTnZiWEJoYm5sT1lXMWxJam9pU1c1emIyeDFkR2x2Ym5NZ0xTQkZiblJsYkNBeU1ESTNJaXdpWTI5dWRHRmpkRWx1Wm04aU9pSkJibTUxWVd3Z2JHbGpaVzV6WlNCbWIzSWdSVzUwWld3Z1pXNWthVzVuSUVwMWJtVWdNekFzSURJd01qY2dYRzVjYm1OdmJTNWxiblJsYkM1dGIzWnBiQzVoZFhSdllXTjBhWFpoWTJsdmJpeGNibU52YlM1bGJuUmxiQzV0YjNacGJDNWhjSEJrWld4cGRtVnllU3hjYm1OdmJTNXZkWFJ6ZVhOMFpXMXpaVzUwWlhKd2NtbHpaUzVsYm5SbGJDNUZiblJsYkZSbGNuSmxibThzWEc1amIyMHViM1YwYzNsemRHVnRjMlZ1ZEdWeWNISnBjMlV1Wlc1MFpXd3VVRVZOWVhsdmNtbHpkR0VnSWl3aVkyOXVkR0ZqZEVWdFlXbHNJam9pYW05dWJXRnlMbkJsY21WNlFHbHVjMjlzZFhScGIyNXpMbkJsSWl3aWMzVmlUR2xqWlc1emFXNW5VSFZpYkdsalMyVjVJam9pV0ZwNk1sZzVMMHhhTDBzM1REZHpNVGN3Wm1jelpWZENTWGxYZDJsMFZHWnZTMFJoZFd0NmVWUklWVDBpTENKemRHRnlkRVJoZEdVaU9qRTNOemcyTkRRNE1EQXdNREFzSW1WNGNHbHlZWFJwYjI1RVlYUmxJam94T0RFME16STRNREF3TURBd0xDSm5jbUZqWlVWdVpFUmhkR1VpT2pFNE1UVXhPVEl3TURBd01EQXNJblZ6YVc1blUwRk5URlJ2YTJWdUlqcG1ZV3h6WlN3aWRYTnBibWRHY21WbFVrRkVTVlZUSWpwbVlXeHpaU3dpZFhOcGJtZEJZM1JwZG1WRWFYSmxZM1J2Y25raU9tWmhiSE5sTENKaWFXOXNhV0pHWVdObFJYaHdiM0owUlc1aFlteGxaQ0k2Wm1Gc2MyVXNJbkoxYm5ScGJXVkZiblpwY205dWJXVnVkQ0k2ZXlKelpYSjJaWElpT21aaGJITmxMQ0prWlhacFkyVlVhV1ZrSWpwbVlXeHpaWDBzSW1abFlYUjFjbVZ6SWpwN0ltSmhjMlVpT25SeWRXVXNJbk4wWlhKbGIweHBkbVZ1WlhOeklqcDBjblZsTENKbGVIQnZjblFpT25SeWRXVjlMQ0psYm1admNtTmxaRkJ5WldabGNtVnVZMlZ6SWpwN0ltMWhibVJoZEc5eWVVeHBkbVZ1WlhOeklqcG1ZV3h6Wlgwc0luWmxjbk5wYjI0aU9pSXFMaW9pZlE9PSJ9"];

    if(!status4F.initSuccess){
        return;
    }
    
    //[VeridiumSDK.sharedSDK register4FUIEnroller];
    [VeridiumSDK.sharedSDK register4FUIExporter];
    [VeridiumSDK.sharedSDK register4FUIAuthenticator];
    

    NSString *ver1 = VeridiumSDK.sdkVersion;
    NSString *ver2 = VeridiumSDK.sharedSDK.lib4FVersion;
    NSLog(@"VeridiumSDK: %@", ver1);
    NSLog(@"Veridium4FSDK: %@", ver2);
    
    return;
}



- (void)viewDidLoad {
    [super viewDidLoad];
}


//NSInteger *fingerValue;
//Boolean *livenessValue;

- (void)onExportFingerWithLeftCode:(NSInteger)leftCode andRightCode:(NSInteger)rightCode andLiveness:(BOOL)liveness {
    
    
  
    if (rightCode == 0) {
          return;
    }
  
  	self.fingerValue = rightCode;
    self.livenessValue = liveness;
    
    [self startConfig];
    [self initiateVeridium];
}



- (void) initiateVeridium {
    VeridiumFourFCaptureConfig *configVeridium = [[VeridiumFourFCaptureConfig alloc] init];
    NSMutableSet *fingers = [[NSMutableSet alloc] init];
    NSNumber *finger = [NSNumber numberWithInteger: self.fingerValue];
    [fingers addObject:finger];
    
    [configVeridium setExportFormat:FORMAT_JSON];
    [configVeridium setFixed_print_width:(512)];
    [configVeridium setFixed_print_height:(512)];
    [configVeridium setWsq_compression_ratio:COMPRESS_5to1];
    [configVeridium setPack_wsq:YES];
    [configVeridium setPack_bmp:NO];
    [configVeridium setPack_raw:NO];
    [configVeridium setPack_png:NO];
    [configVeridium setUseLiveness:YES];
    [configVeridium setLiveness_factor:99];
   // [configVeridium setActive_liveness_beta:*livenessValue];
    [configVeridium setTargetIndexFinger:NO];
    [configVeridium setTargetLittleFinger:NO];
    [configVeridium setPack_audit_image:YES];
    [configVeridium setCalculate_nfiq:YES];
    [configVeridium setNist_type:Nist_type_T14_9];
    [configVeridium setFingers:fingers];
    [configVeridium setCanSwitch:NO];
    //[configVeridium setDo_debug:NO];
    [configVeridium setPackDevOutput:NO];
    configVeridium.do_export = YES;
    
    //[configVeridium configureTimeoutEnabled:YES withTimeoutCanRestart:YES andTimoutSeconds:45 andAllowedRetries:3];
    [configVeridium configureTimeoutEnabled:YES andTimoutSeconds:45 andAllowedRetries:3 andAllowSkipping:NO];
    
    [VeridiumBiometricsFourFService exportTemplate:configVeridium onSuccess:^(NSData * _Nonnull data) {
        NSError *e = nil;
        NSArray *json = [NSJSONSerialization JSONObjectWithData:data options:NSJSONReadingMutableContainers error: &e];
        
        NSArray *fingerPrints = [json valueForKey: @"Fingerprints"];
        NSDictionary *wsq = [[[fingerPrints objectAtIndex:0] valueForKey:@"FingerImpressionImage"] valueForKey:@"BinaryBase64ObjectWSQ"];
        
        NSDictionary *auditImage = [json valueForKey: @"AuditImage_0"];
        NSDictionary *img = [auditImage valueForKey:@"BinaryBase64ObjectJPG"];
        
        NSMutableDictionary * dict = [NSMutableDictionary new];
                                    [dict setObject:wsq forKey:@"wsq"];
                                    [dict setObject:img forKey:@"img"];

        [EntelFingerPlugin.entelFingerPlugin sendResponseFingerDict:dict callbackId:self.callbackId];
        
    } onFail:^{
      [EntelFingerPlugin.entelFingerPlugin sendResponseFinger:@"FAIL" callbackId:self.callbackId];
    } onCancel:^{
      [EntelFingerPlugin.entelFingerPlugin sendResponseFinger:@"CANCEL" callbackId:self.callbackId];
    } onError:^(NSError * _Nonnull error) {
      [EntelFingerPlugin.entelFingerPlugin sendResponseFinger:@"ERROR" callbackId:self.callbackId];
    }];
}

@end