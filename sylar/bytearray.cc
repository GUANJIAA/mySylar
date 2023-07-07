#include "bytearray.h"
#include "log.h"

#include <string.h>
#include <fstream>
#include <sstream>

namespace sylar
{
    static sylar::Logger::ptr g_logger = SYLAR_LOG_NAME("system");

    ByteArray::Node::Node(size_t s)
        : ptr(new char[s]), next(nullptr), size(s) {}

    ByteArray::Node::Node::Node()
        : ptr(nullptr), next(nullptr), size(0) {}

    ByteArray::Node::~Node()
    {
        if (ptr)
        {
            delete[] ptr;
        }
    }

    ByteArray::ByteArray(size_t base_size)
        : m_baseSize(base_size), m_position(0), m_capacity(base_size),
          m_size(0), m_endian(SYLAR_BIG_ENDIAN), m_root(new Node(base_size)), m_cur(m_root) {}

    ByteArray::~ByteArray()
    {
        Node *tmp = m_root;
        while (tmp)
        {
            m_cur = tmp;
            tmp = tmp->next;
            delete m_cur;
        }
    }

    // write
    void ByteArray::writeFint8(int8_t value)
    {
        write(&value, sizeof(value));
    }

    void ByteArray::writeFuint8(uint8_t value)
    {
        write(&value, sizeof(value));
    }

    void ByteArray::writeFint16(int16_t value)
    {
        if (m_endian != SYLAR_BYTE_ORDER)
        {
            value = byteswap(value);
        }
        write(&value, sizeof(value));
    }
    void ByteArray::writeFuint16(uint16_t value)
    {
        if (m_endian != SYLAR_BYTE_ORDER)
        {
            value = byteswap(value);
        }
        write(&value, sizeof(value));
    }

    void ByteArray::writeFint32(int32_t value)
    {
        if (m_endian != SYLAR_BYTE_ORDER)
        {
            value = byteswap(value);
        }
        write(&value, sizeof(value));
    }

    void ByteArray::writeFuint32(uint32_t value)
    {
        if (m_endian != SYLAR_BYTE_ORDER)
        {
            value = byteswap(value);
        }
        write(&value, sizeof(value));
    }

    void ByteArray::writeFint64(int64_t value)
    {
        if (m_endian != SYLAR_BYTE_ORDER)
        {
            value = byteswap(value);
        }
        write(&value, sizeof(value));
    }

    void ByteArray::writeFuint64(uint64_t value)
    {
        if (m_endian != SYLAR_BYTE_ORDER)
        {
            value = byteswap(value);
        }
        write(&value, sizeof(value));
    }

    static uint32_t EncodeZigzag32(const int32_t &val)
    {
        if (val < 0)
        {
            return ((uint32_t)(-val)) * 2 - 1;
        }
        return val * 2;
    }

    void ByteArray::writeInt32(int32_t value)
    {
        writeUint32(EncodeZigzag32(value));
    }

    void ByteArray::writeUint32(uint32_t value)
    {
        uint8_t temp[5];
        uint8_t i = 0;
        while (value >= 0x80)
        {
            temp[i++] = (value & 0x7f) | 0x80;
            value >>= 7;
        }
        temp[i++] = value;
        write(temp, i);
    }

    static uint64_t EncodeZigzag64(const int64_t &val)
    {
        if (val < 0)
        {
            return ((uint64_t)(-val)) * 2 - 1;
        }
        return val * 2;
    }

    void ByteArray::writeInt64(int64_t value)
    {
        writeUint32(EncodeZigzag32(value));
    }

    void ByteArray::writeUint64(uint64_t value)
    {
        uint8_t temp[10];
        uint8_t i = 0;
        while (value >= 0x80)
        {
            temp[i++] = (value & 0x7f) | 0x80;
            value >>= 7;
        }
        temp[i++] = value;
        write(temp, i);
    }

    void ByteArray::writeFloat(float value)
    {
        uint32_t val;
        memcpy(&val, &value, sizeof(value));
        writeFuint32(val);
    }

    void ByteArray::writedouble(double value)
    {
        uint64_t val;
        memcpy(&val, &value, sizeof(value));
        writeFuint64(val);
    }

