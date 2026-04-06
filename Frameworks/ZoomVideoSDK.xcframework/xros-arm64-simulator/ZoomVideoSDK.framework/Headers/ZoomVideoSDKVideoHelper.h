/**
 * @file ZoomVideoSDKVideoHelper.h
 * @brief Interface for managing video operations including camera control, video preview, multi-camera support, spotlight features, and video quality settings.
 */

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#import <ZoomVideoSDK/ZoomVideoSDKConstants.h>
#import <ZoomVideoSDK/ZoomVideoSDKRawDataPipe.h>

@protocol ZoomVideoSDKRawDataPipeDelegate;
@class ZoomVideoSDKRawDataPipe;
@class ZoomVideoSDKVideoCanvas;

/**
 * @class ZoomVideoSDKVideoStatus
 * @brief Video status of user.
 */
@interface ZoomVideoSDKVideoStatus : NSObject
/**
 * @brief Determines whether the camera is turned on.
 */
@property (nonatomic, assign) BOOL          on;

@end


/**
 * @class ZoomVideoSDKVideoPreferenceSetting
 * @brief Video preference interface.When setting custom modes, the maximum and minimum frame rates are provided by the developer. If the current bandwidth cannot maintain the minimum frame rate, the video system will drop to the next lower frame rate. The default maximum and minimum frame rates for other modes are 0.
 */
@interface ZoomVideoSDKVideoPreferenceSetting : NSObject

/**
 * @brief Preferred video mode.
 */
@property (nonatomic, assign) ZoomVideoSDKVideoPreferenceMode mode;

/**
 * @brief Minimum frame rate, default is 0.
 * @note The value of minimumFrameRate range from 0 to 30. Otherwise, will use default frame-rate of Zoom.
 */
@property (nonatomic, assign) NSUInteger minimumFrameRate;

/**
 * @brief Maximum frame rate, default is 0.
 * @note The value of maximumFrameRate range from 0 to 30. Otherwise, will use default frame-rate of Zoom.
 */
@property (nonatomic, assign) NSUInteger maximumFrameRate;

@end

/**
 * @class ZoomVideoSDKCameraDevice
 * @brief Represents a camera device and its state.
 */
@interface ZoomVideoSDKCameraDevice : NSObject
/**
 * @brief Gets the camera device ID.
 */
@property (nonatomic, readonly, nullable, copy) NSString* deviceId;
/**
 * @brief Gets the camera name.
 */
@property (nonatomic, readonly, nullable, copy) NSString* deviceName;
/**
 * @brief Determines whether the camera is currently in use.
 */
@property (nonatomic, readonly, assign)         BOOL isSelectDevice;
/**
 * @brief Gets the camera position.
 */
@property (nonatomic, readonly, assign)         AVCaptureDevicePosition position;
/**
 * @brief Gets the camera device type.
 */
@property (nonatomic, readonly, nullable, copy) AVCaptureDeviceType deviceType;
/**
 * @brief Gets the camera maximum zoom factor. Maximum supported is 10.
 */
@property (nonatomic, readonly, assign)         CGFloat maxZoomFactor;
/**
 * @brief Gets the maximum optical zoom factor.
 */
@property (nonatomic, readonly, assign)         CGFloat videoZoomFactorUpscaleThreshold;
/**
 * @brief Determines whether the camera is selected as a multiple video stream.
 */
@property (nonatomic, readonly, assign)         BOOL isSelectedAsMultiCamera;
/**
 * @brief Determines whether the camera is running as a multiple video stream.
 */
@property (nonatomic, readonly, assign)         BOOL isRunningAsMultiCamera;

@end

/**
 * @class ZoomVideoSDKVideoHelper
 * @brief An interface to control video and manage cameras during a video session.
 */
@interface ZoomVideoSDKVideoHelper : NSObject

/**
 * @brief Subscribes to video raw data with a delegate.
 * @param delegate The delegate which receives raw data of preview video.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 * @deprecated Use \link startVideoPreview:resolution: \endlink instead.
 */
- (ZoomVideoSDKError)startVideoPreview:(id<ZoomVideoSDKRawDataPipeDelegate>_Nullable)delegate DEPRECATED_MSG_ATTRIBUTE("startVideoPreview:resolution:");

