/**
 * @file ZoomVideoSDKCmdChannel.h
 * @brief Interface for command channel operations allowing custom command data exchange between session participants.
 */

#import <Foundation/Foundation.h>

/**
 * @class ZoomVideoSDKCmdChannel
 * @brief The command channel, allows users to send commands or data (such as plain text or a binary encoded into string) to other users in the same session. See [ZoomVideoSDK.h#getCmdChannel].
 */
@interface ZoomVideoSDKCmdChannel : NSObject
/**
 * @brief Sends custom commands or data to other users in the current session. Limit: up to 60 custom commands per second.
 * @param commandContent The custom commands or data, represented in string format.
 * @param user An instance of ZoomVideoSDKUser associated with the user who receives the command. If receiver is set to nil, the command is broadcast to all users.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)sendCommand:(NSString * _Nullable)commandContent receiveUser:(ZoomVideoSDKUser * _Nullable)user;

@end

