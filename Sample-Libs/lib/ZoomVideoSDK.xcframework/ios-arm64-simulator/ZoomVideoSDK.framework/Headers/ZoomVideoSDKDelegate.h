/**
 * @file ZoomVideoSDKDelegate.h
 * @brief Protocol defining callback methods for session events, user events, audio/video events, sharing events, and other SDK notifications.
 */

#import <Foundation/Foundation.h>
#import <ZoomVideoSDK/ZoomVideoSDKConstants.h>
#import <ZoomVideoSDK/ZoomVideoSDKVideoRawData.h>
#import <ZoomVideoSDK/ZoomVideoSDKAudioRawData.h>
#import <ZoomVideoSDK/ZoomVideoSDKChatHelper.h>
#import <ZoomVideoSDK/ZoomVideoSDKPreProcessRawData.h>
#import <ZoomVideoSDK/ZoomVideoSDKVideoSender.h>
#import <ZoomVideoSDK/ZoomVideoSDKShareSender.h>
#import <ZoomVideoSDK/ZoomVideoSDKShareAudioSender.h>
#import <ZoomVideoSDK/ZoomVideoSDKAudioSender.h>
#import <ZoomVideoSDK/ZoomVideoSDKVideoCapability.h>
#import <ZoomVideoSDK/ZoomVideoSDKVideoHelper.h>
#import <ZoomVideoSDK/ZoomVideoSDKAudioHelper.h>
#import <ZoomVideoSDK/ZoomVideoSDKShareHelper.h>
#import <ZoomVideoSDK/ZoomVideoSDKRecordingHelper.h>
#import <ZoomVideoSDK/ZoomVideoSDKLiveStreamHelper.h>
#import <ZoomVideoSDK/ZoomVideoSDKUserHelper.h>
#import <ZoomVideoSDK/ZoomVideoSDKLiveTranscriptionHelper.h>
#import <ZoomVideoSDK/ZoomVideoSDKFileTranserHandle.h>
#import <ZoomVideoSDK/ZoomVideoSDKSubSessionHelper.h>
#import <ZoomVideoSDK/ZoomVideoSDKWhiteboardHelper.h>
#import <ReplayKit/ReplayKit.h>
@class ZoomVideoSDKRawDataPipe;
@class ZoomVideoSDKVideoCanvas;
@class ZoomVideoSDKUser;
@class ZoomVideoSDKProxySettingHandler;
@class ZoomVideoSDKSSLCertificateInfo;
@class ZoomVideoSDKSubSessionUserHelpRequestHandler;

/**
 * @protocol ZoomVideoSDKDelegate
 * @brief A listener class that groups together the callbacks related to a session.
 */
@protocol ZoomVideoSDKDelegate <NSObject>
@optional
/**
 * @brief Callback event when the current user joins the session.
 */
- (void)onSessionJoin;

/**
 * @brief Callback event when the current user leaves the session.
 * @deprecated Use \link onSessionLeave: \endlink instead.
 */
- (void)onSessionLeave DEPRECATED_MSG_ATTRIBUTE("use onSessionLeave: instead");

/**
 * @brief Callback event when the current user leaves the session with reason.
 * @param reason The leave session reason.
 */
- (void)onSessionLeave:(ZoomVideoSDKSessionLeaveReason)reason;

/**
 * @brief Callback event when errors occur.
 * @param ErrorType The error code associated with the error.
 * @param details The detail of error message.
 */
- (void)onError:(ZoomVideoSDKError)ErrorType detail:(NSInteger)details;

/**
 * @brief Callback event when a user joins the session.
 * @param helper The user helper utility.
 * @param userArray The list of users who have just joined the session.
 */
- (void)onUserJoin:(ZoomVideoSDKUserHelper * _Nullable)helper users:(NSArray <ZoomVideoSDKUser *>* _Nullable)userArray;

/**
 * @brief Callback event when a user leaves the session.
 * @param helper The user helper utility.
 * @param userArray The list of users who have just left the session.
 */
- (void)onUserLeave:(ZoomVideoSDKUserHelper * _Nullable)helper users:(NSArray <ZoomVideoSDKUser *>* _Nullable)userArray;

/**
 * @brief Callback event when a user makes changes to their video, such as starting or stopping their video.
 * @param helper The video helper utility.
 * @param userArray The list of users whose video status has changed.
 */
- (void)onUserVideoStatusChanged:(ZoomVideoSDKVideoHelper * _Nullable)helper user:(NSArray <ZoomVideoSDKUser *>* _Nullable)userArray;

/**
 * @brief Callback event when a user makes changes to their audio, such as muting or unmuting their audio.
 * @param helper The audio helper utility.
 * @param userArray The list of users whose audio status has changed.
 */
- (void)onUserAudioStatusChanged:(ZoomVideoSDKAudioHelper * _Nullable)helper user:(NSArray <ZoomVideoSDKUser *>* _Nullable)userArray;

/**
 * @brief Callback event when a user makes changes to their sharing status, such as starting screen sharing, starting view sharing, or stopping sharing.
 * @param helper The share helper object.
 * @param user The user object.
 * @param shareAction The ZoomVideoSDKShareAction object.
 */
