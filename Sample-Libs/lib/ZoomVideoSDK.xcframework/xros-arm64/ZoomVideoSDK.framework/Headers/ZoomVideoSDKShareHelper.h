/**
 * @file ZoomVideoSDKShareHelper.h
 * @brief Interface for screen sharing operations including view sharing, external source sharing, annotation management, and multi-share support.
 */

#import <Foundation/Foundation.h>
#import <ZoomVideoSDK/ZoomVideoSDKAnnotationHelper.h>

@protocol ZoomVideoSDKShareSource;
@protocol ZoomVideoSDKShareAudioSource;
@protocol ZoomVideoSDKSharePreprocessor;
/**
 * @class ZoomVideoSDKShareStatus
 * @brief Share status of user.
 */
@interface ZoomVideoSDKShareStatus : NSObject
/**
 * @brief The user's share status.
 */
@property (nonatomic, assign) ZoomVideoSDKReceiveSharingStatus  sharingStatus;

@end

@class ZoomVideoSDKVideoCanvas;
@class ZoomVideoSDKRawDataPipe;

/**
 * @class ZoomVideoSDKShareAction
 * @brief Share raw data pipe interface.
 */
@interface ZoomVideoSDKShareAction : NSObject
/**
 * @brief Gets the share source ID.
 * @return The user ID of the user object.
 */
- (NSInteger)getShareSourceId;
/**
 * @brief Gets the render canvas object.
 * @return If the function succeeds, it returns a ZoomVideoSDKVideoCanvas object. Otherwise, this function fails and returns nil.
 */
- (ZoomVideoSDKVideoCanvas *_Nullable)getShareCanvas;
/**
 * @brief Gets the share status.
 * @return Share status of the share object.
 */
- (ZoomVideoSDKReceiveSharingStatus )getShareStatus;
/**
 * @brief Gets the share raw data pipe.
 * @return If the function succeeds, it returns a ZoomVideoSDKRawDataPipe object. Otherwise, this function fails and returns nil.
 */
- (ZoomVideoSDKRawDataPipe *_Nullable)getSharePipe;
/**
 * @brief Gets the subscribe fail reason.
 * @return Failed reason of the subscription.
 */
- (ZoomVideoSDKSubscribeFailReason)getSubscribeFailReason;
/**
 * @brief Determines whether annotation privilege is enabled.
 * @return YES if annotation privilege is enabled. Otherwise, NO.
 */
- (BOOL)isAnnotationPrivilegeEnabled;
/**
 * @brief Gets the share type.
 * @return Share type of the share object.
 */
- (ZoomVideoSDKShareType)getShareType;
/**
 * @brief Gets the size of the share content.
 * @return The size of share content.
 */
- (CGSize)getShareSourceContentSize;

@end

/**
 * @class ZoomVideoSDKSharePreprocessParam
 * @brief Start pre-process share's parameter.
 */
@interface ZoomVideoSDKSharePreprocessParam : NSObject
/**
 * @brief Gets the preprocess type.
 */
@property (nonatomic, assign)   ZoomVideoSDKSharePreprocessType type;
/**
 * @brief Gets the view to share and pre-process. No need to pass when type is none.
 */
@property (nonatomic, weak)     UIView *view;
@end

/**
 * @class ZoomVideoSDKSharePreprocessSender
 * @brief Start pre-process share's sender interface.
 */
@interface ZoomVideoSDKSharePreprocessSender : NSObject

/**
 * @brief Sends preprocessed data.
 * @param rawData The processed data object.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)sendPreprocessedData:(ZoomVideoSDKVideoRawData * _Nullable)rawData;

@end



/**
 * @class ZoomVideoSDKShareHelper
 * @brief Share control interface.
 */
@interface ZoomVideoSDKShareHelper : NSObject

/**
 * @brief Shares a selected view.
 * @param view The view to be shared.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 * @warning View, recommend to pass a single UIView's object, such as UIView, UIImageView or WKWebView.
 * @warning It is not recommended to pass UIView after adding subview WKWebView or UIImageView.
 */
- (ZoomVideoSDKError)startShareWithView:(UIView * _Nullable)view;

/**
 * @brief Shares an external source.
 * @param shareDelegate The external share source.
 * @param audioDelegate The external share audio source.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 * @warning If audioDelegate is non-null, it means share user-defined audio at the same time.
 * @deprecated Use \link startSharingExternalSource:andAudioSource:isPlaying: \endlink instead.
 */
