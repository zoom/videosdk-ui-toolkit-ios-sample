/**
 * @file ZoomVideoSDKUser.h
 * @brief Interface for user information and operations including user properties, audio/video status, statistics, and user interaction methods.
 */

#import <Foundation/Foundation.h>
#import <ZoomVideoSDK/ZoomVideoSDKRawDataPipe.h>
#import <ZoomVideoSDK/ZoomVideoSDKVideoCanvas.h>
#import <ZoomVideoSDK/ZoomVideoSDKRemoteCameraControlHelper.h>
#import <ZoomVideoSDK/ZoomVideoSDKShareHelper.h>


/**
 * @enum ZoomVideoSDKStatisticsDirection
 * @brief Direction of media statistics
 */
typedef NS_ENUM(NSInteger, ZoomVideoSDKStatisticsDirection) {
    /** Sending media statistics */
    ZoomVideoSDKStatisticsDirection_Send = 0,
    /** Receiving media statistics */
    ZoomVideoSDKStatisticsDirection_Receive = 1,
};

/**
 * @class ZoomVideoSDKQOSStatistics
 * @brief Base QOS statistics for media streams (Audio, Video, Share).
 * @note Common fields only; Send/Receive-specific fields are in ZoomVideoSDKQOSSendStatistics and ZoomVideoSDKQOSRecvStatistics.
 */
@interface ZoomVideoSDKQOSStatistics : NSObject

/** @brief Gets the direction of statistics (send or receive). */
@property (nonatomic, assign, readonly) ZoomVideoSDKStatisticsDirection direction;

/** @brief Gets the timestamp of the statistics. */
@property (nonatomic, assign, readonly) NSInteger timestamp;

/** @brief Gets the name of the codec. For video/share: "h264", "av1". For audio: "silk", "opus", "pcm", "G722", "G729". */
@property (nonatomic, copy, readonly) NSString * _Nullable codecName;

/** @brief Gets the round-trip time in milliseconds. */
@property (nonatomic, assign, readonly) NSInteger rtt;

/** @brief Gets the jitter in milliseconds. */
@property (nonatomic, assign, readonly) double jitter;

/** @brief Frame width (sent or received per direction). */
@property (nonatomic, assign, readonly) NSInteger width;

/** @brief Frame height (sent or received per direction). */
@property (nonatomic, assign, readonly) NSInteger height;

/** @brief Frame rate in FPS (sent or received per direction). */
@property (nonatomic, assign, readonly) NSInteger fps;

/** @brief Gets the bits per second. */
@property (nonatomic, assign, readonly) NSInteger bps;

/** @brief Gets the total bytes transferred (sent or received per direction). */
@property (nonatomic, assign, readonly) NSInteger bytesTransferred;

/** @brief Gets the number of packets lost during transmission. */
@property (nonatomic, assign, readonly) NSInteger packetsLost;

/** @brief Gets the total number of packets transferred (sent or received per direction). */
@property (nonatomic, assign, readonly) NSInteger packetsTransferred;

/** @brief Gets the network quality level. */
@property (nonatomic, assign, readonly) ZoomVideoSDKNetworkStatus networkLevel;

/** @brief Gets the statistics type (Audio, Video, or Share). */
@property (nonatomic, assign, readonly) ZoomVideoSDKDataType statisticsType;

/** @brief Gets the average packet loss ratio in per thousand (e.g. 100 means 10%). */
@property (nonatomic, assign, readonly) NSInteger avg_loss;

/** @brief Gets the maximum packet loss ratio in per thousand (e.g. 100 means 10%). */
@property (nonatomic, assign, readonly) NSInteger max_loss;

/** @brief Gets the estimated bandwidth in bps. */
@property (nonatomic, assign, readonly) NSInteger bandwidth;

@end

/**
 * @class ZoomVideoSDKQOSSendStatistics
 * @brief QOS statistics for outbound media stream (sending).
 * @note Inherits common fields from ZoomVideoSDKQOSStatistics; adds send-specific properties.
 */
@interface ZoomVideoSDKQOSSendStatistics : ZoomVideoSDKQOSStatistics

