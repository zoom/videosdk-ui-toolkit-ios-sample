//
//  ZoomVideoSDKEmojiReactionHelper.h
//  ZoomVideoSDK
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

/**
 * @class ZoomVideoSDKEmojiReactionHelper
 * @brief Emoji reaction helper interface for sending emoji reactions in a session.
 */
@interface ZoomVideoSDKEmojiReactionHelper : NSObject

/**
 * @brief Sends an emoji reaction to all participants in the session.
 * @param type The emoji reaction type to send.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)sendEmojiReaction:(ZoomVideoSDKEmojiReactionType)type;

@end

NS_ASSUME_NONNULL_END
