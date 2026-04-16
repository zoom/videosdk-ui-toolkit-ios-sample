/**
 * @file ZoomVideoSDKNetworkConnectionHelper.h
 * @brief Interface for network connection management including proxy settings and SSL certificate handling.
 */

#import <Foundation/Foundation.h>

/**
 * @class ZoomVideoSDKProxySettingHandler
 * @brief Handles proxy authentication input and provides metadata about the proxy connection.
 */
@interface ZoomVideoSDKProxySettingHandler : NSObject

/**
 * @brief Gets the address of the proxy host.
 */
@property (nonatomic, copy, readonly) NSString* _Nullable proxyHost;

/**
 * @brief Gets the proxy port.
 */
@property (nonatomic, assign, readonly) NSInteger proxyPort;

/**
 * @brief Gets the description of the proxy.
 */
@property (nonatomic, copy, readonly) NSString* _Nullable proxyDescription;

/**
 * @brief Inputs the username and password when using the proxy.
 * @param userName The username when using the proxy.
 * @param psw The password when using the proxy.
 */
- (void)inputUsername:(NSString * _Nullable)userName password:(NSString * _Nullable)psw;

/**
 * @brief Cancels the process to input the username and password of the proxy.
 */
- (void)cancel;

@end

/**
 * @class ZoomVideoSDKSSLCertVerifiedFailHandler
 * @brief Provides details about the SSL certificate used for secure connections.
 */
@interface ZoomVideoSDKSSLCertificateInfo : NSObject

/**
 * @brief Gets the value of whom the SSL certificate is issued to.
 */
@property (nonatomic, copy, readonly) NSString* _Nullable certIssuedTo;

/**
 * @brief Gets the value of who issues the SSL certificate.
 */
@property (nonatomic, copy, readonly) NSString* _Nullable certIssuedBy;

/**
 * @brief Gets the serial number of the SSL certificate.
 */
@property (nonatomic, copy, readonly) NSString* _Nullable certSerialNum;

/**
 * @brief Gets the SSL certificate's fingerprint.
 */
@property (nonatomic, copy, readonly) NSString* _Nullable certFingerprint;


@end



