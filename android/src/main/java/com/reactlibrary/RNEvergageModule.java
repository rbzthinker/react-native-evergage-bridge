
package com.reactlibrary;

import android.os.Handler;
import android.os.Looper;

import com.evergage.android.Evergage;
import com.evergage.android.Screen;
import com.evergage.android.promote.Item;
import com.evergage.android.promote.Product;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactMethod;
import com.facebook.react.bridge.Callback;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.bridge.UiThreadUtil;

public class RNEvergageModule extends ReactContextBaseJavaModule {
    private static final String TAG = "RNEvergageModule";

    private final ReactApplicationContext reactContext;

    public RNEvergageModule(ReactApplicationContext reactContext) {
        super(reactContext);
        this.reactContext = reactContext;
    }

    /**
     * This method (and getScreenForActivity) must be called from main thread
     * Use UiThreadUtil.runOnUiThread for this to avoid calling runnable from dead thread.
     */
    private Screen getScreen() {
        Screen screen = Evergage.getInstance().getScreenForActivity(this.getCurrentActivity());

        if (null != screen) {
            return screen;
        } else {
            android.util.Log.w(TAG, "Evergage screen is empty - is Evergage initialized?");
            return null;
        }
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
                Screen screen = getScreen();
                if (null != screen) {
                    screen.trackAction(action);
                }
            }
        });
    }

    @ReactMethod
    public void trackItem(final ReadableMap itemProperty) {
        UiThreadUtil.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Screen screen = getScreen();
                if (null != screen) {
                    Product item = new Product(itemProperty.getString("id"));
                    item.alternateId = itemProperty.getString("alternateId");
                    item.description = itemProperty.getString("description");
                    item.currency = itemProperty.getString("currency");
                    item.inventoryCount = itemProperty.getInt("inventoryCount");
                    item.listPrice = itemProperty.getDouble("listPrice");
                    item.price = itemProperty.getDouble("price");
                    item.priceDescription = itemProperty.getString("priceDescription");

                    screen.viewItem(item);
                }
            }
        });
    }

    @Override
    public String getName() {
        return "RNEvergage";
    }
}