/**
 * @file ZoomVideoSDK.h
 * @brief Main Zoom Video SDK interface providing session management, initialization, and access to all helper modules.
 */

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#import <ZoomVideoSDK/ZoomVideoSDKSession.h>
#import <ZoomVideoSDK/ZoomVideoSDKUser.h>
#import <ZoomVideoSDK/ZoomVideoSDKDelegate.h>
#import <ZoomVideoSDK/ZoomVideoSDKConstants.h>
#import <ZoomVideoSDK/ZoomVideoSDKAudioHelper.h>
#import <ZoomVideoSDK/ZoomVideoSDKVideoHelper.h>
#import <ZoomVideoSDK/ZoomVideoSDKUserHelper.h>
#import <ZoomVideoSDK/ZoomVideoSDKVideoCanvas.h>
#import <ZoomVideoSDK/ZoomVideoSDKRawDataPipe.h>
#import <ZoomVideoSDK/ZoomVideoSDKShareHelper.h>
#import <ZoomVideoSDK/ZoomVideoSDKLiveStreamHelper.h>
#import <ZoomVideoSDK/ZoomVideoSDKChatHelper.h>
#import <ZoomVideoSDK/ZoomVideoSDKPhoneHelper.h>
#import <ZoomVideoSDK/ZoomVideoSDKCmdChannel.h>
#import <ZoomVideoSDK/ZoomVideoSDKRecordingHelper.h>
#import <ZoomVideoSDK/ZoomVideoSDKAudioSettingHelper.h>
#import <ZoomVideoSDK/ZoomVideoSDKVideoSettingHelper.h>
#import <ZoomVideoSDK/ZoomVideoSDKTestAudioDeviceHelper.h>
#import <ZoomVideoSDK/ZoomVideoSDKLiveTranscriptionHelper.h>
#import <ZoomVideoSDK/ZoomVideoSDKNetworkConnectionHelper.h>
#import <ZoomVideoSDK/ZoomVideoSDKVirtualBackgroundHelper.h>
#import <ZoomVideoSDK/ZoomVideoSDKCRCHelper.h>
#import <ZoomVideoSDK/ZoomVideoSDKMaskHelper.h>
#import <ZoomVideoSDK/ZoomVideoSDKRemoteCameraControlHelper.h>
#import <ZoomVideoSDK/ZoomVideoSDKSubSessionHelper.h>
#import <ZoomVideoSDK/ZoomVideoSDKWhiteboardHelper.h>
#import <ZoomVideoSDK/ZoomVideoSDKBroadcastStreamingHelper.h>
#import <ZoomVideoSDK/ZoomVideoRealTimeMediaStreamsHelper.h>

/**
 @class ZoomVideoSDKExtendParams
 @brief ZoomVideoSDK extend params.
 */
@interface ZoomVideoSDKExtendParams : NSObject
/**
 * @brief Gets the audio speaker test file path. Only supports mp3 format. The size cannot exceed 1M.
 */
@property (nonatomic, copy) NSString * _Nullable speakerTestFilePath;

/**
 * @brief Gets the wrapper type.
 */
@property (nonatomic, assign) NSInteger wrapperType;

/**
 * @brief Gets the preferred video resolution.
 */
@property (nonatomic, assign) ZoomVideoSDKPreferVideoResolution preferVideoResolution;

@end

/**
 * @class ZoomVideoSDKInitParams
 * @brief Parameter class use to initialize the ZoomVideoSDK.
 */
@interface ZoomVideoSDKInitParams : NSObject
/**
 * @brief [Required] The domain of ZoomVideoSDK.
 */
@property (nonatomic, copy) NSString * _Nullable domain;
/**
 * @brief [Optional] The Prefix of Log file name.
 */
@property (nonatomic, copy) NSString * _Nullable logFilePrefix;
/**
 * @brief [Optional] If you use screen share, you need create group id in your apple developer account, and setup here.
 */
@property (nonatomic, copy) NSString * _Nullable  appGroupId;
/**
 * @brief [Optional] enable/disable log of SDK. log path AppData/tmp.
 */
@property (nonatomic, assign) BOOL                          enableLog;
/**
 * @brief [Optional] The video rawdata memory mode. Default is ZoomVideoSDKRawDataMemoryModeStack.
 */
@property (nonatomic, assign) ZoomVideoSDKRawDataMemoryMode  videoRawdataMemoryMode;
/**
 * @brief [Optional] The share rawdata memory mode. Default is ZoomVideoSDKRawDataMemoryModeStack.
 */
@property (nonatomic, assign) ZoomVideoSDKRawDataMemoryMode  shareRawdataMemoryMode;
/**
 * @brief [Optional] The audio rawdata memory mode. Default is ZoomVideoSDKRawDataMemoryModeStack.
 */
