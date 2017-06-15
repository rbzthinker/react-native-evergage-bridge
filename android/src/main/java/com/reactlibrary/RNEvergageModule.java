
package com.reactlibrary;

import com.evergage.android.Context;
import com.evergage.android.Evergage;
import com.evergage.android.Screen;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactMethod;
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
    private Context getScreen() {
        Screen screen = Evergage.getInstance().getScreenForActivity(this.getCurrentActivity());

        if (null != screen) {
            return screen;
        } else {
            android.util.Log.w(TAG, "Evergage screen is empty, is Evergage initialized? Returning global context.");
            return Evergage.getInstance().getGlobalContext();
        }
    }

    @ReactMethod
    public void start(String account, String dataset) {
        Evergage evergage = Evergage.getInstance();
        evergage.reset(); //.reset() needs to be called to override subsequent config for .start()
        evergage.start(account, dataset);
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

    @Override
    public String getName() {
        return "RNEvergage";
    }
}