//
//  ZoomVideoSDKBroadcastStreamingHelper.h
//  ZoomVideoSDK
//
//  Created by ZOOM  on 2025/7/28.
//  Copyright © 2025 Zoom Video Communications, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN


@interface ZoomVideoSDKBroadcastStreamingHelper : NSObject

/**
 * @brief Determines whether the session supports broadcast streaming.
 * @return YES if supported. Otherwise, NO.
 */
- (BOOL)isBroadcastStreamingSupported;

/**
 * @brief Determines whether the user has permission to start broadcast.
 * @return YES if allowed. Otherwise, NO.
 */
- (BOOL)canStartBroadcast;

/**
 * @brief Starts broadcast streaming asynchronously. The result is notified via the callback '-[ZoomVideoSDKDelegate onStartBroadcastResponse:channelID:]'.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)startBroadcast;

/**
 * @brief Stops broadcast streaming asynchronously. The result is notified via the callback '-[ZoomVideoSDKDelegate onStopBroadcastResponse:]'.
 * @param channelID The broadcast streaming channel ID.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)stopBroadcast:(NSString * _Nonnull)channelID;

/**
 * @brief Gets broadcast status asynchronously. The result is notified via the callback '-[ZoomVideoSDKDelegate onGetBroadcastControlStatus:status:]'.
 * @param channelID The broadcast streaming channel ID.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)getBroadcastStatus:(NSString * _Nonnull)channelID;

@end

/**
 * @class ZoomVideoSDKStreamingJoinContext
 * @brief Streaming join context.
 */
@interface ZoomVideoSDKStreamingJoinContext : NSObject
/**
 * @brief Gets the token.
 */
@property (nonatomic,copy) NSString * token;
/**
 * @brief Gets the channel ID.
 */
@property (nonatomic,copy) NSString * channelID;
@end



/**
 * @class ZoomVideoSDKBroadcastStreamingViewerHelper
 * @brief Broadcast streaming viewer helper interface.
 */
@interface ZoomVideoSDKBroadcastStreamingViewerHelper : NSObject
/**
 * @brief Joins broadcast streaming asynchronously. Result is notified via the callback '-[ZoomVideoSDKDelegate onStreamingJoinStatusChanged:]'.
 * @param joinContext The join context.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)joinStreaming:(ZoomVideoSDKStreamingJoinContext* _Nullable)joinContext;

/**
 * @brief Leaves broadcast streaming asynchronously. Result is notified via the callback '-[ZoomVideoSDKDelegate onStreamingJoinStatusChanged:]'.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)leaveStreaming;

/**
 * @brief Gets the current streaming join status.
 * @return The join status.
 */
- (ZoomVideoSDKStreamingJoinStatus)getStreamingJoinStatus;

/**
 * @brief Subscribes to streaming video.
 * @param resolution The desired video resolution.
 * @param delegate The raw video data callback object.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)subscribeVideoWithDelegate:(id<ZoomVideoSDKBroadcastStreamingVideoDelegate> _Nullable)delegate  resolution:(ZoomVideoSDKVideoResolution)resolution;

/**
 * @brief Unsubscribes from streaming video raw data.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)unSubscribeVideo;

/**
 * @brief Subscribes to streaming audio.
 * @param delegate The raw audio data callback object.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)subscribeAudioWithDelegate:(id<ZoomVideoSDKBroadcastStreamingAudioDelegate> _Nullable)delegate;

/**
 * @brief Unsubscribes from streaming audio raw data.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)unSubscribeAudio;


@end




NS_ASSUME_NONNULL_END
