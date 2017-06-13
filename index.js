import { NativeModules } from 'react-native';

const { RNEvergage } = NativeModules;

const ReactNativeEvergage = {
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
     * Track item given an item object
     * @param {{
     *     id: string,
     *     alternateId: string,
     *     description: string,
     *     currency: string,
     *     inventoryCount: int,
     *     listPrice: float,
     *     price: float,
     *     priceDescription: string
     * }} item
     */
	trackItem: function trackItem(item) {
		RNEvergage.trackItem(item);
	}
};

export default ReactNativeEvergage;
