#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDir>
#include <memory>

#include "paths.h"
#include "qtmodels.h"
#include "controllers.h"
#include "single.h"
#include "dependencyinjector.h"
#include "userserviceimpl.h"
#include "fileserviceimpl.h"
#include "userapicallerimpl.h"
#include "fileapicallerimpl.h"
#include "settingsmanagerimpl.h"
#include "defaultsettingsproviderimpl.h"
#include "httprequestmanagerimpl.h"
#include "configimpl.h"
#include "httprequestfactoryimpl.h"

void registerInstances()
{
    std::shared_ptr<SettingsManager> settingsManager(
        new SettingsManagerImpl(std::make_unique<DefaultSettingsProviderImpl>()));
    Single<DependencyInjector>::instance().registerInstance<SettingsManager>(settingsManager);

    std::shared_ptr<Config> config(new ConfigImpl());
    Single<DependencyInjector>::instance().registerInstance<Config>(config);

    std::shared_ptr<HttpRequestFactory> httpRequestFactory(new HttpRequestFactoryImpl());
    Single<DependencyInjector>::instance().registerInstance<HttpRequestFactory>(httpRequestFactory);

    std::shared_ptr<HttpRequestManager> httpRequestManager(new HttpRequestManagerImpl());
    Single<DependencyInjector>::instance().registerInstance<HttpRequestManager>(httpRequestManager);

    std::shared_ptr<UserService> userService(
        new UserServiceImpl(std::make_unique<UserApiCallerImpl>(httpRequestManager), settingsManager));
    Single<DependencyInjector>::instance().registerInstance<UserService>(userService);

    std::shared_ptr<FileService> fileService(
        new FileServiceImpl(config, std::make_unique<FileApiCallerImpl>(httpRequestManager)));
    Single<DependencyInjector>::instance().registerInstance<FileService>(fileService);
}

int main(int argc, char* argv[])
{
    QtModels::registerQmlTypes("helios");
    Controllers::registerQmlTypes("helios");
    registerInstances();

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QDir(QCoreApplication::applicationDirPath())
                    .filePath(QString::fromStdString(Paths::kQmlPath + Paths::kSeparator + Paths::kMainQmlFile)));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
