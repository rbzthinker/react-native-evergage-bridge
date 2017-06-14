
#import "RNEvergage.h"
#import <Evergage/Evergage.h>

@implementation RNEvergage

- (dispatch_queue_t)methodQueue
{
    return dispatch_get_main_queue();
}
RCT_EXPORT_MODULE()

RCT_EXPORT_METHOD(setUserId:(NSString *)userId) {
    Evergage *evergage = [Evergage sharedInstance];
    evergage.userId = userId;
};

RCT_EXPORT_METHOD(setUserAttribute:(NSString *)name withValue:(NSString *)value) {
    Evergage *evergage = [Evergage sharedInstance];
    [evergage setUserAttribute:value forName:name];
}

RCT_EXPORT_METHOD(trackAction:(NSString *)action) {
    EVGContext *evergage = [[Evergage sharedInstance] globalContext];
    [evergage trackAction:action];
}

RCT_EXPORT_METHOD(trackItem:(NSDictionary *)item) {
    EVGContext *evergage = [[Evergage sharedInstance] globalContext];
    EVGProduct *product = [EVGProduct productFromJSONDictionary:item];
    [evergage viewItem:product];
}

@end
  