/** @brief Gets the width of the input frame (send only). */
@property (nonatomic, assign, readonly) NSInteger frameWidthInput;

/** @brief Gets the height of the input frame (send only). */
@property (nonatomic, assign, readonly) NSInteger frameHeightInput;

/** @brief Gets the frame rate of input (send only). */
@property (nonatomic, assign, readonly) NSInteger frameRateInput;

/** @brief Gets the total bytes sent. */
@property (nonatomic, assign, readonly) NSInteger bytesSent;

/** @brief Gets the total number of packets sent. */
@property (nonatomic, assign, readonly) NSInteger packetsSent;

/** @brief Gets the total packet send delay in milliseconds (send only). */
@property (nonatomic, assign, readonly) NSInteger totalPacketSendDelay;

/** @brief Gets the total time spent encoding in milliseconds (send only). */
@property (nonatomic, assign, readonly) NSInteger totalEncodeTime;

/** @brief Gets the total number of frames encoded (send only). */
@property (nonatomic, assign, readonly) NSInteger framesEncoded;

/** @brief Gets the total bytes of RTP headers (and padding) sent (send only). */
@property (nonatomic, assign, readonly) NSUInteger headerBytesSent;

@end

/**
 * @class ZoomVideoSDKQOSRecvStatistics
 * @brief QOS statistics for inbound media stream (receiving).
 * @note Inherits common fields from ZoomVideoSDKQOSStatistics; adds receive-specific properties.
 */
@interface ZoomVideoSDKQOSRecvStatistics : ZoomVideoSDKQOSStatistics

/** @brief Gets the total bytes received (receive only). */
@property (nonatomic, assign, readonly) unsigned long long bytesReceived;

/** @brief Gets the total number of packets received (receive only). */
@property (nonatomic, assign, readonly) NSInteger packetsReceived;

/** @brief Gets the estimated playout timestamp (receive only). */
@property (nonatomic, assign, readonly) NSInteger estimatedPlayoutTimestamp;

/** @brief Gets the total time spent decoding in milliseconds (receive only). */
@property (nonatomic, assign, readonly) NSInteger totalDecodeTime;

/** @brief Gets the total number of frames decoded (receive only). */
@property (nonatomic, assign, readonly) NSInteger framesDecoded;

/** @brief Gets the accumulated playout delay introduced by the jitter buffer, in seconds (receive only). */
@property (nonatomic, assign, readonly) double jitterBufferDelay;

/** @brief Gets the total audio samples decoded from RTP packets (receive only). */
@property (nonatomic, assign, readonly) unsigned long long jitterBufferEmittedCount;

/** @brief Gets the total bytes of RTP headers (and padding) received (receive only). */
@property (nonatomic, assign, readonly) unsigned long long headerBytesReceived;

/** @brief Gets the non-FEC packets discarded by NetEQ (receive only, audio only). */
@property (nonatomic, assign, readonly) NSUInteger packetsDiscarded;

/** @brief Gets the total number of FEC packets received (receive only, audio only). */
@property (nonatomic, assign, readonly) NSUInteger fecPacketsReceived;

/** @brief Gets the FEC packets discarded by NetEQ (receive only, audio only). */
@property (nonatomic, assign, readonly) NSUInteger fecPacketsDiscarded;

/** @brief Gets the accumulated current target delay of the jitter buffer, in seconds (receive only, audio only). */
@property (nonatomic, assign, readonly) double jitterBufferTargetDelay;

/** @brief Gets the accumulated minimum achievable jitter buffer delay, in seconds (receive only, audio only). */
@property (nonatomic, assign, readonly) double jitterBufferMinimumDelay;

/** @brief Gets the total audio samples generated by NetEQ (receive only, audio only). */
@property (nonatomic, assign, readonly) unsigned long long totalSamplesReceived;

/** @brief Gets the samples produced by concealment instead of real decode (receive only, audio only). */
@property (nonatomic, assign, readonly) unsigned long long concealedSamples;

/** @brief Gets the subset of concealedSamples that are silent (receive only, audio only). */
@property (nonatomic, assign, readonly) unsigned long long silentConcealedSamples;

