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

#ifndef MOCKOBJECTS_H_
#define MOCKOBJECTS_H_

#include <chrono>
#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <tuple>
#include <vector>

#include <boost/any.hpp>
#include <boost/asio.hpp>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <mococrw/key.h>
#include <mococrw/x509.h>

#include <websocketpp/common/connection_hdl.hpp>

#include "joynr/access-control/IAccessController.h"
#include "joynr/DelayedScheduler.h"
#include "joynr/exceptions/JoynrException.h"
#include "joynr/infrastructure/GlobalDomainAccessControllerProxy.h"
#include "joynr/infrastructure/GlobalDomainRoleControllerProxy.h"
#include "joynr/IClusterControllerSignalHandler.h"
#include "joynr/IDispatcher.h"
#include "joynr/IKeychain.h"
#include "joynr/IMessageRouter.h"
#include "joynr/IMessageSender.h"
#include "joynr/IMessagingMulticastSubscriber.h"
#include "joynr/IMessagingStub.h"
#include "joynr/IMessagingStubFactory.h"
#include "joynr/IMqttMessagingSkeleton.h"
#include "joynr/IMulticastAddressCalculator.h"
#include "joynr/ImmutableMessage.h"
#include "joynr/InProcessConnectorFactory.h"
#include "joynr/IProxyBuilder.h"
#include "joynr/IRequestCallerDirectory.h"
#include "joynr/ISubscriptionListener.h"
#include "joynr/ITransportMessageReceiver.h"
#include "joynr/ITransportMessageSender.h"
#include "joynr/IWebSocketSendInterface.h"
#include "joynr/JoynrRuntime.h"
#include "joynr/LocalCapabilitiesDirectory.h"
#include "joynr/Logger.h"
#include "joynr/MulticastSubscriptionRequest.h"
#include "joynr/MessagingQos.h"
#include "joynr/ParticipantIdStorage.h"
#include "joynr/PublicationManager.h"
#include "joynr/ReplyCaller.h"
#include "joynr/Request.h"
#include "joynr/RequestCallerFactory.h"
#include "joynr/Runnable.h"
#include "joynr/Settings.h"
#include "joynr/system/RoutingProxy.h"
#include "joynr/system/RoutingTypes/Address.h"
#include "joynr/system/RoutingTypes/WebSocketAddress.h"
#include "joynr/SubscriptionManager.h"
#include "joynr/SubscriptionStop.h"
#include "joynr/tests/DefaulttestProvider.h"
#include "joynr/tests/TestLocationUpdateSelectiveBroadcastFilter.h"
#include "joynr/tests/testRequestCaller.h"
#include "joynr/types/Version.h"
#include "joynr/vehicle/DefaultGpsProvider.h"
#include "joynr/vehicle/GpsProvider.h"
#include "joynr/vehicle/GpsRequestCaller.h"

#include "libjoynr/in-process/InProcessMessagingSkeleton.h"
#include "libjoynr/websocket/IWebSocketPpClient.h"

#include "libjoynrclustercontroller/access-control/LocalDomainAccessController.h"
#include "libjoynrclustercontroller/capabilities-client/ICapabilitiesClient.h"
#include "libjoynrclustercontroller/http-communication-manager/HttpReceiver.h"
#include "libjoynrclustercontroller/include/joynr/ITransportStatus.h"
#include "libjoynrclustercontroller/mqtt/MosquittoConnection.h"

#include "tests/PrettyPrint.h"
#include "tests/mock/LibJoynrMockObjects.h"

namespace joynr
{
class BroadcastSubscriptionRequest;
class ClusterControllerSettings;
class DiscoveryQos;
class MessagingSettings;
class MulticastPublication;
class MulticastSubscriptionQos;
class OneWayRequest;
class RequestCaller;
class Settings;
class SubscriptionReply;
class WebSocketSettings;

namespace types {
class DiscoveryQos;
} // namespace types
} // namespace joynr

using ::testing::_;
using ::testing::Eq;
using ::testing::Return;

// Disable VC++ warnings due to google mock
// http://code.google.com/p/googlemock/wiki/FrequentlyAskedQuestions#MSVC_gives_me_warning_C4301_or_C4373_when_I_define_a_mock_method
#ifdef _MSC_VER
    #pragma warning( push )
    #pragma warning( disable : 4373 )
#endif

// Disable compiler warnings.
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wreorder"

namespace joynr
{
template <>
inline std::shared_ptr<RequestCaller> RequestCallerFactory::create<MockProvider>(std::shared_ptr<MockProvider> provider)
{
    std::ignore = provider;
    return std::shared_ptr<RequestCaller>(nullptr);
}
} // namespace joynr;

#ifdef _MSC_VER
    #pragma warning( push )
#endif

// restore GCC diagnostic state
#pragma GCC diagnostic pop

#endif // MOCKOBJECTS_H_
