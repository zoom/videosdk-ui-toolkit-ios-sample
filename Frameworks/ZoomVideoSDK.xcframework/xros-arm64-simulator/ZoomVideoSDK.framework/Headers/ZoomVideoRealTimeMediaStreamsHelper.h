//
//  ZoomVideoRealTimeMediaStreamsHelper.h
//  ZoomVideoSDK
//
//  Copyright © 2025 Zoom Video Communications, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <ZoomVideoSDK/ZoomVideoSDKConstants.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZoomVideoRealTimeMediaStreamsHelper : NSObject

/**
 * @brief Determines whether the session supports the real-time media streams feature.
 * @return YES if the real-time media streams feature is supported. Otherwise, NO.
 */
- (BOOL)isSupportRealTimeMediaStreams;

/**
 * @brief Checks if the current user meets the requirements to start real-time media streams.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 * @note Only the host is permitted to start real-time media streams.
 */
- (ZoomVideoSDKError)canStartRealTimeMediaStreams;

/**
 * @brief Starts real-time media streams.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)startRealTimeMediaStreams;

/**
 * @brief Pauses real-time media streams.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)pauseRealTimeMediaStreams;

/**
 * @brief Resumes real-time media streams.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)resumeRealTimeMediaStreams;

/**
 * @brief Stops real-time media streams.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)stopRealTimeMediaStreams;

@end

NS_ASSUME_NONNULL_END
