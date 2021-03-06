#include <QNetworkAccessManager>
#include <QDebug>
#include <QHttpMultiPart>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QCoreApplication>
#include <cassert>

#include "httprequestmanagerimpl.h"
#include "httpreplylistener.h"
#include "urlencodedrequest.h"
#include "urlencodedrequestprivate.h"
#include "formdatarequest.h"
#include "formdatarequestprivate.h"
#include "typeconversions.h"
#include "customactionevent.h"

const QEvent::Type HttpRequestManagerImpl::SendRequestEventType =
    static_cast<QEvent::Type>(QEvent::registerEventType());

HttpRequestManagerImpl::HttpRequestManagerImpl()
    : m_networkAccessManager(new QNetworkAccessManager())
    , m_lastAssignedRequestId(-1)
{
}

void HttpRequestManagerImpl::post(std::shared_ptr<UrlEncodedRequest> request, const HttpReplyCallback& callback)
{
    auto event = new CustomActionEvent(
        [this, request, callback] {
            auto _request = std::dynamic_pointer_cast<UrlEncodedRequestPrivate>(request);
            assert(_request);

            QNetworkRequest networkRequest(QUrl(QString(_request->url().c_str())));
            networkRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

            collectHeaderValues(_request->header(), networkRequest);

            const auto& urlQuery = _request->urlQuery().query();

            auto reply = m_networkAccessManager->post(networkRequest, urlQuery.toUtf8());
            addPendingReply(reply, callback);
        },
        SendRequestEventType);
    QCoreApplication::postEvent(this, event);
}

void HttpRequestManagerImpl::post(std::shared_ptr<FormDataRequest> request, const HttpReplyCallback& callback)
{
    auto event = new CustomActionEvent(
        [this, request, callback] {
            auto _request = std::dynamic_pointer_cast<FormDataRequestPrivate>(request);
            assert(_request);

            QNetworkRequest networkRequest(QUrl(QString(_request->url().c_str())));

            collectHeaderValues(_request->header(), networkRequest);

            auto reply = m_networkAccessManager->post(networkRequest, _request->multiPart());
            addPendingReply(reply, callback);
        },
        SendRequestEventType);
    QCoreApplication::postEvent(this, event);
}

void HttpRequestManagerImpl::get(std::shared_ptr<UrlEncodedRequest> request, const HttpReplyCallback& callback)
{
    auto event = new CustomActionEvent(
        [this, request, callback] {
            auto _request = std::dynamic_pointer_cast<UrlEncodedRequestPrivate>(request);
            assert(_request);

            auto url = _request->url();
            if (url[url.length() - 1] == '/')
            {
                url = url.substr(0, url.length() - 1);
            }
            url += "?" + _request->urlQuery().query().toStdString();

            QNetworkRequest networkRequest(QUrl(QString(url.c_str())));

            collectHeaderValues(_request->header(), networkRequest);

            auto reply = m_networkAccessManager->get(networkRequest);
            addPendingReply(reply, callback);
        },
        SendRequestEventType);
    QCoreApplication::postEvent(this, event);
}

bool HttpRequestManagerImpl::event(QEvent* event)
{
    if (event && event->type() == SendRequestEventType)
    {
        auto customEvent = static_cast<CustomActionEvent*>(event);
        (*customEvent)();
        return true;
    }
    return QObject::event(event);
}

void HttpRequestManagerImpl::repliedReceived(int id)
{
    auto it = m_pendingReplies.find(id);
    if (it == m_pendingReplies.end())
    {
        qWarning() << "Received reply with unknown request id: " << id;
        return;
    }

    const auto&       reply    = std::get<0>(it->second)->reply();
    HttpReplyCallback callback = std::get<1>(it->second);

    const auto& replyString = reply->readAll().toStdString();

    callback(static_cast<HttpStatus>(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt()),
             std::vector<uint8_t>(replyString.cbegin(), replyString.cend()), reply->error() == QNetworkReply::NoError);
    m_pendingReplies.erase(it);
}

void HttpRequestManagerImpl::addPendingReply(QNetworkReply* reply, const HttpReplyCallback& callback)
{
    auto listener =
        std::make_shared<HttpReplyListener>(++m_lastAssignedRequestId, reply, [this](int id) { repliedReceived(id); });
    m_pendingReplies.emplace(m_lastAssignedRequestId, std::make_tuple(listener, callback));
}

void HttpRequestManagerImpl::collectHeaderValues(const std::map<std::string, std::string>& values,
                                                 QNetworkRequest&                          result)
{
    for (const auto& param : values)
    {
        result.setRawHeader(QByteArray(param.first.data(), safe_integral_cast<int>(param.first.size())),
                            QByteArray(param.second.data(), safe_integral_cast<int>(param.second.size())));
    }
}
