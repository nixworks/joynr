FROM payaradocker/payaraserver:4.1.1.161.1

COPY target/discovery-directory-jee.war /discovery-directory-jee.war
COPY target/domain-access-controller-jee.war /domain-access-controller-jee.war
COPY start-me-up.sh /start-me-up.sh

ENV PATH ${PATH}:/opt/payara41/glassfish/bin
ENV joynr_messaging_mqtt_brokeruri tcp://mqttbroker:1883
ENV joynr_servlet_hostpath http://joynrbackend:8080
ENV joynr_messaging_discoverydirectoryurl tcp://mqttbroker:1883
ENV joynr_messaging_domainaccesscontrollerurl tcp://mqttbroker:1883

RUN echo 'AS_ADMIN_PASSWORD=glassfish\n\
EOF\n' \
>> /tmp/pwdfile
RUN asadmin start-domain && \
	asadmin --user admin --passwordfile=/tmp/pwdfile create-managed-scheduled-executor-service --corepoolsize=100 concurrent/joynrMessagingScheduledExecutor && \
	asadmin --user admin --passwordfile=/tmp/pwdfile create-jdbc-connection-pool --datasourceclassname org.apache.derby.jdbc.ClientDataSource --restype javax.sql.XADataSource --property portNumber=1527:password=APP:user=APP:serverName=localhost:databaseName=joynr-discovery-directory:connectionAttributes=\;create\\=true JoynrPool && \
	asadmin --user admin --passwordfile=/tmp/pwdfile create-jdbc-resource --connectionpoolid JoynrPool joynr/DiscoveryDirectoryDS && \
	asadmin --user admin --passwordfile=/tmp/pwdfile create-jdbc-resource --connectionpoolid JoynrPool joynr/DomainAccessControllerDS && \
	asadmin --user admin --passwordfile=/tmp/pwdfile start-database --jvmoptions="-Dderby.storage.useDefaultFilePermissions=true" && \
	asadmin --user admin --passwordfile=/tmp/pwdfile deploy /discovery-directory-jee.war && \
	asadmin --user admin --passwordfile=/tmp/pwdfile deploy /domain-access-controller-jee.war && \
	asadmin --user admin --passwordfile=/tmp/pwdfile set-log-levels io.joynr.messaging=FINE && \
	asadmin --user admin --passwordfile=/tmp/pwdfile set-log-levels io.joynr.dispatching=FINE && \
	asadmin --user admin --passwordfile=/tmp/pwdfile set-log-levels io.joynr.jeeintegration=FINE || \
	true

ENTRYPOINT ["/start-me-up.sh"]
