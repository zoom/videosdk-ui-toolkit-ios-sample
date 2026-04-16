/**
 * @file ZoomVideoSDKAudioRawData.h
 * @brief Interface for audio raw data objects containing audio buffer information and audio channel configurations.
 */

#import <Foundation/Foundation.h>

/**
 * @class ZoomVideoSDKAudioRawData
 * @brief A class that contains the information of audio raw data.
 */
@interface ZoomVideoSDKAudioRawData : NSObject

/**
 * @brief Gets the pointer of audio buffer data.
 */
@property (nonatomic, assign, nullable) char      *buffer;

/**
 * @brief Gets the audio buffer data length.
 */
@property (nonatomic, assign) NSInteger bufferLen;

/**
 * @brief Gets the audio sampling rate.
 */
@property (nonatomic, assign) NSInteger sampleRate;

/**
 * @brief Gets the number of audio channels.
 */
@property (nonatomic, assign) NSInteger channelNum;

/**
 * @brief Gets the timestamp of audio data.
 */
@property(nonatomic, strong, nullable)  NSDate *timeStamp;

/**
 * @brief Determines whether the reference count for the interface pointer can be increased.
 * @return YES if the reference count can be increased. Otherwise, NO.
 * @warning If you call addRef, the SDK will try to hold the raw data buffer until the reference becomes 0. When you finish using the raw data buffer, you must call releaseRef to release it.
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

