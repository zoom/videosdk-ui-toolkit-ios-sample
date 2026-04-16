/**
 * @file ZoomVideoSDKAudioSettingHelper.h
 * @brief Interface for audio settings management including microphone and speaker configuration.
 */

#import <Foundation/Foundation.h>

/**
 * @class ZoomVideoSDKAudioSettingHelper
 * @brief Audio setting interface.
 */
@interface ZoomVideoSDKAudioSettingHelper : NSObject

/**
 * @brief Enables or disables the original input of microphone.
 * @param enable YES to enable the original input of the microphone, NO to disable.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)enableMicOriginalInput:(BOOL)enable;

/**
 * @brief Determines whether the original input of the microphone is enabled.
 * @return YES if the original input of the microphone is enabled. Otherwise, NO.
 */
- (BOOL)isMicOriginalInputEnable;

/**
 * @brief Enables or disables the auto adjust input of microphone.
 * @param enable YES to enable auto adjust input, NO to disable.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)enableAutoAdjustMicVolume:(BOOL)enable;

/**
 * @brief Determines whether the auto adjust input of the microphone is enabled.
 * @return YES if auto adjust input is enabled. Otherwise, NO.
 */
- (BOOL)isAutoAdjustMicVolumeEnabled;

@end
