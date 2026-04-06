/**
 * @file ZoomVideoSDKVideoSettingHelper.h
 * @brief Interface for video settings management including face beauty effect configuration.
 */

#import <Foundation/Foundation.h>
#import <ZoomVideoSDK/ZoomVideoSDK.h>

/**
 * @class ZoomVideoSDKVideoSettingHelper
 * @brief Video setting interface.
 */
@interface ZoomVideoSDKVideoSettingHelper : NSObject

/**
 * @brief Enables or disables the face beauty effect for the video stream.
 * @param enable YES to enable the face beauty effect, NO to disable it.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)enableFaceBeautyEffect:(BOOL)enable;

/**
 * @brief Determines whether the face beauty effect is currently enabled.
 * @return YES if the face beauty effect is enabled. Otherwise, NO.
 */
- (BOOL)isFaceBeautyEffectEnabled;

/**
 * @brief Sets the intensity level of the face beauty effect (the strength value).
 * @param strengthValue The desired face beauty strength value (0-100).
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 * @note The face beauty effect must be enabled before adjusting its strength.
 */
- (ZoomVideoSDKError)setFaceBeautyStrengthValue:(unsigned int)strengthValue;

/**
 * @brief Retrieves the current intensity level of the face beauty effect (the strength value).
 * @return The current face beauty strength value (0-100).
 * @note A higher value indicates a stronger beauty effect.
 */
- (unsigned int)getFaceBeautyStrengthValue;

@end

