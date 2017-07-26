//
//  EVGCampaign.h
//  Evergage
//
//  Copyright (c) 2015 Evergage. All rights reserved.
//

#import <Foundation/Foundation.h>


@class EVGCampaign;

/**
 Callback block implemented by the app to handle potential custom 'Data' campaigns (see `<EVGCampaign>`)
 served in response to Evergage actions generated by the user interacting with the app.
 
 See `<[EVGContext setCampaignHandler:forTarget:]>` for additional info and lifecycle details.
 
 #### Usage Details:
 
 - For a view controller, it is recommended to setup handlers in `viewWillAppear:`. See examples below.
 - Validate the expected campaign data/JSON before processing, since campaigns are dynamic and designed
   in the Evergage web app.
 - The same campaign could be re-served, so when applicable, check if the same content is already active.
   For example, there's no need to re-render the same message/UI to the user if it's still visible.
   This is especially true when testing a specific experience, see `<[Evergage handleOpenURL:]>`.
 - Follow best practices using weak references within the handler block, to avoid retaining objects in memory longer
   than expected, which could lead to memory bloat and unexpected behavior. Examples correctly using weak reference:
 
 Example that expects the featured product's name and updates a UILabel on screen:
 
 #### Obj-C:
    // In ViewController.m
 
    - (void)viewWillAppear:(BOOL)animated {
        [super viewWillAppear:animated];
        
        __weak typeof(self) weakSelf = self;
        EVGCampaignHandler handler = ^(EVGCampaign * __nonnull campaign) {
            // Safest to perform a single method/operation on weakSelf, which will simply be a no-op if weakSelf is nil:
            [weakSelf handleCampaign:campaign];
        };
 
        // The target indicates what/where the handler is designed to consume/display:
        [self.evergageScreen setCampaignHandler:handler forTarget:@"featuredProduct"];
    }
 
    - (void)handleCampaign:(nonnull EVGCampaign *)campaign {
        // Validate the campaign data since it's dynamic JSON. Avoid processing if fails.
        NSString *featuredProductName = campaign.data[@"featuredProductName"];
        if (![featuredProductName isKindOfClass:[NSString class]] || !featuredProductName.length) {
            return;
        }
 
        // Check if the same content is already visible/active (see Usage Details above).
        if (self.activeCampaign && [self.activeCampaign isEqual:campaign]) {
            NSLog(@"Ignoring campaign name %@ since equivalent content is already active", campaign.campaignName);
            return;
        }
 
        // Track the impression for statistics even if the user is in the control group.
        [self.evergageScreen trackImpression:campaign];

        // Only display the campaign if the user is not in the control group.
        if (!campaign.isControlGroup) {
            // Keep active campaign as long as needed for (re)display and comparison
            self.activeCampaign = campaign;
            NSLog(@"New active campaign name %@ for target %@ with data %@",
                  campaign.campaignName, campaign.target, campaign.data);
 
            // Display campaign content
            self.featuredProductLabel.text = [NSString stringWithFormat:@"Our featured product is %@!", featuredProductName];
        }
    }
 
 #### Swift:
    // In ViewController.swift
 
    override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(animated)
 
        // Note self is captured weakly
        let handler = { [weak self] (campaign: EVGCampaign) -> Void in
            // Safest to perform a single method/operation on weakSelf, which will simply be a no-op if weakSelf is nil:
            self?.handleCampaign(campaign: campaign)
        }
 
        // The target indicates what/where the handler is designed to consume/display:
        evergageScreen?.setCampaignHandler(handler, forTarget: "featuredProduct")
    }
 
    func handleCampaign(campaign: EVGCampaign) {
        // Validate the campaign data since it's dynamic JSON. Avoid processing if fails.
        guard let featuredProductName = campaign.data["featuredProductName"] as? String
            else { return }
        if (featuredProductName.isEmpty) { return }
 
        // Check if the same content is already visible/active (see Usage Details above).
        if (activeCampaign && activeCampaign.equals(campaign)) {
        NSLog("Ignoring campaign name %@ since equivalent content is already active",
              campaign.campaignName)
            return
        }

        // Track the impression for statistics even if the user is in the control group.
        evergageScreen?.trackImpression(campaign)

        // Only display the campaign if the user is not in the control group.
        if (!campaign.isControlGroup) {
            // Keep active campaign as long as needed for (re)display and comparison
            activeCampaign = campaign
            NSLog("New active campaign name %@ for target %@ with data %@",
                  campaign.campaignName, campaign.target, campaign.data)
 
            featuredProductLabel.text = String(format: "Our featured product is %@!", featuredProductName)
        }
    }
 */