/** @brief Gets the number of concealment events (receive only, audio only). */
@property (nonatomic, assign, readonly) unsigned long long concealmentEvents;

/** @brief Gets the samples inserted to slow down playout, i.e. deceleration (receive only, audio only). */
@property (nonatomic, assign, readonly) unsigned long long insertedSamplesForDeceleration;

/** @brief Gets the samples removed to speed up playout, i.e. acceleration (receive only, audio only). */
@property (nonatomic, assign, readonly) unsigned long long removedSamplesForAcceleration;

/** @brief Gets the total duration of all audio samples received, in seconds (receive only, audio only). */
@property (nonatomic, assign, readonly) double totalSamplesDuration;

/** @brief Gets the most recent audio level of the received stream, range [0.0, 1.0] (receive only, audio only). */
@property (nonatomic, assign, readonly) double audioLevel;

/** @brief Gets the accumulated audio energy of the received stream (receive only, audio only). */
@property (nonatomic, assign, readonly) double totalAudioEnergy;

@end


/**
 * @class ZoomVideoSDKVideoStatisticInfo
 * @brief Video statistic information. Inherits common QOS fields from ZoomVideoSDKQOSStatistics (width, height, fps, bps, networkLevel).
 */
@interface ZoomVideoSDKVideoStatisticInfo : ZoomVideoSDKQOSStatistics

/**
 * @brief Gets the video network status. Same as \link ZoomVideoSDKQOSStatistics.networkLevel \endlink.
 * @deprecated Use \link ZoomVideoSDKQOSStatistics.networkLevel \endlink instead.
 */
@property (nonatomic, assign) ZoomVideoSDKNetworkStatus videoNetworkStatus DEPRECATED_MSG_ATTRIBUTE("Use networkLevel instead");

@end

/**
 * @class ZoomVideoSDKShareStatisticInfo
 * @brief Share statistic information. Inherits from ZoomVideoSDKQOSStatistics (width, height, fps, bps, networkLevel).
 */
@interface ZoomVideoSDKShareStatisticInfo : ZoomVideoSDKQOSStatistics

/**
 * @brief Gets the share network status. Same as \link ZoomVideoSDKQOSStatistics.networkLevel \endlink.
 * @deprecated Use \link ZoomVideoSDKQOSStatistics.networkLevel \endlink instead.
 */
@property (nonatomic, assign) ZoomVideoSDKNetworkStatus shareNetworkStatus DEPRECATED_MSG_ATTRIBUTE("Use networkLevel instead");

@end

/**
 * @class ZoomVideoSDKAudioStatus
 * @brief Audio status of user.
 */
@interface ZoomVideoSDKAudioStatus : NSObject
/**
 * @brief Determines whether the audio is muted.
 */
@property (nonatomic, assign) BOOL          isMuted;
/**
 * @brief Determines whether the user is talking.
 */
@property (nonatomic, assign) BOOL          talking;
/**
 * @brief Gets the audio type: VOIP (Voice over IP), Telephony, or None.
 */
@property (nonatomic, assign) ZoomVideoSDKAudioType  audioType;

@end

/**
 * @class ZoomVideoSDKUser
 * @brief Zoom Video SDK user information.
 */
@interface ZoomVideoSDKUser : NSObject
/**
 * @brief Gets the user's ID.
 * @return The user's ID.
 */
- (NSInteger)getUserID;
/**
 * @brief Gets the name of the user in the session.
 * @return If the function succeeds, it returns the user's name. Otherwise, this function fails and returns nil.
 */
- (NSString *_Nullable)getUserName;
/**
 * @brief Gets the user's custom identity. Which is passed in JWT token or in SDKSessionContext.customUserId.
 * @return If the function succeeds, it returns the user's custom identity. Otherwise, this function fails and returns nil.
 * @deprecated Use \link getUserKey \endlink instead.
 */
- (NSString *_Nullable)getCustomUserId DEPRECATED_MSG_ATTRIBUTE("Use -getUserKey instead");
/**
 * @brief Gets the user's key.
 * @return If the function succeeds, it returns the user's key. Otherwise, this function fails and returns nil.
 */
