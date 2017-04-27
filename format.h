#include <stdlib.h>


#define QCONF_VECTOR_COUNT_TYPE         uint16_t
#define QCONF_VECTOR_COUNT_LEN          sizeof(uint16_t)
#define QCONF_IDC_SIZE_TYPE             uint8_t
#define QCONF_IDC_SIZE_LEN              sizeof(uint8_t)
#define QCONF_HOST_PATH_SIZE_TYPE       uint16_t
#define QCONF_HOST_PATH_SIZE_LEN        sizeof(uint16_t)
#define QCONF_VALUE_SIZE_TYPE           uint32_t
#define QCONF_VALUE_SIZE_LEN            sizeof(uint32_t)

#if (BYTE_ORDER == LITTLE_ENDIAN)
#define QCONF_IS_LITTLE_ENDIAN true
#else
#define QCONF_IS_LITTLE_ENDIAN false
#endif

#define qconf_encode_num(buf, num, d_type)\
    if (QCONF_IS_LITTLE_ENDIAN)\
        memcpy(buf, &num, sizeof(d_type)); \
    else\
    {\
        for (size_t _i = 0; _i < sizeof(d_type); ++_i)\
            *(buf + _i) = (num >> _i * 8) & 0xff;\
    }

#define qconf_decode_num(buf, num, d_type)\
    num = 0;\
    if (QCONF_IS_LITTLE_ENDIAN)\
        memcpy(&num, buf, sizeof(d_type));\
    else\
    {\
        for (size_t _i = 0; _i < sizeof(d_type); ++_i)\
            num += (*reinterpret_cast<const uint8_t*>(buf + _i) << _i * 8);\
    }

#define qconf_string_append(dst, src, d_type)\
    {\
        d_type size = src.size();\
        char buf[sizeof(d_type)] = {0};\
        qconf_encode_num(buf, size, d_type);\
        dst.append(buf, sizeof(d_type));\
        dst.append(src);\
    }

#define qconf_string_sub(string, sub_pos, sub_string, d_type, ret)\
    {\
        ret = QCONF_ERR_DATA_FORMAT;\
        if (string.size() >= sub_pos + sizeof(d_type))\
        {\
            d_type size = 0;\
            qconf_decode_num(string.data() + sub_pos, size, d_type);\
            sub_pos += sizeof(d_type);\
            if (string.size() >= sub_pos + size)\
            {\
                sub_string.assign(string, sub_pos, size);\
                sub_pos += size;\
                ret = QCONF_OK;\
            }\
        }\
    }