    // length:int16, data
    void ByteArray::writeStringF16(const std::string &value)
    {
        writeFuint16(value.size());
        write(value.c_str(), value.size());
    }
    // length:int32, data
    void ByteArray::writeStringF32(const std::string &value)
    {
        writeFuint32(value.size());
        write(value.c_str(), value.size());
    }
    // length:int64, data
    void ByteArray::writeStringF64(const std::string &value)
    {
        writeFuint64(value.size());
        write(value.c_str(), value.size());
    }
    // length:varint, data
    void ByteArray::writeStringVint(const std::string &value)
    {
        writeUint64(value.size());
        write(value.c_str(), value.size());
    }
    // data
    void ByteArray::writeStringWithoutLength(const std::string &value)
    {
        write(value.c_str(), value.size());
    }

    // read
    int8_t ByteArray::readFint8()
    {
        int8_t val;
        read(&val, sizeof(val));
        return val;
    }

    uint8_t ByteArray::readFuint8()
    {
        uint8_t val;
        read(&val, sizeof(val));
        return val;
    }

#define XX(type)                        \
    type val;                           \
    ByteArray::read(&val, sizeof(val)); \
    if (m_endian == SYLAR_BYTE_ORDER)   \
    {                                   \
        return val;                     \
    }                                   \
    else                                \
    {                                   \
        return byteswap(val);           \
    }

    int16_t ByteArray::readFint16()
    {
        // SYLAR_LOG_DEBUG(g_logger) << "test";
        XX(int16_t);
        // SYLAR_LOG_DEBUG(g_logger) << "test";
    }

    uint16_t ByteArray::readFuint16()
    {
        XX(uint16_t);
    }

    int32_t ByteArray::readFint32()
    {
        XX(int32_t);
    }

    uint32_t ByteArray::readFuint32()
    {
        XX(uint32_t);
    }

    int64_t ByteArray::readFint64()
    {
        XX(int64_t);
    }

    uint64_t ByteArray::readFuint64()
    {
        XX(uint64_t);
    }

#undef XX

    static uint32_t DecodeZigzag32(const uint32_t &val)
    {
        return (val >> 1) ^ -(val & 1);
    }

    int32_t ByteArray::readInt32()
    {
        return DecodeZigzag32(readUint32());
    }

    uint32_t ByteArray::readUint32()
    {
        uint32_t result = 0;
        for (int i = 0; i < 32; i += 7)
        {
            uint8_t b = readFuint8();
            if (b < 0x80)
            {
                result |= ((uint32_t)b) << i;
                break;
            }
            else
            {
                result |= (((uint32_t)(b & 0x7f)) << i);
            }
        }
        return result;
    }

    static uint64_t DecodeZigzag64(const uint64_t &val)
    {
        return (val >> 1) ^ -(val & 1);
    }

    int64_t ByteArray::readInt64()
    {
        return DecodeZigzag64(readUint64());
    }

    uint64_t ByteArray::readUint64()
    {
        uint64_t result = 0;
        for (int i = 0; i < 64; i += 7)
        {
            uint8_t b = readFuint8();
            if (b < 0x80)
            {
                result |= ((uint64_t)b) << i;
                break;
            }
            else
            {
                result |= (((uint64_t)(b & 0x7f)) << i);
            }
        }
        return result;
    }

    float ByteArray::readFloat()
    {
        uint32_t val = readFuint32();
        float value;
        memcpy(&value, &val, sizeof(val));
        return value;
    }

    double ByteArray::readDouble()
    {
        uint64_t val = readFuint64();
        double value;
        memcpy(&value, &val, sizeof(val));
        return value;
    }

    // length:int16, data
    std::string ByteArray::readStringF16()
    {
        uint16_t len = readFuint16();
        std::string buff;
        buff.resize(len);
        read(&buff[0], len);
        return buff;
    }

    // length:int32, data
    std::string ByteArray::readStringF32()
    {
        uint32_t len = readFuint32();
        std::string buff;
        buff.resize(len);
        read(&buff[0], len);
        return buff;
    }

    // length:int64, data
    std::string ByteArray::readStringF64()
    {
        uint64_t len = readFuint64();
        std::string buff;
        buff.resize(len);
        read(&buff[0], len);
        return buff;
    }

    // length:varint, data
    std::string ByteArray::readStringVint()
    {
        uint64_t len = readFuint64();
        std::string buff;
        buff.resize(len);
        read(&buff[0], len);
        return buff;
    }

    // 内部操作
    void ByteArray::clear()
    {
        m_position = m_size = 0;
        m_capacity = m_baseSize;
        Node *tmp = m_root->next;
        while (tmp)
        {
            m_cur = tmp;
            tmp = tmp->next;
            delete m_cur;
        }
        m_cur = m_root;
        m_root->next = nullptr;
    }

