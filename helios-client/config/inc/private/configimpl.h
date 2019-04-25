#include <map>

#include "config.h"

/**
 * @brief ConfigImpl
 * @brief Private implementation of Config
 */
class ConfigImpl : public Config
{
public:
    /**
     * @brief Constructor
     */
    ConfigImpl();

public:  // from Config
    std::any get(const std::string& key) const override;

private:
    /**
     * @brief Registry which holds all the config values indexed by their keys
     */
    std::map<std::string, std::any> m_valuesRegistry;
};
