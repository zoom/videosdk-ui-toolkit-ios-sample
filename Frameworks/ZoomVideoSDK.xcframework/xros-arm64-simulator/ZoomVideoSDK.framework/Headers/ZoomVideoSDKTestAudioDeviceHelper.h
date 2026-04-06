/**
 * @file ZoomVideoSDKTestAudioDeviceHelper.h
 * @brief Interface for audio device testing including microphone recording and speaker playback testing.
 */

#import <Foundation/Foundation.h>

/**
 * @class ZoomVideoSDKTestAudioDeviceHelper
 * @brief Test audio device interface.
 */
@interface ZoomVideoSDKTestAudioDeviceHelper : NSObject

/**
 * @brief Starts the microphone test. This will start recording the input from the microphone. Once the recording is complete, call stopMicTest to finish the recording.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)startMicTest;

/**
 * @brief Stops the microphone test. Before calling this, you must have successfully started the microphone test by calling startMicTest. Otherwise this returns an error.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)stopMicTest;

/**
 * @brief Plays the microphone recorded sound. You must complete a microphone test by successfully executing startMicTest and stopMicTest before calling this. Otherwise this returns an error.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)playMicTest;

/**
 * @brief Starts the speaker test.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)startSpeakerTest;

/**
 * @brief Stops the speaker test. Before calling this, you must have successfully started the speaker test by calling startSpeakerTest. Otherwise this returns an error.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)stopSpeakerTest;

@end

