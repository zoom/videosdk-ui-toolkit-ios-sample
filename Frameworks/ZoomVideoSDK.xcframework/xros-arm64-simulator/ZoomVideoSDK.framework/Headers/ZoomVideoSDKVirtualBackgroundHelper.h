/**
 * @file ZoomVideoSDKVirtualBackgroundHelper.h
 * @brief Interface for virtual background operations including adding, setting, and managing virtual background images and blur effects.
 */

#import <Foundation/Foundation.h>

/**
 * @class ZoomVideoSDKVirtualBackgroundItem
 * @brief Virtual background item interface.
 */
@interface ZoomVideoSDKVirtualBackgroundItem : NSObject
/**
 * @brief Gets the current item image file path.
 */
@property (nonatomic, copy, readonly) NSString * _Nullable imageFilePath;
/**
 * @brief Gets the current item image name.
 */
@property (nonatomic, copy, readonly) NSString * _Nullable imageName;
/**
 * @brief Gets the current item background type.
 */
@property (nonatomic, assign, readonly) ZoomVideoSDKVirtualBackgroundDataType type;
/**
 * @brief Determines whether the virtual background can be deleted.
 */
@property (nonatomic, assign, readonly) BOOL canVirtualBackgroundBeDeleted;

@end

/**
 * @class ZoomVideoSDKVirtualBackgroundHelper
 * @brief Virtual background helper.
 */
@interface ZoomVideoSDKVirtualBackgroundHelper : NSObject
/**
 * @brief Determines whether the user can support smart virtual backgrounds.
 * @return YES if the user can support it and you can use it. Otherwise, NO.
 * @warning Device should be iPhone 8, 8 Plus, X or above or be iPad Pro 9.7 above, OS should be iOS 11 or above. And should embed zoomcml.xcframework.
 */
- (BOOL)isSupportVirtualBackground;

/**
 * @brief Adds a virtual background object.
 * @param image The image of virtual background.
 * @return If the function succeeds, it returns a ZoomVideoSDKVirtualBackgroundItem object. Otherwise, this function fails and returns nil.
 */
- (ZoomVideoSDKVirtualBackgroundItem * _Nullable)addVirtualBackgroundItem:(UIImage *_Nullable)image;

/**
 * @brief Removes a virtual background object.
 * @param imageItem The ZoomVideoSDKVirtualBackgroundItem object to be removed.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)removeVirtualBackgroundItem:(ZoomVideoSDKVirtualBackgroundItem *_Nullable)imageItem;

/**
 * @brief Gets a collection of virtual background items.
 * @return If the function succeeds, it returns an NSArray of ZoomVideoSDKVirtualBackgroundItem objects. Otherwise, this function fails and returns nil.
 */
- (NSArray<ZoomVideoSDKVirtualBackgroundItem * > * _Nullable)getVirtualBackgroundItemList;

/**
 * @brief Selects a virtual background item.
 * @param imageItem The ZoomVideoSDKVirtualBackgroundItem object to select.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)setVirtualBackgroundItem:(ZoomVideoSDKVirtualBackgroundItem *_Nullable)imageItem;

/**
 * @brief Gets the selected virtual background item.
 * @return If the function succeeds, it returns a ZoomVideoSDKVirtualBackgroundItem object. Otherwise, this function fails and returns nil.
 */
- (ZoomVideoSDKVirtualBackgroundItem * _Nullable)getSelectedVirtualBackgroundItem;

@end

