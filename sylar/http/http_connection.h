#ifndef __SYLAR_HTTP_CONNECTION_H__
#define __SYLAR_HTTP_CONNECTION_H__

#include "../socket_stream.h"
#include "../thread.h"
#include "../uri.h"
#include "http_parser.h"
#include "http.h"

#include <memory>
#include <vector>
#include <list>
#include <stdint.h>

namespace sylar
{
    namespace http
    {
        struct HttpResult
        {
            typedef std::shared_ptr<HttpResult> ptr;

            enum class Error
            {
                OK = 0,
                INVALID_URL = 1,
                INVALID_HOST = 2,
                CONNECT_FAIL = 3,
                SEND_CLOSE_BY_PEER = 4,
                SEND_SOCKET_ERROR = 5,
                TIMEOUT = 6,
                CREATE_SOCKET_ERROR = 7,
                POOL_GET_CONNECTION = 8,
                POOL_INVALID_CONNECTION = 9,
            };

            HttpResult(int _result, HttpResponse::ptr _response, const std::string &_error)
                : result(_result), response(_response), error(_error) {}

            std::string toString() const;

            int result;
            HttpResponse::ptr response;
            std::string error;
        };

        class HttpConnectionPool;

        class HttpConnection : public SocketStream
        {
            friend class HttpConnectionPool;

        public:
            typedef std::shared_ptr<HttpConnection> ptr;

            static HttpResult::ptr DoGet(const std::string &url,
                                         uint64_t timeout_ms,
                                         const std::map<std::string, std::string> &headers = {},
                                         const std::string &body = "");

            static HttpResult::ptr DoGet(Uri::ptr uri,
                                         uint64_t timeout_ms,
                                         const std::map<std::string, std::string> &headers = {},
                                         const std::string &body = "");

            static HttpResult::ptr DoPost(const std::string &url,
                                          uint64_t timeout_ms,
                                          const std::map<std::string, std::string> &headers = {},
                                          const std::string &body = "");

            static HttpResult::ptr DoPost(Uri::ptr uri,
                                          uint64_t timeout_ms,
                                          const std::map<std::string, std::string> &headers = {},
                                          const std::string &body = "");

            static HttpResult::ptr DoRequest(HttpMethod Method,
                                             const std::string &url,
                                             uint64_t timeout_ms,
                                             const std::map<std::string, std::string> &headers = {},
                                             const std::string &body = "");

            static HttpResult::ptr DoRequest(HttpMethod Method,
                                             Uri::ptr uri,
                                             uint64_t timeout_ms,
                                             const std::map<std::string, std::string> &headers = {},
                                             const std::string &body = "");

            static HttpResult::ptr DoRequest(HttpRequest::ptr req,
                                             Uri::ptr uri,
                                             uint64_t timeout_ms);

            HttpConnection(Socket::ptr sock, bool owner = true);
            ~HttpConnection();
            HttpResponse::ptr recvResponse();
            int sendRequest(HttpRequest::ptr rsp);

        private:
            uint64_t m_createTime = 0;
            uint64_t m_request = 0;
        };

        class HttpConnectionPool
        {
        public:
            typedef std::shared_ptr<HttpConnectionPool> ptr;
            typedef Mutex MutexType;

            HttpConnectionPool(const std::string &host,
                               const std::string &vhost,
                               uint32_t port,
                               uint32_t max_size,
                               uint32_t max_alive_time,
                               uint32_t max_request);

            HttpConnection::ptr getConnection();

            HttpResult::ptr doGet(const std::string &url,
                                  uint64_t timeout_ms,
                                  const std::map<std::string, std::string> &headers = {},
                                  const std::string &body = "");

            HttpResult::ptr doGet(Uri::ptr uri,
                                  uint64_t timeout_ms,
                                  const std::map<std::string, std::string> &headers = {},
                                  const std::string &body = "");

            HttpResult::ptr doPost(const std::string &url,
                                   uint64_t timeout_ms,
                                   const std::map<std::string, std::string> &headers = {},
                                   const std::string &body = "");

            HttpResult::ptr doPost(Uri::ptr uri,
                                   uint64_t timeout_ms,
                                   const std::map<std::string, std::string> &headers = {},
                                   const std::string &body = "");

            HttpResult::ptr doRequest(HttpMethod Method,
                                      const std::string &url,
                                      uint64_t timeout_ms,
                                      const std::map<std::string, std::string> &headers = {},
                                      const std::string &body = "");

            HttpResult::ptr doRequest(HttpMethod Method,
                                      Uri::ptr uri,
                                      uint64_t timeout_ms,
                                      const std::map<std::string, std::string> &headers = {},
                                      const std::string &body = "");

            HttpResult::ptr doRequest(HttpRequest::ptr req,
                                      uint64_t timeout_ms);

        private:
            static void ReleasePtr(HttpConnection *ptr, HttpConnectionPool *pool);

        private:
            std::string m_host;
            std::string m_vhost;
            uint32_t m_port;
            uint32_t m_maxSize;
            uint32_t m_maxAliveTime;
            uint32_t m_maxRequest;

            MutexType m_mutex;
            std::list<HttpConnection *> m_conns;
            std::atomic<int32_t> m_total = {0};
        };
    }
}

#endif