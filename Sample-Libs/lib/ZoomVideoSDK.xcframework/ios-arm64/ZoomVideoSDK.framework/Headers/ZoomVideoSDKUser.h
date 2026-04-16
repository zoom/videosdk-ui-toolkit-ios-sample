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
 * @class ZoomVideoSDKVideoStatisticInfo
 * @brief Video statistic information.
 */
@interface ZoomVideoSDKVideoStatisticInfo : NSObject
/**
 * @brief Gets the frame width.
 */
@property (nonatomic, assign) NSInteger     width;
/**
 * @brief Gets the frame height.
 */
@property (nonatomic, assign) NSInteger     height;
/**
 * @brief Gets the frame per second.
 */
@property (nonatomic, assign) NSInteger     fps;
/**
 * @brief Gets the bits per second.
 */
@property (nonatomic, assign) NSInteger     bps;
/**
 * @brief Gets the video network status.
 */
@property (nonatomic, assign) ZoomVideoSDKNetworkStatus videoNetworkStatus;
@end

/**
 * @class ZoomVideoSDKShareStatisticInfo
 * @brief Share statistic information.
 */
@interface ZoomVideoSDKShareStatisticInfo : NSObject
/**
 * @brief Gets the frame width.
 */
@property (nonatomic, assign) NSInteger     width;
/**
 * @brief Gets the frame height.
 */
@property (nonatomic, assign) NSInteger     height;
/**
 * @brief Gets the frame per second.
 */
@property (nonatomic, assign) NSInteger     fps;
/**
 * @brief Gets the bits per second.
 */
@property (nonatomic, assign) NSInteger     bps;
/**
 * @brief Gets the share network status.
 */
@property (nonatomic, assign) ZoomVideoSDKNetworkStatus shareNetworkStatus;
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
