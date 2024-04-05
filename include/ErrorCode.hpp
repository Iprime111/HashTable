#ifndef ERROR_CODE_HPP_
#define ERROR_CODE_HPP_

enum class ErrorCode {
    NO_ERRORS        = 0,
    ALLOCATION_ERROR = 1 << 0,
    NULL_POINTER     = 1 << 1,
    LIST_ERROR       = 1 << 2,
};

#endif
