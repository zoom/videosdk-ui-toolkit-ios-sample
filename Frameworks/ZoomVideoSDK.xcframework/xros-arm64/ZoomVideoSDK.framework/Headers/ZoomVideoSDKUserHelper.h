/**
 * @file ZoomVideoSDKUserHelper.h
 * @brief Interface for user management operations including changing names, assigning roles, and removing users from sessions.
 */

#import <Foundation/Foundation.h>

@class ZoomVideoSDKUser;
/**
 * @class ZoomVideoSDKUserHelper
 * @brief User control interface.
 */
@interface ZoomVideoSDKUserHelper : NSObject

/**
 * @brief Changes a specific user's name.
 * @param inputName The user's new name.
 * @param changeUser The user in the session.
 * @return YES if the name change succeeds. Otherwise, NO.
 */
- (BOOL)changeName:(NSString* _Nullable)inputName withUser:(ZoomVideoSDKUser * _Nullable)changeUser;

/**
 * @brief Assigns a user as the session host.
 * @param user The user in the session.
 * @return YES if the user is now the host. Otherwise, NO.
 * @warning Only session host can run this function, and user ID should not be myself.
 */
- (BOOL)makeHost:(ZoomVideoSDKUser * _Nullable)user;

/**
 * @brief Assigns a user as the session manager.
 * @param user The user in the session.
 * @return YES if the user is now the manager. Otherwise, NO.
 * @warning Only session host can run this function, and user ID should not be myself.
 */
- (BOOL)makeManager:(ZoomVideoSDKUser * _Nullable)user;

/**
 * @brief Revokes manager rights from a user.
 * @param user The user in the session.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 * @warning Only session host can run this function, and user ID should not be myself.
 */
- (ZoomVideoSDKError)revokeManager:(ZoomVideoSDKUser * _Nullable)user;

/**
 * @brief Removes user from session.
 * @param user The user in the session.
 * @return YES if the function succeeds. Otherwise, NO.
 * @warning The method is available only for the host, and the host cannot remove himself.
 */
- (BOOL)removeUser:(ZoomVideoSDKUser * _Nullable)user;

/**
 * @brief Reclaims host permission.
 * @return YES if the function succeeds. Otherwise, NO.
 */
- (BOOL)reclaimHost;

@end
