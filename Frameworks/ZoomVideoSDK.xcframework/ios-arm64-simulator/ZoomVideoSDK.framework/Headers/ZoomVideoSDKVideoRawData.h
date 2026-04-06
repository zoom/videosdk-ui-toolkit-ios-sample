/**
 * @file ZoomVideoSDKVideoRawData.h
 * @brief Interface for video raw data objects containing video frame information, buffers, and metadata.
 */

#import <Foundation/Foundation.h>
#import <ZoomVideoSDK/ZoomVideoSDKConstants.h>

/**
 * @class ZoomVideoSDKVideoRawData
 * @brief A class that contains the information of video raw data.
 */
@interface ZoomVideoSDKVideoRawData : NSObject

/**
 * @brief Gets the YUVI420 Y buffer. The Y component represents the luma or brightness values.
 */
@property (nonatomic, assign, nullable) char * yBuffer;

/**
 * @brief Gets the YUVI420 U buffer. The U component represents the chroma values.
 */
@property (nonatomic, assign, nullable) char *uBuffer;

/**
 * @brief Gets the YUVI420 V buffer. The V component represents the chroma values.
 */
@property (nonatomic, assign, nullable) char *vBuffer;

/**
 * @brief Gets the YUVI420 data buffer. The alpha component represents the alpha values.
 */
@property (nonatomic, assign, nullable) char *alphaBuffer;

/**
 * @brief Gets the alpha buffer data length.
 */
@property (nonatomic, assign) NSUInteger alphaBufferLen;

/**
 * @brief Gets the stream size.
 */
@property (nonatomic, assign) CGSize size;

/**
 * @brief Determines whether video raw data is limited.
 */
@property (nonatomic, assign) BOOL isLimited;

/**
 * @brief Gets the raw data format of video data.
 */
@property (nonatomic, assign) ZoomVideoSDKFrameDataFormat format;

/**
 * @brief Gets the video raw data rotation.
 */
@property (nonatomic, assign) ZoomVideoSDKVideoRawDataRotation rotation;

/**
 * @brief Gets the timestamp of video data.
 */
@property(nonatomic, strong, nullable)  NSDate *timeStamp;

/**
 * @brief Determines whether the reference count for accessing the raw data buffer can be increased.
 * @return YES if the reference count can be increased. Otherwise, NO.
 * @warning If you addRef, the SDK will try to hold the raw data buffer until the reference becomes 0. When you finish using the raw data buffer, you must call releaseRef.
 */
- (BOOL)canAddRef;

/**
 * @brief Increases reference count by 1. Adding a reference will ensure that the raw data buffer will not be released.
 * @return YES if the function succeeds. Otherwise, NO.
 * @warning If you addRef, the SDK will try to hold the raw data buffer until the reference becomes 0. When you finish using the raw data buffer, you must call releaseRef.
 */
- (BOOL)addRef;

/**
 * @brief Decreases reference count by 1.
 * @return The current reference count after release.
 * @warning If you addRef, the SDK will try to hold the raw data buffer until the reference becomes 0. When you finish using the raw data buffer, you must call releaseRef.
 */
- (NSInteger)releaseRef;

@end