/**
 * @brief Subscribes to preview video raw data with a delegate.
 * @param delegate The delegate which receives raw data of preview video.
 * @param resolution The resolution at which to open the camera.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)startVideoPreview:(id<ZoomVideoSDKRawDataPipeDelegate>_Nullable)delegate resolution:(ZoomVideoSDKVideoResolution)resolution;

/**
 * @brief Unsubscribes from preview video raw data with the subscribed delegate.
 * @param delegate The delegate which receives raw data of preview video.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)stopVideoPreview:(id<ZoomVideoSDKRawDataPipeDelegate> _Nullable)delegate;

/**
 * @brief Subscribes to preview with a canvas view.
 * @param view The view which draws the preview video.
 * @param aspect The aspect mode of the preview.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)startVideoCanvasPreview:(UIView * _Nullable)view andAspectMode:(ZoomVideoSDKVideoAspect)aspect;

/**
 * @brief Unsubscribes from preview with a canvas view.
 * @param view The view which draws raw data of preview video.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)stopVideoCanvasPreview:(UIView * _Nullable)view;

/**
 * @brief Starts sending local video data from the camera.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)startVideo;

/**
 * @brief Stops sending local video data from the camera.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)stopVideo;

/**
 * @brief Rotates the video when the device is rotated.
 * @param rotation The orientation you want to rotate.
 * @return If the function succeeds, it returns YES. Otherwise, NO.
 * @warning The function only works for internal video source, it does not work for capture video source.
 */
- (BOOL)rotateMyVideo:(UIDeviceOrientation)rotation;

/**
 * @brief Mirrors the current user's video.
 * @param enable YES to enable mirror my video, NO to reset the mirror effect back.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 * @warning Valid only for canvas.
 */
- (ZoomVideoSDKError)mirrorMyVideo:(BOOL)enable;

/**
 * @brief Determines whether mirror my video is enabled.
 * @return YES if mirror my video is enabled. Otherwise, NO.
 * @warning This function only works for canvas.
 */
- (BOOL)isMyVideoMirrored;

/**
 * @brief Switches to the next available camera.
 */
- (void)switchCamera API_UNAVAILABLE(visionos);

/**
 * @brief Gets the camera device list.
 * @return If the function succeeds, it returns an NSArray of ZoomVideoSDKCameraDevice objects. Otherwise, this function fails and returns nil.
 * @warning Only iOS 17.0 or above and iPad device can get the external camera devices.
 */
- (NSArray <ZoomVideoSDKCameraDevice *>* _Nullable)getCameraDeviceList;

/**
 * @brief Switches camera by camera ID.
 * @param cameraId The target camera ID.
 * @return If the function succeeds, it returns YES. Otherwise, NO.
 */
- (BOOL)switchCamera:(NSString * _Nullable)cameraId API_UNAVAILABLE(visionos);

/**
 * @brief Gets the current camera device in use.
 * @return If the function succeeds, it returns a ZoomVideoSDKCameraDevice object. Otherwise, this function fails and returns nil.
 */
- (ZoomVideoSDKCameraDevice * _Nullable)getSelectedCamera;

/**
 * @brief Adjusts user's video resolution and frame rate.
 * @param preferenceSetting An instance of ZoomVideoSDKVideoPreferenceSetting.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)setVideoQualityPreference:(ZoomVideoSDKVideoPreferenceSetting * _Nullable)preferenceSetting;

#pragma mark - OriginalAspec -
/**
 * @brief Determines whether current aspect ratio is the original aspect ratio of video.
 * @return YES if it is original aspect ratio. Otherwise, NO.
 */
- (BOOL)isOriginalAspectRatioEnabled;

/**
 * @brief Sets the aspect ratio of the video sent out.
 * @param enabled NO means the aspect ratio is 16:9, YES means using the original aspect ratio of video.
 * @return If the function succeeds, it returns YES. Otherwise, NO.
 * @warning If session is using video source and data_mode is not VideoSourceDataMode_None, default always uses original aspect ratio of video.
 */
- (BOOL)enableOriginalAspectRatio:(BOOL)enabled;

#pragma mark - alpha channel -
/**
 * @brief Determines whether alpha channel mode can be enabled.
 * @return YES if it can be enabled. Otherwise, NO.
 * @warning Only host can enable the alpha mode. Your account must have this feature turned on by Support.
 */
- (BOOL)canEnableAlphaChannelMode;

