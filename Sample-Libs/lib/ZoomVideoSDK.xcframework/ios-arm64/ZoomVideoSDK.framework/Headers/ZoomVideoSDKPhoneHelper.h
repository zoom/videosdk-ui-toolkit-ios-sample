/**
 * @file ZoomVideoSDKPhoneHelper.h
 * @brief Interface for phone invitation operations including dial-in/dial-out functionality and country information management.
 */

#import <Foundation/Foundation.h>

/**
 * @class ZoomVideoSDKPhoneSupportCountryInfo
 * @brief Support country information interface.
 */
@interface ZoomVideoSDKPhoneSupportCountryInfo : NSObject
/**
 * @brief The country ID.
 */
@property (nonatomic, copy) NSString     * _Nullable countryID;
/**
 * @brief The country name.
 */
@property (nonatomic, copy) NSString     * _Nullable countryName;
/**
 * @brief The country code.
 */
@property (nonatomic, copy) NSString     * _Nullable countryCode;
@end


/**
 * @class ZoomVideoSDKDialInNumberInfo
 * @brief Dial in number information interface.
 */
@interface ZoomVideoSDKDialInNumberInfo : NSObject
/**
 * @brief The country ID.
 */
@property (nonatomic, copy) NSString     * _Nullable countryID;
/**
 * @brief The country code.
 */
@property (nonatomic, copy) NSString     * _Nullable countryCode;
/**
 * @brief The country name.
 */
@property (nonatomic, copy) NSString     * _Nullable countryName;
/**
 * @brief Dial in number.
 */
@property (nonatomic, copy) NSString     * _Nullable number;
/**
 * @brief Dial in number format string for display.
 */
@property (nonatomic, copy) NSString     * _Nullable displayNumber;
/**
 * @brief Dial in number type.
 */
@property (nonatomic, assign) ZoomVideoSDKDialInNumType     type;

/**
 * @brief Determines whether the dial number is an account dedicated number.
 */
@property (nonatomic, assign) BOOL isDedicateID;
@end


/**
 * @class ZoomVideoSDKInvitePhoneUserInfo
 * @brief Information for inviting phone users.
 */
@interface ZoomVideoSDKInvitePhoneUserInfo : NSObject
/**
 * @brief The country code of the specified user must be in the support list.
 */
@property (nonatomic, copy) NSString     * _Nullable countryCode;

/**
 * @brief The phone number of specified user.
 */
@property (nonatomic, copy) NSString     * _Nullable phoneNumber;

/**
 * @brief The screen name of the specified user in the session.
 */
@property (nonatomic, copy) NSString     * _Nullable name;

/**
 * @brief Determines whether the user needs to press number one to agree to join the session.
 */
@property (nonatomic, assign) BOOL     bPressOne;

/**
 * @brief Determines whether to play a greeting prompt tone when joining the session.
 */
@property (nonatomic, assign) BOOL     bGreeting;
@end

/**
 * @class ZoomVideoSDKPhoneHelper
 * @brief Provides methods for managing phone-related session features such as dial-in and phone invites.
 */
@interface ZoomVideoSDKPhoneHelper : NSObject

/**
 * @brief Determines whether the session supports join by phone.
 * @return YES if the session supports join by phone. Otherwise, NO.
 */
- (BOOL)isSupportPhoneFeature;

/**
 * @brief Gets the list of country information where the session supports join by telephone.
 * @return If the function succeeds, it returns an NSArray of ZoomVideoSDKPhoneSupportCountryInfo objects. Otherwise, this function fails and returns nil.
 */
- (NSArray <ZoomVideoSDKPhoneSupportCountryInfo *>* _Nullable)getSupportCountryInfo;

/**
 * @brief Invites the specified user to join the session by call out.
 * @param countryCode The country code of the specified user. Must be in the support list.
 * @param phoneNumber The phone number of the specified user.
 * @param name The screen name of the specified user in the session.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 * @deprecated Use \link invitePhoneUser: \endlink instead.
 */
- (ZoomVideoSDKError)inviteByPhone:(NSString *_Nonnull)countryCode phoneNumber:(NSString *_Nonnull)phoneNumber name:(NSString *_Nonnull)name
DEPRECATED_MSG_ATTRIBUTE("use invitePhoneUser: instead");

/**
 * @brief Invites the specified user to join the session by call out.
 * @param inviteInfo The invite action information.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)invitePhoneUser:(ZoomVideoSDKInvitePhoneUserInfo * _Nullable)inviteInfo;

/**
 * @brief Cancels the invitation that is being called out by phone.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)cancelInviteByPhone;

/**
 * @brief Gets the status of the invitation by phone.
 * @return If the function succeeds, the method returns the ZoomVideoSDKPhoneStatus of the invitation by phone.
 */
- (ZoomVideoSDKPhoneStatus)getInviteByPhoneStatus;

/**
 * @brief Gets the video SDK dial in number information list.
 * @return If the function succeeds, it returns an NSArray of ZoomVideoSDKDialInNumberInfo objects. Otherwise, this function fails and returns nil.
 */
- (NSArray <ZoomVideoSDKDialInNumberInfo *> * _Nullable)getSessionDialInNumbers;

@end
