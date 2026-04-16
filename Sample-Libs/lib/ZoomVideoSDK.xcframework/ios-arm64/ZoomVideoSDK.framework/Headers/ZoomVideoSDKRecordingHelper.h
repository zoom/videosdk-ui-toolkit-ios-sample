/**
 * @file ZoomVideoSDKRecordingHelper.h
 * @brief Interface for cloud recording operations including start, stop, pause, resume recording and recording status management.
 */

#import <Foundation/Foundation.h>


/**
 * @class ZoomVideoSDKRecordAgreementHandler
 * @brief Class for recording agreement action.
 */
@interface ZoomVideoSDKRecordAgreementHandler : NSObject
/**
 * @brief Gets the recording agreement type.
 */
@property (assign, nonatomic, readonly) ZoomVideoSDKRecordAgreementType agreementType;
/**
 * @brief Accepts the individual record or traditional record.
 * @return YES if the function succeeds. Otherwise, NO.
 */
- (BOOL)accept;
/**
 * @brief Declines the individual record or traditional record. For individual record, will not record that person. For traditional record, will leave the meeting.
 * @return YES if the function succeeds. Otherwise, NO.
 */
- (BOOL)decline;

@end

/**
 * @class ZoomVideoSDKRecordingHelper
 * @brief Class for using cloud recording in the session.
 */
@interface ZoomVideoSDKRecordingHelper : NSObject

/**
 * @brief Checks if the current user meets the requirements to start cloud recording.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 * @note The following are the prerequisites to use the helper class: A cloud recording add-on plan. Cloud recording feature enabled on the Web portal.
 */
- (ZoomVideoSDKError)canStartRecording;

/**
 * @brief Starts cloud recording.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)startCloudRecording;

/**
 * @brief Stops cloud recording.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)stopCloudRecording;

/**
 * @brief Pauses the ongoing cloud recording.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)pauseCloudRecording;

/**
 * @brief Resumes the previously paused cloud recording.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)resumeCloudRecording;

/**
 * @brief Gets the current status of cloud recording.
 * @return Cloud recording status value defined in [ZoomVideoSDKRecordingStatus].
 */
- (ZoomVideoSDKRecordingStatus)getCloudRecordingStatus;

@end
