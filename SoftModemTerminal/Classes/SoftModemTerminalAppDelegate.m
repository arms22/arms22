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

	AVAudioSession *session = [AVAudioSession sharedInstance];
	session.delegate = self;
	if(session.inputIsAvailable){
		[session setCategory:AVAudioSessionCategoryPlayAndRecord error:nil];
	}else{
		[session setCategory:AVAudioSessionCategoryPlayback error:nil];
	}
	[session setActive:YES error:nil];
	[session setPreferredIOBufferDuration:0.023220 error:nil];

	recognizer = [[FSKRecognizer alloc] init];
	[recognizer addReceiver:mainViewController];

	generator = [[FSKSerialGenerator alloc] init];
	[generator play];

	analyzer = [[AudioSignalAnalyzer alloc] init];
	[analyzer addRecognizer:recognizer];

	if(session.inputIsAvailable){
		[analyzer record];
	}

	return YES;
}

- (void)restartAnalyzerAndGenerator:(BOOL)isInputAvailable
{
	AVAudioSession *session = [AVAudioSession sharedInstance];
	[session setActive:YES error:nil];
	[analyzer stop];
	[generator stop];
	if(isInputAvailable){
		[session setCategory:AVAudioSessionCategoryPlayAndRecord error:nil];
		[analyzer record];
	}else{
		[session setCategory:AVAudioSessionCategoryPlayback error:nil];
	}
	[generator play];
}

- (void)inputIsAvailableChanged:(BOOL)isInputAvailable
{
	NSLog(@"inputIsAvailableChanged %d",isInputAvailable);
	[self restartAnalyzerAndGenerator:isInputAvailable];
}

- (void)beginInterruption
{
	NSLog(@"beginInterruption");
}

- (void)endInterruption
{
	NSLog(@"endInterruption");
}

- (void)dealloc {
    [mainViewController release];
    [window release];
    [super dealloc];
}

@end
