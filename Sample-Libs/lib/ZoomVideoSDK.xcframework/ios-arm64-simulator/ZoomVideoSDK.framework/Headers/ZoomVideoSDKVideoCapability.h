/**
 * @file ZoomVideoSDKVideoCapability.h
 * @brief Interface for video capability information including supported resolutions and video feature capabilities.
 */

#import <Foundation/Foundation.h>

/**
 * @class ZoomVideoSDKVideoCapability
 * @brief Capability information of video source.
 */
@interface ZoomVideoSDKVideoCapability : NSObject
/**
 * @brief Gets the width of video.
 */
@property (nonatomic, assign) int width;
/**
 * @brief Gets the height of video.
 */
@property (nonatomic, assign) int height;
/**
 * @brief Gets the frame rate of video.
 */
@property (nonatomic, assign) int frame;

@end
