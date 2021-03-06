#include <QVariant>
#include <QString>
#include <algorithm>

#include "userserviceimpl.h"
#include "userapicaller.h"
#include "settingsmanager.h"
#include "userservicelistener.h"
#include "useraccount.h"

namespace SettingsKeys
{
/**
 * @brief Username persistence setting key
 */
const std::string kUsername = "username";
/**
 * @brief Authentication token pesistence setting key
 */
const std::string kAuthToken = "token";
}  // namespace SettingsKeys

UserServiceImpl::UserServiceImpl(std::unique_ptr<UserApiCaller>          userApiCaller,
                                 const std::shared_ptr<SettingsManager>& settingsManager)
    : m_apiCaller(std::move(userApiCaller))
    , m_settingsManager(settingsManager)
{
}

const UserSession& UserServiceImpl::session() const
{
    return m_session;
}

bool UserServiceImpl::loggedIn() const
{
    return m_session.valid();
}

void UserServiceImpl::login(const UserAccount& account, bool persist)
{
    m_apiCaller->login(account.username(), account.password(),
                       [this, account, persist](ApiCallStatus status, const std::string& authToken) {
                           handleLoggedIn(status, UserSession(account.username(), authToken), persist);
                       });
}

void UserServiceImpl::restoreSession()
{
    auto varUsername = m_settingsManager->get(SettingsKeys::kUsername);
    auto varToken    = m_settingsManager->get(SettingsKeys::kAuthToken);

    if (varUsername.type() == QVariant::String && varToken.type() == QVariant::String)
    {
        auto        username = varUsername.toString().toStdString();
        auto        token    = varToken.toString().toStdString();
        UserSession session(username, token);
        m_apiCaller->checkToken(username, token,
                                [this, session](ApiCallStatus status) { handleCheckToken(status, session); });
    }
}

void UserServiceImpl::logout()
{
    if (!m_session.valid())
    {
        return;
    }

    m_apiCaller->logout(m_session.authToken(), [this](ApiCallStatus status) { handleLoggedOut(status); });
}

void UserServiceImpl::createUser(const UserAccount& account)
{
    if (!account.valid())
    {
        return;
    }

    m_apiCaller->registerUser(account.username(), account.password(),
                              [this](ApiCallStatus status) { handleUserCreated(status); });
}

void UserServiceImpl::handleLoggedIn(ApiCallStatus status, const UserSession& session, bool persist)
{
    bool        success = false;
    std::string errorString;
    switch (status)
    {
        case ApiCallStatus::SUCCESS:
        {
            m_session = session;
            success   = true;
            break;
        }
        case ApiCallStatus::INVALID_USERNAME:
        {
            errorString = "Invalid username";
            break;
        }
        case ApiCallStatus::INVALID_PASSWORD:
        {
            errorString = "Invalid password";
            break;
        }
        default:
        {
            errorString = "Unknown error";
            break;
        }
    }

    Observable::notifyAll(&UserServiceListener::loginCompleted, success, errorString);

    if (persist)
    {
        m_settingsManager->set(SettingsKeys::kUsername, QString::fromStdString(session.username()));
        m_settingsManager->set(SettingsKeys::kAuthToken, QString::fromStdString(session.authToken()));
        m_settingsManager->save();
    }
}

void UserServiceImpl::handleLoggedOut(ApiCallStatus status)
{
    bool        success = false;
    std::string errorString;
    if (status == ApiCallStatus::SUCCESS)
    {
        success = true;
        m_settingsManager->reset(SettingsKeys::kUsername);
        m_settingsManager->reset(SettingsKeys::kAuthToken);
        m_settingsManager->save();
        m_session.clear();
    }
    else
    {
        errorString = (status == ApiCallStatus::UNAUTHORIZED) ? "Invalid token" : "Unknown error";
    }

    Observable::notifyAll(&UserServiceListener::logoutCompleted, success, errorString);
}

void UserServiceImpl::handleUserCreated(ApiCallStatus status)
{
    bool        success = false;
    std::string errorString;
    switch (status)
    {
        case ApiCallStatus::SUCCESS:
        {
            success = true;
            break;
        }
        case ApiCallStatus::USERNAME_ALREADY_EXISTS:
        {
            errorString = "This username already exists";
            break;
        }
        case ApiCallStatus::INVALID_USERNAME:
        {
            errorString = "Invalid username";
            break;
        }
        case ApiCallStatus::INVALID_PASSWORD:
        {
            errorString = "Invalid password";
            break;
        }
        default:
        {
            errorString = "Unknown error";
            break;
        }
    }

    Observable::notifyAll(&UserServiceListener::userCreationCompleted, success, errorString);
}

void UserServiceImpl::handleCheckToken(ApiCallStatus status, const UserSession& session)
{
    if (status == ApiCallStatus::SUCCESS)
    {
        if (session.valid())
        {
            m_session = session;
            Observable::notifyAll(&UserServiceListener::loginCompleted, true, std::string());
        }
    }
    else
    {
        m_settingsManager->reset(SettingsKeys::kUsername);
        m_settingsManager->reset(SettingsKeys::kAuthToken);
        m_settingsManager->save();
    }
}
