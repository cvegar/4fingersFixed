#import <UIKit/UIKit.h>
#import "EntelFingerPlugin.h"
@import VeridiumCore;
@import Veridium4FBiometrics;
@import Veridium4FUI; 

@interface FingerViewController : UIViewController

+ (instancetype)sharedHelper:(NSString *)callbackid;
- (void)onExportFingerWithLeftCode:(NSInteger)leftCode andRightCode:(NSInteger)rightCode andLiveness:(BOOL)liveness;
- (void)startConfig;

@property (strong, nonatomic) NSString *callbackId;
@property int leftCodeFinger;
@property int rightCodeFinger;
@property BOOL liveness;

@end

