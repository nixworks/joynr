package io.joynr.integration;

/*
 * #%L
 * %%
 * Copyright (C) 2011 - 2013 BMW Car IT GmbH
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

import io.joynr.arbitration.ArbitrationStrategy;
import io.joynr.arbitration.DiscoveryQos;
import io.joynr.dispatching.subscription.PubSubTestProviderImpl;
import io.joynr.messaging.MessagingPropertyKeys;
import io.joynr.messaging.MessagingQos;
import io.joynr.proxy.ProxyBuilder;
import io.joynr.pubsub.SubscriptionQos;
import io.joynr.pubsub.subscription.AttributeSubscriptionListener;
import io.joynr.runtime.JoynrRuntime;
import joynr.OnChangeSubscriptionQos;
import joynr.PeriodicSubscriptionQos;
import joynr.tests.testProxy;

import org.junit.After;
import org.junit.Before;
import org.junit.Ignore;
import org.junit.Test;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.util.Properties;
import java.util.Timer;
import java.util.TimerTask;
import java.util.UUID;

import static org.mockito.Matchers.anyInt;
import static org.mockito.Matchers.eq;
import static org.mockito.Mockito.*;

/*
 * This testClass registers one consumer and one provider both on the same runtime. It can be used to test local
 * communication.
 */
public abstract class AbstractLocalCommunicationTest {

    private static final Logger logger = LoggerFactory.getLogger(AbstractLocalCommunicationTest.class);
    private JoynrRuntime runtimeA;
    private PubSubTestProviderImpl provider;
    private String domain;
    private testProxy proxy;

    @Mock
    private AttributeSubscriptionListener<Integer> listener;
    private SubscriptionQos subscriptionQos;
    private int lengthInMS = 2000;

    // Overridden by test environment implementations
    protected abstract JoynrRuntime getRuntime(Properties joynrConfig);

    @Before
    public void setUp() throws Exception {
        MockitoAnnotations.initMocks(this);

        logger.info("setup beginning...");

        String channelId = UUID.randomUUID().toString() + "-end2endA";

        Properties customProperties = new Properties();
        customProperties.put(MessagingPropertyKeys.CHANNELID, channelId);
        runtimeA = getRuntime(customProperties);

        provider = new PubSubTestProviderImpl();
        domain = "TestDomain" + System.currentTimeMillis();

        runtimeA.registerProvider(domain, provider);

        ProxyBuilder<testProxy> proxyBuilder;

        MessagingQos messagingQos = new MessagingQos(20000);
        DiscoveryQos discoveryQos = new DiscoveryQos(50000, ArbitrationStrategy.HighestPriority, Long.MAX_VALUE);

        proxyBuilder = runtimeA.getProxyBuilder(domain, testProxy.class);
        proxy = proxyBuilder.setMessagingQos(messagingQos).setDiscoveryQos(discoveryQos).build();

    }

    @After
    public void tearDown() throws InterruptedException {
        runtimeA.shutdown(true);
        Thread.sleep(200);
    }

    // This is a manual test that subscribes for 30 seconds, and checks if all subscriptions arrive. This should also
    // work
    // when connection is lost during subscription. It will most likely not work, when connection is not present during
    // startup.
    @Test
    @Ignore
    public void registerPeriodicSubscriptionAndReceiveUpdatesForLongTime() throws InterruptedException {
        int times = 5;
        final int initialValue = 42;

        int period = lengthInMS / times;
        provider.setATTRIBUTEWITHCAPITALLETTERS(initialValue);
        subscriptionQos = new PeriodicSubscriptionQos(period, // period_ms,
                                                      System.currentTimeMillis() + lengthInMS, // expiryDate
                                                      lengthInMS, // alertInterval_ms,
                                                      lengthInMS / 4 // publicationTtl_ms
        );

        proxy.subscribeToATTRIBUTEWITHCAPITALLETTERS(listener, subscriptionQos);
        new Timer().scheduleAtFixedRate(new TimerTask() {
            int value = initialValue;

            @Override
            public void run() {
                value++;
                provider.setATTRIBUTEWITHCAPITALLETTERS(value);
            }
        }, period, period);

        Thread.sleep(lengthInMS);// - (System.currentTimeMillis() - currentTime));
        verify(listener, times(0)).onError();
        // verify(listener, times(times)).receive(anyInt());
        // TODO verify publications shipped correct data
        for (int i = 42; i < 42 + times; i++) {
            verify(listener, times(1)).onReceive(eq(i));
        }
        verifyNoMoreInteractions(listener);
    }

    /* This is a manual test that subscribes for 30 seconds, and checks if all subscriptions arrive. In this case,
     * the test expect publication in case of value change
     */

    @Test
    @Ignore
    public void registerSubscriptionOnChangeAndReceiveUpdatesForLongTime() throws InterruptedException {
        final int times = 5;
        final int initialValue = 42;

        int period = lengthInMS / times;
        provider.ATTRIBUTEWITHCAPITALLETTERSChanged(initialValue);
        subscriptionQos = new OnChangeSubscriptionQos(lengthInMS / 4, System.currentTimeMillis() + lengthInMS, // expiryDate
                                                      lengthInMS / 4);

        new Timer().scheduleAtFixedRate(new TimerTask() {
            int value = initialValue;

            @Override
            public void run() {
                value++;
                if (value < initialValue + times) {
                    provider.ATTRIBUTEWITHCAPITALLETTERSChanged(value);
                }
            }
        }, period, period);
        proxy.subscribeToATTRIBUTEWITHCAPITALLETTERS(listener, subscriptionQos);

        Thread.sleep(lengthInMS + 100);// - (System.currentTimeMillis() - currentTime));
        verify(listener, times(0)).onError();
        verify(listener, times(times)).onReceive(anyInt());
        // TODO verify publications shipped correct data
        for (int i = 42; i < 42 + times; i++) {
            verify(listener, times(1)).onReceive(eq(i));
        }
        verifyNoMoreInteractions(listener);
    }

}