- (void)onUserShareStatusChanged:(ZoomVideoSDKShareHelper * _Nullable)helper user:(ZoomVideoSDKUser * _Nullable)user shareAction:(ZoomVideoSDKShareAction*_Nullable)shareAction;

/**
 * @brief Callback event when a user fails to start sharing.
 * @param helper The share helper object.
 * @param user The user object.
 */
- (void)onFailedToStartShare:(ZoomVideoSDKShareHelper* _Nonnull)helper user:(ZoomVideoSDKUser* _Nullable)user;

/**
 * @brief Callback event when the share setting changes.
 * @param setting The share setting.
 */
- (void)onShareSettingChanged:(ZoomVideoSDKShareSetting)setting;

/**
 * @brief Callback event when a user makes changes to their live stream status.
 * @param helper The live stream helper utility.
 * @param status The live stream status.
 */
- (void)onLiveStreamStatusChanged:(ZoomVideoSDKLiveStreamHelper * _Nullable)helper status:(ZoomVideoSDKLiveStreamStatus)status;

/**
 * @brief Callback event when the chat privilege of participant has changed.
 * @param helper The chat helper utility.
 * @param currentPrivilege The new chat privilege.
 */
- (void)onChatPrivilegeChanged:(ZoomVideoSDKChatHelper * _Nullable)helper privilege:(ZoomVideoSDKChatPrivilegeType)currentPrivilege;

/**
 * @brief Callback event when receiving a chat message.
 * @param helper The chat helper utility.
 * @param chatMessage The object which contains the message content.
 */
- (void)onChatNewMessageNotify:(ZoomVideoSDKChatHelper * _Nullable)helper message:(ZoomVideoSDKChatMessage * _Nullable)chatMessage;

/**
 * @brief Callback event when a user deletes a chat message.
 * @param helper The chat helper utility.
 * @param msgID The deleted message's ID.
 * @param type Indicates by whom the message was deleted, which is defined in [ZoomVideoSDKChatMsgDeleteBy].
 */
- (void)onChatMsgDeleteNotification:(ZoomVideoSDKChatHelper * _Nullable)helper messageID:(NSString * _Nonnull)msgID deleteBy:(ZoomVideoSDKChatMsgDeleteBy)type;

/**
 * @brief Callback event when the session host changes.
 * @param helper The user helper utility.
 * @param user The host user.
 */
- (void)onUserHostChanged:(ZoomVideoSDKUserHelper * _Nullable)helper users:(ZoomVideoSDKUser * _Nullable)user;

/**
 * @brief Callback event when the manager of the session changes.
 * @param user The manager user.
 */
- (void)onUserManagerChanged:(ZoomVideoSDKUser * _Nullable)user;

/**
 * @brief Callback event when a user changes their name.
 * @param user The user who changed their name.
 */
- (void)onUserNameChanged:(ZoomVideoSDKUser * _Nullable)user;

/**
 * @brief Callback event when the active audio changes.
 * @param helper The audio helper utility.
 * @param userArray The active audio list.
 */
- (void)onUserActiveAudioChanged:(ZoomVideoSDKUserHelper * _Nullable)helper users:(NSArray <ZoomVideoSDKUser *>* _Nullable)userArray;

/**
 * @brief Callback event when the session requires a password to join.
 * @param completion The block for this callback.
 */
- (void)onSessionNeedPassword:(ZoomVideoSDKError (^ _Nullable)(NSString * _Nullable password, BOOL leaveSessionIgnorePassword))completion;

/**
 * @brief Callback event when the provided session password is wrong or invalid.
 * @param completion The block for this callback.
 */
- (void)onSessionPasswordWrong:(ZoomVideoSDKError (^ _Nullable)(NSString * _Nullable password, BOOL leaveSessionIgnorePassword))completion;

/**
 * @brief Callback event when mixed (all users) audio raw data is received.
 * @param rawData The audio raw data.
 */
- (void)onMixedAudioRawDataReceived:(ZoomVideoSDKAudioRawData * _Nullable)rawData;

/**
 * @brief Callback event when individual user's audio raw data is received.
 * @param rawData The raw audio data.
 * @param user The user who received the audio raw data.
 */
- (void)onOneWayAudioRawDataReceived:(ZoomVideoSDKAudioRawData * _Nullable)rawData user:(ZoomVideoSDKUser * _Nullable)user;

/**
 * @brief Callback event when receiving shared raw audio data.
 * @param rawData The raw audio data.
 */
- (void)onSharedAudioRawDataReceived:(ZoomVideoSDKAudioRawData * _Nullable)rawData;

/**
 * @brief Callback event when the invite by phone status changes to any other valid status such as Calling, Ringing, Success, or Failed.
 * @param status The invite by phone status.
 * @param failReason The invite by phone failed reason.
 */
- (void)onInviteByPhoneStatus:(ZoomVideoSDKPhoneStatus)status failReason:(ZoomVideoSDKPhoneFailedReason)failReason;

/**
 * @brief Callback event when the invite by phone user joined session successfully.
 * @param user The successfully joined user.
 * @param phoneNumber The phone number of callout.
 */
