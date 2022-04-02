#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <string_view>


#if defined(_WIN32) && defined(LOERR_ENABLE_PREDEFINE_WINDOWS)
#    include <Windows.h> // For GetLastError, SetLastError, FormatMessage
#elif defined(LOERR_ENABLE_PREDEFINE_UNIX)
#    include <cerrno>   // For errno
#    include <cstring>  // For strerror
#    include <unistd.h> // For gettid
#endif

namespace loerr::detail {

#if defined(_WIN32)
constexpr auto LOERR_DETAIL_SLASH = '\\';
#else
constexpr auto LOERR_DETAIL_SLASH = '/';
#endif

// Format src location
constexpr const char* get_file_name(const char* file_path)
{
    char c = '\0';
    int index = 0;
    int last_slash_index = 0;
    do
    {
        c = file_path[index];
        if (c == LOERR_DETAIL_SLASH)
            last_slash_index = index;
        ++index;
    } while (c != '\0');

    return file_path + last_slash_index + 1;
}

#if defined(_WIN32) && defined(LOERR_ENABLE_PREDEFINE_WINDOWS)
// Using for windows to format error code content
inline std::string formate_error_code(DWORD error_code)
{
    PSTR error_text = nullptr;
    ::FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        error_code,
        MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
        (LPSTR)&error_text,
        0, nullptr);

    std::string error_str(error_text);
    LocalFree(error_text);
    return error_str;
}
#endif

inline void simple_transfer_func(std::string_view error_conent, int line_num, const char* src_location, const char* func_name, int error_code)
{
    int thread_id = 0;
#if defined(_WIN32) && defined(LOERR_ENABLE_PREDEFINE_WINDOWS)
    thread_id = static_cast<int>(GetCurrentThreadId());
#elif defined(LOERR_ENABLE_PREDEFINE_UNIX)
    thread_id = static_cast<int>(gettid());
#else
    thread_id = -1;
#endif
    if (*error_conent.rbegin() == '\n')
        error_conent.remove_suffix(1);
    std::stringstream o;
    o << "thread(" << thread_id << "): " << func_name << " (" << error_code << ") " << error_conent << "\n" << src_location <<" : " << line_num << " \n\n";
    printf("%s", o.str().c_str());
    // printf("thread(%d): %s (%d) %s\n%s : %d \n\n", thread_id, func_name, error_code, error_conent.data(), src_location, line_num);
}

} // namespace loerr::detail