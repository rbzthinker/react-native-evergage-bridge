#import "RNEvergage.h"
#import <Evergage/Evergage.h>

@implementation RNEvergage {
    NSMutableArray *events;
}

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
    evergage.logLevel = EVGLogLevelAll;
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

RCT_EXPORT_METHOD(viewProduct:(NSDictionary *)productMap) {
    EVGContext *screen = [self getScreen];
    EVGProduct *product = [EVGProduct productWithId:productMap[@"id"]
                                               name:productMap[@"name"]
                                              price:productMap[@"price"]
                                                url:productMap[@"url"]
                                           imageUrl:productMap[@"imageUrl"]
                                     evgDescription:@""
                           ];
    [screen viewItem:product];
}

- (NSString *)getEventNameForTarget:(NSString *)target {
    return [NSString stringWithFormat:@"EvergageCampaignHandler-%@", target];
}

- (NSArray<NSString *> *)supportedEvents {
    return events;
}

RCT_EXPORT_METHOD(setCampaignHandler:(NSString *)target) {
    EVGContext *screen = [self getScreen];
    NSString *eventName = [self getEventNameForTarget:target];
    [events addObject:eventName];
    
    EVGCampaignHandler handler = ^(EVGCampaign * __nonnull campaign) {
        NSString *eventName = [self getEventNameForTarget:target];
        [self sendEventWithName:eventName body:campaign.data];
    };
    
    [screen setCampaignHandler:handler forTarget:target];
}

@end











