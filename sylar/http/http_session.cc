#include "http_session.h"

namespace sylar
{
    namespace http
    {
        HttpSession::HttpSession(Socket::ptr sock, bool owner)
            : SocketStream(sock, owner) {}

        HttpRequest::ptr HttpSession::recvRequest()
        {
            HttpRequestParser::ptr parser(new HttpRequestParser);

            uint64_t buffer_size = HttpRequestParser::GetHttpRequestBufferSize();
            // uint64_t buffer_size = 100;
            std::shared_ptr<char> buffer(new char[buffer_size], [](char *ptr)
                                         { delete[] ptr; });
            char *data = buffer.get();
            int offset = 0;
            do
            {
                int len = read(data + offset, buffer_size - offset);
                if (len <= 0)
                {
                    close();
                    return nullptr;
                }
                len += offset;
                size_t nparser = parser->execute(data, len);
                if (parser->hasError())
                {
                    close();
                    return nullptr;
                }
                offset = len - nparser;
                if (offset == (int)buffer_size)
                {
                    close();
                    return nullptr;
                }
                if (parser->isFinished())
                {
                    break;
                }
            } while (true);
            int64_t length = parser->getContentLength();
            if (length > 0)
            {
                std::string body;
                body.resize(length);

                int len = 0;
                if (length >= offset)
                {
                    memcpy(&body[0], data, offset);
                    body.append(data, offset);
                }
                else
                {
                    memcpy(&body[0], data, length);
                    body.append(data, length);
                }
                length -= offset;
                if (length > 0)
                {
                    if (readFixSize(&body[len], length) <= 0)
                    {
                        close();
                        return nullptr;
                    }
                }
                parser->getData()->setBody(body);
            }
            return parser->getData();
        }

        int HttpSession::sendResponse(HttpResponse::ptr rsp)
        {
            std::stringstream ss;
            ss << *rsp;
            std::string data = ss.str();
            return writeFixSize(data.c_str(), data.size());
        }
    }
}