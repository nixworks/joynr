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

#include <cstdint>
#include <chrono>
#include <memory>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "tests/JoynrTest.h"

#include "joynr/CcMessageRouter.h"
#include "joynr/ClusterControllerSettings.h"
#include "joynr/LibJoynrMessageRouter.h"

#include "joynr/Semaphore.h"
#include "tests/utils/MockObjects.h"
#include "joynr/system/RoutingTypes/MqttAddress.h"
#include "joynr/system/RoutingTypes/WebSocketAddress.h"
#include "joynr/system/RoutingTypes/WebSocketClientAddress.h"
#include "joynr/MessagingStubFactory.h"
#include "joynr/MessageQueue.h"
#include "joynr/MulticastMessagingSkeletonDirectory.h"
#include "joynr/MqttMulticastAddressCalculator.h"
#include "joynr/SingleThreadedIOService.h"
#include "joynr/WebSocketMulticastAddressCalculator.h"
#include "joynr/Message.h"
#include "joynr/MutableMessage.h"
#include "joynr/ImmutableMessage.h"

using namespace joynr;

template <typename T>
class MessageRouterTest : public ::testing::Test {
public:
    MessageRouterTest() :
        singleThreadedIOService(),
        settings(),
        messagingSettings(settings),
        messageQueue(nullptr),
        messagingStubFactory(nullptr),
        messageRouter(nullptr),
        mutableMessage(),
        multicastMessagingSkeletonDirectory(std::make_shared<MulticastMessagingSkeletonDirectory>()),
        brokerURL("mqtt://globalTransport.example.com"),
        mqttTopic(""),
        localTransport(std::make_shared<const joynr::system::RoutingTypes::WebSocketAddress>(
                         joynr::system::RoutingTypes::WebSocketProtocol::Enum::WS,
                         "host",
                         4242,
                         "path")
                       ),
        webSocketClientAddress(std::make_shared<const joynr::system::RoutingTypes::WebSocketClientAddress>("testWebSocketClientAddress")),
        globalTransport(std::make_shared<const joynr::system::RoutingTypes::MqttAddress>(brokerURL, mqttTopic))
    {
        singleThreadedIOService.start();

        messagingStubFactory = std::make_shared<MockMessagingStubFactory>();
        messageRouter = createMessageRouter();

        JoynrTimePoint now = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
        mutableMessage.setExpiryDate(now + std::chrono::milliseconds(100));
        mutableMessage.setType(Message::VALUE_MESSAGE_TYPE_ONE_WAY());
    }

    ~MessageRouterTest() override {
        std::remove(settingsFileName.c_str());
    }

protected:
    template<typename U = T,
             typename =  std::enable_if_t<std::is_same<U, LibJoynrMessageRouter>::value>>
    std::unique_ptr<LibJoynrMessageRouter> createMessageRouter(std::vector<std::shared_ptr<ITransportStatus>> transportStatuses = {})
    {
        auto messageQueueForMessageRouter = std::make_unique<MessageQueue<std::string>>();
        messageQueue = messageQueueForMessageRouter.get();

        auto transportNotAvailableQueue = std::make_unique<MessageQueue<std::shared_ptr<ITransportStatus>>>();
        transportNotAvailableQueueRef = transportNotAvailableQueue.get();

        auto libJoynrMessageRouter = std::make_unique<LibJoynrMessageRouter>(
                    webSocketClientAddress,
                    messagingStubFactory,
                    singleThreadedIOService.getIOService(),
                    std::make_unique<WebSocketMulticastAddressCalculator>(localTransport),
                    std::move(transportStatuses),
                    6,
                    std::move(messageQueueForMessageRouter),
                    std::move(transportNotAvailableQueue)
                );

        return std::move(libJoynrMessageRouter);
    }

    template<typename U = T,
             typename =  std::enable_if_t<std::is_same<U, CcMessageRouter>::value>>
    std::unique_ptr<CcMessageRouter> createMessageRouter(std::vector<std::shared_ptr<ITransportStatus>> transportStatuses = {})
    {
        const std::string globalCcAddress("globalAddress");
        const std::string messageNotificationProviderParticipantId("messageNotificationProviderParticipantId");
        ClusterControllerSettings ccSettings(settings);

        auto messageQueueForMessageRouter = std::make_unique<MessageQueue<std::string>>();
        messageQueue = messageQueueForMessageRouter.get();

        auto transportNotAvailableQueue = std::make_unique<MessageQueue<std::shared_ptr<ITransportStatus>>>();
        transportNotAvailableQueueRef = transportNotAvailableQueue.get();

        return std::make_unique<CcMessageRouter>(
                    messagingStubFactory,
                    multicastMessagingSkeletonDirectory,
                    std::unique_ptr<IPlatformSecurityManager>(),
                    singleThreadedIOService.getIOService(),
                    std::make_unique<MqttMulticastAddressCalculator>(globalTransport, ccSettings.getMqttMulticastTopicPrefix()),
                    globalCcAddress,
                    messageNotificationProviderParticipantId,
                    std::move(transportStatuses),
                    6,
                    std::move(messageQueueForMessageRouter),
                    std::move(transportNotAvailableQueue)
                );
    }

    SingleThreadedIOService singleThreadedIOService;
    std::string settingsFileName;
    Settings settings;
    MessagingSettings messagingSettings;
    MessageQueue<std::string>* messageQueue;
    MessageQueue<std::shared_ptr<ITransportStatus>>* transportNotAvailableQueueRef;
    std::shared_ptr<MockMessagingStubFactory> messagingStubFactory;

    std::unique_ptr<T> messageRouter;

    MutableMessage mutableMessage;
    std::shared_ptr<MulticastMessagingSkeletonDirectory> multicastMessagingSkeletonDirectory;
    std::string brokerURL;
    std::string mqttTopic;

    std::shared_ptr<const joynr::system::RoutingTypes::WebSocketAddress> localTransport;
    const std::shared_ptr<const joynr::system::RoutingTypes::WebSocketClientAddress> webSocketClientAddress;
    std::shared_ptr<const joynr::system::RoutingTypes::MqttAddress> globalTransport;

    void routeMessageToAddress(){
        joynr::Semaphore semaphore(0);
        std::shared_ptr<ImmutableMessage> immutableMessage = mutableMessage.getImmutableMessage();
        auto mockMessagingStub = std::make_shared<MockMessagingStub>();
        ON_CALL(*messagingStubFactory, create(_)).WillByDefault(Return(mockMessagingStub));
        ON_CALL(*mockMessagingStub, transmit(immutableMessage, A<const std::function<void(const joynr::exceptions::JoynrRuntimeException&)>&>()))
                .WillByDefault(ReleaseSemaphore(&semaphore));
        EXPECT_CALL(*mockMessagingStub, transmit(immutableMessage, A<const std::function<void(const joynr::exceptions::JoynrRuntimeException&)>&>()));
        messageRouter->route(immutableMessage);
        EXPECT_TRUE(semaphore.waitFor(std::chrono::seconds(2)));
    }
};

typedef ::testing::Types<
        LibJoynrMessageRouter,
        CcMessageRouter
    > MessageRouterTypes;

TYPED_TEST_CASE(MessageRouterTest, MessageRouterTypes);
