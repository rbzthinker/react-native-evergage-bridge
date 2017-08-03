//
//  UIViewController+Evergage.h
//  Evergage
//
//  Copyright (c) 2015 Evergage. All rights reserved.
//

#import <UIKit/UIKit.h>

#import <Evergage/EVGItems.h>
#import <Evergage/EVGScreen.h>

// Reference to avoid app requiring -ObjC flag to link this category
FOUNDATION_EXPORT void * __nonnull const EVGCategoryUIViewControllerEvergage;


/**
 `UIViewController` category used to enable Evergage to automatically track screens, dynamically map them to actions,
 and provide APIs for tracking and campaigns.
 
 See `<evergageScreen>` for access to `<EVGContext>` APIs, and the <Tracking> Guide
 for more info about dynamically mapping screens to view actions.
 */
@interface UIViewController (Evergage)

/**
 Provides APIs to track item views and activity, track manual actions, and handle custom data campaigns for this screen.
 
 UI-related and lazily created on first access from main thread.
 
 `nil` if Evergage is disabled, or not yet accessed from main thread.
 
 @see EVGScreen
 @see EVGContext
 */
@property (nullable, nonatomic, readonly, strong) EVGScreen *evergageScreen;

/**
 The optional 'Evergage Name' for this UIViewController.
 
 Uniquely naming your UIViewControllers may make action-mapping screens in the Evergage web-app even faster and easier.
 
 Should be set before viewDidAppear, such as within Interface Builder (Attribute Inspector on the view controller)
 or programmatically in viewDidLoad or viewWillAppear.
 */
@property (nullable, atomic, copy) IBInspectable NSString *evergageName;

@end