- (void)onCalloutJoinSuccess:(ZoomVideoSDKUser * _Nullable)user phoneNumber:(NSString * _Nullable)phoneNumber;

/**
 * @brief Callback event when the command channel is ready to be used. When the SDK attempts to establish a connection for the command channel when joining a session, this callback is triggered once the connection attempt for the command channel is completed.
 * @param success YES if the command channel is ready to be used. NO if the command channel was unable to connect.
 */
- (void)onCmdChannelConnectResult:(BOOL)success;

/**
 * @brief Callback event when receiving a message, data, or a command from the command channel. Once the command channel is active, this callback is triggered each time a message is received.
 * @param commandContent The received command.
 * @param sendUser The user who sent the command.
 */
- (void)onCommandReceived:(NSString * _Nullable)commandContent sendUser:(ZoomVideoSDKUser * _Nullable)sendUser;

/**
 * @brief Callback event when the current user is granted camera control access.
 * @param user The user who received the request.
 * @param isApproved The result of the camera control request.
 * @note Once the current user sends the camera control request, this callback will be triggered with the result of the request.
 */
- (void)onCameraControlRequestResult:(ZoomVideoSDKUser* _Nullable)user approved:(BOOL)isApproved;

/**
 * @brief Callback event when cloud recording status has started, paused, stopped, resumed, or otherwise changed.
 * @param status The cloud recording status.
 * @param handler The handler that can handle the action user Accept or Decline.
 */
- (void)onCloudRecordingStatus:(ZoomVideoSDKRecordingStatus)status recordAgreementHandler:(ZoomVideoSDKRecordAgreementHandler * _Nullable)handler;

/**
 * @brief Callback event when user agreement status changes.
 * @param user The user which agreed to the record.
 */
- (void)onUserRecordAgreementNotification:(ZoomVideoSDKUser * _Nullable)user;

/**
 * @brief Callback event when a host requests you to unmute yourself.
 */
- (void)onHostAskUnmute;

/**
 * @brief Callback event when someone in a given session enables or disables multi-camera. All participants in the session receive this callback.
 * @param status The status for multiple camera user join or left.
 * @param user The user who enabled multi-camera.
 * @param videoPipe The data pipe for the multi-camera.
 */
- (void)onMultiCameraStreamStatusChanged:(ZoomVideoSDKMultiCameraStreamStatus)status parentUser:(ZoomVideoSDKUser *_Nullable)user videoPipe:(ZoomVideoSDKRawDataPipe *_Nullable)videoPipe;

/**
 * @brief Callback event when someone enables or disables multi-camera. This callback is received by all participants.
 * @param status The status for multiple camera user join or left.
 * @param user The user who enabled multi-camera.
 * @param videoCanvas The video canvas for the multi-camera.
 */
- (void)onMultiCameraStreamStatusChanged:(ZoomVideoSDKMultiCameraStreamStatus)status parentUser:(ZoomVideoSDKUser *_Nullable)user videoCanvas:(ZoomVideoSDKVideoCanvas *_Nullable)videoCanvas;

/**
 * @brief Callback event when the mic status changes during testing.
 * @param status The mic status.
 */
- (void)onTestMicStatusChanged:(ZoomVideoSDKTestMicStatus)status;

/**
 * @brief Callback event when the current mic or speaker volume changes during testing.
 * @param micVolume The volume of the mic when testing or in session.
 * @param speakerVolume The volume of the speaker when testing.
 */
- (void)onMicSpeakerVolumeChanged:(int)micVolume speakerVolume:(int)speakerVolume;

/**
 * @brief Callback event when the audio level of participants changes.
 * @param level The current audio level of the user, in the range [0, 9].
 * @param bAudioSharing YES if the audio level is from shared audio, such as computer audio, NO if it's from the microphone.
 * @param user The user whose audio level has changed.
 */
- (void)onAudioLevelChanged:(NSUInteger)level audioSharing:(BOOL)bAudioSharing user:(ZoomVideoSDKUser * _Nullable)user;

/**
 * @brief Callback event when the SDK requires system permissions to continue functioning.
 * @param permissionType The type of system permission that is required.
 */
- (void)onSystemPermissionRequired:(ZoomVideoSDKSystemPermissionType)permissionType;

/**
 * @brief Callback event when live transcription status changes.
 * @param status The live transcription status.
 */
- (void)onLiveTranscriptionStatus:(ZoomVideoSDKLiveTranscriptionStatus)status;

/**
 * @brief Callback event when a live transcription message is received.
 * @param messageInfo The live transcription message.
 */
- (void)onLiveTranscriptionMsgReceived:(ZoomVideoSDKLiveTranscriptionMessageInfo *_Nullable)messageInfo;

/**
 * @brief Callback event when an original language message is received.
 * @param messageInfo The spoken language message.
 */
- (void)onOriginalLanguageMsgReceived:(ZoomVideoSDKLiveTranscriptionMessageInfo *_Nullable)messageInfo;

/**
 * @brief Callback event when a live translation error occurs.
 * @param spokenLanguage The spoken message language.
 * @param transcriptLanguage The message language you want to translate.
 */
- (void)onLiveTranscriptionMsgError:(ZoomVideoSDKLiveTranscriptionLanguage *_Nullable)spokenLanguage transLanguage:(ZoomVideoSDKLiveTranscriptionLanguage *_Nullable)transcriptLanguage;

