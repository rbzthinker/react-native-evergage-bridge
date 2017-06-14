//
//  EVGContext.h
//  Evergage
//
//  Copyright © 2016 Evergage. All rights reserved.
//

#import <Foundation/Foundation.h>

#import <Evergage/EVGCampaign.h>
#import <Evergage/EVGItems.h>

/**
 Provides APIs to handle data campaigns, track item views and activity, and track manual actions,
 all within a lifecycle-managed context such as `<EVGScreen>`.
 
 Typically used as `<[UIViewController(Evergage) evergageScreen]>` and if necessary `<[Evergage globalContext]>`.
 
 @since 1.2.0
 */
@interface EVGContext : NSObject

- (nullable instancetype)init __attribute__((unavailable("Use UIViewController's 'evergageScreen' or Evergage's 'globalContext'")));

/// -----------------------------------
/// @name Lifecycle
/// -----------------------------------

/**
 If this context is currently active or not.
 
 A typical `<EVGScreen>` context is active when the screen is visible and the app is not backgrounded.
 
 Besides some initial setup just before becoming visible/active (`<setCampaignHandler:forTarget:>`, `<viewItem:>` etc),
 activity generally occurs while a context is active.
 
 Context activity, in addition to app state and user idleness, can affect campaign delivery, see `<setCampaignHandler:forTarget:>`
 
 @since 1.2.0
 */
@property (nonatomic, readonly) BOOL isActive;


/// -----------------------------------
/// @name Campaigns and Stats
/// -----------------------------------

/**
 To optionally support custom 'Data' campaigns, register a campaign handler for
 the app-defined target within this context. Can set nil to clear.
 
 The target should be a unique app-defined string that identifies the type of campaign data consumable by the handler.
 Specifically, `<EVGCampaign>`s designed for a `target` should all share a compatible `data` structure,
 in order to be consumable by any `<EVGCampaignHandler>` registered for that target.
 
 While the context, app, and user is active, the handler may receive asynchronous callbacks (on the main thread)
 with campaigns for the target, in response to actions tracked. The handler code should show/update the campaign to the user,
 when appropriate, and follow best practices using weak references, see `<EVGCampaignHandler>` for example code.
 
 Campaigns may be held for delivery while either:
 
 - The context, app, or user is inactive
 - No handler is found for the campaign's target within this context
 
 For each context and target, only the most recent campaign will be held.
 
 Lifecycle details:
 
 - For an `UIViewController/<EVGScreen>`, it is recommended to always call this method just before becoming visible, specifically `viewWillAppear:`.
 - In order to prevent accidental leaks in the handler block (see `<EVGCampaignHandler>` example code to prevent this), a screen has a limited time to become visible, after which its handlers and held campaigns will be cleared.
 - When a screen transitions to not visible (`viewWillDisappear:`), all handlers and held campaigns are cleared.
 - When a context transitions to not active (typically on app background or `viewWillDisappear:` for a screen), all held campaigns are cleared.
 
 @param handler The campaign handler this context should use for the specified target.
 @param target A unique app-defined string that identifies the type of campaign data consumable by the handler.
 @see EVGCampaignHandler
 @see EVGCampaign
 */
- (void)setCampaignHandler:(nullable EVGCampaignHandler)handler forTarget:(nonnull NSString *)target;

/**
 Tracks an impression for the provided campaign. Call this method after showing the campaign to the user or
 after the campaign would be shown but the user is in the control group.
 
 @param campaign The campaign for which an impression should be tracked.
 */
- (void)trackImpression:(nonnull EVGCampaign *)campaign;

/**
 Tracks a clickthrough for the provided campaign.
 
 @param campaign The campaign for which a clickthrough should be tracked.
 */
- (void)trackClickthrough:(nonnull EVGCampaign *)campaign;

/**
 Tracks a dismissal for the provided campaign.
 
 @param campaign The campaign for which a dismissal should be tracked.
 */
- (void)trackDismissal:(nonnull EVGCampaign *)campaign;


/// --------------------------------
/// @name Tracking Item Views
/// --------------------------------

/**
 Tracks that an item is being viewed.
 Set `nil` to indicate no longer viewing an item/category/tag.
 
 Recommended to call just before visible, viewWillAppear for a view controller.
 
 Evergage will automatically track the time spent viewing the item while the context, app, and user is active.
 The item will remain the one viewed until this method or `<viewItemDetail:>` are called again.
 
 @param item The item being viewed.
 */
- (void)viewItem:(nullable EVGItem *)item;

/**
 Tracks that the details of an item are being viewed, such as viewing other product images or a specifications tab.
 Set `nil` to indicate no longer viewing an item.
 
 Recommended to call just before visible, viewWillAppear for a view controller.
 
 Evergage will automatically track the time spent viewing the item while the context, app, and user is active.
 The item will remain the one viewed until this method or `<viewItem:>` are called again.
 
 @param item The item whose details are being viewed.
 */
- (void)viewItemDetail:(nullable EVGItem *)item;

/**
 Tracks that a category is being viewed.
 Set `nil` to indicate no longer viewing an item/category/tag.
 
 Recommended to call just before visible, viewWillAppear for a view controller.
 
 @param category The category being viewed.
 */
- (void)viewCategory:(nullable EVGCategory *)category;

/**
 Tracks that a tag is being viewed.
 Set `nil` to indicate no longer viewing an item/category/tag.
 
 Recommended to call just before visible, viewWillAppear for a view controller.
 
 @param tag The tag being viewed.
 */
- (void)viewTag:(nullable EVGTag *)tag;


/// --------------------------------
/// @name Tracking Item Activity
/// --------------------------------

/**
 Tracks that a line item is being added to the shopping cart.
 
 @param lineItem The line item being added.
 */
- (void)addToCart:(nonnull EVGLineItem *)lineItem;

/**
 Tracks that an order was purchased.
 
 If the order contains no lineItems, the lineItems currently in the cart will be
 used. If orderId is set and multiple purchase events are received for the same orderId, all but the first will be
 ignored.
 
 @param order The order that was purchased.
 */
- (void)purchase:(nonnull EVGOrder *)order;

/**
 Tracks that an item was reviewed.
 
 @param item The item that was reviewed.
 */
- (void)review:(nonnull EVGItem *)item;

/**
 Tracks that an item was reviewed, as well as the optional details of the review.
 
 @param item The item that was reviewed.
 @param reviewDetails The optional details of the review, such as the rating.
 */
- (void)review:(nonnull EVGItem *)item reviewDetails:(nullable EVGReview *)reviewDetails;

/**
 Tracks that an item was shared, for instance by email or on a social network.
 
 @param item The item that was shared.
 */
- (void)share:(nonnull EVGItem *)item;

/**
 Tracks that an item was commented on. For instance, an article or blog might accept comments.
 
 @param item The item that was commented on.
 */
- (void)comment:(nonnull EVGItem *)item;

/**
 Tracks that an item was marked by the user as a favorite item. This is an explicit action taken by the user (often
 indicated by a single star).
 
 @param item The item that was marked as favorite.
 */
- (void)favorite:(nonnull EVGItem *)item;


/// -------------------------------
/// @name Manually Tracking Actions
/// -------------------------------

/**
 Sends an event to Evergage describing an action that is not automatically tracked.
 
 This method should be preferred over `<[Evergage trackAction:]>`, since this method enables this context
 to potentially receive a campaign in response, via `<setCampaignHandler:forTarget:>`.
 
 @param action A short string that identifies the type of action.
 */
- (void)trackAction:(nonnull NSString *)action;

@end