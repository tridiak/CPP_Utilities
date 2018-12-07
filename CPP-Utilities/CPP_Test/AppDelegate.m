//
//  AppDelegate.m
//  CPP_Test
//
//  Created by tridiak on 8/11/18.
//  Copyright © 2018 tridiak. All rights reserved.
//

#import "AppDelegate.h"
#import "Usage.hpp"

@interface AppDelegate ()

@property (weak) IBOutlet NSWindow *window;
@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
	// Insert code here to initialize your application
	HeirarchyUsage();
	
	exit(0);
}


- (void)applicationWillTerminate:(NSNotification *)aNotification {
	// Insert code here to tear down your application
}


@end