/**
 * @brief Callback event when the spoken language has changed.
 * @param spokenLanguage The spoken message language.
 */
- (void)onSpokenLanguageChanged:(ZoomVideoSDKLiveTranscriptionLanguage* _Nullable)spokenLanguage;

/**
 * @brief Callback event when the proxy requests to input the username and password. Use the handler to configure the related information.
 * @param handler The handler will be destroyed once the function calls end.
 */
- (void)onProxySettingNotification:(ZoomVideoSDKProxySettingHandler *_Nonnull)handler;

/**
 * @brief Callback event when the SSL is verified. Use the handler to check the related information.
 * @param handler The handler will be destroyed once the function calls end.
 */
- (void)onSSLCertVerifiedFailNotification:(ZoomVideoSDKSSLCertificateInfo *_Nonnull)handler;


/**
 * @brief Callback event when a user's network status changes (Lite SDK only).
 * @param status The user's network status.
 * @param user The user whose network status has changed.
 * @deprecated Use \link onUserNetworkStatusChanged:level:user: \endlink instead.
 */
- (void)onUserVideoNetworkStatusChanged:(ZoomVideoSDKNetworkStatus)status user:(ZoomVideoSDKUser *_Nullable)user DEPRECATED_MSG_ATTRIBUTE("Use -onUserNetworkStatusChanged:level:user: instead");

/**
 * @brief Callback event when the current user's share network quality changes.
 * @param shareNetworkStatus The share network quality.
 * @param isSendingShare YES if it refers to the sending share, NO if it refers to the receiving share.
 * @deprecated Use \link onUserNetworkStatusChanged:level:user: \endlink instead.
 */
- (void)onUserShareNetworkStatusChanged:(ZoomVideoSDKNetworkStatus)shareNetworkStatus isSending:(BOOL)isSendingShare DEPRECATED_MSG_ATTRIBUTE("Use -onUserNetworkStatusChanged:level:user: instead");

/**
 * @brief Callback event when the user's network quality changes.
 * @param type The data type whose network status changed.
 * @param level The new network quality level for the specified data type.
 * @param user The user whose network status has changed.
 */
- (void)onUserNetworkStatusChanged:(ZoomVideoSDKDataType)type level:(ZoomVideoSDKNetworkStatus)level user:(ZoomVideoSDKUser * _Nullable)user;

/**
 * @brief Callback event when a user's overall network status changes.
 * @param level The new overall network quality level.
 * @param user The user whose overall network status has changed.
 */
- (void)onUserOverallNetworkStatusChanged:(ZoomVideoSDKNetworkStatus)level user:(ZoomVideoSDKUser * _Nullable)user;


/**
 * @brief Callback event of the call CRC device's status.
 * @param state The call out status.
 */
- (void)onCallCRCDeviceStatusChanged:(ZoomVideoSDKCRCCallStatus)state;

/**
 * @brief Callback event when the annotation helper is cleaned up. After this callback, SDK will release the ZoomVideoSDKAnnotationHelper as well.
 * @param helper The helper cleanup object.
 */
- (void)onAnnotationHelperCleanUp:(ZoomVideoSDKAnnotationHelper *_Nullable)helper;

/**
 * @brief Callback event when the annotation privilege changes.
 * @param enable YES if the user is able to annotate, NO if the user is not able to annotate.
 * @param user The user who changed viewer's annotation privilege.
 * @deprecated Use \link -[ZoomVideoSDKDelegate onAnnotationPrivilegeChangeWithUser:shareAction:] \endlink instead.
 */
- (void)onAnnotationPrivilegeChange:(BOOL)enable shareOwner:(ZoomVideoSDKUser *_Nullable)user DEPRECATED_MSG_ATTRIBUTE("use -[ZoomVideoSDKDelegate onAnnotationPrivilegeChangeWithUser:shareAction:] instead");

/**
 * @brief Callback event when the annotation privilege changes.
 * @param user The user object who changed viewer's annotation privilege.
 * @param shareAction The share object.
 */
- (void)onAnnotationPrivilegeChangeWithUser:(ZoomVideoSDKUser *_Nullable)user shareAction:(ZoomVideoSDKShareAction*_Nullable)shareAction;

/**
 * @brief Callback event when the annotation tool type changes.
 * @param helper The helper cleanup object.
 * @param view The view that is annotating.
 * @param toolType The new annotation tool type currently selected.
 */
- (void)onAnnotationToolTypeChanged:(ZoomVideoSDKAnnotationHelper *_Nullable)helper view:(UIView *_Nullable)view toolType:(ZoomVideoSDKAnnotationToolType)toolType;
/**
 * @brief Callback event when a user makes changes to their share content type, such as switching camera share to normal share. Find the share type in \link ZoomVideoSDKShareType \endlink.
 * @param shareHelper The share helper utility.
 * @param user The current start or stop share user.
 * @param shareAction The share object.
 */
- (void)onShareContentChanged:(ZoomVideoSDKShareHelper *_Nullable)shareHelper user:(ZoomVideoSDKUser *_Nullable)user shareAction:(ZoomVideoSDKShareAction *_Nullable)shareAction;

