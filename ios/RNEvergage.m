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
    EVGTag *tagProductBrand = [EVGTag tagWithId:productMap[@"brand"] type:EVGTagTypeBrand];
    NSArray *tags = [NSArray arrayWithObject:tagProductBrand, nil];
    [product setTags:tags];
    [screen viewItem:product];
}

- (NSString *)getEventName {
    return @"EvergageCampaignHandler";
}

- (NSArray<NSString *> *)supportedEvents {
    return @[[self getEventName]];
}

RCT_EXPORT_METHOD(setCampaignHandler:(NSString *)target) {
    EVGContext *screen = [self getScreen];
    
    EVGCampaignHandler handler = ^(EVGCampaign * __nonnull campaign) {
        [self sendEventWithName:[self getEventName]
                           body:@{@"target":target,
                                  @"data": campaign.data}];
    };
    
    [screen setCampaignHandler:handler forTarget:target];
}

@end











