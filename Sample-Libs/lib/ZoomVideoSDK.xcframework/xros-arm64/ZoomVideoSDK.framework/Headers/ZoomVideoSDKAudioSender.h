/**
 * @file ZoomVideoSDKAudioSender.h
 * @brief Interface for sending custom audio data through external audio source protocols.
 */

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN
/**
 * @class ZoomVideoSDKAudioSender
 * @brief A class for users to send audio raw data.
 */
@interface ZoomVideoSDKAudioSender : NSObject

/**
 * @brief Sends audio raw data. Channel number must be mono, and sampling bits must be 16.
 * @param data The address of audio data.
 * @param dataLength The length of audio data. It must be even numbers.
 * @param sampleRate The sampling rate of audio data.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 * @warning Supported audio data properties: Sample Rate: 8000, 11025, 32000, 44100, 48000, 50000, 50400, 96000, 192000, 2822400. Channels: mono. Resolution: little-endian, 16bit.
 */
- (ZoomVideoSDKError)send:(char *)data dataLength:(NSUInteger)dataLength sampleRate:(NSUInteger)sampleRate;

@end

NS_ASSUME_NONNULL_END
