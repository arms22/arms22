//
//  SoftModemTerminalAppDelegate.h
//  SoftModemTerminal
//
//  Created by arms22 on 10/05/02.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import <UIKit/UIKit.h>

@class MainViewController;
@class AudioSignalAnalyzer, FSKSerialGenerator, FSKRecognizer;

@interface SoftModemTerminalAppDelegate : NSObject <UIApplicationDelegate> {
	AudioSignalAnalyzer* analyzer;
	FSKSerialGenerator* generator;
	FSKRecognizer* recognizer;
    UIWindow *window;
    MainViewController *mainViewController;
}

@property (nonatomic, retain) AudioSignalAnalyzer* analyzer;
@property (nonatomic, retain) FSKSerialGenerator* generator;

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) MainViewController *mainViewController;

+ (SoftModemTerminalAppDelegate*) getInstance;

@end

