/**
 * @file ZoomVideoSDKRawDataPipe.h
 * @brief Interface for raw data pipe operations including video data subscription and rendering delegate methods.
 */

#import <Foundation/Foundation.h>
#import <ZoomVideoSDK/ZoomVideoSDKDelegate.h>

/**
 * @class ZoomVideoSDKRawDataPipe
 * @brief A class to manage the subscription and unsubscription of video or share raw data.
 */
@interface ZoomVideoSDKRawDataPipe : NSObject

/**
 * @brief Gets the raw data type.
 * @return Share or Video data type. Refer to ZoomVideoSDKRawDataType for the list of enum [link ZoomVideoSDKRawDataType].
 */
- (ZoomVideoSDKVideoType)rawdataPipeType;

/**
 * @brief Gets the user's video status.
 * @return If the function succeeds, it returns a ZoomVideoSDKVideoStatus object. Otherwise, this function fails and returns nil.
 */
- (ZoomVideoSDKVideoStatus *_Nullable)videoStatus;

/**
 * @brief Gets the user's share status.
 * @return If the function succeeds, it returns a ZoomVideoSDKShareStatus object. Otherwise, this function fails and returns nil.
 */
- (ZoomVideoSDKShareStatus *_Nullable)shareStatus;

/**
 * @brief Subscribes to video or share raw data. You can subscribe your preview video data with user ID 0 before entering the session, you can call it just after you called joinSession. Otherwise, you can subscribe video or share raw data using the real user ID in callback onUserJoin.
 * @param delegate The object that receives the ZoomVideoSDKRawDataPipeDelegate event.
 * @param resolution The resolution you want to subscribe. Valid only for video data.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 * @warning The param resolution cannot use ZoomVideoSDKVideoResolution_Auto in this function. Must specify the resolution you want.
 */
- (ZoomVideoSDKError)subscribeWithDelegate:(id<ZoomVideoSDKRawDataPipeDelegate> _Nullable)delegate
                                resolution:(ZoomVideoSDKVideoResolution)resolution;

/**
 * @brief Unsubscribes from video or share raw data.
 * @param delegate The object that receives the ZoomVideoSDKRawDataPipeDelegate event. Which was passed in subscribeWithDelegate:resolution:.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)unSubscribeWithDelegate:(id<ZoomVideoSDKRawDataPipeDelegate> _Nullable)delegate;

@end

