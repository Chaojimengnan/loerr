#pragma once

// Define this to enable transfer error code, error line, and error content
//#define LOERR_ENABLE_TRANSFER

// Define this to enable predefine utility macros
//#define LOERR_ENABLE_PREDEFINE

// Define this to enable Windows error handler support
// #define LOERR_ENABLE_PREDEFINE_WINDOWS

// Define this to enable UNIX or UNIX like error handler support
// #define LOERR_ENABLE_PREDEFINE_UNIX

// Define this to enable SDL error handler support
//#define LOERR_ENABLE_PREDEFINE_SDL

/// Define this to enable Windows Socket error handler support
// #define LOERR_ENABLE_PREDEFINE_WINDOWS_SOCKET

#ifdef LOERR_ENABLE_TRANSFER

#    include <type_traits>

namespace loerr {
template <typename ResetErrorCodeFunc, typename ErrorContentFunc, typename TransferFunc>
void transfer_if_error(bool result, int error_code, int normal_code_val, ErrorContentFunc error_content_func,
    ResetErrorCodeFunc reset_error_code_func, TransferFunc transfer_func, int line_num, const char* src_location, const char* func_name)
{
    if (result)
    {
        auto&& error_content = error_content_func(error_code);
        transfer_func(std::forward<decltype(error_content)>(error_content), line_num, src_location, func_name, error_code);
        reset_error_code_func(normal_code_val);
    }
}

template <typename TransferFunc, typename ContentType>
void transfer_if_value(bool result, TransferFunc transfer_func, ContentType error_content,
    int line_num, const char* src_location, const char* func_name)
{
    if (result)
    {
        transfer_func(error_content, line_num, src_location, func_name, -1);
    }
}

} // namespace loerr

#    if defined(LOERR_ENABLE_PREDEFINE)
#        include "loerr-detail.h"

// NOLINTBEGIN(cppcoreguidelines-macro-usage)
#        define LOERR_TRANSFER_IF_ERROR(result, error_code, normal_code_val, error_content_func, reset_error_code_func, transfer_func)          \
            ::loerr::transfer_if_error(result, error_code, normal_code_val, error_content_func, reset_error_code_func, transfer_func, __LINE__, \
                ::loerr::detail::get_file_name(__FILE__), __FUNCTION__)

#        define LOERR_TRANSFER_IF_ERROR_EQUAL(normal_val, error_code, error_content_func, reset_error_code_func, transfer_func)          \
            LOERR_TRANSFER_IF_ERROR((normal_val) == (error_code), error_code, normal_val, error_content_func, reset_error_code_func, transfer_func)
#        define LOERR_TRANSFER_IF_ERROR_NOT_EQUAL(normal_val, error_code, error_content_func, reset_error_code_func, transfer_func)          \
            LOERR_TRANSFER_IF_ERROR((normal_val) != (error_code), error_code, normal_val, error_content_func, reset_error_code_func, transfer_func)


#        define LOERR_TRANSFER_IF_VALUE(result, transfer_func, error_content)          \
            ::loerr::transfer_if_value(result, transfer_func, error_content, __LINE__, \
                ::loerr::detail::get_file_name(__FILE__), __FUNCTION__)

#        define LOERR_TRANSFER_IF_VALUE_NOT_EQUAL(val, normal_val, transfer_func) LOERR_TRANSFER_IF_VALUE((val) != (normal_val), transfer_func, "function fails\n")
#        define LOERR_TRANSFER_IF_VALUE_EQUAL(val, normal_val, transfer_func) LOERR_TRANSFER_IF_VALUE((val) == (normal_val), transfer_func, "function fails\n")

#        define LOERR_OUTPUT_IF_VALUE_NOT_EQUAL(val, normal_val) LOERR_TRANSFER_IF_VALUE_NOT_EQUAL(val, normal_val, ::loerr::detail::simple_transfer_func)
#        define LOERR_OUTPUT_IF_VALUE_EQUAL(val, normal_val) LOERR_TRANSFER_IF_VALUE_EQUAL(val, normal_val, ::loerr::detail::simple_transfer_func)