@property (nonatomic, assign) ZoomVideoSDKRawDataMemoryMode  audioRawdataMemoryMode;
/**
 * @brief [Optional] The SDK extend params.
 */
@property (nonatomic, strong) ZoomVideoSDKExtendParams * _Nullable extendParam;
@end

/**
 * @class ZoomVideoSDKVideoOptions
 * @brief The video option of join session.
 */
@interface ZoomVideoSDKVideoOptions : NSObject

/**
 * @brief Determines whether local video is on.
 */
@property (assign, nonatomic) BOOL localVideoOn;
/**
 * @brief Sets the multitaskingCameraAccessEnabled for AVCaptureSession of local camera. For more information, refer to https://developer.apple.com/documentation/avfoundation/avcapturesession/4013228-multitaskingcameraaccesssupporte.
 */
@property (assign, nonatomic) BOOL multitaskingCameraAccessEnabled;

@end

/**
 * @class ZoomVideoSDKAudioOptions
 * @brief The audio option of join session.
 */
@interface ZoomVideoSDKAudioOptions : NSObject

/**
 * @brief Determines whether local audio is connected.
 */
@property (assign, nonatomic) BOOL connect;

/**
 * @brief Determines whether local audio is muted.
 */
@property (assign, nonatomic) BOOL mute;

/**
 * @brief Determines whether my voice is in the mixed audio raw data. Default is YES.
 */
@property (assign, nonatomic) BOOL isMyVoiceInMix;

/**
 * @brief Determines whether to automatically adjust the volume of speaker if the volume is muted or the volume is low. Default is NO.
 */
@property (nonatomic, assign) BOOL autoAdjustSpeakerVolume;
@end

/**
 * @class ZoomVideoSDKSessionContext
 * @brief A Class contains the session information.
 */
@interface ZoomVideoSDKSessionContext : NSObject
/**
 * @brief [Required] Session Name.
 */
@property (nonatomic, copy) NSString * _Nullable sessionName;
/**
 * @brief [Optional] Session Password.
 */
@property (nonatomic, copy) NSString * _Nullable sessionPassword;
/**
 * @brief [Required] User Name.
 */
@property (nonatomic, copy) NSString * _Nullable userName;
/**
 * @brief [Required] JWT token to join session.
 */
@property (nonatomic, copy) NSString * _Nullable token;
/**
 * @brief [Optional] The amount of time in minutes after which an idle session will end.
 * @note Default value: 40. If the value is less than 0, the session will stay alive indefinitely.When there is only one user remaining in a session, that session is considered idle.
 */
@property (nonatomic, assign) NSInteger sessionIdleTimeoutMins;
/**
 * @brief [Optional] Whether to cancel the bandwidth limit, If YES is set, the network bandwidth is no longer limited, and better audio and video quality can be obtained.Bandwidth is not limited by default. If it is a wifi network, the bandwidth is not limited, and this setting is invalid.
 */
@property (nonatomic, assign) BOOL enable5GHighBandWidth;
/**
 * @brief [Optional]  Audio Option.
 */
@property (nonatomic, strong) ZoomVideoSDKAudioOptions * _Nullable audioOption;
/**
 * @brief [Optional] Video Option.
 */
@property (nonatomic, strong) ZoomVideoSDKVideoOptions * _Nullable videoOption;

/**
 * @brief [Optional] Session external video source delegate.
 */
@property (weak, nonatomic) id<ZoomVideoSDKVideoSource> _Nullable externalVideoSourceDelegate;

/**
 * @brief [Optional] Session pre-processer delegate.
 */
@property (weak, nonatomic) id<ZoomVideoSDKVideoSourcePreProcessor> _Nullable preProcessorDelegate;

/**
 * @brief [Optional] Session external audio source delegate.
 */
@property (weak, nonatomic) id<ZoomVideoSDKVirtualAudioMic> _Nullable virtualAudioMicDelegate;

/**
 * @brief [Optional] Session virtual speaker.
 */
@property (weak, nonatomic) id<ZoomVideoSDKVirtualAudioSpeaker> _Nullable virtualAudioSpeakerDelegate;
@end

/**
 * @class ZoomVideoSDK
 * @brief Zoom Video SDK API manager. Main singleton object that controls the video session creation, event callbacks and other main features of video SDK.
 * @warning Access to the class and all the other components of the VideoSDK by merging <ZoomVideoSDK/ZoomVideoSDK.h> into source code.
 * @warning The user can only obtain SDK configuration by initializing the class.
 */
@interface ZoomVideoSDK : NSObject

/**
 * @brief The delegate of ZoomVideoSDK, a listener object that groups together all the callbacks related to a session.
 */
@property (weak, nonatomic) id<ZoomVideoSDKDelegate> _Nullable delegate;

/**
 * @brief Returns ZoomVideoSDK instance.
 */
