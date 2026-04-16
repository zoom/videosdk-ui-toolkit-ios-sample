/**
 * @file ZoomVideoSDKFileTranserHandle.h
 * @brief Interface for file transfer operations including sending, receiving, and managing file transfer status.
 */

#import <Foundation/Foundation.h>
#import <ZoomVideoSDK/ZoomVideoSDK.h>

#pragma mark - File Transfer -

/**
 * @class ZoomVideoSDKFileTransferProgress
 * @brief Represents the progress details of a file transfer.
 */
@interface ZoomVideoSDKFileTransferProgress : NSObject

/**
 * @brief Gets the ratio of the file transfer completed.
 */
@property(nonatomic, assign)    NSUInteger ratio;
/**
 * @brief Gets the size of the file transferred so far in bytes.
 */
@property(nonatomic, assign)    NSUInteger completeSize;
/**
 * @brief Gets the speed of the file transfer in bits per second.
 */
@property(nonatomic, assign)    NSUInteger bitPerSecond;
@end

/**
 * @class ZoomVideoSDKFileStatus
 * @brief Represents the status of a file transfer, including current state and progress.
 */
@interface ZoomVideoSDKFileStatus : NSObject
/**
 * @brief Gets the status of the file transfer.
 */
@property(nonatomic, assign)    ZoomVideoSDKFileTransferStatus transStatus;
/**
 * @brief Gets the progress of the file transfer.
 */
@property(nonatomic, strong, nullable)    ZoomVideoSDKFileTransferProgress *transProgress;
@end

/**
 * @class ZoomVideoSDKFileTransferBaseInfo
 * @brief Represents a file being transferred in the session.
 */
@interface ZoomVideoSDKFileTransferBaseInfo : NSObject
/**
 * @brief Gets the timestamp.
 * @return If the function succeeds, it returns the timestamp. Otherwise, this function fails and returns 0.
 */
- (NSTimeInterval)getTimeStamp;
/**
 * @brief Determines whether the file is sent to all users.
 * @return YES if the file is sent to all users. Otherwise, NO.
 */
- (BOOL)isSendToAll;
/**
 * @brief Gets the file size.
 * @return If the function succeeds, it returns the file size. Otherwise, this function fails and returns 0.
 */
- (NSUInteger)getFileSize;
/**
 * @brief Gets the file name.
 * @return If the function succeeds, it returns the file name. Otherwise, this function fails and returns nil.
 */
- (NSString * _Nullable)getFileName;
/**
 * @brief Gets the file transfer status.
 * @return If the function succeeds, it returns a ZoomVideoSDKFileStatus object. Otherwise, this function fails and returns nil.
 */
- (ZoomVideoSDKFileStatus *_Nullable)getStatus;
@end

/**
 * @class ZoomVideoSDKSendFile
 * @brief Represents a file being sent in the session.
 */
@interface ZoomVideoSDKSendFile : ZoomVideoSDKFileTransferBaseInfo
/**
 * @brief Gets the receiver of the file.
 * @return If the function succeeds, it returns the receiver. Otherwise, this function fails and returns nil.
 */
- (ZoomVideoSDKUser * _Nullable)getReceiver;
/**
 * @brief Cancels sending the file.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)cancelSend;
@end

/**
 * @class ZoomVideoSDKReceiveFile
 * @brief Represents a file being received in the session.
 */
@interface ZoomVideoSDKReceiveFile : ZoomVideoSDKFileTransferBaseInfo
/**
 * @brief Gets the sender of the file.
 * @return If the function succeeds, it returns the sender. Otherwise, this function fails and returns nil.
 */
- (ZoomVideoSDKUser * _Nullable)getSender;
/**
 * @brief Cancels receiving the file.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)cancelReceive;
/**
 * @brief Starts receiving the file.
 * @param path The path to save the file.
 * @return If the function succeeds, it returns Errors_Success. Otherwise, this function returns an error.
 */
- (ZoomVideoSDKError)startReceive:(NSString * _Nullable)path;
@end

