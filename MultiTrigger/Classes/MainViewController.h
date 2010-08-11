//
//  MainViewController.h
//  MultiTrigger
//
//  Created by arms22 on 10/05/02.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import "FlipsideViewController.h"
#import "CharReceiver.h"

@interface MainViewController : UIViewController <FlipsideViewControllerDelegate, UITextFieldDelegate, CharReceiver> {
	IBOutlet UILabel	*labelIntervalRemaingTime;
	IBOutlet UILabel	*labelIntervalTime;
	IBOutlet UIButton	*btnShutter;
	IBOutlet UIButton	*btnAF;
	
	NSTimer				*intervalTimer;
	NSTimeInterval		shutterIntervalTime;
	NSDate				*shutterDate;
}

- (IBAction)showInfo;

- (void)sendDeviceCommand:(unsigned char)command option:(unsigned char)option;

- (IBAction)intervalModeSwitchValueChanged:(UISwitch*)sender;
- (IBAction)intervalTimeSliderValueChanged:(UISlider*)sender;

- (IBAction)shutterDown;
- (IBAction)shutterUp;

- (IBAction)afDown;
- (IBAction)afUp;

@end
