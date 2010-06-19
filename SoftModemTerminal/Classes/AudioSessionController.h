//
//  AudioSessionController.h
//  NoiseCanceller
//
//  Created by arms22 on 09/02/13.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <AudioToolbox/AudioToolbox.h>

@class AudioSessionController;

@protocol AudioSessionControllerDelegate <NSObject>
@required
- (void)audioSessionEndInterruption:(AudioSessionController*)audioSession;
- (void)audioSessionBeginInterruption:(AudioSessionController*)audioSession;

@optional
- (void)audioSessionAudioRouteChange:(AudioSessionController*)audioSession routeChangeReason:(int)reason oldRoute:(NSString*)oldRoute;
- (void)audioSessionAudioInputAvailable:(AudioSessionController*)audioSession;
@end



@interface AudioSessionController : NSObject {
	id<AudioSessionControllerDelegate> delegate;
	UInt32 audioCategory;
	BOOL active;
}
@property (readwrite, assign)									id<AudioSessionControllerDelegate> delegate;
@property (readwrite, assign)									UInt32 audioCategory;
@property (readwrite, assign, setter=setActive:)				BOOL active;
@property (readonly, assign, getter=audioInputIsAvailable)		BOOL audioInputAvailable;
@property (readonly, assign, getter=getHardwareSampleRate)		Float64 hardwareSampleRate;
+ (AudioSessionController*)sharedInstance;
@end