+ (ZoomVideoSDK * _Nullable)shareInstance;

/**
 * @brief Initializes the Zoom SDK with the appropriate parameters in the ZoomVideoSDKInitParams object.
 * @param context The parameter configuration of the SDK.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 * @warning Need to initialize first before doing the following steps.
 */
- (ZoomVideoSDKError)initialize:(ZoomVideoSDKInitParams * _Nonnull)context;

/**
 * @brief Uninitializes the Zoom SDK.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 * @warning Only cleanup successfully after initialized.
 */
- (ZoomVideoSDKError)cleanup;

/**
 * @brief Joins a session with the appropriate [ZoomVideoSDKSessionContext] parameters. When successful, the SDK will attempt to join a session. Use the callbacks in the delegate to confirm whether the SDK actually joined.
 * @param context The context which contains the parameters.
 * @return If the function succeeds, it returns a ZoomVideoSDKSession object. Otherwise, this function fails and returns nil.
 */
- (ZoomVideoSDKSession * _Nullable)joinSession:(ZoomVideoSDKSessionContext * _Nonnull)context;

/**
 * @brief Leaves a session previously joined through joinSession method call. When successful, the SDK will attempt to leave a session. Use the callbacks in the delegate to confirm whether the SDK actually left.
 * @param end YES if the host should end the entire session, NO if the host should just leave the session.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 * @warning Only host can end session. You can get the isHost information from in-Session 'userInfo'.
 */
- (ZoomVideoSDKError)leaveSession:(BOOL)end;

/**
 * @brief Gets the current session information.
 * @return If the function succeeds, it returns a ZoomVideoSDKSession object. Otherwise, this function fails and returns nil.
 */
- (ZoomVideoSDKSession * _Nullable)getSession;

/**
 * @brief Determines whether there is an active session between participants.
 * @return YES if there is an active session. Otherwise, NO.
 */
- (BOOL)isInSession;

/**
 * @brief Gets the Zoom SDK internal version.
 * @return If the function succeeds, it returns the SDK version. Otherwise, this function fails and returns nil.
 */
- (NSString * _Nullable)getSDKVersion;

/**
 * @brief Exports a log file to local disk.
 * @return If the function succeeds, it returns the exported log file path. Otherwise, this function fails and returns nil.
 */
- (NSString * _Nullable)exportLog;

/**
 * @brief Cleans all exported logs.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)cleanAllExportedLogs;
/**
 * @brief Gets an instance to manage audio controls related to the current video SDK session.
 * @return If the function succeeds, it returns a ZoomVideoSDKAudioHelper object. Otherwise, this function fails and returns nil.
 */
- (ZoomVideoSDKAudioHelper * _Nullable)getAudioHelper;

/**
 * @brief Gets an instance to manage cameras and video during a video SDK session.
 * @return If the function succeeds, it returns a ZoomVideoSDKVideoHelper object. Otherwise, this function fails and returns nil.
 */
- (ZoomVideoSDKVideoHelper * _Nullable)getVideoHelper;

/**
 * @brief Gets an instance to manage users present in a video SDK session.
 * @return If the function succeeds, it returns a ZoomVideoSDKUserHelper object. Otherwise, this function fails and returns nil.
 */
- (ZoomVideoSDKUserHelper * _Nullable)getUserHelper;

/**
 * @brief Gets an instance to manage screen sharing during a video SDK session.
 * @return If the function succeeds, it returns a ZoomVideoSDKShareHelper object. Otherwise, this function fails and returns nil.
 */
- (ZoomVideoSDKShareHelper * _Nullable)getShareHelper;

/**
 * @brief Gets an instance to manage live streaming during a video SDK session.
 * @return If the function succeeds, it returns a ZoomVideoSDKLiveStreamHelper object. Otherwise, this function fails and returns nil.
 */
- (ZoomVideoSDKLiveStreamHelper * _Nullable)getLiveStreamHelper;

/**
 * @brief Gets the incoming live stream helper object.
 * @return If the function succeeds, it returns a ZoomVideoSDKIncomingLiveStreamHelper object. Otherwise, this function fails and returns nil.
 */
- (ZoomVideoSDKIncomingLiveStreamHelper * _Nullable)getIncomingLiveStreamHelper;

/**
 * @brief Gets an instance to send and receive chat messages within video SDK session participants.
 * @return If the function succeeds, it returns a ZoomVideoSDKChatHelper object. Otherwise, this function fails and returns nil.
 */
- (ZoomVideoSDKChatHelper * _Nullable)getChatHelper;

/**
 * @brief Gets an instance to manage phone invitations during a video SDK session.
 * @return If the function succeeds, it returns a ZoomVideoSDKPhoneHelper object. Otherwise, this function fails and returns nil.
 */
- (ZoomVideoSDKPhoneHelper * _Nullable)getPhoneHelper;