typedef void (^EVGCampaignHandler)(EVGCampaign * __nonnull campaign);


/**
 A custom 'Data' campaign to be handled by the app, served in response to
 Evergage actions generated by the user interacting with the app.
 
 A campaign is delivered to a `<EVGCampaignHandler>` registered for a unique app-defined
 `<target>` in the scope of an `<EVGContext>` (typically a UIViewController).
 See `<[EVGContext setCampaignHandler:forTarget:]>` for additional info and lifecycle details.
 */
@interface EVGCampaign : NSObject

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnullability"
#pragma clang diagnostic ignored "-Wnullability-completeness"
- (nullable instancetype)init __attribute__((unavailable("Use campaigns delivered to an EVGCampaignHandler.")));
#pragma clang diagnostic pop

/**
 A unique app-defined string that identifies a type of data campaign consumable by the app.
 
 Specifically, campaigns designed for a target should all share a compatible `<data>` structure,
 in order to be consumable by any `<EVGCampaignHandler>` registered for that target.
 
 @see [EVGContext setCampaignHandler:forTarget:]
 */
@property (nonnull, readonly, nonatomic, copy) NSString *target;

/**
 The identifier for this campaign.
 */
@property (nonnull, readonly, nonatomic, copy) NSString *campaignId;

/**
 The name of this campaign.
 */
@property (nonnull, readonly, nonatomic, copy) NSString *campaignName;

/**
 The identifier of the experience in this campaign.
 */
@property (nonnull, readonly, nonatomic, copy) NSString *experienceId;

/**
 The name of the experience in this campaign.
 */
@property (nonnull, readonly, nonatomic, copy) NSString *experienceName;

/**
 The identifier of the message in this campaign.
 */
@property (nonnull, readonly, nonatomic, copy) NSString *messageId;

/**
 Array of related promoted items, if any, in minimal JSON form.
 Automatically used in stat tracking.
 */
@property (nullable, readonly, nonatomic, strong) NSArray<NSDictionary<NSString *,id> *> *promotedItemKeys;

/**
 The data campaign's JSON payload.
 
 The structure should be designed for the app-defined `<target>`, in order to be consumable by
 any `<EVGCampaignHandler>` registered for that target.
 
 @see [EVGContext setCampaignHandler:forTarget:]
 */
@property (nonnull, readonly, nonatomic, strong) NSDictionary<NSString *,id> *data;

/**
 If this campaign is for a user in the control group.
 
 If `NO`, this campaign should be handled normally. When applicable, show the campaign
 to the user and track the impression.
 
 If `YES`, this campaign should not take effect. Do not show to the user, but do track impression IF
 the campaign would have been shown.
 
 To ensure accurate statistics, make sure to execute all the logic that could affect whether this campaign would
 be shown to the user, regardless of this property. If all those checks pass and `isControlGroup == NO`, show
 the campaign to the user. Finally, call `<[EVGContext trackImpression:]>` to notify Evergage that the user
 saw the campaign (`isControlGroup == NO`) or would have seen it (`isControlGroup == YES`).
 */
@property (readonly, nonatomic, assign) BOOL isControlGroup;

/**
 Indicates whether this campaign is equivalent to another campaign.
 
 Typically used in `<CampaignHandler>` to avoid re-processing a campaign that's equivalent
 to one already visible/active.
 
 The equals method checks for equivalence in the source JSON returned by the server.
 Campaigns are not equal if there are differences in message content.
 
 @param object The other campaign to compare for equality to this campaign.
 @since 1.2.1
 */
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnullability"
#pragma clang diagnostic ignored "-Wnullability-completeness"
- (BOOL)isEqual:(id)object;
#pragma clang diagnostic pop

/**
 Returns a hash code value for the campaign based off of the campaign target, control group,
 and the ids and names of the campaign, experience, and message, but not message content.
 
 Campaigns with different message content (due to campaign edits or dynamic message
 content) currently return the same hashcode. Use `<equals>` to determine full equivalence or not.
 
 @since 1.2.1
 */
- (NSUInteger)hash;

@end