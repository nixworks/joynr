/*
 * #%L
 * %%
 * Copyright (C) 2017 BMW Car IT GmbH
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
#ifndef TESTS_MOCK_MOCKLOCALCAPABILITIESDIRECTORY_H
#define TESTS_MOCK_MOCKLOCALCAPABILITIESDIRECTORY_H

#include <gmock/gmock.h>

#include "joynr/LocalCapabilitiesDirectory.h"

class MockLocalCapabilitiesDirectory : public joynr::LocalCapabilitiesDirectory {
public:
    MockLocalCapabilitiesDirectory(joynr::ClusterControllerSettings& ccSettings, std::shared_ptr<joynr::IMessageRouter> mockMessageRouter, boost::asio::io_service& ioService):
        LocalCapabilitiesDirectory(ccSettings, nullptr, "localAddress", mockMessageRouter, ioService, "clusterControllerId")
   {}

    MOCK_METHOD3(
            lookup,
            void(
                const std::string& participantId,
                std::function<void(const joynr::types::DiscoveryEntryWithMetaInfo&)> onSuccess,
                std::function<void(const joynr::exceptions::ProviderRuntimeException&)> onError
            ));
};

#endif // TESTS_MOCK_MOCKLOCALCAPABILITIESDIRECTORY_H