    void ByteArray::write(const void *buf, size_t size)
    {
        if (size == 0)
        {
            return;
        }
        addCapacity(size);

        size_t npos = m_position % m_baseSize;
        size_t ncap = m_cur->size - npos;
        size_t bpos = 0;

        while (size > 0)
        {
            if (ncap >= size)
            {
                memcpy(m_cur->ptr + npos, (const char *)buf + bpos, size);
                if (m_cur->size == (npos + size))
                {
                    m_cur = m_cur->next;
                }
                m_position += size;
                bpos += size;
                size = 0;
            }
            else
            {
                memcpy(m_cur->ptr + npos, (const char *)buf + bpos, ncap);
                m_position += ncap;
                bpos += ncap;
                size -= ncap;
                m_cur = m_cur->next;
                ncap = m_cur->size;
                npos = 0;
            }
        }

        if (m_position > m_size)
        {
            m_size = m_position;
        }
    }

    void ByteArray::read(void *buf, size_t size)
    {
        if (size > getReadSize())
        {
            throw std::out_of_range("not enough len");
        }

        size_t npos = m_position % m_baseSize;
        size_t ncap = m_cur->size - npos;
        size_t bpos = 0;
        while (size > 0)
        {
            if (ncap >= size)
            {
                memcpy((char *)buf + bpos, m_cur->ptr + npos, size);
                if (m_cur->size == (npos + size))
                {
                    m_cur = m_cur->next;
                }
                m_position += size;
                bpos += size;
                size = 0;
            }
            else
            {
                memcpy((char *)buf + bpos, m_cur->ptr + npos, ncap);
                m_position += ncap;
                bpos += ncap;
                size -= ncap;
                m_cur = m_cur->next;
                ncap = m_cur->size;
                npos = 0;
            }
        }
    }

    void ByteArray::read(void *buf, size_t size, size_t position) const
    {
        if (size > (m_size - position))
        {
            throw std::out_of_range("not enough len");
        }

        size_t npos = position % m_baseSize;
        size_t ncap = m_cur->size - npos;
        size_t bpos = 0;
        Node *cur = m_cur;
        while (size > 0)
        {
            if (ncap >= size)
            {
                memcpy((char *)buf + bpos, cur->ptr + npos, size);
                if (cur->size == (npos + size))
                {
                    cur = cur->next;
                }
                position += size;
                bpos += size;
                size = 0;
            }
            else
            {
                memcpy((char *)buf + bpos, cur->ptr + npos, ncap);
                position += ncap;
                bpos += ncap;
                size -= ncap;
                cur = cur->next;
                ncap = cur->size;
                npos = 0;
            }
        }
    }

    void ByteArray::setPosition(size_t val)
    {
        if (val > m_capacity)
        {
            throw std::out_of_range("set_position out of range");
        }
        m_position = val;
        if (m_position > m_size)
        {
            m_size = m_position;
        }
        m_cur = m_root;
        while (val > m_cur->size)
        {
            val -= m_cur->size;
            m_cur = m_cur->next;
        }
        if (val == m_cur->size)
        {
            m_cur = m_cur->next;
        }
    }

    bool ByteArray::writeToFile(const std::string &name) const
    {
        std::ofstream ofs;
        ofs.open(name, std::ios::trunc | std::ios::binary);
        if (!ofs)
        {
            SYLAR_LOG_ERROR(g_logger) << "writeToFile name=" << name
                                      << " error, errno=" << errno
                                      << " errstr=" << strerror(errno);
            return false;
        }

        int64_t read_size = getReadSize();
        int64_t pos = m_position;
        Node *cur = m_cur;

        while (read_size > 0)
        {
            int diff = pos % m_baseSize;
            int64_t len = (read_size > (int64_t)m_baseSize ? m_baseSize : read_size) - diff;

            ofs.write(cur->ptr + diff, len);
            cur = cur->next;
            pos += len;
            read_size -= len;
        }

        return true;
    }

    bool ByteArray::readFromFile(const std::string &name)
    {
        std::ifstream ifs;
        ifs.open(name, std::ios::binary);
        if (!ifs)
        {
            SYLAR_LOG_ERROR(g_logger) << "readFromFile name=" << name
                                      << " error, errno=" << errno
                                      << " errstr=" << strerror(errno);
            return false;
        }

        std::shared_ptr<char> buffer(new char[m_baseSize], [](char *ptr)
                                     { delete[] ptr; });
        while (!ifs.eof())
        {
            ifs.read(buffer.get(), m_baseSize);
            write(buffer.get(), ifs.gcount());
        }
        return true;
    }