/**
 * @brief Callback event when the size of a user's share capture changes.
 * @param shareHelper The share helper object associated with the share.
 * @param user The user whose share capture size has changed.
 * @param shareAction The share action details, such as start, stop, or resize.
 */
- (void)onShareCaptureSizeChanged:(ZoomVideoSDKShareHelper *_Nullable)shareHelper user:(ZoomVideoSDKUser *_Nullable)user shareAction:(ZoomVideoSDKShareAction *_Nullable)shareAction;

/**
 * @brief Callback event for the subscribed user's video fail reason.
 * @param failReason The fail reason.
 * @param user The pointer to the user whose view we would like to subscribe.
 * @param view The view that failed to subscribe.
 */
- (void)onVideoCanvasSubscribeFail:(ZoomVideoSDKSubscribeFailReason)failReason user:(ZoomVideoSDKUser *_Nullable)user view:(UIView *_Nullable)view;

/**
 * @brief Callback event for the subscribed user's share fail reason.
 * @param failReason The fail reason.
 * @param user The pointer to the user whose view we would like to subscribe.
 * @param view The view that failed to subscribe.
 * @deprecated use -[ZoomVideoSDKDelegate onShareCanvasSubscribeFailWithUser:view:shareAction:] instead.
 */
- (void)onShareCanvasSubscribeFail:(ZoomVideoSDKSubscribeFailReason)failReason user:(ZoomVideoSDKUser *_Nullable)user view:(UIView *_Nullable)view DEPRECATED_MSG_ATTRIBUTE("use -[ZoomVideoSDKDelegate onShareCanvasSubscribeFailWithUser:view:shareAction:] instead");

/**
 * @brief Callback event for the share canvas that failed to subscribe.
 * @param user The pointer to the user object whose view we would like to subscribe to.
 * @param view The view that failed to subscribe.
 * @param shareAction The pointer to a share object.
 */
- (void)onShareCanvasSubscribeFailWithUser:(ZoomVideoSDKUser *_Nullable)user view:(UIView *_Nullable)view shareAction:(ZoomVideoSDKShareAction*_Nullable)shareAction;

/**
 * @brief Callback event when send file status changes.
 * @param file The send file object.
 * @param status The status of file transfer.
 */
- (void)onSendFileStatus:(ZoomVideoSDKSendFile * _Nullable)file status:(ZoomVideoSDKFileTransferStatus)status;

/**
 * @brief Callback event when receive file status changes.
 * @param file The receive file object.
 * @param status The status of file transfer.
 */
- (void)onReceiveFileStatus:(ZoomVideoSDKReceiveFile * _Nullable)file status:(ZoomVideoSDKFileTransferStatus)status;

/**
 * @brief Callback event when video alpha channel mode changes.
 * @param isAlphaChannelOn YES if alpha channel mode is on, NO otherwise.
 */
- (void)onVideoAlphaChannelStatusChanged:(BOOL)isAlphaChannelOn;

/**
 * @brief Callback event when spotlighted video user changes.
 * @param videoHelper The video helper object.
 * @param userList The list of users who have been spotlighted.
 */
- (void)onSpotlightVideoChanged:(ZoomVideoSDKVideoHelper * _Nullable)videoHelper userList:(NSArray <ZoomVideoSDKUser *>* _Nullable)userList;

/**
 * @brief Callback event when binding the incoming live stream.
 * @param helper The incoming live stream helper utility.
 * @param success YES if success, NO otherwise.
 * @param streamKeyID The corresponding stream key ID.
 */
- (void)onBindIncomingLiveStreamResponse:(ZoomVideoSDKIncomingLiveStreamHelper * _Nullable)helper success:(BOOL)success streamKeyID:(NSString *_Nullable)streamKeyID;

/**
 * @brief Callback event when unbinding the incoming live stream.
 * @param helper The incoming live stream helper utility.
 * @param success YES if success, NO otherwise.
 * @param streamKeyID The corresponding stream key ID.
 */
- (void)onUnbindIncomingLiveStreamResponse:(ZoomVideoSDKIncomingLiveStreamHelper * _Nullable)helper success:(BOOL)success streamKeyID:(NSString *_Nullable)streamKeyID;

/**
 * @brief Callback event when getting the streams status.
 * @param helper The incoming live stream helper utility.
 * @param success YES if success, NO otherwise.
 * @param streamsStatusList The streams status list.
 */
- (void)onIncomingLiveStreamStatusResponse:(ZoomVideoSDKIncomingLiveStreamHelper * _Nullable)helper success:(BOOL)success streamsStatusList:(NSArray<ZoomVideoSDKIncomingLiveStreamStatus *> *_Nullable)streamsStatusList;

/**
 * @brief Callback event when starting the bound stream.
 * @param helper The incoming live stream helper utility.
 * @param success YES if success, NO otherwise.
 * @param streamKeyID The corresponding stream key ID.
 */
- (void)onStartIncomingLiveStreamResponse:(ZoomVideoSDKIncomingLiveStreamHelper * _Nullable)helper success:(BOOL)success streamKeyID:(NSString *_Nullable)streamKeyID;

