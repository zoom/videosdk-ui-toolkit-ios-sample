/**
 * @file ZoomVideoSDKCRCHelper.h
 * @brief Interface for CRC (Circuit-Reliable Communication) operations including H.323 and SIP protocol support.
 */

#import <Foundation/Foundation.h>

/**
 * @class ZoomVideoSDKCRCHelper
 * @brief Helper interface for Cloud Room Connector (CRC) features.
 */
@interface ZoomVideoSDKCRCHelper : NSObject

/**
 * @brief Determines whether the CRC feature is enabled.
 * @return YES if the CRC is enabled. Otherwise, NO.
 * @warning Only available after joining the session. Only available for the host or manager.
 */
- (BOOL)isCRCEnabled;

/**
 * @brief Gets the dial-in SIP address of a live session when CRC is enabled.
 * @return If the function succeeds, it returns the live session SIP address. Otherwise, this function fails and returns nil.
 */
- (NSString * _Nullable)getSessionSIPAddress;

/**
 * @brief Calls the CRC device.
 * @param ipAddr The CRC device's IP address.
 * @param protocol The protocol of the CRC device.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 * @warning Only available for the host or co-host.
 */
- (ZoomVideoSDKError)callCRCDevice:(NSString * _Nullable)ipAddr protocol:(ZoomVideoSDKCRCProtocol)protocol;

/**
 * @brief Cancels the call to the CRC device.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 * @warning Only available for the host or co-host.
 */
- (ZoomVideoSDKError)cancelCallCRCDevice;


@end