- (ZoomVideoSDKError)startSharingExternalSource:(id<ZoomVideoSDKShareSource> _Nullable)shareDelegate andAudioSource:(id <ZoomVideoSDKShareAudioSource> _Nullable)audioDelegate DEPRECATED_MSG_ATTRIBUTE("use startSharingExternalSource:andAudioSource:isPlaying: instead");

/**
 * @brief Shares an external source.
 * @param shareDelegate The external share source.
 * @param audioDelegate The external share audio source.
 * @param isPlaying YES to play shared audio raw data, NO not to play.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 * @warning If audioDelegate is non-null, it means share user-defined audio at the same time.
 */
- (ZoomVideoSDKError)startSharingExternalSource:(id<ZoomVideoSDKShareSource> _Nullable)shareDelegate andAudioSource:(id <ZoomVideoSDKShareAudioSource> _Nullable)audioDelegate isPlaying:(BOOL)isPlaying;

/**
 * @brief Enables or disables local playback of shared audio raw data.
 * @param bPlay YES to play shared audio raw data, NO not to play.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)enablePlaySharingAudioRawdata:(BOOL)bPlay;

/**
 * @brief Stops view or screen share.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)stopShare;

/**
 * @brief Pauses share. For camera share, the presenter can pause the share to support annotation.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)pauseShare;

/**
 * @brief Resumes share. For camera share, the presenter can restart the camera share.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)resumeShare;

/**
 * @brief Locks sharing the view or screen. Only the host can call this method.
 * @param lock YES to lock sharing, NO to unlock.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 * @warning Only host or manager can call the function.
 */
- (ZoomVideoSDKError)lockShare:(BOOL)lock;

/**
 * @brief Determines whether sharing the view or screen is locked.
 * @return YES if sharing is locked. Otherwise, NO.
 */
- (BOOL)isShareLocked;

/**
 * @brief Determines whether the current user is sharing.
 * @return YES if the current user is sharing. Otherwise, NO.
 */
- (BOOL)isSharingOut;

/**
 * @brief Determines whether another user is sharing.
 * @return YES if another user is sharing. Otherwise, NO.
 */
- (BOOL)isOtherSharing;

/**
 * @brief Determines whether the current user is sharing the screen.
 * @return YES if the current user is sharing the screen. Otherwise, NO.
 */
- (BOOL)isScreenSharingOut;

/**
 * @brief Determines whether the SDK has enabled share device audio when screen sharing.
 * @return YES if share device audio is enabled. Otherwise, NO.
 */
- (BOOL)isShareDeviceAudioEnabled;

/**
 * @brief Enables or disables the device audio shared together when screen sharing.
 * @param enable YES to share audio out, NO otherwise.
 * @return YES if the function succeeds. Otherwise, NO.
 */
- (BOOL)enableShareDeviceAudio:(BOOL)enable;

/**
 * @brief Determines whether annotation feature is supported.
 * @return YES if annotation feature is supported. Otherwise, NO.
 */
- (BOOL)isAnnotationFeatureSupport;

/**
 * @brief Disables or enables viewer's annotation by the share owner.
 * @param disable YES to disable annotation, NO to enable.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 * @warning Only the share owner can call this function.
 */
- (ZoomVideoSDKError)disableViewerAnnotation:(BOOL)disable;

/**
 * @brief Determines whether annotation on current sharing is disabled.
 * @return YES if annotation is disabled. Otherwise, NO.
 * @warning Only the share owner can call this function.
 */
- (BOOL)isViewerAnnotationDisabled;

/**
 * @brief Creates annotation helper based on shared view.
 * @param view The shared view. Pass nil to return the helper for self sharing.
 * @return If the function succeeds, it returns a ZoomVideoSDKAnnotationHelper object. Otherwise, this function fails and returns nil.
 * @warning The view passed in this function should be subscribed share view. And if the share view aspect mode is full fill, the annotate is not supported. When the share owner does not support the feature of annotate, the others should not do annotate in that case.
 */
- (ZoomVideoSDKAnnotationHelper * _Nullable)createAnnotationHelper:(UIView * _Nullable)view;

/**
 * @brief Destroys annotation helper.
 * @param helper The ZoomVideoSDKAnnotationHelper.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)destroyAnnotationHelper:(ZoomVideoSDKAnnotationHelper * _Nullable)helper;

/**
 * @brief Sets the vanishing tool time config.
 * @param displayTime The time in milliseconds the tool remains visible before fading.
 * @param vanishingTime The time in milliseconds for the tool to fade out after displayTime.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 * @note This setting only takes effect for the user's own share. The displayTime can be 0 or up to 15000 milliseconds (15 seconds), and the vanishingTime must be greater than 1000 milliseconds (1 second) and less than or equal to 15000 milliseconds (15 seconds).
 */
