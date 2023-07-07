#include "../sylar/http/http.h"
#include "../sylar/sylar.h"

static sylar::Logger::ptr g_logger = SYLAR_LOG_ROOT();

void test_1()
{
    sylar::http::HttpRequest::ptr req(new sylar::http::HttpRequest);
    req->setHeader("host", "www.sylar.top");
    req->setBody("hello sylar");

    req->dump(std::cout) << std::endl;
}

void test_2()
{
    sylar::http::HttpResponse::ptr rsp(new sylar::http::HttpResponse);
    rsp->setHeader("X-X", "sylar");
    rsp->setStatus((sylar::http::HttpStatus)400);
    rsp->setBody("hello sylar");

    rsp->dump(std::cout) << std::endl;
}

int main(int argc, char **argv)
{
    test_1();
    test_2();
    return 0;
}