package io.joynr.joynrandroidruntime;

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

import android.content.Context;
import android.os.AsyncTask;
import android.util.Log;
import com.google.inject.Injector;
import com.google.inject.Module;
import io.joynr.joynrandroidruntime.messaging.AndroidLongPollingMessagingModule;
import io.joynr.messaging.ConfigurableMessagingSettings;
import io.joynr.messaging.MessagingPropertyKeys;
import io.joynr.runtime.InProcessRuntime;
import io.joynr.runtime.JoynrInjectorFactory;
import io.joynr.runtime.JoynrRuntime;
import io.joynr.runtime.JoynrRuntimeImpl;
import io.joynr.runtime.PropertyLoader;

import java.io.File;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Properties;

public class InitRuntimeTask extends AsyncTask<Object, String, JoynrRuntime> {

    public static final long INIT_TIMEOUT = 30000;
    private UILogger uiLogger;
    private Context applicationContext;
    private Properties joynrConfig;
    private List<Module> modules;

    public InitRuntimeTask(Context applicationContext, UILogger uiLogger) {
        this(PropertyLoader.loadProperties("res/raw/demo.properties"),
             applicationContext,
             uiLogger,
             Collections.<Module> emptyList());
    }

    public InitRuntimeTask(Properties joynrConfig,
                           Context applicationContext,
                           UILogger uiLogger,
                           List<Module> joynrModules) {
        this.joynrConfig = joynrConfig;
        this.applicationContext = applicationContext;
        this.uiLogger = uiLogger;

        // Make note of custom modules and add what is needed for Android long polling
        this.modules = new ArrayList<Module>();
        this.modules.add(new AndroidLongPollingMessagingModule());
        this.modules.addAll(joynrModules);
    }

    @Override
    protected JoynrRuntime doInBackground(Object... params) {
        try {
            Log.d("JAS", "starting joynr runtime");
            publishProgress("Starting joynr runtime...\n");

            // create/make persistence file absolute
            File appWorkingDir = applicationContext.getFilesDir();
            String persistenceFileName = appWorkingDir.getPath()
                    + File.separator
                    + joynrConfig.getProperty(MessagingPropertyKeys.PERSISTENCE_FILE,
                                              MessagingPropertyKeys.DEFAULT_PERSISTENCE_FILE);
            joynrConfig.setProperty(MessagingPropertyKeys.PERSISTENCE_FILE, persistenceFileName);

            // create/make participant ID persistence file absolute
            String participantIdPersistenceFileName = appWorkingDir.getPath()
                    + File.separator
                    + joynrConfig.getProperty(ConfigurableMessagingSettings.PROPERTY_PARTICIPANTIDS_PERSISISTENCE_FILE,
                                              ConfigurableMessagingSettings.DEFAULT_PARTICIPANTIDS_PERSISTENCE_FILE);
            joynrConfig.setProperty(ConfigurableMessagingSettings.PROPERTY_PARTICIPANTIDS_PERSISISTENCE_FILE,
                                    participantIdPersistenceFileName);

            publishProgress("Properties loaded\n");

            joynrConfig.setProperty(ConfigurableMessagingSettings.PROPERTY_DISCOVERY_REQUEST_TIMEOUT, "120000");

            // Create an injector with all the required custom modules
            Module[] moduleArray = modules.toArray(new Module[modules.size()]);
            Injector injectorA = new JoynrInjectorFactory(joynrConfig, moduleArray).createChildInjector();

            JoynrRuntimeImpl runtime = injectorA.getInstance(InProcessRuntime.class);
            if (runtime != null) {
                Log.d("JAS", "joynr runtime started");
            } else {
                Log.e("JAS", "joynr runtime not started");
            }
            publishProgress("joynr runtime started.\n");

            return runtime;

        } catch (Exception e) {
            e.printStackTrace();
            publishProgress(e.getMessage());
        }
        return null;
    }

    protected void onProgressUpdate(String... progress) {
        uiLogger.logText(progress);
    }

};
