
#import <Foundation/Foundation.h>

@interface NSData (VeridiumCrypto)
- (NSData* _Nonnull)AES256EncryptWithKey:(NSString * _Nonnull)key;
- (NSData* _Nullable)AES256DecryptWithKey:(NSString * _Nonnull)key;

- (NSData * _Nullable)signWithPrivateKeyOpenSSL:(NSData *_Nonnull)privateKey; // pub:(NSData *_Nonnull)publicKey ;
- (NSData * _Nullable)signWithPrivateKeyPEM:(NSData *_Nonnull)privateKey;

@end

@interface NSString (VeridiumCrypto)
- (NSString* _Nonnull)AES256EncryptToBase64StringWithKey:(NSString * _Nonnull)key;
- (NSString* _Nullable)AES256DecryptBase64StringWithKey:(NSString * _Nonnull)key;
@end

