/**
 * @file ZoomVideoSDKAnnotationHelper.h
 * @brief Interface for managing annotation features including tool settings, drawing operations, and annotation permissions during screen sharing.
 */

#import <Foundation/Foundation.h>
#import <ZoomVideoSDK/ZoomVideoSDKConstants.h>

/**
 * @class ZoomVideoSDKAnnotationHelper
 * @brief Annotation helper interface for Zoom Video SDK.
 */
@interface ZoomVideoSDKAnnotationHelper: NSObject

/**
 * @brief Determines whether annotation was disabled by the share owner.
 * @return YES if annotation was disabled by the share owner. Otherwise, NO.
 * @deprecated Use \link canDoAnnotation \endlink instead.
 */
- (BOOL)isSenderDisableAnnotation DEPRECATED_MSG_ATTRIBUTE("use canDoAnnotation instead");

/**
 * @brief Determines whether the current user can do annotation on the share.
 * @return YES if the user can do annotation. Otherwise, NO.
 */
- (BOOL)canDoAnnotation;

/**
 * @brief Starts annotation.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)startAnnotation;

/**
 * @brief Stops annotation.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)stopAnnotation;

/**
 * @brief Sets the annotation tool color.
 * @param toolColor The specified color.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)setToolColor:(nullable UIColor *)toolColor;

/**
 * @brief Gets the annotation tool color.
 * @return If the function succeeds, it returns the current tool color. Otherwise, this function fails and returns nil.
 */
- (nullable UIColor *)getToolColor;

/**
 * @brief Sets the annotation tool type.
 * @param type The specified annotation tool type.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 * @warning The tool type ZoomVideoSDKAnnotationToolType_Picker and ZoomVideoSDKAnnotationToolType_SpotLight are not supported for viewer.
 */
- (ZoomVideoSDKError)setToolType:(ZoomVideoSDKAnnotationToolType)type;

/**
 * @brief Gets the annotation tool type.
 * @return The current tool type.
 */
- (ZoomVideoSDKAnnotationToolType)getToolType;

/**
 * @brief Sets the annotation tool width.
 * @param width The specified annotation tool width.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)setToolWidth:(NSUInteger)width;

/**
 * @brief Gets the annotation tool width.
 * @return The current tool width.
 */
- (NSUInteger)getToolWidth;

/**
 * @brief Undoes one annotation content step.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)undo;

/**
 * @brief Redoes one annotation content step.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)redo;

/**
 * @brief Clears the annotation content.
 * @param type The specified clear type.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)clear:(ZoomVideoSDKAnnotationClearType)type;
@end


