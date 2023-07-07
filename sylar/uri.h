#ifndef __SYLAR_URI_H__
#define __SYLAR_URI_H__

#include <memory>
#include <string>
#include <stdint.h>

#include "address.h"

namespace sylar
{
    class Uri
    {
    public:
        typedef std::shared_ptr<Uri> ptr;

        static Uri::ptr Create(const std::string &uri);

        Uri();

        const std::string &getScheme() const { return m_scheme; }
        const std::string &getUserinfo() const { return m_userinfo; }
        const std::string &getHost() const { return m_host; }
        const std::string &getPath() const;
        const std::string &getQuery() const { return m_query; }
        const std::string &getFragment() const { return m_fragment; }
        int32_t getPort() const;

        void setScheme(const std::string &val) { m_scheme = val; }
        void setUserinfo(const std::string &val) { m_userinfo = val; }
        void setHost(const std::string &val) { m_host = val; }
        void setPath(const std::string &val) { m_path = val; }
        void setQuery(const std::string &val) { m_query = val; }
        void setFragment(const std::string &val) { m_fragment = val; }
        void setPort(int32_t val) { m_port = val; }

        std::ostream &dump(std::ostream &os) const;
        std::string toString() const;
        Address::ptr createAddress() const;

    private:
        bool isDefaultPort() const;

    private:
        std::string m_scheme;
        std::string m_userinfo;
        std::string m_host;
        std::string m_path;
        std::string m_query;
        std::string m_fragment;
        int32_t m_port;
    };
}

#endif