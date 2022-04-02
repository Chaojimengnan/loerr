# loerr

Generate time: 2022-04-02 13:54:53

This is a header-only library for programs that use error codes to handle errors, and focuses on the underlying system error code handling, such as Win32, POSIX all use error codes.

It comes with a series of simple macros that make it easier to print the error content, error location.

These macros forward error messages, error locations to a user-defined handler, and because they use macros, you can easily turn off error forwarding without any runtime overhead.