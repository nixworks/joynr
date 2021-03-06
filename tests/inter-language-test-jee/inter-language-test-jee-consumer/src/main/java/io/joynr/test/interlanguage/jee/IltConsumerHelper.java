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
package io.joynr.test.interlanguage.jee;

import io.joynr.jeeintegration.api.ServiceLocator;
import io.joynr.exceptions.JoynrRuntimeException;

public class IltConsumerHelper {
    private static ThreadLocal<ServiceLocator> serviceLocatorThreadLocal = new ThreadLocal<>();

    public static ServiceLocator getServiceLocator() {
        if (serviceLocatorThreadLocal.get() == null) {
            throw new JoynrRuntimeException("ServiceLocator not set on thread / JUnit and service container threads differ");
        }
        return serviceLocatorThreadLocal.get();
    }

    public static void setServiceLocator(ServiceLocator serviceLocator) {
        serviceLocatorThreadLocal.set(serviceLocator);
    }

    public static void removeServiceLocator() {
        serviceLocatorThreadLocal.remove();
    }
}
