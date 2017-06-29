
#import "RNEvergage.h"
#import <Evergage/Evergage.h>

@implementation RNEvergage

- (dispatch_queue_t)methodQueue
{
    return dispatch_get_main_queue();
}

- (EVGContext *)getScreen {
    return [[Evergage sharedInstance] globalContext];
}

RCT_EXPORT_MODULE()

RCT_EXPORT_METHOD(setUserId:(NSString *)userId) {
    Evergage *evergage = [Evergage sharedInstance];
    evergage.userId = userId;
};

RCT_EXPORT_METHOD(start:(NSString *)account withDataset:(NSString *)dataset) {
    Evergage *evergage = [Evergage sharedInstance];
    [evergage reset];
#ifdef DEBUG
    evergage.logLevel = EVGLogLevelWarn;
    [evergage allowDesignConnections];
#endif
    [evergage startWithEvergageAccountKey:account dataset:dataset];
}

RCT_EXPORT_METHOD(setUserAttribute:(NSString *)name withValue:(NSString *)value) {
    Evergage *evergage = [Evergage sharedInstance];
    [evergage setUserAttribute:value forName:name];
}

RCT_EXPORT_METHOD(trackAction:(NSString *)action) {
    EVGContext *screen = [self getScreen];
    [screen trackAction:action];
}

RCT_EXPORT_METHOD(setCampaignHandler:(NSString *)target withCallback:(RCTResponseSenderBlock)callback) {
    EVGContext *screen = [self getScreen];
    
    EVGCampaignHandler handler = ^(EVGCampaign * __nonnull campaign) {
        callback(@[campaign.campaignName, campaign.data]);
    };
    
    [screen setCampaignHandler:handler forTarget:target];
}

@end