/**
 * @brief Gets an instance to use command channel features during a video SDK session.
 * @return If the function succeeds, it returns a ZoomVideoSDKCmdChannel object. Otherwise, this function fails and returns nil.
 */
- (ZoomVideoSDKCmdChannel * _Nullable)getCmdChannel;

/**
 * @brief Gets an instance to manage cloud recordings during a video SDK session.
 * @return If the function succeeds, it returns a ZoomVideoSDKRecordingHelper object. Otherwise, this function fails and returns nil.
 */
- (ZoomVideoSDKRecordingHelper * _Nullable)getRecordingHelper;

/**
 * @brief Gets the audio setting helper.
 * @return If the function succeeds, it returns a ZoomVideoSDKAudioSettingHelper object. Otherwise, this function fails and returns nil.
 */
- (ZoomVideoSDKAudioSettingHelper * _Nullable)getAudioSettingHelper;

/**
 * @brief Gets the video setting helper.
 * @return If the function succeeds, it returns a ZoomVideoSDKVideoSettingHelper object. Otherwise, this function fails and returns nil.
 */
- (ZoomVideoSDKVideoSettingHelper * _Nullable)getVideoSettingHelper;

/**
 * @brief Gets the test audio device helper.
 * @return If the function succeeds, it returns a ZoomVideoSDKTestAudioDeviceHelper object. Otherwise, this function fails and returns nil.
 */
- (ZoomVideoSDKTestAudioDeviceHelper * _Nullable)getTestAudioDeviceHelper;

/**
 * @brief Gets the live transcription helper object.
 * @return If the function succeeds, it returns a ZoomVideoSDKLiveTranscriptionHelper object. Otherwise, this function fails and returns nil.
 */
- (ZoomVideoSDKLiveTranscriptionHelper * _Nullable)getLiveTranscriptionHelper;

/**
 * @brief Gets the virtual background helper object.
 * @return If the function succeeds, it returns a ZoomVideoSDKVirtualBackgroundHelper object. Otherwise, this function fails and returns nil.
 */
- (ZoomVideoSDKVirtualBackgroundHelper * _Nullable)getVirtualBackgroundHelper;

/**
 * @brief Gets the CRC helper object.
 * @return If the function succeeds, it returns a ZoomVideoSDKCRCHelper object. Otherwise, this function fails and returns nil.
 */
- (ZoomVideoSDKCRCHelper * _Nullable)getCRCHelper;

/**
 * @brief Gets the mask helper object.
 * @return If the function succeeds, it returns a ZoomVideoSDKMaskHelper object. Otherwise, this function fails and returns nil.
 */
- (ZoomVideoSDKMaskHelper * _Nullable)getMaskHelper;

/**
 * @brief Gets the SubSession stream helper object. Only host and manager can call this interface.
 * @return If the function succeeds, it returns a ZoomVideoSDKSubSessionHelper object. Otherwise, this function fails and returns nil.
 */
- (ZoomVideoSDKSubSessionHelper * _Nullable)getsubSessionHelper;

/**
 * @brief Gets the session type of this session.
 * @return If the function succeeds, it returns the session type. Otherwise, this function returns an error.
 * @deprecated Use \link -[ZoomVideoSDKSession getSessionType] \endlink instead.
 */
-(ZoomVideoSDKSessionType)getSessionType DEPRECATED_MSG_ATTRIBUTE("use -[ZoomVideoSDKSession getSessionType] instead");

/**
 * @brief Gets the broadcast streaming controller object.
 * @return If the function succeeds, it returns a ZoomVideoSDKBroadcastStreamingHelper object. Otherwise, this function fails and returns nil.
 */
- (ZoomVideoSDKBroadcastStreamingHelper *_Nullable)getBroadcastStreamingHelper;

/**
 * @brief Gets the broadcast streaming viewer object.
 * @return If the function succeeds, it returns a ZoomVideoSDKBroadcastStreamingViewerHelper object. Otherwise, this function fails and returns nil.
 */
- (ZoomVideoSDKBroadcastStreamingViewerHelper * _Nullable)getBroadcastStreamingViewerHelper;

/**
 * @brief Gets the whiteboard helper object.
 * @return If the function succeeds, it returns a ZoomVideoSDKWhiteboardHelper object. Otherwise, this function fails and returns nil.
 */
- (ZoomVideoSDKWhiteboardHelper * _Nullable)getWhiteboardHelper  API_UNAVAILABLE(visionos);

/**
 * @brief Gets an instance to manage RTMS (Real-Time Media Streams) during a video SDK session.
 * @return If the function succeeds, it returns a ZoomVideoRealTimeMediaStreamsHelper object. Otherwise, this function fails and returns nil.
 */
- (ZoomVideoRealTimeMediaStreamsHelper * _Nullable)getRealTimeMediaStreamsHelper;

@end
