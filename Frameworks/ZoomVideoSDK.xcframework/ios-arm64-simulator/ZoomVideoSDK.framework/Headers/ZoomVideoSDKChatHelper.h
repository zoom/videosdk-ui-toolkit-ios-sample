/**
 * @file ZoomVideoSDKChatHelper.h
 * @brief Interface for chat messaging operations including sending messages, deleting messages, and managing chat privileges.
 */

#import <Foundation/Foundation.h>

@class ZoomVideoSDKUser;

/**
 * @class ZoomVideoSDKChatMessage
 * @brief A class that contains all the information for a message.
 */
@interface ZoomVideoSDKChatMessage : NSObject

/**
 * @brief The message ID.
 */
@property (nonatomic, strong) NSString   * _Nullable messageID;
/**
 * @brief The sender user.
 */
@property (nonatomic, strong) ZoomVideoSDKUser   * _Nullable senderUser;

/**
 * @brief The receiver user.
 */
@property (nonatomic, strong) ZoomVideoSDKUser   * _Nullable receiverUser;

/**
 * @brief The message content.
 */
@property (nonatomic, strong) NSString     * _Nullable content;
/**
 * @brief The message sent time in timestamp.
 */
@property (nonatomic, assign) long long    timeStamp;
/**
 * @brief Determines whether the message is sent to all users.
 */
@property (nonatomic, assign) BOOL         isChatToAll;
/**
 * @brief Determines whether the message is sent by me.
 */
@property (nonatomic, assign) BOOL         isSelfSend;

@end

/**
 * @class ZoomVideoSDKChatHelper
 * @brief A class to operate the instant message in session.
 */
@interface ZoomVideoSDKChatHelper : NSObject


/**
 * @brief Sends a chat message to a specific user.
 * @param user The receiver.
 * @param content The message content.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)SendChatToUser:(ZoomVideoSDKUser * _Nullable)user Content:(NSString * _Nullable)content;

/**
 * @brief Sends a chat message to all users.
 * @param content The message content.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)SendChatToAll:(NSString * _Nullable)content;

/**
 * @brief Determines whether a specific message can be deleted.
 * @param msgID The message ID.
 * @return YES if the message can be deleted. Otherwise, NO.
 */
- (BOOL)canChatMessageBeDeleted:(NSString *__nonnull)msgID;

/**
 * @brief Deletes a specific chat message from the Zoom server.
 * @param msgID The message ID.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 * @warning This does not delete the message in your user interface.
 */
- (ZoomVideoSDKError)deleteChatMessage:(NSString * __nonnull)msgID;

/**
 * @brief Determines whether chat is disabled.
 * @return YES if chat is disabled. Otherwise, NO.
 */
- (BOOL)IsChatDisabled;

/**
 * @brief Determines whether private chat is disabled.
 * @return YES if private chat is disabled. Otherwise, NO.
 */
- (BOOL)IsPrivateChatDisabled;

/**
 * @brief Sets participant chat privilege when in session.
 * @param privilege The chat privilege of the participant.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 * @warning Only meeting host or manager can run the function.
 */
- (ZoomVideoSDKError)changeChatPrivilege:(ZoomVideoSDKChatPrivilegeType)privilege;

/**
 * @brief Gets participant chat privilege when in session.
 * @return The result of participant chat Privilege.
 */
- (ZoomVideoSDKChatPrivilegeType)getChatPrivilege;


@end
