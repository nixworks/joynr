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
package joynr;

import java.util.List;

import io.joynr.exceptions.JoynrRuntimeException;

public class SubscriptionPublication extends AbstractPublication {

    private static final long serialVersionUID = 1L;

    private String subscriptionId;

    public SubscriptionPublication() {
    }

    public SubscriptionPublication(List<? extends Object> response, String subscriptionId) {
        super(response);
        this.subscriptionId = subscriptionId;
    }

    public SubscriptionPublication(JoynrRuntimeException error, String subscriptionId) {
        super(error);
        this.subscriptionId = subscriptionId;
    }

    public String getSubscriptionId() {
        return subscriptionId;
    }

    public void setSubscriptionId(String subscriptionId) {
        this.subscriptionId = subscriptionId;
    }

    @Override
    public String toString() {
        return "SubscriptionPublication [" + "subscriptionId=" + subscriptionId + ", "
                + (getResponse() != null ? "response=" + getResponse() + ", " : "")
                + (getError() != null ? "error=" + getError() : "") + "]";
    }

    @Override
    public boolean equals(Object o) {
        if (this == o)
            return true;
        if (o == null || getClass() != o.getClass())
            return false;
        if (!super.equals(o))
            return false;

        SubscriptionPublication that = (SubscriptionPublication) o;

        return subscriptionId.equals(that.subscriptionId);

    }

    @Override
    public int hashCode() {
        final int prime = 31;
        int result = super.hashCode();
        result = prime * result + subscriptionId.hashCode();
        return result;
    }
}