- (NSString *_Nullable)getUserKey;
/**
 * @brief Gets the user's reference.
 * @return If the function succeeds, it returns the user's reference. Otherwise, this function fails and returns nil.
 */
- (NSString *_Nullable)getUserReference;
/**
 * @brief Determines whether the user is the host.
 * @return YES if the user is the host. Otherwise, NO.
 */
- (BOOL)isHost;
/**
 * @brief Determines whether the user is the manager.
 * @return YES if the user is the manager. Otherwise, NO.
 */
- (BOOL)isManager;
/**
 * @brief Determines whether the user agreed with the individual record.
 * @return YES if the user agreed with the individual record. Otherwise, NO.
 */
- (BOOL)isIndividualRecordAgreed;
/**
 * @brief Determines whether the user is spotlighted.
 * @return YES if the user is spotlighted. Otherwise, NO.
 */
- (BOOL)isVideoSpotLighted;
/**
 * @brief Gets the user's audio status.
 * @return If the function succeeds, it returns a ZoomVideoSDKAudioStatus object. Otherwise, this function fails and returns nil.
 */
- (ZoomVideoSDKAudioStatus *_Nullable)audioStatus;
/**
 * @brief Gets the user's video statistic information.
 * @return If the function succeeds, it returns a ZoomVideoSDKVideoStatisticInfo object. Otherwise, this function fails and returns nil.
 */
- (ZoomVideoSDKVideoStatisticInfo *_Nullable)getVideoStatisticInfo;
/**
 * @brief Gets the user's share statistic information.
 * @return If the function succeeds, it returns a ZoomVideoSDKShareStatisticInfo object. Otherwise, this function fails and returns nil.
 */
- (ZoomVideoSDKShareStatisticInfo *_Nullable)getShareStatisticInfo;
/**
 * @brief Gets the user's video pipe.
 * @return If the function succeeds, it returns a ZoomVideoSDKRawDataPipe object. Otherwise, this function fails and returns nil.
 */
- (ZoomVideoSDKRawDataPipe *_Nullable)getVideoPipe;
/**
 * @brief Gets the user's multi-camera stream list.
 * @return If the function succeeds, it returns an NSArray of ZoomVideoSDKRawDataPipe objects. Otherwise, this function fails and returns nil.
 */
- (NSArray <ZoomVideoSDKRawDataPipe *> *_Nullable)getMultiCameraStreamList;

/**
 * @brief Gets the user's video canvas.
 * @return If the function succeeds, it returns a ZoomVideoSDKVideoCanvas object. Otherwise, this function fails and returns nil.
 */
- (ZoomVideoSDKVideoCanvas *_Nullable)getVideoCanvas;
/**
 * @brief Gets the user's multi-camera canvas list.
 * @return If the function succeeds, it returns an NSArray of ZoomVideoSDKVideoCanvas objects. Otherwise, this function fails and returns nil.
 */
- (NSArray <ZoomVideoSDKVideoCanvas *> *_Nullable)getMultiCameraCanvasList;

/**
 * @brief Gets the helper class instance to access the remote camera control.
 * @return If the function succeeds, it returns a ZoomVideoSDKRemoteCameraControlHelper object. Otherwise, this function fails and returns nil.
 */
- (ZoomVideoSDKRemoteCameraControlHelper *_Nullable)getRemoteCameraControlHelper;

/**
 * @brief Gets the user's share-action list.
 * @return If the function succeeds, it returns an NSArray of ZoomVideoSDKShareAction objects. Otherwise, this function fails and returns nil.
 */
- (NSArray <ZoomVideoSDKShareAction *>*_Nullable)getShareActionList;

/**
 * @brief Sets the user's local volume. This does not affect how other participants hear the user.
 * @param volume The volume value can be >= 0 and <= 10. If volume is 0, the user won't be able to hear related audio.
 * @param isSharingAudio YES to set the volume of shared audio, NO to set the volume of the microphone.
 * @return If the function succeeds, it returns YES. Otherwise, NO.
 * @deprecated Use \link setUserPlaybackVolume:isSharingAudio: \endlink instead.
 */
