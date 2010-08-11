//
//  MainViewController.m
//  MultiTrigger
//
//  Created by arms22 on 10/05/02.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import "MainViewController.h"
#import "MultiTriggerAppDelegate.h"
#import "FSKSerialGenerator.h"
#include <ctype.h>

#define MULTI_TRIGGER_SHUTTER_ON	0x01
#define MULTI_TRIGGER_SHUTTER_OFF	0x02
#define MULTI_TRIGGER_AF_ON			0x03
#define MULTI_TRIGGER_AF_OFF		0x04
#define MULTI_TRIGGER_SHOOT	        0x05


NSTimeInterval sliderValueToInterval[] = {
	0.5,
	1.0,
	2.0,
	4.0,
	8.0,
	15.0,
	30.0,
	60.0,
	2 * 60.0,
	4 * 60.0,
	8 * 60.0,
	15 * 60.0,
	30 * 60.0,
//	1 * 60 * 60.0,
//	2 * 60 * 60.0,
//	4 * 60 * 60.0,
//	8 * 60 * 60.0,
};
//1"	2"	4"	8"	15"	30"	1'	2'	4'	8'	15'	30'	1h	2h	4h	8h

@implementation MainViewController


- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
        // Custom initialization
		intervalTimer = nil;
    }
    return self;
}


/*
// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
	[super viewDidLoad];
}
*/


- (void)flipsideViewControllerDidFinish:(FlipsideViewController *)controller {
    
	[self dismissModalViewControllerAnimated:YES];
}

- (IBAction)showInfo {    
	
	FlipsideViewController *controller = [[FlipsideViewController alloc] initWithNibName:@"FlipsideView" bundle:nil];
	controller.delegate = self;
	
	controller.modalTransitionStyle = UIModalTransitionStyleFlipHorizontal;
	[self presentModalViewController:controller animated:YES];
	
	[controller release];
}

- (void)sendDeviceCommand:(unsigned char)command option:(unsigned char)option {
	[[MultiTriggerAppDelegate getInstance].generator writeByte:0xff];
	[[MultiTriggerAppDelegate getInstance].generator writeByte:command];
	[[MultiTriggerAppDelegate getInstance].generator writeByte:option];
}

- (NSString *)makeIntervalTimeString:(NSTimeInterval)interval {
	unsigned long itrvl = (unsigned long)interval;
	int hour, min, sec, hndr;
	
	hour = itrvl / (60 * 60);
	itrvl -= hour * (60 * 60);
	min = itrvl / (60);
	itrvl -= min * (60);
	sec = itrvl;
	hndr = (int)(interval * 100) % 100;
	
//	hour = itrvl / (60 * 60 * 1000);
//	itrvl -= hour * (60 * 60 * 1000);
//	min = itrvl / (60 * 1000);
//	itrvl -= min * (60 * 1000);
//	sec = itrvl / 1000;
//	itrvl -= sec * (1000);
//	hndr = itrvl / 10;
	
	return [NSString stringWithFormat:@"%02d:%02d.%02d",min, sec, hndr];
//	labelIntervalTime.text = [NSString stringWithFormat:@"%02d:%02d:%02d.%02d",
//							  hour, min, sec, hndr];
}

- (void)intervalTimerFireMethod:(NSTimer*)theTimer {
	
	NSLog(@"intervalTimerFireMethod");
	
	NSTimeInterval remaing = [shutterDate timeIntervalSinceNow];
	if (remaing > 0.0f) {
		labelIntervalRemaingTime.text = [self makeIntervalTimeString:remaing];
	} else {
		labelIntervalRemaingTime.text = [self makeIntervalTimeString:0.0f];
		
		[self sendDeviceCommand:MULTI_TRIGGER_SHOOT option:0];
		
		[shutterDate release];
		shutterDate = [[NSDate dateWithTimeIntervalSinceNow:shutterIntervalTime] retain];
	}
}

- (IBAction)intervalModeSwitchValueChanged:(UISwitch*)sender {
	NSLog(@"intervalModeSwitchValueChanged %d",sender.on);
	
	if (sender.on) {
		[shutterDate release];
		shutterDate = [[NSDate dateWithTimeIntervalSinceNow:shutterIntervalTime] retain];
		
		intervalTimer = [NSTimer scheduledTimerWithTimeInterval:0.05
														 target:self
													   selector:@selector(intervalTimerFireMethod:)
													   userInfo:nil
														repeats:YES];
		[intervalTimer retain];
		[intervalTimer fire];
	} else {
		if (intervalTimer) {
			[intervalTimer invalidate];
			[intervalTimer release];
			intervalTimer = nil;
		}
	}
}

- (IBAction)intervalTimeSliderValueChanged:(UISlider*)sender {
	NSLog(@"intervalTimeSliderValueChanged %f",sender.value);
	
	shutterIntervalTime = sliderValueToInterval[(int)sender.value];
	
	[shutterDate release];
	shutterDate = [[NSDate dateWithTimeIntervalSinceNow:shutterIntervalTime] retain];
	
	labelIntervalTime.text = [self makeIntervalTimeString:shutterIntervalTime];	
}

- (IBAction)shutterDown {
	NSLog(@"shutterDown");
	[self sendDeviceCommand:MULTI_TRIGGER_SHUTTER_ON option:0];
}

- (IBAction)shutterUp {
	NSLog(@"shutterUp");
	[self sendDeviceCommand:MULTI_TRIGGER_SHUTTER_OFF option:0];
}

- (IBAction)afDown {
	NSLog(@"afDown");
	[self sendDeviceCommand:MULTI_TRIGGER_AF_ON option:0];
}

- (IBAction)afUp {
	NSLog(@"afUp");
	[self sendDeviceCommand:MULTI_TRIGGER_AF_OFF option:0];
}

//- (BOOL)textFieldShouldReturn:(UITextField *)textField
//{	
//	[[MultiTriggerAppDelegate getInstance].generator writeByte:0xff];
//	[[MultiTriggerAppDelegate getInstance].generator print:textField.text];
//	textField.text = @"";
//	return YES;
//}

- (void) receivedChar:(char)input {
//	if(isprint(input)){
//		textReceived.text = [textReceived.text stringByAppendingFormat:@"%c", input];
//	}
}

- (void)didReceiveMemoryWarning {
	// Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
	
	// Release any cached data, images, etc. that aren't in use.
}


- (void)viewDidUnload {
	// Release any retained subviews of the main view.
	// e.g. self.myOutlet = nil;
}


/*
// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
	// Return YES for supported orientations.
	return (interfaceOrientation == UIInterfaceOrientationPortrait);
}
*/


- (void)dealloc {
    [super dealloc];
}


@end
