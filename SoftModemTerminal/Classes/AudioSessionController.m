//
//  AudioSessionController.m
//  NoiseCanceller
//
//  Created by arms22 on 09/02/13.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "AudioSessionController.h"


@implementation AudioSessionController

static void AudioSessionControllerInterruptionListener(void *inUserData,
											   UInt32 inInterruption)
{
	AudioSessionController *THIS = (AudioSessionController *)inUserData;
    if (inInterruption == kAudioSessionEndInterruption) {
		[THIS setActive:THIS->active];
		[THIS.delegate audioSessionEndInterruption:THIS];
    }
    else if (inInterruption == kAudioSessionBeginInterruption) {
		[THIS.delegate audioSessionBeginInterruption:THIS];
		AudioSessionSetActive(false);
    }
}

static void AudioSessionControllerPropertyListener(void *inClientData,
										   AudioSessionPropertyID inID,
										   UInt32 inDataSize,
										   const void *inData)
{
	AudioSessionController *THIS = (AudioSessionController*)inClientData;
	id delegate = THIS.delegate;
	if(inID == kAudioSessionProperty_AudioRouteChange){
		if([delegate respondsToSelector:@selector(audioSessionAudioRouteChange:routeChangeReason:oldRoute:)]){
			NSNumber *reasonNumber = [(NSDictionary*)inData objectForKey:(NSString*)CFSTR(kAudioSession_AudioRouteChangeKey_Reason)];
			NSString *oldRouteString = [(NSDictionary*)inData objectForKey:(NSString*)CFSTR(kAudioSession_AudioRouteChangeKey_OldRoute)];
			[delegate audioSessionAudioRouteChange:THIS routeChangeReason:[reasonNumber intValue] oldRoute:oldRouteString];
		}
	}
	else if(inID == kAudioSessionProperty_AudioInputAvailable){
		if([delegate respondsToSelector:@selector(audioSessionAudioInputAvailable:)])
			[delegate audioSessionAudioInputAvailable:THIS];
	}
}

@synthesize delegate;
@synthesize audioCategory;
@synthesize active;

- (void)setupAudioSession;
{
    AudioSessionInitialize(NULL,
						   NULL,
						   AudioSessionControllerInterruptionListener,
						   self);
	audioCategory = kAudioSessionCategory_AmbientSound;
	active = YES;
	
	AudioSessionPropertyID propertyID;
	propertyID = kAudioSessionProperty_AudioRouteChange;
	AudioSessionAddPropertyListener(propertyID,
									AudioSessionControllerPropertyListener,
									self);
	
	propertyID = kAudioSessionProperty_AudioInputAvailable;
	AudioSessionAddPropertyListener(propertyID,
									AudioSessionControllerPropertyListener,
									self);
}

+ (AudioSessionController*)sharedInstance;
{
	static AudioSessionController *_sharedInstance;
	@synchronized(_sharedInstance){
		if (_sharedInstance == nil) {
			_sharedInstance = [[AudioSessionController alloc] init];
		}
	}
	return _sharedInstance;
}

- (id)init;
{
    if(self = [super init]){
        [self setupAudioSession];
    }
    return self;
}

- (void)setActive:(BOOL)flag;
{
	active = flag;
	if(active){
		AudioSessionSetProperty(kAudioSessionProperty_AudioCategory,
								sizeof(audioCategory),
								&audioCategory);
	}
	AudioSessionSetActive(active);
}

- (BOOL)audioInputIsAvailable;
{
	UInt32 audioInputIsAvailable; 
	UInt32 propertySize = sizeof (audioInputIsAvailable);
	
	AudioSessionGetProperty(kAudioSessionProperty_AudioInputAvailable,
							&propertySize,
							&audioInputIsAvailable);
	
	return (audioInputIsAvailable ? YES : NO);
}

- (Float64)getHardwareSampleRate;
{
	Float64 sample_rate;
	UInt32 size = sizeof(sample_rate);
	
	AudioSessionGetProperty(kAudioSessionProperty_CurrentHardwareSampleRate,
							&size,
							&sample_rate);
	
	return sample_rate;
}

@end
