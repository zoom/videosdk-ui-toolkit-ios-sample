/**
 * @file ZoomVideoSDKPreProcessRawData.h
 * @brief Interface for video raw data preprocessing including custom video processing and filtering operations.
 */

#import <Foundation/Foundation.h>

/**
 * @class ZoomVideoSDKPreProcessRawData
 * @brief A class that contains the information of device capture video frame.
 */
@interface ZoomVideoSDKPreProcessRawData : NSObject

/**
 * @brief Gets the stream size.
 */
@property (nonatomic, assign) CGSize size;

/**
 * @brief Gets the Y stride.
 */
@property (nonatomic, assign) int yStride;

/**
 * @brief Gets the U stride.
 */
@property (nonatomic, assign) int uStride;

/**
 * @brief Gets the V stride.
 */
@property (nonatomic, assign) int vStride;

/**
 * @brief Determines whether video raw data is limited.
 */
@property (nonatomic, assign) BOOL isLimited;

/**
 * @brief Gets the YUVI420 Y buffer. The Y component represents the luma or brightness value of the color.
 * @param lineNum The line number.
 * @return Y buffer.
 */
- (char *)getYBuffer:(int)lineNum;

/**
 * @brief Gets the YUVI420 U buffer. The U component represents the chroma value.
 * @param lineNum The line number.
 * @return U buffer.
 */
- (char *)getUBuffer:(int)lineNum;

/**
 * @brief Gets the YUVI420 V buffer. The V component represents the chroma value.
 * @param lineNum The line number.
 * @return V buffer.
 */
- (char *)getVBuffer:(int)lineNum;

/**
 * @brief Gets the raw data format of video data.
 */
@property (nonatomic, assign) ZoomVideoSDKFrameDataFormat format;

/**
 * @brief Gets the video raw data rotation.
 */
@property (nonatomic, assign) ZoomVideoSDKVideoRawDataRotation rotation;

@end