    bool ByteArray::isLittleEndian() const
    {
        return m_endian == SYLAR_LITTLE_ENDIAN;
    }

    void ByteArray::setIsLittleEndian(bool val)
    {
        if (val)
        {
            m_endian = SYLAR_LITTLE_ENDIAN;
        }
        else
        {
            m_endian = SYLAR_BIG_ENDIAN;
        }
    }

    std::string ByteArray::toString() const
    {
        std::string str;
        str.resize(getReadSize());
        if (str.empty())
        {
            return str;
        }
        read(&str[0], str.size(), m_position);
        return str;
    }

    std::string ByteArray::toHexString() const
    {
        std::stringstream ss;
        std::string str = toString();

        for (size_t i = 0; i < str.size(); ++i)
        {
            if (i > 0 && i % 32 == 0)
            {
                ss << std::endl;
            }
            // ss<<std::setw(2)<<std::setfill('0')<<std::hex
            // <<(int)(uint8_t)str[i]<<" ";
        }

        return ss.str();
    }

    uint64_t ByteArray::getReadBuffers(std::vector<iovec> &buffers, uint64_t len) const
    {
        len = len > getReadSize() ? getReadSize() : len;
        if (len == 0)
        {
            return 0;
        }

        uint64_t size = len;

        size_t npos = m_position % m_baseSize;
        size_t ncap = m_cur->size - npos;
        struct iovec iov;
        Node *cur = m_cur;

        while (len > 0)
        {
            if (ncap >= len)
            {
                iov.iov_base = cur->ptr + npos;
                iov.iov_len = len;
                len = 0;
            }
            else
            {
                iov.iov_base = cur->ptr + npos;
                iov.iov_len = ncap;
                len -= ncap;
                cur = cur->next;
                ncap = cur->size;
                npos = 0;
            }
            buffers.push_back(iov);
        }
        return size;
    }

    uint64_t ByteArray::getReadBuffers(std::vector<iovec> &buffers,
                                       uint64_t len, uint64_t position) const
    {
        len = len > getReadSize() ? getReadSize() : len;
        if (len == 0)
        {
            return 0;
        }

        uint64_t size = len;

        size_t npos = position % m_baseSize;

        size_t count = position / m_baseSize;

        Node *cur = m_root;
        while (count > 0)
        {
            cur = cur->next;
            --count;
        }

        size_t ncap = m_cur->size - npos;
        struct iovec iov;
        cur = m_cur;

        while (len > 0)
        {
            if (ncap >= len)
            {
                iov.iov_base = cur->ptr + npos;
                iov.iov_len = len;
                len = 0;
            }
            else
            {
                iov.iov_base = cur->ptr + npos;
                iov.iov_len = ncap;
                len -= ncap;
                cur = cur->next;
                ncap = cur->size;
                npos = 0;
            }
            buffers.push_back(iov);
        }
        return size;
    }

    uint64_t ByteArray::getWriteBuffers(std::vector<iovec> &buffers, uint64_t len)
    {
        if (len == 0)
        {
            return 0;
        }
        addCapacity(len);
        uint64_t size = len;

        size_t npos = m_position % m_baseSize;
        size_t ncap = m_cur->size - npos;
        struct iovec iov;
        Node *cur = m_cur;
        while (len > 0)
        {
            if (ncap >= len)
            {
                iov.iov_base = cur->ptr + npos;
                iov.iov_len = len;
                len = 0;
            }
            else
            {
                iov.iov_base = cur->ptr + npos;
                iov.iov_len = ncap;

                len -= ncap;
                cur = cur->next;
                ncap = cur->size;
                npos = 0;
            }
            buffers.push_back(iov);
        }
        return size;
    }

    void ByteArray::addCapacity(size_t size)
    {
        if (size == 0)
        {
            return;
        }

        size_t old_cap = getCapacity();
        if (old_cap >= size)
        {
            return;
        }

        size = size - old_cap;
        size_t count = (size / m_baseSize) + (((size % m_baseSize) > old_cap) ? 1 : 0);
        Node *tmp = m_root;
        while (tmp->next)
        {
            tmp = tmp->next;
        }

        Node *first = NULL;
        for (size_t i = 0; i < count; ++i)
        {
            tmp->next = new Node(m_baseSize);
            if (first == NULL)
            {
                first = tmp->next;
            }
            tmp = tmp->next;
            m_capacity += m_baseSize;
        }

        if (old_cap == 0)
        {
            m_cur = first;
        }
    }
}