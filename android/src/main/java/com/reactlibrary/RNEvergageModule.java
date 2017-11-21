
package com.reactlibrary;

import android.support.annotation.NonNull;
import android.util.Log;

import com.evergage.android.Campaign;
import com.evergage.android.CampaignHandler;
import com.evergage.android.Context;
import com.evergage.android.Evergage;
import com.evergage.android.promote.Category;
import com.evergage.android.promote.Product;
import com.evergage.android.promote.Tag;
import com.facebook.react.bridge.Arguments;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactMethod;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.bridge.UiThreadUtil;
import com.facebook.react.bridge.WritableMap;
import com.facebook.react.modules.core.RCTNativeAppEventEmitter;

import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class RNEvergageModule extends ReactContextBaseJavaModule {
    private static final String TAG = "RNEvergageModule";
    private static final String EVERGAGE_CAMPAIGN_EVENT = "EvergageCampaignHandler"; //If you change this, remember to also change it in index.js

    /**
     * Map with campaign target name as key and campaign handler as value
     * key: campaign Target
     */
    private Map<String, CampaignHandler> campaignHandlers = new HashMap<>();

    private final ReactApplicationContext reactContext;

    private Map<String, Campaign> campaignData = new HashMap<>();

    public RNEvergageModule(ReactApplicationContext reactContext) {
        super(reactContext);
        this.reactContext = reactContext;
    }

    /**
     * This method (and getScreenForActivity) must be called from main thread
     * Use UiThreadUtil.runOnUiThread for this to avoid calling runnable from dead thread.
     */
    private Context getScreen() {
        return Evergage.getInstance().getGlobalContext();
    }

    @ReactMethod
    public void start(final String account, final String dataset) {
        UiThreadUtil.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Evergage evergage = Evergage.getInstance();
                evergage.reset(); //.reset() needs to be called to override subsequent config for .start()
                evergage.start(account, dataset);
            }
        });
    }

    @ReactMethod
    public void setUserId(String userId) {
        Evergage.getInstance().setUserId(userId);
    }

    @ReactMethod
    public void setUserAttribute(String name, String value) {
        Evergage.getInstance().setUserAttribute(name, value);
    }

    @ReactMethod
    public void trackDismissal(final String target) {
        UiThreadUtil.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Context screen = getScreen();
                if (null != screen) {
                    screen.trackDismissal(campaignData.get(target));
                }
            }
        });
    }

    @ReactMethod
    public void trackClickThrough(final String target) {
        UiThreadUtil.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Context screen = getScreen();
                if (null != screen) {
                    screen.trackClickthrough(campaignData.get(target));
                }
            }
        });
    }

    @ReactMethod
    public void trackImpression(final String target) {
        UiThreadUtil.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Context screen = getScreen();
                if (null != screen) {
                    screen.trackImpression(campaignData.get(target));
                }
            }
        });
    }

    @ReactMethod
    public void trackAction(final String action) {
        UiThreadUtil.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Context screen = getScreen();
                if (null != screen) {
                    screen.trackAction(action);
                }
            }
        });
    }

    @ReactMethod
    public void viewProduct(final ReadableMap productMap) {
        UiThreadUtil.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Context screen = getScreen();
                if (null != screen) {
                    Product product = new Product(productMap.getString("id")); //required
                    product.price = canGetValueForKey(productMap, "price") ? productMap.getDouble("price") : null;
                    product.listPrice = canGetValueForKey(productMap, "retailPrice") ? productMap.getDouble("retailPrice") : null;
                    product.inventoryCount = canGetValueForKey(productMap, "stock") ? productMap.getInt("stock") : null;
                    product.alternateId = canGetValueForKey(productMap, "sku") ? productMap.getString("sku") : null;

                    //The following are required fields for campaign data to train correctly
                    product.imageUrl = canGetValueForKey(productMap, "imageUrl") ? productMap.getString("imageUrl") : null;
                    product.url = canGetValueForKey(productMap, "url") ? productMap.getString("url") : null;
                    product.name = canGetValueForKey(productMap, "name") ? productMap.getString("name") : null;

                    //tags
                    List<Tag> tags = new ArrayList<>();
                    //check for brand name key value
                    if(canGetValueForKey(productMap, "brand")){
                        Tag tagProductBrand = new Tag(productMap.getString("brand"), Tag.Type.Brand);
                        tags.add(tagProductBrand);
                    }
                    if(canGetValueForKey(productMap, "gender")){
                        Tag tagProductGender = new Tag(productMap.getString("gender"), Tag.Type.Gender);
                        tags.add(tagProductGender);
                    }
                    if(canGetValueForKey(productMap, "classification")){
                        Tag tagProductClass = new Tag(productMap.getString("classification"), Tag.Type.ItemClass);
                        tags.add(tagProductClass);
                    }

                    //Categories
                    List<Category> categories = new ArrayList<>();

                    if(canGetValueForKey(productMap, "category")){
                        String strProductCategory = productMap.getString("category");
                        String strProductSubCategory = productMap.getString("subcategory");
                        if(strProductSubCategory.length()>0){
                            strProductCategory = strProductCategory + "|" + strProductSubCategory;
                        }
                        Category productCategory = new Category(strProductCategory);
                        categories.add(productCategory);
                    }

                    if(tags.size()>0)
                        product.tags = tags;
                    if(categories.size()>0)
                        product.categories = categories;

                    screen.viewItem(product);
                }
            }
        });
    }

    @ReactMethod
    public void setCampaignHandler(final String target) {
        UiThreadUtil.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Context screen = getScreen();

                CampaignHandler handler = new CampaignHandler() {
                    @Override
                    public void handleCampaign(@NonNull Campaign campaign) {
                        /*
                          Validate campaign data since it's dynamic JSON
                          (although the key-value pair returned from evergage is always with type
                           <String, String>)
                         */
                        campaignData.put(target, campaign);
                        JSONObject data = campaign.getData();
                        try {
                            WritableMap map = Arguments.createMap();
                            map.putString("target", target);
                            map.putMap("data", JsonConvert.jsonToReact(data));

                            //Call JS Callback on frontend with campaign name and JSON data
                            //React-native's Callback object can only be invoked ONCE, so we use RCTNativeAppEventEmitter instead
                            RCTNativeAppEventEmitter eventEmitter = getReactApplicationContext().getJSModule(RCTNativeAppEventEmitter.class);
                            eventEmitter.emit(EVERGAGE_CAMPAIGN_EVENT, map);
                        } catch (JSONException e) {
                            e.printStackTrace();
                        }
                    }
                };
                campaignHandlers.put(target, handler);
                screen.setCampaignHandler(handler, target);
            }
        });
    }

    @Override
    public String getName() {
        return "RNEvergage";
    }

    /**
     * Check if given React-native ReadableMap has a given key and the value is not equal to null
     * (in which will throw RuntimeException)
     * @param map React-Native ReadableMap
     * @param key The key to be checked for
     * @return true if the map has a given key and the value is not null
     */
    private boolean canGetValueForKey(ReadableMap map, String key) {
        return map.hasKey(key) && !map.isNull(key);
    }
}