- (BOOL)setUserVolume:(float)volume isShareAudio:(BOOL)isSharingAudio DEPRECATED_MSG_ATTRIBUTE("Use -setUserPlaybackVolume:isSharingAudio: instead");

/**
 * @brief Gets user volume.
 * @param volume The obtained value.
 * @param isSharingAudio YES to get the volume of shared audio, NO to get the volume of the microphone.
 * @return If the function succeeds, it returns YES. Otherwise, NO.
 * @deprecated Use \link getUserPlaybackVolume:isSharingAudio: \endlink instead.
 */
- (BOOL)getUserVolume:(float * _Nullable)volume isShareAudio:(BOOL)isSharingAudio DEPRECATED_MSG_ATTRIBUTE("Use -getUserPlaybackVolume:isSharingAudio: instead");

/**
 * @brief Determines which audio you can set, shared audio or microphone.
 * @param isSharingAudio YES to check whether you can set the volume of shared audio, NO to check whether you can set the volume of the microphone.
 * @return YES if you can set the volume. Otherwise, NO.
 * @deprecated Use \link canSetUserPlaybackVolume: \endlink instead.
 */
- (BOOL)canSetUserVolume:(BOOL)isSharingAudio DEPRECATED_MSG_ATTRIBUTE("Use -canSetUserPlaybackVolume: instead");

/**
 * @brief Sets the user's local volume. This does not affect how other participants hear the user.
 * @param volume The volume value can be between 0 and 10. If 0, the user won't hear the related audio.
 * @param isSharingAudio YES to set the volume of shared audio (such as shared computer audio), NO to set the microphone volume.
 * @return If the function succeeds, it returns YES. Otherwise, NO.
 */
- (BOOL)setUserPlaybackVolume:(float)volume isSharingAudio:(BOOL)isSharingAudio;

/**
 * @brief Gets the local user volume.
 * @param volume The user volume.
 * @param isSharingAudio YES to get the volume of shared audio (such as shared computer audio), NO to get the microphone volume.
 * @return If the function succeeds, it returns YES. Otherwise, NO.
 */
- (BOOL)getUserPlaybackVolume:(float * _Nullable)volume isSharingAudio:(BOOL)isSharingAudio;

/**
 * @brief Checks whether you can set the volume for shared audio or microphone.
 * @param isShareAudio YES to check if you can set the volume of shared audio, NO to check if you can set the volume of the microphone.
 * @return YES if you can set the volume. Otherwise, NO.
 */
- (BOOL)canSetUserPlaybackVolume:(BOOL)isShareAudio;

/**
 * @brief Gets the current audio level of a remote user.
 * @return If the function succeeds, it returns the current audio level. Otherwise, this function fails and returns 0.
 */
- (unsigned int)getAudioLevel;

/**
 * @brief Sends file to current user.
 * @param filePath The local path of the file.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 * @warning This interface will be related with chat privilege. See @{ZoomVideoSDKChatPrivilegeType}.
 */
- (ZoomVideoSDKError)transferFile:(NSString * _Nullable)filePath;

/**
 * @brief Determines whether the user is an incoming live stream user.
 * @return YES if the user is an incoming live stream. Otherwise, NO.
 */
- (BOOL)isIncomingLiveStreamUser;

/**
 * @brief Gets the whiteboard share status of the user.
 * @return The whiteboard share status.
 */
- (ZoomVideoSDKWhiteboardStatus)getWhiteboardStatus;

/**
 * @brief Determines whether the user is in SubSession.
 * @return YES if the user is in SubSession. Otherwise, NO.
 */
- (BOOL)isInSubSession;

/**
 * @brief Determines whether the user is in failover.
 * @return YES if the user is in failover. Otherwise, NO.
 */
- (BOOL)isInFailover;

/**
 * @brief Gets the network quality level of the specified data type for the user.
 * @param type The data type, such as audio, video, or share.
 * @return The current network status level for the specified data type.
 */
- (ZoomVideoSDKNetworkStatus)getNetworkLevel:(ZoomVideoSDKDataType)type;

/**
 * @brief Gets the overall network quality level of the user.
 * @return The overall network status level.
 */
- (ZoomVideoSDKNetworkStatus)getOverallNetworkLevel;
@end
