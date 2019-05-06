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
#include "userapicallerimpl.h"
#include "settingsmanagerimpl.h"
#include "defaultsettingsproviderimpl.h"
#include "httprequestmanagerimpl.h"

void registerInstances()
{
    std::shared_ptr<SettingsManager> settingsManager(
        new SettingsManagerImpl(std::make_unique<DefaultSettingsProviderImpl>()));
    Single<DependencyInjector>::instance().registerInstance<SettingsManager>(settingsManager);

    std::shared_ptr<HttpRequestManager> httpRequestManager(new HttpRequestManagerImpl());

    std::shared_ptr<UserService> userService(
        new UserServiceImpl(std::make_unique<UserApiCallerImpl>(httpRequestManager), settingsManager));

    Single<DependencyInjector>::instance().registerInstance<UserService>(userService);
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