/**
 * @brief Enables or disables video alpha channel mode.
 * @param enable YES to enable alpha channel mode, NO to disable.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)enableAlphaChannelMode:(BOOL)enable;

/**
 * @brief Determines whether alpha channel mode is enabled.
 * @return YES if alpha channel mode is enabled. Otherwise, NO.
 */
- (BOOL)isAlphaChannelModeEnabled;

/**
 * @brief Determines whether the device hardware capabilities are capable of supporting video alpha mode.
 * @return YES if the current device supports it. Otherwise, NO.
 * @warning Device should be iPhone 8, 8 Plus, X or above or be iPad Pro 9.7 above, OS should be iOS 11 or above. And should embed zoomcml.xcframework.
 */
- (BOOL)isDeviceSupportAlphaChannelMode;

#pragma mark - spotlight -
/**
 * @brief Spotlights user's video.
 * @param user The user which you want to spotlight.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)spotLightVideo:(ZoomVideoSDKUser * _Nullable)user;

/**
 * @brief Unspotlights user's video.
 * @param user The user which you want to unspotlight.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)unSpotLightVideo:(ZoomVideoSDKUser * _Nullable)user;

/**
 * @brief Sets all user's video unspotlight.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)unSpotlightAllVideos;

/**
 * @brief Gets spotlighted video user list.
 * @return If the function succeeds, it returns an NSArray of ZoomVideoSDKUser objects. Otherwise, this function fails and returns nil.
 */
- (NSArray <ZoomVideoSDKUser *>* _Nullable)getSpotlightedVideoUserList;

#pragma mark - multi stream -

/**
 * @brief Determines whether the current device (phone, pad) supports the multi camera feature.
 * @return YES if the device supports multi camera feature. Otherwise, NO.
 */
- (BOOL)isMultiStreamSupported API_UNAVAILABLE(visionos);

/**
 * @brief Enables multiple video stream support if you have multiple cameras and other participants can see multiple videos of you.
 * @param cameraDeviceID The camera ID for the camera to enable. See @{ZoomVideoSDKCameraDevice}.
 * @param customDeviceName The custom device name of the camera. If you don't pass this parameter, the SDK generates a default name.
 * @return If the function succeeds, it returns YES. Otherwise, NO.
 * @warning Only two cameras can open at the same time.
 */
- (BOOL)enableMultiStreamVideo:(NSString * _Nullable)cameraDeviceID customDeviceName:(NSString * _Nullable)customDeviceName API_UNAVAILABLE(visionos);

/**
 * @brief Disables the multiple video stream.
 * @param cameraDeviceID The camera ID that you want to disable. See @{ZoomVideoSDKCameraDevice}.
 * @return If the function succeeds, it returns YES. Otherwise, NO.
 */
- (BOOL)disableMultiStreamVideo:(NSString * _Nullable)cameraDeviceID API_UNAVAILABLE(visionos);

/**
 * @brief Turns off the multiple video stream.
 * @param cameraDeviceID The camera ID running as a multiple camera. See @{ZoomVideoSDKCameraDevice}.
 * @return If the function succeeds, it returns YES. Otherwise, NO.
 */
- (BOOL)muteMultiStreamVideo:(NSString * _Nullable)cameraDeviceID API_UNAVAILABLE(visionos);

/**
 * @brief Turns on the multiple video stream.
 * @param cameraDeviceID The camera ID running as a multiple camera. See @{ZoomVideoSDKCameraDevice}.
 * @return If the function succeeds, it returns YES. Otherwise, NO.
 */
- (BOOL)unmuteMultiStreamVideo:(NSString * _Nullable)cameraDeviceID API_UNAVAILABLE(visionos);

/**
 * @brief Gets the device ID associated with my multi-camera pipe.
 * @param pipe One of my multi-camera pipes.
 * @return If the function succeeds, it returns the video device ID. Otherwise, this function fails and returns nil.
 */
- (NSString * _Nullable)getDeviceIDByMyPipe:(ZoomVideoSDKRawDataPipe * _Nullable)pipe;

/**
 * @brief Gets the device ID associated with my multi-camera canvas.
 * @param canvas One of my multi-camera canvases.
 * @return If the function succeeds, it returns the video device ID. Otherwise, this function fails and returns nil.
 */
- (NSString *_Nullable)getDeviceIDByMyCanvas:(ZoomVideoSDKVideoCanvas *_Nullable)canvas;

@end
