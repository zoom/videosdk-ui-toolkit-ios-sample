/**
 * @file ZoomVideoSDKVoiceInterpretationHelper.h
 * @brief Interface for voice interpretation operations.
 */

#import <Foundation/Foundation.h>
#import <ZoomVideoSDK/ZoomVideoSDKConstants.h>

/**
 * @class ZoomVideoSDKVoiceInterpretationLanguage
 * @brief Represents a language used in voice interpretation.
 */
@interface ZoomVideoSDKVoiceInterpretationLanguage : NSObject
/**
 * @brief The unique ID of the language.
 */
@property(nonatomic, assign, readonly) NSInteger languageID;
/**
 * @brief The display name of the language.
 */
@property(nonatomic, copy, readonly) NSString * _Nullable languageName;
@end

/**
 * @class ZoomVideoSDKVoiceInterpretationVoiceInfo
 * @brief Represents a voice style used in voice interpretation.
 */
@interface ZoomVideoSDKVoiceInterpretationVoiceInfo : NSObject
/**
 * @brief The unique ID of the voice style.
 */
@property(nonatomic, assign, readonly) NSInteger voiceTypeID;
/**
 * @brief The display name of the voice style.
 */
@property(nonatomic, copy, readonly) NSString * _Nullable voiceTypeName;
/**
 * @brief The URL of the voice sample file.
 */
@property(nonatomic, copy, readonly) NSString * _Nullable voiceSampleFileUrl;
@end

/**
 * @class ZoomVideoSDKVoiceInterpretationHelper
 * @brief Provides methods for controlling and interacting with voice interpretation.
 */
@interface ZoomVideoSDKVoiceInterpretationHelper : NSObject

/**
 * @brief Query whether voice interpretation capability is enabled in the current session.
 * @return YES if enabled; otherwise NO.
 */
- (BOOL)isVoiceInterpretationEnabled;

/**
 * @brief Query whether voice interpretation is ready for use.
 * @return YES if ready; otherwise NO.
 */
- (BOOL)isVoiceInterpretationReady;

/**
 * @brief Query whether voice interpretation has been started.
 * @return YES if started; otherwise NO.
 */
- (BOOL)isVoiceInterpretationStarted;

/**
 * @brief Start voice interpretation.
 * @return If the function succeeds, the return value is \c Errors_Success. Otherwise, this function returns an error. See \c ZoomVideoSDKError.
 */
- (ZoomVideoSDKError)startVoiceInterpretation;

/**
 * @brief Stop voice interpretation.
 * @return If the function succeeds, the return value is \c Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)stopVoiceInterpretation;

/**
 * @brief Get the current speaking language for the local user.
 * @return The current speaking language, or nil if unavailable.
 */
- (ZoomVideoSDKVoiceInterpretationLanguage * _Nullable)getMySpeakingLanguage;

/**
 * @brief Get the available speaking language list.
 * @return The speaking language list, or nil if unavailable.
 */
- (NSArray <ZoomVideoSDKVoiceInterpretationLanguage *> * _Nullable)getMySpeakingLanguageList;

/**
 * @brief Set the speaking language for the local user.
 * @param language The target speaking language.
 * @return If the function succeeds, the return value is \c Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)setMySpeakingLanguage:(ZoomVideoSDKVoiceInterpretationLanguage * _Nullable)language;

/**
 * @brief Get the current translated language for the local user.
 * @return The current translated language, or nil if unavailable.
 */
- (ZoomVideoSDKVoiceInterpretationLanguage * _Nullable)getTranslatedLanguage;

/**
 * @brief Get the available translated language list.
 * @return The translated language list, or nil if unavailable.
 */
- (NSArray <ZoomVideoSDKVoiceInterpretationLanguage *> * _Nullable)getTranslatedLanguagesList;

/**
 * @brief Set the translated language for the local user.
 * @param language The target translated language.
 * @return If the function succeeds, the return value is \c Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)setTranslatedLanguage:(ZoomVideoSDKVoiceInterpretationLanguage * _Nullable)language;

/**
 * @brief Get the audio balance between original and translated voice.
 * @return Audio balance in the range [0.0, 1.0]. 0.0 means original only; 1.0 means translated only.
 */
- (CGFloat)getAudioBalanceForOriginalAndTranslatedVoice;

/**
 * @brief Set the audio balance between original and translated voice.
 * @param balance Audio balance in the range [0.0, 1.0]. 0.0 means original only; 1.0 means translated only.
 * @return If the function succeeds, the return value is \c Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)setAudioBalanceForOriginalAndTranslatedVoice:(CGFloat)balance;

/**
 * @brief Get the current translated voice style.
 * @return The current voice style, or nil if unavailable.
 */
- (ZoomVideoSDKVoiceInterpretationVoiceInfo * _Nullable)getTranslatedVoiceStyle;

/**
 * @brief Get the available translated voice style list.
 * @return The translated voice style list, or nil if unavailable.
 */
- (NSArray <ZoomVideoSDKVoiceInterpretationVoiceInfo *> * _Nullable)getTranslatedVoiceStyleList;

/**
 * @brief Set the translated voice style.
 * @param voiceInfo The target voice style.
 * @return If the function succeeds, the return value is \c Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)setTranslatedVoiceStyle:(ZoomVideoSDKVoiceInterpretationVoiceInfo * _Nullable)voiceInfo;

@end
