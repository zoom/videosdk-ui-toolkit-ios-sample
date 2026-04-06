/**
 * @file ZoomVideoSDKSubSessionHelper.h
 * @brief Interface for sub-session management including creating, managing, and controlling breakout rooms and sub-sessions.
 */

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

/**
 * @class ZoomVideoSDKSubSessionUserHelpRequestHandler
 * @brief Subsession user help request handler.
 */
@interface ZoomVideoSDKSubSessionUserHelpRequestHandler : NSObject
/**
 * @brief Gets the username of the user who sent this request.
 * @return If the function succeeds, it returns the username. Otherwise, this function fails and returns nil.
 */
- (NSString * _Nullable)getRequestUserName;

/**
 * @brief Gets the name of the subsession where the user sent this request.
 * @return If the function succeeds, it returns the subsession name. Otherwise, this function fails and returns nil.
 */
- (NSString * _Nullable)getRequestSubSessionName;

/**
 * @brief Ignores this request.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 * @note 'onSubSessionUserHelpRequestResult' is the corresponding callback notification.
 */
- (ZoomVideoSDKError)ignore;

/**
 * @brief Joins the subsession which request help is from.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)joinSubSessionByUserRequest;
@end

/**
 * @class ZoomVideoSDKSubSessionUser
 * @brief Represents a subsession user info.
 */
@interface ZoomVideoSDKSubSessionUser : NSObject
/**
 * @brief Gets the subsession user's username.
 * @return The username.
 */
- (NSString *)getUserName;
/**
 * @brief Gets the subsession user's GUID.
 * @return The user GUID.
 */
- (NSString *)getUserGUID;
@end

/**
 * @class ZoomVideoSDKSubSessionKit
 * @brief Represents a subsession instance.
 */
@interface ZoomVideoSDKSubSessionKit : NSObject
/**
 * @brief Gets the subsession's name.
 * @return The subsession name.
 */
- (NSString * _Nullable)getSubSessionName;
/**
 * @brief Gets the subsession's ID.
 * @return The subsession ID.
 */
- (NSString * _Nullable)getSubSessionID;

/**
 * @brief Gets the user list in this subsession.
 * @return If the function succeeds, it returns an NSArray of ZoomVideoSDKSubSessionUser objects. Otherwise, this function fails and returns nil.
 */
- (NSArray <ZoomVideoSDKSubSessionUser*>* _Nullable)getSubSessionUserList;
/**
 * @brief Joins this subsession.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)joinSubSession;
@end

/**
 * @class ZoomVideoSDKSubSessionManager
 * @brief Manages subsession lifecycle and communication.
 */
@interface ZoomVideoSDKSubSessionManager : NSObject
/**
 * @brief Starts subsession.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 * @note 'onSubSessionStatusChanged' is the corresponding callback notification.
 */
- (ZoomVideoSDKError)startSubSession;
/**
 * @brief Stops subsession.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 * @note 'onSubSessionStatusChanged' is the corresponding callback notification.
 */
- (ZoomVideoSDKError)stopSubSession;
/**
 * @brief Determines whether the subsession is started.
 * @return YES if the subsession is started. Otherwise, NO.
 */
- (BOOL)isSubSessionStarted;
/**
 * @brief Broadcasts a message to subsession.
 * @param message The message to be broadcast.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 * @warning 'onBroadcastMessageFromMainSession' is the corresponding callback notification.
 */
- (ZoomVideoSDKError)broadcastMessage:(NSString *_Nonnull)message;

@end

/**
 * @class ZoomVideoSDKSubSessionParticipant
 * @brief SubSession attendee helper interface.
 */
@interface ZoomVideoSDKSubSessionParticipant : NSObject
/**
 * @brief Returns to main session.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)returnToMainSession;
/**
 * @brief Requests for help.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 * @note 'onSubSessionUserHelpRequestResult' is the corresponding callback notification.
 */
- (ZoomVideoSDKError)requestForHelp;

@end

/**
 * @class ZoomVideoSDKSubSessionHelper
 * @brief Subsession helper interface.
 * @note Only session host and manager can get this helper.
 */
@interface ZoomVideoSDKSubSessionHelper : NSObject
/**
 * @brief Adds a subsession name to prepared list.
 * @param subSessionNames The array of subsession names to add.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)addSubSessionToPreList:(NSArray <NSString *>* _Nonnull)subSessionNames;
/**
 * @brief Removes a subsession name from prepared list.
 * @param subSessionNames The array of subsession names to remove.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)removeSubSessionFromPreList:(NSArray <NSString *> *_Nonnull)subSessionNames;
/**
 * @brief Clears the prepared list.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)clearSubSessionPreList;
/**
 * @brief Gets the subsession name prepared list.
 * @return If the function succeeds, it returns an array of SubSession name.
 */
- (NSArray <NSString*>* _Nullable)getSubSessionPreList;
/**
 * @brief Withdraws all committed subsession list and commits the prepared list.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 * @note If the function succeeds, the prepared list will be cleared, 'onSubSessionListUpdate' is the corresponding callback notification.
 */
- (ZoomVideoSDKError)commitSubSessionList;

/**
 * @brief Gets the committed subsession list.
 * @return If the function succeeds, it returns an NSArray of ZoomVideoSDKSubSessionKit objects. Otherwise, this function fails and returns nil.
 */
- (NSArray <ZoomVideoSDKSubSessionKit*>* _Nullable)getCommittedSubSessionList;
/**
 * @brief Withdraws all committed subsession list.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 * @note If the function succeeds, the prepared list will be cleared, 'onSubSessionStatusChanged' is the corresponding callback notification.
 */
- (ZoomVideoSDKError)withdrawSubSessionList;

@end

NS_ASSUME_NONNULL_END
