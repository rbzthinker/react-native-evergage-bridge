import { NativeModules, NativeAppEventEmitter } from 'react-native';

const { RNEvergage } = NativeModules;

const ReactNativeEvergage = {
    /**
     * Start Evergage with given account and dataset
     * @param {string} account
     * @param {string} dataset
     */
    start: function start(account, dataset) {
        RNEvergage.start(account, dataset);
    },
    /**
     * Set User Id
     * User id is converted to string to avoid iOS native bridging error
     * @param {string} userId
     */
    setUserId: function setUserId(userId) {
        RNEvergage.setUserId(''+userId);
    },
    /**
     * Set User Attribute of a name/value pair for this user.
     * Name and value is converted to string to avoid iOS native bridging error.

     * @param {string} name - the name for the attribute
     * @param {string} value - the value for the attribute
     */
    setUserAttribute: function setUserAttribute(name, value) {
        RNEvergage.setUserAttribute(''+name, ''+value);
    },
    /**
     * Send event name, calls trackAction for currect screen
     * @param {string} event - name of the event (action)
     */
    sendEvent: function sendEvent(event) {
        RNEvergage.trackAction(event);
    },
    /**
     * Send product(item) that user has viewed
     * @param item
     */
    viewProduct: function viewItem(product) {
        RNEvergage.viewProduct(product);
    },
    /**
     * Set Evergage Campaign Handler for given target name,
     * if a campaign handler is called, the callback function would be invoked
     * @param {string} target - target name
     * @param {campaignCallback} callback - callback function to be invoked
     */
    setCampaignHandler: (target, callback) => {
        RNEvergage.setCampaignHandler(target, callback);
    },
    /**
     * JSDoc definition for campaignCallback.
     * @callback campaignCallback
     * @param {string} campaignName - name of the campaign
     * @param {Object.<string, string>} jsonData - JSON data with string-string key value pair
     */
};

export default ReactNativeEvergage;

