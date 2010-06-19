//
//  SoftModemTerminalAppDelegate.m
//  SoftModemTerminal
//
//  Created by arms22 on 10/05/02.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import "SoftModemTerminalAppDelegate.h"
#import "MainViewController.h"
#import "AudioSignalAnalyzer.h"
#import "FSKSerialGenerator.h"
#import "FSKRecognizer.h"
#import "AudioSessionController.h"

@implementation SoftModemTerminalAppDelegate

@synthesize analyzer;
@synthesize generator;
@synthesize window;
@synthesize mainViewController;

+ (SoftModemTerminalAppDelegate*) getInstance
{
	return (SoftModemTerminalAppDelegate*)[UIApplication sharedApplication].delegate;
}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {    
    
	MainViewController *aController = [[MainViewController alloc] initWithNibName:@"MainView" bundle:nil];
	self.mainViewController = aController;
	[aController release];

    mainViewController.view.frame = [UIScreen mainScreen].applicationFrame;
	[window addSubview:[mainViewController view]];
    [window makeKeyAndVisible];

	[AudioSessionController sharedInstance].audioCategory = kAudioSessionCategory_PlayAndRecord;
	[AudioSessionController sharedInstance].active = YES;

	recognizer = [[FSKRecognizer alloc] init];
	[recognizer addReceiver:mainViewController];

	generator = [[FSKSerialGenerator alloc] init];
	[generator play];

	analyzer = [[AudioSignalAnalyzer alloc] init];
	[analyzer addRecognizer:recognizer];
	[analyzer record];

	return YES;
}


- (void)dealloc {
    [mainViewController release];
    [window release];
    [super dealloc];
}

@end
