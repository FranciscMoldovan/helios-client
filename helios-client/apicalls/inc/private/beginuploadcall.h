#ifndef BEGINUPLOADCALL_H
#define BEGINUPLOADCALL_H

#include <string>
#include <vector>

#include "apicall.h"
#include "apicalldefs.h"
#include "urlencodedrequest.h"

/**
 * @class BeginUploadCall
 * @brief Manages a begin upload call
 */
class BeginUploadCall : public ApiCall
{
public:
    BeginUploadCall(const std::string& authToken, const std::string& path,
                    const ApiCallbacks::BeginUploadCallback& callback);

public:
    /**
     * @brief Returns the http request
     * @return std::shared_ptr<UrlEncodedRequest>
     */
    std::shared_ptr<UrlEncodedRequest> request();

public:  // from ApiCall
    void send(std::shared_ptr<ApiCallVisitor> visitor) override;
    void receive(HttpStatus status, const std::vector<uint8_t>& reply) override;

private:
    /**
     * @brief Http request
     */
    std::shared_ptr<UrlEncodedRequest> m_request;

    /**
     * @brief Callback
     */
    ApiCallbacks::BeginUploadCallback m_callback;

    /**
     * @brief Request url
     */
    static const std::string s_kUrl;

    /**
     * @brief Authentication token header parameter name
     */
    static const std::string s_kTokenParam;

    /**
     * @brief File path parameter name
     */
    static const std::string s_kPathParam;

    /**
     * @brief Invalid path error message
     */
    static const std::string s_kErrorInvalidPath;
};

#endif  // BEGINUPLOADCALL_H
