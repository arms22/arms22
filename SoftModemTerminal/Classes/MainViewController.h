//
//  MainViewController.h
//  SoftModemTerminal
//
//  Created by arms22 on 10/05/02.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import "FlipsideViewController.h"
#import "CharReceiver.h"

@interface MainViewController : UIViewController <FlipsideViewControllerDelegate, UITextFieldDelegate, CharReceiver> {
	IBOutlet UITextView *textReceived;
}

- (IBAction)showInfo;

@end