/**
 * @brief Callback event when stopping the bound stream.
 * @param helper The incoming live stream helper utility.
 * @param success YES if success, NO otherwise.
 * @param streamKeyID The corresponding stream key ID.
 */
- (void)onStopIncomingLiveStreamResponse:(ZoomVideoSDKIncomingLiveStreamHelper * _Nullable)helper success:(BOOL)success streamKeyID:(NSString *_Nullable)streamKeyID;

/**
 * @brief Callback event when the share content size has changed.
 * @param helper The share helper object.
 * @param user The user object whose share content size has changed.
 * @param shareAction The ZoomVideoSDKShareAction object.
 * @note A share content size change may occur in two cases: when the first frame of shared content is received (from no content to having a size), or when the content size actually changes during sharing.
 */
- (void)onShareContentSizeChanged:(ZoomVideoSDKShareHelper * _Nullable)helper user:(ZoomVideoSDKUser * _Nullable)user shareAction:(ZoomVideoSDKShareAction*_Nullable)shareAction;

#pragma mark - external camera change -
/**
 * @brief Callback event when the external UVC camera status changes.
 * @param status The UVC camera status.
 * @warning Only iOS 17.0 or above and iPad device can receive this callback.
 */
- (void)onUVCCameraStatusChange:(ZoomVideoSDKUVCCameraStatus)status;

#pragma mark - sub-session -
/**
 * @brief Callback event when the SubSession status changes.
 * @param status The SubSession status.
 * @param pSubSessionKitList The new SubSession list.
 */
- (void)onSubSessionStatusChanged:(ZoomVideoSDKSubSessionStatus)status subSession:(NSArray <ZoomVideoSDKSubSessionKit*>* _Nonnull)pSubSessionKitList;

/**
 * @brief Callback event when the user has SubSession manager privilege.
 * @param pManager The SubSession manager object.
 */
- (void)onSubSessionManagerHandle:(ZoomVideoSDKSubSessionManager* _Nullable)pManager;

/**
 * @brief Callback event when the user has SubSession attendee privilege.
 * @param pParticipant The SubSession attendee object.
 */
- (void)onSubSessionParticipantHandle:(ZoomVideoSDKSubSessionParticipant* _Nullable)pParticipant;

/**
 * @brief Callback event when the users of a SubSession have updated.
 * @param pSubSessionKit The SubSession kit object.
 */
- (void)onSubSessionUsersUpdate:(ZoomVideoSDKSubSessionKit* _Nonnull)pSubSessionKit;

/**
 * @brief Callback event when the user receives the main session broadcast message.
 * @param sMessage The message content.
 * @param sUserName The name of the user who broadcast this message.
 */
- (void)onBroadcastMessageFromMainSession:(NSString* _Nonnull) sMessage userName:(NSString* _Nonnull)sUserName;

/**
 * @brief Callback event when the user receives the help request from SubSession.
 * @param pHandler The request handler object.
 */
- (void)onSubSessionUserHelpRequestHandler:(ZoomVideoSDKSubSessionUserHelpRequestHandler*_Nonnull) pHandler;

/**
 * @brief Callback event when the result of help request is received.
 * @param result The result of help request.
 */
- (void)onSubSessionUserHelpRequestResult:(ZoomVideoSDKUserHelpRequestResult)result;

#pragma mark - ZoomVideoSDK audio source change -
/**
 * @brief Callback event when the current user's audio source changes.
 * @param device The updated audio device.
 */
- (void)onMyAudioSourceTypeChanged:(ZoomVideoSDKAudioDevice *_Nullable)device;

#pragma mark - In app screen share -
/**
 * @brief Callback event when in-app screen sharing session encounters an error or success.
 * @param code The error code.
 */
- (void)onInAppScreenShareErrorCode:(RPRecordingErrorCode)code;

/**
 * @brief Callback event when the availability of in-app screen sharing changes. Use this callback to enable or disable screen sharing controls in your UI based on current availability.
 * @param available YES if available, NO otherwise.
 */
- (void)onInAppScreenShareAvailableChanged:(BOOL)available;

/**
 * @brief Callback event when the response of broadcast streaming is received.
 * @param isSuccess YES if broadcast streaming started successfully, NO otherwise.
 * @param channelID The backend service to identify the broadcast streaming channel ID returned by the backend.
 */
- (void)onStartBroadcastResponse:(BOOL)isSuccess channelID:(NSString* _Nonnull)channelID ;

/**
 * @brief Callback event when the response of broadcast streaming is received.
 * @param isSuccess YES if broadcast stopped successfully, NO otherwise.
 */
- (void)onStopBroadcastResponse:(BOOL)isSuccess;

/**
 * @brief Callback event when the response that gets broadcast status is received.
 * @param isSuccess YES if the request succeeded, NO otherwise.
 * @param status The current broadcast status.
 */
- (void)onGetBroadcastControlStatus:(BOOL)isSuccess status:(ZoomVideoSDKBroadcastControlStatus)status;

/**
 * @brief Callback event when the viewer's join status changes.
 * @param status The current join status.
 */
- (void)onStreamingJoinStatusChanged:(ZoomVideoSDKStreamingJoinStatus)status ;

