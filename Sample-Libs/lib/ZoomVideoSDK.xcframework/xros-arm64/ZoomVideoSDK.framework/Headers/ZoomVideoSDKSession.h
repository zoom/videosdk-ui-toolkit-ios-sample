/**
 * @file ZoomVideoSDKSession.h
 * @brief Interface for session management operations including session properties, user management, and file transfer.
 */

#import <Foundation/Foundation.h>
#import <ZoomVideoSDK/ZoomVideoSDKUser.h>

#pragma mark - Statistic Information -
/**
 * @class ZoomVideoSDKSessionAudioStatisticInfo
 * @brief Session audio statistic information.
 */
@interface ZoomVideoSDKSessionAudioStatisticInfo : NSObject
/**
 * @brief The frequency in kilohertz (KHz) sent by the current user.
 */
@property(nonatomic, assign, readonly) NSInteger  sendFrequency;
/**
 * @brief The audio latency (delay in network data transfer) experienced by the current user while sending data.
 */
@property(nonatomic, assign, readonly) NSInteger  sendLatency;
/**
 * @brief The audio jitter (change in latency) experienced by the current user while sending data.
 */
@property(nonatomic, assign, readonly) NSInteger  sendJitter;
/**
 * @brief The average audio packet loss sent by the current user.
 */
@property(nonatomic, assign, readonly) CGFloat    sendPacketLossAvg;
/**
 * @brief The maximum audio packet loss sent by the current user.
 */
@property(nonatomic, assign, readonly) CGFloat    sendPacketLossMax;
/**
 * @brief The frequency in kilohertz (KHz) received by the current user.
 */
@property(nonatomic, assign, readonly) NSInteger  recvFrequency;
/**
 * @brief The audio latency (delay in network data transfer) experienced by the current user while receiving data.
 */
@property(nonatomic, assign, readonly) NSInteger  recvLatency;
/**
 * @brief The audio jitter (change in latency) experienced by the current user while receiving data.
 */
@property(nonatomic, assign, readonly) NSInteger  recvJitter;
/**
 * @brief The average audio packet loss received by the current user.
 */
@property(nonatomic, assign, readonly) CGFloat    recvPacketLossAvg;
/**
 * @brief The maximum audio packet loss received by the current user.
 */
@property(nonatomic, assign, readonly) CGFloat    recvPacketLossMax;

@end

/**
 * @class ZoomVideoSDKSessionASVStatisticInfo
 * @brief The Session video or share statistic information.
 */
@interface ZoomVideoSDKSessionASVStatisticInfo : NSObject
/**
 * @brief The frame width in pixels sent by the current user.
 */
@property(nonatomic, assign, readonly) NSInteger  sendFrameWidth;
/**
 * @brief The frame height in pixels sent by the current user.
 */
@property(nonatomic, assign, readonly) NSInteger  sendFrameHeight;
/**
 * @brief The frame rate in FPS sent by the current user.
 */
@property(nonatomic, assign, readonly) NSInteger  sendFps;
/**
 * @brief The video latency (delay in network data transfer) experienced by the current user while sending data.
 */
@property(nonatomic, assign, readonly) NSInteger  sendLatency;
/**
 * @brief The video jitter (change in latency) experienced by the current user while sending data.
 */
@property(nonatomic, assign, readonly) NSInteger  sendJitter;
/**
 * @brief The average video packet loss sent by the current user.
 */
@property(nonatomic, assign, readonly) CGFloat    sendPacketLossAvg;
/**
 * @brief The maximum video packet loss sent by the current user.
 */
@property(nonatomic, assign, readonly) CGFloat    sendPacketLossMax;
/**
 * @brief The frame width in pixels received by the current user.
 */
@property(nonatomic, assign, readonly) NSInteger  recvFrameWidth;
/**
 * @brief The frame height in pixels received by the current user.
 */
@property(nonatomic, assign, readonly) NSInteger  recvFrameHeight;
/**
 * @brief The frame rate in FPS received by the current user.
 */
@property(nonatomic, assign, readonly) NSInteger  recvFps;
/**
 * @brief The video latency (delay in network data transfer) experienced by the current user while receiving data.
 */
@property(nonatomic, assign, readonly) NSInteger  recvLatency;
/**
 * @brief The video jitter (change in latency) experienced by the current user while receiving data.
 */
@property(nonatomic, assign, readonly) NSInteger  recvJitter;
/**
 * @brief The video average packet loss received by the current user.
 */
