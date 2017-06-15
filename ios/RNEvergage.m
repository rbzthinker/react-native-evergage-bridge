
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

RCT_EXPORT_METHOD(start:(NSString *)account withDataset:(NSString *)dataset) {
    Evergage *evergage = [Evergage sharedInstance];
    [evergage startWithEvergageAccountKey:account dataset:dataset];
}

RCT_EXPORT_METHOD(setUserAttribute:(NSString *)name withValue:(NSString *)value) {
    Evergage *evergage = [Evergage sharedInstance];
    [evergage setUserAttribute:value forName:name];
}

RCT_EXPORT_METHOD(trackAction:(NSString *)action) {
    EVGContext *evergage = [[Evergage sharedInstance] globalContext];
    [evergage trackAction:action];
}

@end
  
