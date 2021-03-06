#include <QDebug>

#include "userapicallerimpl.h"
#include "single.h"
#include "dependencyinjector.h"
#include "apicallvisitor.h"
#include "registerusercall.h"
#include "logincall.h"
#include "logoutcall.h"
#include "checktokencall.h"

UserApiCallerImpl::UserApiCallerImpl(const std::shared_ptr<HttpRequestManager>& requestManager)
{
    m_visitor = std::make_shared<ApiCallVisitor>(requestManager);
}

void UserApiCallerImpl::registerUser(const std::string& username, const std::string& password,
                                     const ApiCallbacks::RegisterUserCallback& callback) const
{
    auto call = new RegisterUserCall(username, password, callback);
    call->send(m_visitor);
}

void UserApiCallerImpl::login(const std::string& username, const std::string& password,
                              const ApiCallbacks::LoginCallback& callback) const
{
    auto call = new LoginCall(username, password, callback);
    call->send(m_visitor);
}

void UserApiCallerImpl::logout(const std::string& token, const ApiCallbacks::LogoutCallback& callback) const
{
    auto call = new LogoutCall(token, callback);
    call->send(m_visitor);
}

void UserApiCallerImpl::checkToken(const std::string& username, const std::string& token,
                                   const ApiCallbacks::CheckTokenCallback& callback) const
{
    auto call = new CheckTokenCall(username, token, callback);
    call->send(m_visitor);
}
