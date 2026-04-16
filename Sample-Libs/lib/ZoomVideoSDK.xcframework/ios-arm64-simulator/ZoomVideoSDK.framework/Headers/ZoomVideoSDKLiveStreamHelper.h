/**
 * @file ZoomVideoSDKLiveStreamHelper.h
 * @brief Interface for live streaming operations including starting/stopping live streams and managing incoming live stream status.
 */

#import <Foundation/Foundation.h>
/**
 * @class ZoomVideoSDKLiveStreamSetting
 * @brief Live stream settings configuration class for Zoom Video SDK.
 * This class contains layout and close caption settings for live streaming.
 */
@interface ZoomVideoSDKLiveStreamSetting : NSObject

/**
 * The layout type for the live stream.
 */
@property(nonatomic, assign) ZoomVideoSDKLiveStreamLayout layout;

/**
 * The close caption setting for the live stream.
 */
@property(nonatomic, assign) ZoomVideoSDKLiveStreamCloseCaption closeCaption;

@end

/**
 * @class ZoomVideoSDKLiveStreamParams
 * @brief Live stream parameters for starting live stream.
 * This class contains all the necessary parameters to configure and start a live stream.
 */
@interface ZoomVideoSDKLiveStreamParams : NSObject

/**
 * The live stream URL where the stream will be sent (e.g., YouTube, Facebook Live).
 */
@property(nonatomic, copy, nullable) NSString* streamUrl;

/**
 * The live stream key for authentication with the streaming platform.
 */
@property(nonatomic, copy, nullable) NSString* key;

/**
 * The live stream broadcast URL where viewers can watch the stream.
 */
@property(nonatomic, copy, nullable) NSString* broadcastUrl;

/**
 * Live stream settings including layout and close caption options.
 */
@property(nonatomic, retain, nullable) ZoomVideoSDKLiveStreamSetting* setting;

@end


/**
 * @class ZoomVideoSDKLiveStreamHelper
 * @brief A class to operate the live stream.
 */
@interface ZoomVideoSDKLiveStreamHelper : NSObject

/**
 * @brief Starts live stream.
 * @param streamingURL The live stream URL by which you can live the session.
 * @param key The stream key offered by the third platform on which you want to live stream your session.
 * @param broadcastURL The URL of live stream website.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 * @warning Only session host can start live stream successfully.
 * @deprecated Use \link startLiveStreamWithParams: \endlink instead.
 */
- (ZoomVideoSDKError)startLiveStreamWithStreamingURL:(NSString * _Nullable)streamingURL StreamingKey:(NSString * _Nullable)key BroadcastURL:(NSString * _Nullable)broadcastURL __deprecated_msg("Use startLiveStreamWithParams: instead");

/**
 * @brief Starts a live stream of the current session using ZoomVideoSDKLiveStreamParams.
 * @param param The live stream parameters containing URL, key, broadcast URL and settings.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 * @warning Only session host can start live stream successfully.
 */
- (ZoomVideoSDKError)startLiveStreamWithParams:(ZoomVideoSDKLiveStreamParams * _Nullable)param;

/**
 * @brief Stops live streaming.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 * @warning The function is available only for host.
 */
- (ZoomVideoSDKError)stopLiveStream;

/**
 * @brief Checks if you can start live streaming.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)canStartLiveStream;

/**
 * @brief Gets the current live stream settings for the current session.
 * @return If the function succeeds, it returns the current live stream settings. Otherwise, this function fails and returns nil.
 */
- (ZoomVideoSDKLiveStreamSetting * _Nullable)getCurrentLiveStreamSetting;

/**
 * @brief Updates the live stream settings for the current session.
 * @param setting The new live stream settings to apply.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 * @warning The function is available only for host.
 * @note If the same settings as the previous call are passed, this function will return Errors_Wrong_Usage.
 */
- (ZoomVideoSDKError)updateLiveStreamSetting:(ZoomVideoSDKLiveStreamSetting * _Nullable)setting;

/**
 * @brief Determines whether the current user can get or update live stream settings.
 * @return YES if the user can get or update live stream settings. Otherwise, NO.
 * @note Live stream must be started, and only the person who started the live stream can get or update the setting.
 */
- (BOOL)canGetOrUpdateLiveStreamSetting;

@end

/**
 * @class ZoomVideoSDKIncomingLiveStreamStatus
 * @brief Incoming live stream status.
 */
@interface ZoomVideoSDKIncomingLiveStreamStatus : NSObject
/**
 * @brief Gets the incoming live stream ID.
 */
@property(nonatomic, copy) NSString* _Nullable streamKeyID;
/**
 * @brief Determines whether the streaming software (such as OBS) is connected to the Zoom platform.
 */
@property(nonatomic, assign) BOOL isRTMPConnected;
/**
 * @brief Determines whether the video stream has been pushed to the session.
 */
@property(nonatomic, assign) BOOL isStreamPushed;

@end

/**
 * @class ZoomVideoSDKIncomingLiveStreamHelper
 * @brief Incoming live stream helper interface.
 */
@interface ZoomVideoSDKIncomingLiveStreamHelper : NSObject

/**
 * @brief Binds incoming live stream with a stream key ID.
 * @param streamKeyID The stream key ID to bind.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 * @warning Only host can bind incoming live stream.
 */
- (ZoomVideoSDKError)bindIncomingLiveStream:(NSString *_Nullable)streamKeyID;

/**
 * @brief Unbinds the bound incoming live stream.
 * @param streamKeyID The stream key ID to unbind.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 * @warning Only host can unbind incoming live stream.
 */
- (ZoomVideoSDKError)unbindIncomingLiveStream:(NSString *_Nullable)streamKeyID;

/**
 * @brief Gets the status of bound streams.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 * @note Will receive callback "-onIncomingLiveStreamStatusResponse:".
 * @warning Only host can get incoming live stream status.
 */
- (ZoomVideoSDKError)getIncomingLiveStreamStatus;

/**
 * @brief Starts the bound stream as a special participant.
 * @param streamKeyID The stream key ID to start.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 * @warning Only host can start incoming live stream.
 */
- (ZoomVideoSDKError)startIncomingLiveStream:(NSString *_Nullable)streamKeyID;

/**
 * @brief Stops the bound stream as a special participant.
 * @param streamKeyID The stream key ID to stop.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 * @warning Only host can stop incoming live stream.
 */
- (ZoomVideoSDKError)stopIncomingLiveStream:(NSString *_Nullable)streamKeyID;

@end

