/**
 * @file ZoomVideoSDKLiveTranscriptionHelper.h
 * @brief Interface for live transcription operations including starting/stopping transcription and managing transcription content.
 */

#import <Foundation/Foundation.h>

/**
 * @class ZoomVideoSDKLiveTranscriptionLanguage
 * @brief Represents a language used in live transcription.
 */
@interface ZoomVideoSDKLiveTranscriptionLanguage : NSObject

/**
 * @brief Gets the language ID of transcription language.
 */
@property(nonatomic, assign, readonly) NSInteger languageID;

/**
 * @brief Gets the localized language name of transcription language.
 */
@property(nonatomic, copy, readonly) NSString * _Nullable languageName;

@end

/**
 * @class ZoomVideoSDKLiveTranscriptionMessageInfo
 * @brief Represents a single message information of live transcription.
 */
@interface ZoomVideoSDKLiveTranscriptionMessageInfo : NSObject
/**
 * @brief Gets the message ID of transcription message.
 */
@property (nonatomic, copy)NSString * _Nullable messageID;
/**
 * @brief Gets the speaker ID of transcription message.
 */
@property (nonatomic, copy)NSString * _Nullable speakerID;
/**
 * @brief Gets the speaker name of transcription message.
 */
@property (nonatomic, copy)NSString * _Nullable speakerName;
/**
 * @brief Gets the message content of transcription message.
 */
@property (nonatomic, copy)NSString * _Nullable messageContent;
/**
 * @brief Gets the timestamp of transcription message.
 */
@property (nonatomic, assign)NSInteger timeStamp;
/**
 * @brief Gets the message type of transcription message.
 */
@property (nonatomic, assign)ZoomVideoSDKLiveTranscriptionOperationType messageType;

@end

/**
 * @class ZoomVideoSDKLiveTranscriptionHelper
 * @brief Provides methods for controlling and interacting with live transcription.
 */
@interface ZoomVideoSDKLiveTranscriptionHelper : NSObject

/**
 * @brief Determines whether the user can start live transcription.
 * @return YES if the user can start live transcription. Otherwise, NO.
 */
- (BOOL)canStartLiveTranscription;

/**
 * @brief Gets the current live transcription status.
 * @return The current live transcription status.
 */
- (ZoomVideoSDKLiveTranscriptionStatus)getLiveTranscriptionStatus;

/**
 * @brief Starts live transcription. If the session allows multi-language transcription, all users can start live transcription.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)startLiveTranscription;

/**
 * @brief Stops live transcription. If the session allows multi-language transcription, all users can stop live transcription.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)stopLiveTranscription;

/**
 * @brief Gets the list of all available spoken languages in session.
 * @return If the function succeeds, it returns an NSArray of ZoomVideoSDKLiveTranscriptionLanguage objects. Otherwise, this function fails and returns nil.
 */
- (NSArray <ZoomVideoSDKLiveTranscriptionLanguage*>* _Nullable )getAvailableSpokenLanguages;

/**
 * @brief Sets the spoken language of the current user.
 * @param languageID The spoken language ID.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)setSpokenLanguage:(NSInteger)languageID;

/**
 * @brief Gets the spoken language of the current user.
 * @return If the function succeeds, it returns the current spoken language. Otherwise, this function fails and returns nil.
 */
- (ZoomVideoSDKLiveTranscriptionLanguage * _Nullable)getSpokenLanguage;

/**
 * @brief Enables or disables receiving original and translated content. If you enable this feature, you must start live transcription.
 * @param enable YES to enable, NO to disable.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)enableReceiveSpokenLanguageContent:(BOOL)enable;

/**
 * @brief Determines whether the view history translation message is available.
 * @return YES if the view history transcription message is available. Otherwise, NO.
 */
- (BOOL)isAllowViewFullTranscriptEnable;

/**
 * @brief Gets the list of all history translation messages in a session.
 * @return If the function succeeds, it returns an NSArray of ZoomVideoSDKLiveTranscriptionMessageInfo objects. Otherwise, this function fails and returns nil.
 */
- (NSArray <ZoomVideoSDKLiveTranscriptionMessageInfo *> * _Nullable)getHistoryTranslationMessageList;

/**
 * @brief Determines whether the feature to receive original and translated is available.
 * @return YES if the feature to receive original and translated is available. Otherwise, NO.
 */
- (BOOL)isReceiveSpokenLanguageContentEnabled;

/**
 * @brief Gets the list of all available translation languages in a session.
 * @return If the function succeeds, it returns an NSArray of ZoomVideoSDKLiveTranscriptionLanguage objects. Otherwise, this function fails and returns nil.
 */
- (NSArray <ZoomVideoSDKLiveTranscriptionLanguage*>* _Nullable)getAvailableTranslationLanguages;

/**
 * @brief Sets the translation language of the current user.
 * @param languageID The translation language ID. If the language ID is set to -1, live translation will be disabled.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)setTranslationLanguage:(NSInteger)languageID;

/**
 * @brief Gets the translation language of the current user.
 * @return If the function succeeds, it returns the current translation language. Otherwise, this function fails and returns nil.
 */
- (ZoomVideoSDKLiveTranscriptionLanguage * _Nullable)getTranslationLanguage;

@end


