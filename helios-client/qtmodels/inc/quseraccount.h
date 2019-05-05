#ifndef QUSERACCOUNT_H
#define QUSERACCOUNT_H

#include <QMetaType>
#include <QString>
#include <memory>

class UserAccount;

/**
 * @brief The UserAccount class
 */
class QUserAccount
{
    Q_GADGET

    /**
     * @property username
     * @brief Identification name of the user account
     */
    Q_PROPERTY(QString username READ username WRITE setUsername)

    /**
     * @property password
     * @brief Authentication password of the user account
     */
    Q_PROPERTY(QString password READ password WRITE setPassword)

public:
    /**
     * @brief Constructor
     */
    QUserAccount();

    /**
     * @brief Constructor
     * @param data - Internal data
     */
    QUserAccount(const std::shared_ptr<UserAccount>& data);

    /**
     * @brief Getter for username
     * @return QString
     */
    QString username() const;

    /**
     * @brief Setter for username
     * @param newVal - const QString&
     */
    void setUsername(const QString& newVal);

    /**
     * @brief Getter for password
     * @return QString
     */
    QString password() const;

    /**
     * @brief Setter for password
     * @param newVal - const QString&
     */
    void setPassword(const QString& newVal);

    /**
     * @brief Returns the internal data pointer
     * @return std::shared_ptr<UserAccount>
     */
    std::shared_ptr<UserAccount> data();

private:
    /**
     * @brief Internal data
     */
    std::shared_ptr<UserAccount> m_data;
};

Q_DECLARE_METATYPE(QUserAccount)

#endif  // QUSERACCOUNT_H