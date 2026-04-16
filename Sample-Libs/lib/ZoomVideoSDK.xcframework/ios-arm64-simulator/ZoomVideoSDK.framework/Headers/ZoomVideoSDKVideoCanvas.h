/**
 * @file ZoomVideoSDKVideoCanvas.h
 * @brief Interface for video canvas operations including video rendering, view management, and aspect ratio control.
 */

#import <UIKit/UIKit.h>

/**
 * @class ZoomVideoSDKVideoCanvas
 * @brief The video canvas is the display area of the video streams on the user’s local device. To instantiate a local video canvas instance and show the video stream of a user, simply initialize the ZoomVideoSDKVideoCanvas object, subscribe the userId of a specific user, and add it to your UIView.
 */
@interface ZoomVideoSDKVideoCanvas : NSObject

/**
 * @brief Gets the canvas type.
 * @return If the function succeeds, it returns the canvas type. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKVideoType)canvasType;

/**
 * @brief Gets the user's video status.
 * @return If the function succeeds, it returns a ZoomVideoSDKVideoStatus object. Otherwise, this function fails and returns nil.
 * @warning This interface is marked as deprecated, then it will be replaced by ZoomVideoSDKRawDataPipe.videoStatus() and ZoomVideoSDKVideoCanvas.videoStatus().
 */
- (ZoomVideoSDKVideoStatus *_Nullable)videoStatus;

/**
 * @brief Gets the user's share status.
 * @return If the function succeeds, it returns a ZoomVideoSDKShareStatus object. Otherwise, this function fails and returns nil.
 * @warning This interface is marked as deprecated, then it will be replaced by ZoomVideoSDKRawDataPipe.shareStatus() and ZoomVideoSDKVideoCanvas.shareStatus().
 */
- (ZoomVideoSDKShareStatus *_Nullable)shareStatus;

/**
 * @brief Subscribes to the user's video or share view.
 * @param view The UIView object that you need to render data.
 * @param aspect The render's aspect mode.
 * @param resolution The render's resolution. Valid only for video canvas.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)subscribeWithView:(UIView * _Nullable)view
                            aspectMode:(ZoomVideoSDKVideoAspect)aspect
                         andResolution:(ZoomVideoSDKVideoResolution)resolution;

/**
 * @brief Subscribes to the user's video or share view in Picture-in-Picture mode for video calls.
 * @param view The UIView object that you need to render data.
 * @param aspect The render's aspect mode.
 * @param resolution The render's resolution. Valid only for video canvas.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 * @note To know more about PiP mode for video calls, please visit https://developer.apple.com/documentation/avkit/adopting_picture_in_picture_for_video_calls.
 */
- (ZoomVideoSDKError)subscribeWithPiPView:(UIView * _Nullable)view
                               aspectMode:(ZoomVideoSDKVideoAspect)aspect
                            andResolution:(ZoomVideoSDKVideoResolution)resolution;

/**
 * @brief Unsubscribes from video or share data.
 * @param view The UIView object to unsubscribe.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)unSubscribeWithView:(UIView * _Nullable)view;

/**
 * @brief Sets the video aspect mode.
 * @param aspect The aspect mode of video.
 * @param view The UIView object to render video.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)setAspectMode:(ZoomVideoSDKVideoAspect)aspect toView:(UIView * _Nullable)view;

/**
 * @brief Sets the video resolution.
 * @param resolution The video resolution. Valid only for video canvas.
 * @param view The UIView object to render video.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)setResolution:(ZoomVideoSDKVideoResolution)resolution toView:(UIView * _Nullable)view;

/**
 * @brief Captures a snapshot of the current video frame from the specified UIView and returns it as a UIImage.
 * This method captures the current frame being displayed in the video or share canvas.
 *
 * @param view The UIView currently subscribed to this canvas.
 * @return A UIImage containing the captured frame, or nil if the snapshot could not be taken. The returned image will be autoreleased.
 * @note The view must be currently subscribed to this canvas; otherwise the method returns nil.
 * @note After taking a snapshot, a notification is sent to other users in the session via the onCanvasSnapshotTaken callback.
 */
- (UIImage * _Nullable)takeSnapshot:(UIView * _Nullable)view;

@end
