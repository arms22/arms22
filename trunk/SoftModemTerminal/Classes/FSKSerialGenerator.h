//
//  FSKSerialGenerator.h
//  FSK Terminal
//
//  Created by George Dean on 1/7/09.
//  Copyright 2009 Perceptive Development. All rights reserved.
//

#import "AudioSignalGenerator.h"


@interface FSKSerialGenerator : AudioSignalGenerator {
	float nsBitProgress;
	unsigned sineTableIndex;

	unsigned bitCount;
	UInt16 bits;
	
	BOOL hasQueuedBytes;
	BOOL sendCarrier;
	
	NSInputStream* bytesToSend;
	NSOutputStream* queuedBytes;
}

@property (nonatomic, retain) NSInputStream* bytesToSend;
@property (nonatomic, retain) NSOutputStream* queuedBytes;

- (void) writeByte:(UInt8)byte;
- (void) print:(NSString*)string;

@end
