/*
 * #%L
 * %%
 * Copyright (C) 2011 - 2017 BMW Car IT GmbH
 * %%
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * #L%
 */
const Util = require("../../util/Util");
const Typing = require("../../util/Typing");

/**
 * @name SubscriptionPublication
 * @constructor
 *
 * @param {String}
 *            settings.subscriptionId
 * @param {Object}
 *            settings.response
 * @param {Object}
 *            settings.error The exception object in case of publication failure
 */
function SubscriptionPublication(settings) {
    if (settings.response) {
        settings.response = Util.ensureTypedValues(settings.response);
    }

    /**
     * @name SubscriptionPublication#subscriptionId
     * @type String
     */
    this.subscriptionId = settings.subscriptionId;
    /**
     * @name SubscriptionPublication#response
     * @type Object
     */
    this.response = settings.response;
    /**
     * @name SubscriptionPublication#error
     * @type Object
     */
    this.error = settings.error;

    /**
     * The joynr type name
     *
     * @name SubscriptionPublication#_typeName
     * @type String
     */
    Typing.augmentTypeName(this, "joynr");

    return Object.freeze(this);
}

module.exports = SubscriptionPublication;