#        if defined(_WIN32) && defined(LOERR_ENABLE_PREDEFINE_WINDOWS)
#            define LOERR_WINDOWS_TRANSFER_IF_ERROR(transfer_func) LOERR_TRANSFER_IF_ERROR_NOT_EQUAL(0, GetLastError(), ::loerr::detail::formate_error_code, SetLastError, transfer_func)
#            define LOERR_WINDOWS_OUTPUT_IF_ERROR() LOERR_WINDOWS_TRANSFER_IF_ERROR(::loerr::detail::simple_transfer_func)
#            if defined(LOERR_ENABLE_PREDEFINE_WINDOWS_SOCKET)
#                define LOERR_WINDOWS_SOCKET_TRANSFER_IF_ERROR(transfer_func) LOERR_TRANSFER_IF_ERROR_NOT_EQUAL(0, WSAGetLastError(), ::loerr::detail::formate_error_code, WSASetLastError, transfer_func)
#                define LOERR_WINDOWS_SOCKET_OUTPUT_IF_ERROR() LOERR_WINDOWS_SOCKET_TRANSFER_IF_ERROR(::loerr::detail::simple_transfer_func)
#            endif
#        elif defined(LOERR_ENABLE_PREDEFINE_UNIX)
#            define LOERR_UNIX_TRANSFER_IF_ERROR(transfer_func) LOERR_TRANSFER_IF_ERROR_NOT_EQUAL( \
                0, errno, strerror, [](const auto& val) { errno = val; }, transfer_func)
#            define LOERR_UNIX_OUTPUT_IF_ERROR() LOERR_UNIX_TRANSFER_IF_ERROR(::loerr::detail::simple_transfer_func)
#        endif

#        if defined(LOERR_ENABLE_PREDEFINE_SDL)
#            include <SDL.h>
#            define LOERR_TRANSFER_SDL_ERROR(transfer_func) LOERR_TRANSFER_IF_VALUE(true, transfer_func, SDL_GetError())
#            define LOERR_OUTPUT_SDL_ERROR() LOERR_TRANSFER_SDL_ERROR(::loerr::detail::simple_transfer_func)
#        endif
// NOLINTEND(cppcoreguidelines-macro-usage)

#    endif

#else

#    if defined(LOERR_ENABLE_PREDEFINE)
#        define LOERR_TRANSFER_IF_ERROR(result, error_code, error_content_func, reset_error_code_func, transfer_func)
#        define LOERR_TRANSFER_IF_ERROR_EQUAL(normal_val, error_code, error_content_func, reset_error_code_func, transfer_func)
#        define LOERR_TRANSFER_IF_ERROR_NOT_EQUAL(normal_val, error_code, error_content_func, reset_error_code_func, transfer_func)
#        define LOERR_TRANSFER_IF_VALUE(val, result, transfer_func, error_content)
#        define LOERR_TRANSFER_IF_VALUE_NOT_EQUAL(val, normal_val, transfer_func)
#        define LOERR_TRANSFER_IF_VALUE_EQUAL(val, normal_val, transfer_func)
#        define LOERR_OUTPUT_IF_VALUE_NOT_EQUAL(val, normal_val)
#        define LOERR_OUTPUT_IF_VALUE_EQUAL(val, normal_val)
#        if defined(_WIN32) && defined(LOERR_ENABLE_PREDEFINE_WINDOWS)
#            define LOERR_WINDOWS_TRANSFER_IF_ERROR(transfer_func)
#            define LOERR_WINDOWS_OUTPUT_IF_ERROR()
#            ifdef LOERR_ENABLE_PREDEFINE_WINDOWS_SOCKET
#                define LOERR_WINDOWS_SOCKET_TRANSFER_IF_ERROR(transfer_func)
#                define LOERR_WINDOWS_SOCKET_OUTPUT_IF_ERROR()
#            endif
#        elif defined(LOERR_ENABLE_PREDEFINE_UNIX)
#            define LOERR_UNIX_TRANSFER_IF_ERROR(transfer_func)
#            define LOERR_UNIX_OUTPUT_IF_ERROR()
#        endif
#        if defined(LOERR_ENABLE_PREDEFINE_SDL)
#            define LOERR_TRANSFER_SDL_ERROR(transfer_func)
#            define LOERR_OUTPUT_SDL_ERROR()
#        endif
#    endif

#endif