/**
 * @file ZoomVideoSDKShareSender.h
 * @brief Interface for sending custom share content data through external share source protocols.
 */

#import <Foundation/Foundation.h>
/**
 * @class ZoomVideoSDKShareSender
 * @brief A class for users to send video raw data.
 */
@interface ZoomVideoSDKShareSender : NSObject
/**
 * @brief Sends one frame data.
 * @param frameBuffer The frame buffer YUVI420 buffer.
 * @param width The frame width.
 * @param height The frame height.
 * @param frameLength The buffer length.
 * @param format The buffer format.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)sendShareFrame:(char*)frameBuffer width:(int)width height:(int)height frameLength:(int)frameLength format:(ZoomVideoSDKFrameDataFormat)format;

@end

