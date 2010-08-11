//
//  MainViewController.m
//  SoftModemTerminal
//
//  Created by arms22 on 10/05/02.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import "MainViewController.h"
#import "SoftModemTerminalAppDelegate.h"
#import "FSKSerialGenerator.h"
#include <ctype.h>

@implementation MainViewController


- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
	if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
		// Custom initialization
		multiByteLength = 0;
		multiBytePos = 0;
		memset(multiBytes, 0, sizeof(multiBytes));		
	}
	return self;
}


// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
	[super viewDidLoad];

	[[NSNotificationCenter defaultCenter] addObserver:self
											 selector:@selector(keyboardWillShow:)
												 name:UIKeyboardWillShowNotification
											   object:nil];
	[[NSNotificationCenter defaultCenter] addObserver:self
											 selector:@selector(keyboardWillHide:)
												 name:UIKeyboardWillHideNotification
											   object:nil];
}

#pragma mark -
#pragma mark Responding to keyboard events

- (void)keyboardWillShow:(NSNotification *)notification {
	
	/*
	 Reduce the size of the text view so that it's not obscured by the keyboard.
	 Animate the resize so that it's in sync with the appearance of the keyboard.
	 */
	
	NSDictionary *userInfo = [notification userInfo];
	
	// Get the origin of the keyboard when it's displayed.
	NSValue* aValue = [userInfo objectForKey:UIKeyboardFrameEndUserInfoKey];
	
	// Get the top of the keyboard as the y coordinate of its origin in self's view's coordinate system. The bottom of the text view's frame should align with the top of the keyboard's final position.
	CGRect keyboardRect = [aValue CGRectValue];
	keyboardRect = [self.view convertRect:keyboardRect fromView:nil];
	
	CGRect newTextViewFrame = textReceived.frame;
	newTextViewFrame.size.height -= keyboardRect.size.height;
	
	// Get the duration of the animation.
	NSValue *animationDurationValue = [userInfo objectForKey:UIKeyboardAnimationDurationUserInfoKey];
	NSTimeInterval animationDuration;
	[animationDurationValue getValue:&animationDuration];
	
	// Animate the resize of the text view's frame in sync with the keyboard's appearance.
	[UIView beginAnimations:nil context:NULL];
	[UIView setAnimationDuration:animationDuration];
	
	textReceived.frame = newTextViewFrame;
	
	[UIView commitAnimations];
}


- (void)keyboardWillHide:(NSNotification *)notification {
	
	NSDictionary* userInfo = [notification userInfo];
	
	CGRect newTextViewFrame = textReceived.frame;
	newTextViewFrame.size.height = self.view.bounds.size.height - newTextViewFrame.origin.y;
	
	/*
	 Restore the size of the text view (fill self's view).
	 Animate the resize so that it's in sync with the disappearance of the keyboard.
	 */
	NSValue *animationDurationValue = [userInfo objectForKey:UIKeyboardAnimationDurationUserInfoKey];
	NSTimeInterval animationDuration;
	[animationDurationValue getValue:&animationDuration];
	
	[UIView beginAnimations:nil context:NULL];
	[UIView setAnimationDuration:animationDuration];
	
	textReceived.frame = newTextViewFrame;
	
	[UIView commitAnimations];
}



- (void)flipsideViewControllerDidFinish:(FlipsideViewController *)controller
{
	[self dismissModalViewControllerAnimated:YES];
}

- (IBAction)showInfo
{
	FlipsideViewController *controller = [[FlipsideViewController alloc] initWithNibName:@"FlipsideView" bundle:nil];
	controller.delegate = self;
	
	controller.modalTransitionStyle = UIModalTransitionStyleFlipHorizontal;
	[self presentModalViewController:controller animated:YES];
	
	[controller release];
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField
{	
	[[SoftModemTerminalAppDelegate getInstance].generator writeBytes:[textField.text UTF8String]
															  length:[textField.text lengthOfBytesUsingEncoding:NSUTF8StringEncoding]];
	//[[SoftModemTerminalAppDelegate getInstance].generator writeByte:'\n'];
	textField.text = @"";
	return YES;
}

- (void) receivedChar:(char)input
{
	unsigned char byte = (unsigned char)input;
	NSMutableString *newstr = [NSMutableString string];
	
	/*
	 C0-DF 2byte
	 E0-EF 3byte
	 F0-F7 4byte
	 */
	if(multiBytePos < multiByteLength){
		multiBytes[multiBytePos++] = byte;
		if(multiBytePos == multiByteLength){
			multiBytes[multiBytePos] = 0;
			NSString *utf8 = [NSString stringWithUTF8String:multiBytes];
			if(utf8 && [utf8 length]){
				[newstr appendString:utf8];
			}else{
				int i;
				for(i=0; i<multiByteLength; i++)
					[newstr appendFormat:@"(%x)", multiBytes[i]];
			}
		}
	}else{
		if(0xCF <= byte && byte <= 0xDF){
			multiByteLength = 2;
			multiBytePos = 0;
			multiBytes[multiBytePos++] = byte;
		}
		else if(0xE0 <= byte && byte <= 0xEF){
			multiByteLength = 3;
			multiBytePos = 0;
			multiBytes[multiBytePos++] = byte;
		}
		else if(0xF0 <= byte && byte <= 0xF7){
			multiByteLength = 4;			
			multiBytePos = 0;
			multiBytes[multiBytePos++] = byte;
		}
		else{
			if(isascii(input)){
				[newstr appendFormat:@"%c", input];
			}else{
				[newstr appendFormat:@"(%x)", byte];
			}
		}
	}
	
	if([newstr length] > 0){
		NSRange range = NSMakeRange([textReceived.text length], [newstr length]);
		textReceived.text = [textReceived.text stringByAppendingString:newstr];
		[textReceived scrollRangeToVisible:range];
	}
}

- (void)didReceiveMemoryWarning {
	// Releases the view if it doesn't have a superview.
	[super didReceiveMemoryWarning];
	
	// Release any cached data, images, etc. that aren't in use.
}


- (void)viewDidUnload {
	// Release any retained subviews of the main view.
	// e.g. self.myOutlet = nil;
	[[NSNotificationCenter defaultCenter] removeObserver:self
													name:UIKeyboardDidShowNotification
												  object:nil];
	[[NSNotificationCenter defaultCenter] removeObserver:self
													name:UIKeyboardWillHideNotification
												  object:nil];
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
