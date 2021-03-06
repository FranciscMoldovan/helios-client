#ifndef APICALLVISITOR_H
#define APICALLVISITOR_H

#include <memory>

class HttpRequestManager;
class RegisterUserCall;
class LoginCall;
class LogoutCall;
class CheckTokenCall;
class CreateDirectoryCall;
class BeginUploadCall;
class UploadCall;
class BeginDownloadCall;
class DownloadCall;
class EndTransferCall;
class GetFileSizeCall;
class ListCall;
class RemoveCall;
class MoveCall;
class IsDirCall;

/**
 * @class ApiCallVisitor
 * @brief Visits API calls, redirecting them to HttpRequestManager
 */
class ApiCallVisitor
{
public:
    /**
     * @brief Constructor
     * @param requestManager - HttpRequestManager
     */
    ApiCallVisitor(const std::weak_ptr<HttpRequestManager>& requestManager);

    /**
     * @brief Visit RegisterUserCall
     * @param call - RegisterUserCall
     */
    void visit(RegisterUserCall* call) const;

    /**
     * @brief Visit LoginCall
     * @param call - LoginCall
     */
    void visit(LoginCall* call) const;

    /**
     * @brief Visit LogoutCall
     * @param call - LogoutCall
     */
    void visit(LogoutCall* call) const;

    /**
     * @brief Visit CreateDirectoryCall
     * @param call - CreateDirectoryCall
     */
    void visit(CreateDirectoryCall* call) const;

    /**
     * @brief Visit BeginUploadCall
     * @param call - BeginUploadCall
     */
    void visit(BeginUploadCall* call) const;

    /**
     * @brief Visit UploadCall
     * @param call - UploadCall
     */
    void visit(UploadCall* call) const;

    /**
     * @brief Visit BeginDownloadCall
     * @param call - BeginDownloadCall
     */
    void visit(BeginDownloadCall* call) const;

    /**
     * @brief Visit DownloadCall
     * @param call - DownloadCall
     */
    void visit(DownloadCall* call) const;

    /**
     * @brief Visit EndTransferCall
     * @param call - EndTransferCall
     */
    void visit(EndTransferCall* call) const;

    /**
     * @brief Visit GetFileSizeCall
     * @param call - GetFileSizeCall
     */
    void visit(GetFileSizeCall* call) const;

    /**
     * @brief Visit ListCall
     * @param call - ListCall
     */
    void visit(ListCall* call) const;

    /**
     * @brief Visit RemoveCall
     * @param call - RemoveCall
     */
    void visit(RemoveCall* call) const;

    /**
     * @brief Visit MoveCall
     * @param call - MoveCall
     */
    void visit(MoveCall* call) const;

    /**
     * @brief Visit CheckTokenCall
     * @param call - CheckTokenCall
     */
    void visit(CheckTokenCall* call) const;

    /**
     * @brief Visit IsDirCall
     * @param call - IsDirCall
     */
    void visit(IsDirCall* call) const;

private:
    /**
     * @brief Handle calls that encapsulate post requests
     * @param call - Generic call
     * @tparam Call - Call type
     */
    template <typename Call>
    void handlePost(Call* call) const;

    /**
     * @brief Handle calls that encapsulate get requests
     * @param call - Generic call
     * @tparam Call - Call type
     */
    template <typename Call>
    void handleGet(Call* call) const;

private:
    /**
     * @brief Http request manager
     */
    std::weak_ptr<HttpRequestManager> m_requestManager;
};

#include "apicallvisitor.tpp"

#endif  // APICALLVISITOR_H
