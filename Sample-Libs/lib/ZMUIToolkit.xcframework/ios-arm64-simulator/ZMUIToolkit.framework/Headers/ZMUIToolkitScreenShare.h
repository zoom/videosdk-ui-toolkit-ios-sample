//
//  ZMUIToolkitScreenShare.h
//  ZMUIToolkit
//

#import <ZoomVideoSDKScreenShare/ZoomVideoSDKScreenShareService.h>

NS_ASSUME_NONNULL_BEGIN

// Simple type aliases - just inherit from the Zoom classes
typedef id<ZoomVideoSDKScreenShareServiceDelegate> ZMUIToolkitScreenShareServiceDelegate;

@interface ZMUIToolkitScreenShareInitParams : ZoomVideoSDKScreenShareServiceInitParams
@end

@interface ZMUIToolkitScreenShareService : ZoomVideoSDKScreenShareService
@end

NS_ASSUME_NONNULL_END