@property(nonatomic, assign, readonly) CGFloat    recvPacketLossAvg;
/**
 * @brief The maximum video packet loss received by the current user.
 */
@property(nonatomic, assign, readonly) CGFloat    recvPacketLossMax;
@end


#pragma mark - ZoomVideoSDKSession -
/**
 * @class ZoomVideoSDKSession
 * @brief Zoom Video SDK session.
 */
@interface ZoomVideoSDKSession : NSObject

/**
 * @brief Gets the session's name.
 * @return If the function succeeds, it returns the session's name. Otherwise, this function fails and returns nil.
 */
- (NSString * _Nullable)getSessionName;

/**
 * @brief Gets the session ID.
 * @return If the function succeeds, it returns the session ID. Otherwise, this function fails and returns nil.
 * @warning Only the host can get the session ID.
 */
- (NSString * _Nullable)getSessionID;

/**
 * @brief Gets the session number.
 * @return The session number.
 */
- (unsigned long long)getSessionNumber;

/**
 * @brief Gets the session's password.
 * @return If the function succeeds, it returns the session's password. Otherwise, this function fails and returns nil.
 */
- (NSString * _Nullable)getSessionPassword;

/**
 * @brief Gets the session phone passcode.
 * @return If the function succeeds, it returns the session phone passcode. Otherwise, this function fails and returns nil.
 */
- (NSString * _Nullable)getSessionPhonePasscode;

/**
 * @brief Gets the host's name.
 * @return If the function succeeds, it returns the host's name. Otherwise, this function fails and returns nil.
 */
- (NSString * _Nullable)getSessionHostName;

/**
 * @brief Gets the session's host user object.
 * @return If the function succeeds, it returns a ZoomVideoSDKUser object. Otherwise, this function fails and returns nil.
 */
- (ZoomVideoSDKUser * _Nullable)getSessionHost;

/**
 * @brief Gets a list of the session's remote users.
 * @return If the function succeeds, it returns an NSArray of ZoomVideoSDKUser objects. Otherwise, this function fails and returns nil.
 */
- (NSArray <ZoomVideoSDKUser *>* _Nullable)getRemoteUsers;

/**
 * @brief Gets the session's user object for myself.
 * @return If the function succeeds, it returns a ZoomVideoSDKUser object. Otherwise, this function fails and returns nil.
 */
- (ZoomVideoSDKUser * _Nullable)getMySelf;

/**
 * @brief Gets the session's audio statistic information.
 * @return If the function succeeds, it returns a ZoomVideoSDKSessionAudioStatisticInfo object. Otherwise, this function fails and returns nil.
 */
- (ZoomVideoSDKSessionAudioStatisticInfo * _Nullable)getSessionAudioStatisticInfo;

/**
 * @brief Gets the session's video statistic information.
 * @return If the function succeeds, it returns a ZoomVideoSDKSessionASVStatisticInfo object. Otherwise, this function fails and returns nil.
 */
- (ZoomVideoSDKSessionASVStatisticInfo * _Nullable)getSessionVideoStatisticInfo;

/**
 * @brief Gets the session's screen share statistic information.
 * @return If the function succeeds, it returns a ZoomVideoSDKSessionASVStatisticInfo object. Otherwise, this function fails and returns nil.
 */
- (ZoomVideoSDKSessionASVStatisticInfo * _Nullable)getSessionShareStatisticInfo;

#pragma mark - file transfer -

/**
 * @brief Determines whether file transfer is enabled.
 * @return YES if file transfer is enabled. Otherwise, NO.
 */
- (BOOL)isFileTransferEnable;

/**
 * @brief Sends file to all users in current session.
 * @param filePath The local path of the file.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 * @warning This interface will be related with chat privilege. See @{ZoomVideoSDKChatPrivilegeType}.
 */
- (ZoomVideoSDKError)transferFile:(NSString * _Nullable)filePath;

/**
 * @brief Gets the list of allowed file types in transfer.
 * @return If the function succeeds, it returns the value of allowed file types in transfer, comma-separated if there are multiple values. Otherwise, this function fails and returns nil. Exe files are by default forbidden from being transferred.
 */
- (NSString *_Nullable)getTransferFileTypeWhiteList;

/**
 * @brief Gets the maximum size for file transfer.
 * @return The maximum number of bytes for file transfer.
 */
- (unsigned long long)getMaxTransferFileSize;

/**
 * @brief Gets the session type of this session.
 * @return The session type.
 */
- (ZoomVideoSDKSessionType)getSessionType;

@end