- (ZoomVideoSDKError)setAnnotationVanishingToolTime:(NSUInteger)displayTime vanishingTime:(NSUInteger)vanishingTime;

/**
 * @brief Gets the current vanishing tool display time.
 * @note This function can only retrieve the timer settings for your own share.
 * @return The time in milliseconds the tool remains visible before fading.
 */
- (NSUInteger)getAnnotationVanishingToolDisplayTime;

/**
 * @brief Gets the current vanishing tool vanishing time.
 * @return The time in milliseconds for the tool to fade out after displayTime.
 * @note This function can only retrieve the timer settings for your own share.
 */
- (NSUInteger)getAnnotationVanishingToolVanishingTime;

#pragma mark - multi share -
/**
 * @brief Enables or disables participants can share simultaneously.
 * @param enable YES to enable, NO to disable.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 * @warning When you switch multi share from enable to disable, all sharing will be stopped.
 */
- (ZoomVideoSDKError)enableMultiShare:(BOOL)enable;

/**
 * @brief Determines whether multi share is enabled.
 * @return YES if multi share is enabled. Otherwise, NO.
 */
- (BOOL)isMultiShareEnabled;

#pragma mark - share camera and anno -
/**
 * @brief Shares the selected camera. Query the selected camera using \link ZoomVideoSDKVideoHelper::getCameraDeviceList \endlink.
 * @param parentView The share view to show the camera for the presenter.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 * @note The presenter can use \link ZoomVideoSDKShareHelper::pauseShare \endlink to enable viewer annotation. If the camera is paused, the presenter can use \link ZoomVideoSDKShareHelper::resumeShare \endlink to resume. Notice: The user should start video before start share camera, otherwise the error \link Errors_Session_Share_Camera_Video_Not_Start \endlink.
 */
- (ZoomVideoSDKError)startShareCamera:(UIView *_Nullable)parentView  API_UNAVAILABLE(visionos);

#pragma mark - share with pre-process -
/**
 * @brief Starts share preprocessing.
 * @param param The share preprocessing parameters.
 * @param preProcessDelegate The object that handles preprocessing events.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 * @note If pAudioSource is non-null, it means share user-defined audio at the same time.
 * @note This share type should use the CptShare.framework, zm_annoter_dynamic.framework.
 */
- (ZoomVideoSDKError)startShareWithPreprocessing:(ZoomVideoSDKSharePreprocessParam * _Nullable)param sharePreprocessor:(id<ZoomVideoSDKSharePreprocessor> _Nullable)preProcessDelegate;

/**
 * @brief Applies a UIImage overlay to the shared image stream when using (when you share with startShareWithPreprocessing:sharePreprocessor: or startInAppScreenShare). Use this method to cover the shared content with an image during preprocessing.
 * @param image The UIImage to overlay on top of the shared image. The UIImage is fully covered the current share frame before it is sent to viewers.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)applyPreprocessingOverlayImage:(UIImage *_Nullable)image;

/**
 * @brief Removes the currently applied UIImage overlay from the shared image stream when using (when you share with startShareWithPreprocessing:sharePreprocessor: or startInAppScreenShare).
 * @note Once removed, the raw shared content is sent directly to viewers without any overlay. Call this method when you no longer need to display the overlay (for example, when a covered image is no longer required).
 */
- (void)removePreprocessingOverlayImage;

#pragma mark - in-app screen share -

/**
 * @brief Checks whether the system supports in-app screen share.
 * @return YES if the device supports in-app screen share. Otherwise, NO.
 */
- (BOOL)isSupportInAppScreenShare;

/**
 * @brief Starts in-app screen share.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 * @warning Can only be called once every 500ms.
 */
- (ZoomVideoSDKError)startInAppScreenShare;

/**
 * @brief Sets the annotation view when the view hierarchy changes.
 * @param annoParentView The top-level view for the shared content.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 * @warning This API is only available in in-app screen share mode.
 */
- (ZoomVideoSDKError)setAnnotationView:(UIView *_Nullable)annoParentView;

/**
 * @brief Limits the screen sharing sending resolution to Full HD (1920x1080).
 * @param limit YES to limit the sending resolution to Full HD (1920x1080), NO to send the screen share at its original resolution.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)limitShareSendingResolutionToFullHD:(BOOL)limit;

@end


