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
/**
 * @brief Gets the viewer's display name for use when sending chat messages.
 */
@property (nonatomic,copy) NSString * _Nullable userName;
@end

/**
 * @protocol ZoomVideoSDKBroadcastStreamingViewerDelegate
 * @brief Delegate protocol for receiving broadcast streaming viewer interactive callbacks.
 */
@protocol ZoomVideoSDKBroadcastStreamingViewerDelegate <NSObject>
@optional
/**
 * @brief Callback event when a chat message is received from the broadcast stream.
 * @param senderName The name of the message sender.
 * @param content The chat message content.
 * @param isSelfSend YES if the message was sent by the current viewer, NO otherwise.
 */
- (void)onStreamingChatMessageReceived:(NSString * _Nonnull)senderName content:(NSString * _Nonnull)content isSelfSend:(BOOL)isSelfSend;

/**
 * @brief Callback event when an emoji reaction is received from the broadcast stream.
 * @param type The emoji reaction type.
 */
- (void)onStreamingEmojiReactionReceived:(ZoomVideoSDKEmojiReactionType)type;

/**
 * @brief Callback event when a live transcription message is received from the broadcast stream.
 * @param content The live transcription content.
 */
- (void)onStreamingLiveTranscriptionMsgReceived:(NSString * _Nonnull)content;
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

/**
 * @brief Subscribes broadcast streaming viewer interactive callbacks.
 * @param delegate The viewer delegate callback object.If delegate is nill, Not accepting callback from Viewer Delegate
 */
- (void)subscribeViewerWithDelegate:(id<ZoomVideoSDKBroadcastStreamingViewerDelegate> _Nullable)delegate;

/**
 * @brief Sends a chat message to all participants in the broadcast stream.
 * @param content The chat message content to send.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)sendChatToAll:(NSString * _Nonnull)content;

/**
 * @brief Sends an emoji reaction to all participants in the broadcast stream.
 * @param type The emoji reaction type to send.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)sendEmojiReaction:(ZoomVideoSDKEmojiReactionType)type;

/**
 * @brief Determines whether the viewer can subscribe to a translation language for live transcription.
 * @return YES if the viewer can subscribe to a translation language. Otherwise, NO.
 */
- (BOOL)canSubscribeTranslationLanguage;

/**
 * @brief Subscribes to a translation language for live transcription.
 * @param language The translation language to subscribe to.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)subscribeTranslationLanguage:(ZoomVideoSDKBroadcastStreamingTranslationLanguage)language;

/**
 * @brief Unsubscribes from the currently subscribed translation language for live transcription.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)unsubscribeTranslationLanguage;

@end




NS_ASSUME_NONNULL_END
