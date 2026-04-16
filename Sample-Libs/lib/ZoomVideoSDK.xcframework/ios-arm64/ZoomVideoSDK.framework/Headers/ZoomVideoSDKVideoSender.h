/**
 * @file ZoomVideoSDKVideoSender.h
 * @brief Interface for sending custom video data through external video source protocols.
 */

#import <Foundation/Foundation.h>
/**
 * @class ZoomVideoSDKVideoSender
 * @brief A class for users to send video raw data.
 */
@interface ZoomVideoSDKVideoSender : NSObject

/**
 * @brief Sends one frame data.
 * @param frameBuffer The YUVI420 buffer.
 * @param width The frame width.
 * @param height The frame height.
 * @param dataLength The buffer length.
 * @param rotation The buffer rotation.
 * @param format The buffer format.
 */
- (void)sendVideoFrame:(char *)frameBuffer width:(NSUInteger)width height:(NSUInteger)height dataLength:(NSUInteger)dataLength rotation:(ZoomVideoSDKVideoRawDataRotation)rotation format:(ZoomVideoSDKFrameDataFormat)format;

@end

