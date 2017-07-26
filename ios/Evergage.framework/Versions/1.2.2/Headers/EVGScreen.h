//
//  EVGScreen.h
//  Evergage
//
//  Copyright (c) 2015 Evergage. All rights reserved.
//

#import <Foundation/Foundation.h>

#import <Evergage/EVGContext.h>
#import <Evergage/EVGCampaign.h>
#import <Evergage/EVGItems.h>


/**
 A 'screen' of the app, and the context for its associated user activity.
 
 Typically an app screen is represented by a `UIViewController`.  Its `EVGScreen` can be retrieved via `<[UIViewController(Evergage) evergageScreen]>`.
 
 See `<EVGContext>` for APIs to track item views and activity, track manual actions, and handle data campaigns.
 
 Please contact Evergage if you need a non-UIViewController 'screen'.
 */
@interface EVGScreen : EVGContext

/**
 If the screen is visible.
 
 For the standard `<[UIViewController(Evergage) evergageScreen]>`, Evergage automatically:
 
 - Updates visibility during `viewDidAppear:` and `viewWillDisappear:`, for eligible view controllers.
 - Tracks the 'view action' when the screen becomes visible and the app is/becomes active, if an action is dynamically mapped to the screen using the Evergage web app.
   See <Tracking> Guide for more details.
 */
@property (readonly, getter=isVisible) BOOL visible;

@end
