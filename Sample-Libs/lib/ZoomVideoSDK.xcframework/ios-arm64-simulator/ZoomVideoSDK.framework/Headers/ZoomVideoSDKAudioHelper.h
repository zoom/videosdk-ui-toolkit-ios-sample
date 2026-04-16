/**
 * @file ZoomVideoSDKAudioHelper.h
 * @brief Interface for managing audio operations including start/stop, mute/unmute, audio device control, and audio raw data subscription.
 */

#import <Foundation/Foundation.h>
#import <ZoomVideoSDK/ZoomVideoSDK.h>

/**
 * @class ZoomVideoSDKAudioDevice
 * @brief Represents an audio device used in the Zoom Video SDK.
 */
@interface ZoomVideoSDKAudioDevice : NSObject
/**
 * @brief Gets the audio device type.
 * @return The audio device type.
 */
- (AVAudioSessionPort _Nullable)getAudioSourceType;

/**
 * @brief Gets the audio device name.
 * @return If the function succeeds, it returns the audio device name. Otherwise, this function fails and returns nil.
 */
- (NSString *_Nullable)getAudioName;
@end


/**
 * @class ZoomVideoSDKAudioHelper
 * @brief A class to operate audio actions.
 */
@interface ZoomVideoSDKAudioHelper : NSObject

/**
 * @brief Starts audio.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)startAudio;

/**
 * @brief Stops audio.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)stopAudio;

/**
 * @brief Mutes the user's audio.
 * @param user The user whose audio will be muted. Use nil to mute self.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 * @warning Only host or manager can mute others audio.
 */
- (ZoomVideoSDKError)muteAudio:(ZoomVideoSDKUser * _Nullable)user;

/**
 * @brief Asks to unmute the user's audio.
 * @param user The user whose audio will be unmuted. Use nil to unmute self.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 * @warning Only host or manager can unmute others audio.
 * @warning This function will trigger the callback \link onHostAskUnmute \endlink.
 */
- (ZoomVideoSDKError)unmuteAudio:(ZoomVideoSDKUser * _Nullable)user;

/**
 * @brief Mutes all users' VOIP audio except self.
 * @param allowUnmute YES to allow users to unmute themselves, NO otherwise.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 * @warning Only host or manager can mute all users' audio.
 */
- (ZoomVideoSDKError)muteAllAudio:(BOOL)allowUnmute;

/**
 * @brief Allows others to unmute themselves or not.
 * @param allowUnmute YES to allow users to unmute themselves, NO otherwise.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 * @warning Only host or manager can change this property.
 */
- (ZoomVideoSDKError)allowAudioUnmutedBySelf:(BOOL)allowUnmute;

/**
 * @brief Asks to unmute all users' VOIP audio.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 * @warning Only host or manager can ask to unmute all users' audio.
 * @warning This function will trigger the callback \link onHostAskUnmute \endlink.
 */
- (ZoomVideoSDKError)unmuteAllAudio;

/**
 * @brief Subscribes to audio raw data.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 * @warning Please check the callback "- (void)onMixedAudioRawDataReceived:(ZoomVideoSDKAudioRawData *)rawData".
 * @warning Please check the callback "- (void)onOneWayAudioRawDataReceived:(ZoomVideoSDKAudioRawData *)rawData user:(ZoomVideoSDKUser *)user".
 */
- (ZoomVideoSDKError)subscribe;

/**
 * @brief Unsubscribes from audio raw data.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 * @warning Please check the callback "- (void)onMixedAudioRawDataReceived:(ZoomVideoSDKAudioRawData *)rawData".
 * @warning Please check the callback "- (void)onOneWayAudioRawDataReceived:(ZoomVideoSDKAudioRawData *)rawData user:(ZoomVideoSDKUser *)user".
 */
- (ZoomVideoSDKError)unSubscribe;

/**
 * @brief Resets the audio session including category and mode for ZoomVideoSDK audio session environment when developer changes audio session in session.
 * @return If the function succeeds, it returns YES. Otherwise, NO.
 * @deprecated Use \link setSDKAudioSessionEnv \endlink instead.
 */
- (BOOL)resetAudioSession DEPRECATED_MSG_ATTRIBUTE("Use setSDKAudioSessionEnv instead");

/**
 * @brief Sets the AVAudioSession environment for Zoom Video SDK session.
 * @return If the function succeeds, it returns YES. Otherwise, NO.
 * @warning If developer changes AVAudioSession for special needs in SDK session, it will cause the audio output abnormal. Then developer can set SDK AVAudioSession environment.
 */
- (BOOL)setSDKAudioSessionEnv;

/**
 * @brief Cleans the audio session including category and mode.
 * @warning SDK will restore audio session status after session end. If developer calls overrideOutputAudioPort interface with AVAudioSessionPortBuiltInSpeaker in AVAudioSessionCategoryPlayAndRecord category before join session, then SDK cannot restore audio session BuiltInSpeaker output when session ends.
 * @deprecated No longer maintained.
 */
- (void)cleanAudioSession DEPRECATED_MSG_ATTRIBUTE("No longer maintain anymore!");

/**
 * @brief Gets the current audio output port description.
 * @return If the function succeeds, it returns a ZoomVideoSDKAudioDevice object. Otherwise, this function fails and returns nil.
 */
- (ZoomVideoSDKAudioDevice * _Nullable)getCurrentAudioOutputRoute;

/**
 * @brief Sets the audio output device.
 * @param device The device to set as the audio output.
 * @return If the function succeeds, it returns YES. Otherwise, NO.
 */
- (BOOL)setAudioOutputRoute:(ZoomVideoSDKAudioDevice *_Nullable)device;

/**
 * @brief Gets the available audio output port descriptions.
 * @return If the function succeeds, it returns an NSArray of ZoomVideoSDKAudioDevice objects for available audio output port descriptions. Otherwise, this function fails and returns nil.
 */
- (NSArray <ZoomVideoSDKAudioDevice *>* _Nullable)getAvailableAudioOutputRoute;

/**
 * @brief Displays the system AirPlay picker.
 * @param parentView The parent view where the AirPlay picker will be shown.
 * @return If the function succeeds, it returns YES. Otherwise, NO.
 */
- (BOOL)showAudioOutputDeviceAirPlayPicker:(UIView *_Nullable)parentView;

/**
 * @brief Gets the current audio input device.
 * @return If the function succeeds, it returns a ZoomVideoSDKAudioDevice object. Otherwise, this function fails and returns nil.
 */
- (ZoomVideoSDKAudioDevice *_Nullable)getCurrentAudioInputDevice;

/**
 * @brief Gets the list of available audio input devices.
 * @return If the function succeeds, it returns an NSArray of ZoomVideoSDKAudioDevice objects for available audio input devices. Otherwise, this function fails and returns nil.
 */
- (NSArray <ZoomVideoSDKAudioDevice *> *_Nullable)getAvailableAudioInputsDevice;

/**
 * @brief Sets the audio input device.
 * @param device The device to set as the audio input.
 * @return If the function succeeds, it returns YES. Otherwise, NO.
 */
- (BOOL)setAudioInputDevice:(ZoomVideoSDKAudioDevice *_Nullable)device;
@end

