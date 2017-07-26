
package com.reactlibrary;

import android.support.annotation.NonNull;

import com.evergage.android.Campaign;
import com.evergage.android.CampaignHandler;
import com.evergage.android.Context;
import com.evergage.android.Evergage;
import com.evergage.android.promote.Product;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactMethod;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.bridge.UiThreadUtil;
import com.facebook.react.bridge.WritableMap;
import com.facebook.react.modules.core.RCTNativeAppEventEmitter;

import org.json.JSONException;
import org.json.JSONObject;

import java.util.HashMap;
import java.util.Map;

public class RNEvergageModule extends ReactContextBaseJavaModule {
    private static final String TAG = "RNEvergageModule";
    private static final String EVERGAGE_CAMPAIGN_EVENT_SUFFIX = "EvergageCampaignHandler-"; //If you change this, remember to also change it in index.js

    /**
     * Map with campaign target name as key and campaign handler as value
     * key: campaign Target
     */
    private Map<String, CampaignHandler> campaignHandlers = new HashMap<>();

    private final ReactApplicationContext reactContext;

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
                    product.price = productMap.getDouble("price");
                    product.listPrice = productMap.getDouble("retailPrice");
                    product.inventoryCount = productMap.getInt("stock");
                    product.alternateId = productMap.getString("sku");

                    //The following are required fields for campaign data to train correctly
                    product.imageUrl = productMap.getString("imageUrl");
                    product.url = productMap.getString("url");
                    product.name = productMap.getString("name");

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
                        JSONObject data = campaign.getData();
                        try {
                            WritableMap map = JsonConvert.jsonToReact(data);

                            //Call JS Callback on frontend with campaign name and JSON data
                            //React-native's Callback object can only be invoked ONCE, so we use RCTNativeAppEventEmitter instead
                            RCTNativeAppEventEmitter eventEmitter = getReactApplicationContext().getJSModule(RCTNativeAppEventEmitter.class);
                            eventEmitter.emit(EVERGAGE_CAMPAIGN_EVENT_SUFFIX + target, map);
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
}