/**
 * @brief Callback event when a whiteboard file export completes.
 * @param format The export format.
 * @param data The export whiteboard data as NSData.
 */
- (void)onWhiteboardExported:(ZoomVideoSDKWhiteboardExportFormatType)format data:(NSData*_Nonnull)data  API_UNAVAILABLE(visionos);

/**
 * @brief Callback event when a user changes their whiteboard sharing status, such as starting or stopping whiteboard sharing.
 * @param user The user whose whiteboard sharing status changed.
 * @param whiteboardHelper The whiteboard helper object.
 */
-(void)onUserWhiteboardShareStatusChanged:(ZoomVideoSDKUser *_Nonnull)user whiteboardhelper:(ZoomVideoSDKWhiteboardHelper*_Nonnull)whiteboardHelper  API_UNAVAILABLE(visionos);

/**
 * @brief Callback invoked when a canvas snapshot is successfully taken.
 * @param user The user who took the snapshot.
 * @param isShare YES if the snapshot is of shared content; NO if it is of video content.
 */
-(void)onCanvasSnapshotTaken:(ZoomVideoSDKUser *_Nonnull)user isShare:(BOOL)isShare;

/**
 * @brief Callback invoked when a canvas snapshot is attempted but cannot be taken due to compatibility issues with the target user.
 * @param user The target user whose snapshot could not be taken due to compatibility issues.
 */
-(void)onCanvasSnapshotIncompatible:(ZoomVideoSDKUser *_Nonnull)user;

/**
 * @brief Callback event when the real-time media stream's status changes.
 * @param status The real-time media streams status.
 */
- (void)onRealTimeMediaStreamsStatus:(ZoomVideoSDKRealTimeMediaStreamsStatus)status;

/**
 * @brief Callback event when the real-time media stream failure reason is received.
 * @param failReason The real-time media stream failure reason.
 */
- (void)onRealTimeMediaStreamsFail:(ZoomVideoSDKRealTimeMediaStreamsFailReason)failReason;

@end

#pragma mark - ZoomVideoSDKVirtualAudioSpeaker
/**
 * @protocol ZoomVideoSDKVirtualAudioSpeaker
 * @brief An interface that handles the virtual audio speaker.
 */
@protocol ZoomVideoSDKVirtualAudioSpeaker <NSObject>

@optional

/**
 * @brief Callback event when receiving the mixed audio raw data from the virtual speaker.
 * @param rawData The audio raw data.
 */
- (void)onVirtualSpeakerMixedAudioReceived:(ZoomVideoSDKAudioRawData * _Nullable)rawData;

/**
 * @brief Callback event when receiving the one way audio raw data of a selected user from the virtual speaker.
 * @param rawData The audio raw data.
 * @param user The user whose audio raw data is received.
 */
- (void)onVirtualSpeakerOneWayAudioReceived:(ZoomVideoSDKAudioRawData * _Nullable)rawData user:(ZoomVideoSDKUser * _Nullable)user;

/**
 * @brief Callback event when receiving the share audio raw data such as buffer, sampleRate, etc.
 * @param rawData The audio raw data.
 */
- (void)onVirtualSpeakerSharedAudioReceived:(ZoomVideoSDKAudioRawData * _Nullable)rawData;

@end

#pragma mark - ZoomVideoSDKRawDataPipeDelegate
/**
 * @protocol ZoomVideoSDKRawDataPipeDelegate
 * @brief Methods to manage events for receiving video raw data.
 */
@protocol ZoomVideoSDKRawDataPipeDelegate <NSObject>

@optional

/**
 * @brief Callback event when receiving video's NV12 data (CVPixelBufferRef).
 * @param pixelBuffer The video's CVPixelBufferRef data.
 * @param rotation The video rotation.
 */
- (void)onPixelBuffer:(CVPixelBufferRef _Nullable )pixelBuffer
             rotation:(ZoomVideoSDKVideoRawDataRotation)rotation;

/**
 * @brief Callback event when receiving video's YUV420 data.
 * @param rawData The video's YUV420 data.
 */
- (void)onRawDataFrameReceived:(ZoomVideoSDKVideoRawData * _Nullable)rawData;

/**
 * @brief Callback event when the sender stops or starts sending raw data.
 * @param userRawdataStatus The raw data sending status.
 */
- (void)onRawDataStatusChanged:(ZoomVideoSDKUserRawdataStatus)userRawdataStatus;

@end

#pragma mark - ZoomVideoSDKVideoSourcePreProcessor
/**
 * @protocol ZoomVideoSDKVideoSourcePreProcessor
 * @brief Methods to modify default device capture raw data.
 */
@protocol ZoomVideoSDKVideoSourcePreProcessor <NSObject>

@optional
/**
 * @brief Callback event when device capture video frame is received.
 * @param rawData The pointer of YUV process data I420 object.
 */
- (void)onPreProcessRawData:(ZoomVideoSDKPreProcessRawData * _Nullable)rawData;

@end

#pragma mark - ZoomVideoSDKVideoSource
/**
 * @protocol ZoomVideoSDKVideoSource
 * @brief Custom external video source interface.
 */
@protocol ZoomVideoSDKVideoSource <NSObject>

