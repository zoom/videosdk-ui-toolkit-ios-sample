/**
 * @file ZoomVideoSDKRemoteCameraControlHelper.h
 * @brief Interface for remote camera control operations including pan, tilt, zoom, and camera movement management.
 */

#import <Foundation/Foundation.h>
#import <ZoomVideoSDK/ZoomVideoSDKConstants.h>

/**
 * @class ZoomVideoSDKRemoteCameraControlHelper
 * @brief Remote camera control helper object interface.
 */
@interface ZoomVideoSDKRemoteCameraControlHelper : NSObject
/**
 * @brief Requests to control remote camera.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
-(ZoomVideoSDKError)requestControlRemoteCamera;

/**
 * @brief Gives up control of the remote camera.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
-(ZoomVideoSDKError)giveUpControlRemoteCamera;

/**
 * @brief Turns the camera to the left.
 * @param range The rotation range, 10 <= range <= 100.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
-(ZoomVideoSDKError)turnLeft:(unsigned int)range;

/**
 * @brief Turns the camera to the right.
 * @param range The rotation range, 10 <= range <= 100.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
-(ZoomVideoSDKError)turnRight:(unsigned int)range;

/**
 * @brief Turns the camera up.
 * @param range The rotation range, 10 <= range <= 100.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
-(ZoomVideoSDKError)turnUp:(unsigned int)range;

/**
 * @brief Turns the camera down.
 * @param range The rotation range, 10 <= range <= 100.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
-(ZoomVideoSDKError)turnDown:(unsigned int)range;

/**
 * @brief Zooms the camera in.
 * @param range The rotation range, 10 <= range <= 100.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
-(ZoomVideoSDKError)zoomIn:(unsigned int)range;

/**
 * @brief Zooms the camera out.
 * @param range The rotation range, 10 <= range <= 100.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
-(ZoomVideoSDKError)zoomOut:(unsigned int)range;
@end