@optional

/**
 * @brief Callback event when video source is prepared.
 * @param rawDataSender The video sender object.
 * @param supportCapabilityArray The list of supported video capability.
 * @param suggestCapability The suggested video capability.
 */
- (void)onInitialize:(ZoomVideoSDKVideoSender *_Nonnull)rawDataSender
supportCapabilityArray:(NSArray *_Nonnull)supportCapabilityArray
   suggestCapability:(ZoomVideoSDKVideoCapability *_Nonnull)suggestCapability;

/**
 * @brief Callback event when video size or fps changes.
 * @param supportCapabilityArray The list of supported video capability.
 * @param suggestCapability The suggested video capability.
 */
- (void)onPropertyChange:(NSArray *_Nonnull)supportCapabilityArray
       suggestCapability:(ZoomVideoSDKVideoCapability *_Nonnull)suggestCapability;

/**
 * @brief Callback event when video source should start sending raw data.
 */
- (void)onStartSend;

/**
 * @brief Callback event when video source should stop sending raw data. For example, when user mutes video.
 */
- (void)onStopSend;

/**
 * @brief Callback event when video source is uninitialized. For example, when leaving session.
 */
- (void)onUninitialized;

@end

/**
 * @protocol ZoomVideoSDKVirtualAudioMic
 * @brief An interface that handles a virtual audio microphone.
 */
@protocol ZoomVideoSDKVirtualAudioMic <NSObject>

@optional
/**
 * @brief Callback event when virtual audio microphone is initialized.
 * @param rawDataSender You can send audio data based on this object.
 */
- (void)onMicInitialize:(ZoomVideoSDKAudioSender *_Nonnull)rawDataSender;

/**
 * @brief Callback event when microphone should start sending raw data. For example, this could be sent if the user unmuted audio.
 */
- (void)onMicStartSend;

/**
 * @brief Callback event when microphone should stop sending raw data. For example, this could be sent if the user muted audio.
 */
- (void)onMicStopSend;

/**
 * @brief Callback event when the microphone is uninitialized. For example, if the user left the session.
 */
- (void)onMicUninitialized;

@end

#pragma mark - ZoomVideoSDKShareSource
/**
 * @protocol ZoomVideoSDKShareSource
 * @brief Custom external share source interface.
 */
@protocol ZoomVideoSDKShareSource <NSObject>

@optional
/**
 * @brief Callback event when share source can start sending raw data.
 * @param rawDataSender The sender object.
 */
- (void)onShareSendStarted:(ZoomVideoSDKShareSender *_Nullable)rawDataSender;

/**
 * @brief Callback event when share source stops sending raw data.
 */
- (void)onShareSendStopped;

@end

#pragma mark - ZoomVideoSDKShareAudioSource
/**
 * @protocol ZoomVideoSDKShareAudioSource
 * @brief Custom external share audio source interface.
 */
@protocol ZoomVideoSDKShareAudioSource <NSObject>

@optional
/**
 * @brief Callback event when audio source should start sending raw data.
 * @param sender The object of ZoomVideoSDKShareAudioSender to send share source.
 */
- (void)onStartSendAudio:(ZoomVideoSDKShareAudioSender *_Nonnull)sender;

/**
 * @brief Callback event when audio source should stop sending raw data.
 */
- (void)onStopSendAudio;

@end

#pragma mark - ZoomVideoSDKSharePreprocessor
/**
 * @protocol ZoomVideoSDKSharePreprocessor
 * @brief The share pre-processor interface.
 */
@protocol ZoomVideoSDKSharePreprocessor <NSObject>

@optional
/**
 * @brief Callback event when calling startShareWithPreprocessing successfully.
 * @param rawData You can get the YUV data address through this object and then pre-process the data.
 * @param sender Use this object to send the processed data out.
 */
- (void)onCapturedRawDataReceived:(ZoomVideoSDKVideoRawData *_Nullable)rawData sharePreprocessSender:(ZoomVideoSDKSharePreprocessSender *_Nullable)sender;

/**
 * @brief Callback event when calling stopShare successfully. In this event notification, you can perform some stop and destroy actions if necessary.
 */
- (void)onShareStopped;

@end



@protocol ZoomVideoSDKBroadcastStreamingVideoDelegate <NSObject>
@optional
/**
 * @brief Callback event when subscribed video data is received.
 * @param rawData The video's YUV420 data.
 */
- (void)onVideoFrameRecieved:(ZoomVideoSDKVideoRawData * _Nullable)rawData;

/**
 * @brief Callback event when subscribed video data is received.
 * @param pixelBuffer The video's CVPixelBufferRef data.
 * @param rotation The video rotation.
 */
- (void)onPixelBuffer:(CVPixelBufferRef _Nullable )pixelBuffer  rotation:(ZoomVideoSDKVideoRawDataRotation)rotation;
@end

@protocol ZoomVideoSDKBroadcastStreamingAudioDelegate <NSObject>
@optional
/**
 * @brief Callback event when subscribed audio data is received.
 * @param rawData The audio data object.
 */
- (void)onAudioRawDataReceived:(ZoomVideoSDKAudioRawData * _Nullable)rawData;
@end
