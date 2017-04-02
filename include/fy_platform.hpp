
#ifndef FY_GUIDANCE_PLATFORM_H
#define FY_GUIDANCE_PLATFORM_H

#if defined(__CYGWIN__)
#    define FY_PLATFORM_CYGWIN 1
#    define FY_PLATFORM_DESKTOP 1
#endif

#if defined(__MINGW32__) || defined(__MINGW64__)
#    define FY_PLATFORM_MINGW 1
#    define FY_PLATFORM_DESKTOP 1
#endif

#if defined(FY_PLATFORM_ANDROID) || defined(__ANDROID__)
#    undef FY_PLATFORM_ANDROID
#    define FY_PLATFORM_ANDROID 1
#    define FY_PLATFORM_NAME "Android"
#    define FY_PLATFORM_LINUX 1
#    define FY_PLATFORM_UNIX 1
#    define FY_PLATFORM_POSIX 1
#    define FY_PLATFORM_MOBILE 1
#    define FY_ASM_STYLE_ATT 1
#    if defined(__arm__)
#        define FY_ABI_ARM_LINUX 1
#        define FY_PROCESSOR_ARM32 1
#        define FY_PLATFORM_DESCRIPTION "Android on ARM"
#    elif defined(__i386__)
#        define FY_PROCESSOR_X86 1
#        define FY_PLATFORM_DESCRIPTION "Android on x86"
#    else 
#        error Unknown processor
#    endif
#elif defined(FY_PLATFORM_APPLE) || (defined(__APPLE__) && __APPLE__)
#    include <TargetConditionals.h>
#    undef FY_PLATFORM_APPLE
#    define FY_PLATFORM_APPLE 1
#    define FY_PLATFORM_POSIX 1
#    define FY_ASM_STYLE_ATT 1
#    if defined(FY_PLATFORM_IPHONE) || defined(__IPHONE__)
#        undef FY_PLATFORM_IPHONE
#        define FY_PLATFORM_IPHONE 1
#        define FY_PLATFORM_NAME "iPhone"
#        define FY_PLATFORM_MOBILE 1
#        if defined(__arm__)
#            define FY_ABI_ARM_APPLE 1
#            define FY_PROCESSOR_ARM32 1
#            define FY_PLATFORM_DESCRIPTION "iPhone on ARM"
#            define FY_SYSTEM_LITTLE_ENDIAN 1
#        elif defined(__aarch64__) || defined(__AARCH64)
#            define FY_ABI_ARM64_APPLE 1
#            define FY_PROCESSOR_ARM64 1
#            define FY_PLATFORM_DESCRIPTION "iPhone on ARM64"
#            define FY_SYSTEM_LITTLE_ENDIAN 1
#        elif defined(__i386__)
#            define FY_PLATFORM_IPHONE_SIMULATOR 1
#            define FY_PROCESSOR_X86 1
#            define FY_PLATFORM_DESCRIPTION "iPhone simulator on x86"
#            define FY_SYSTEM_LITTLE_ENDIAN 1
#        elif defined(__x86_64) || defined(__amd64)
#            define FY_PLATFORM_IPHONE_SIMULATOR 1
#            define FY_PROCESSOR_X86_64 1
#            define FY_PLATFORM_DESCRIPTION "iPhone simulator on x64"
#        else
#            error Unknown processor
#        endif
//Macintosh OSX
#    elif defined(FY_PLATFORM_OSX) || defined(__MACH__) || (defined(__MSL__) && (__dest_os == __mac_os_x))
#        undef FY_PLATFORM_OSX
#        define FY_PLATFORM_OSX 1
#        define FY_PLATFORM_UNIX 1
#        define FY_PLATFORM_POSIX 1
#        define FY_PLATFORM_NAME "OSX"
#        define FY_PLATFORM_DESKTOP 1
#        if defined(__i386__) || defined(__intel__)
#            define FY_PROCESSOR_X86 1
#            define FY_SYSTEM_LITTLE_ENDIAN 1
#            define FY_PLATFORM_DESCRIPTION "OSX on x86"
#        elif defined(__x86_64) || defined(__amd64)
#            define FY_PROCESSOR_X86_64 1
#            define FY_SYSTEM_LITTLE_ENDIAN 1
#            define FY_PLATFORM_DESCRIPTION "OSX on x64"
#        elif defined(__arm__)
#            define FY_ABI_ARM_APPLE 1
#            define FY_PROCESSOR_ARM32 1
#            define FY_SYSTEM_LITTLE_ENDIAN 1
#            define FY_PLATFORM_DESCRIPTION "OSX on ARM"
#        elif defined(__aarch64__) || defined(__AARCH64)
#            define FY_ABI_ARM64_APPLE 1
#            define FY_PROCESSOR_ARM64 1
#            define FY_SYSTEM_LITTLE_ENDIAN 1
#            define FY_PLATFORM_DESCRIPTION "OSX on ARM64"
#        elif defined(__POWERPC64__) || defined(__powerpc64__)
#            define FY_PROCESSOR_POWERPC64 1
#            define FY_SYSTEM_BIG_ENDIAN 1
#            define FY_PLATFORM_DESCRIPTION "OSX on PowerPC 64"
#        elif defined(__POWERPC__) || defined(__powerpc__)
#            define FY_PROCESSOR_POWERPC32 1
#            define FY_SYSTEM_BIG_ENDIAN 1
#            define FY_PLATFORM_DESCRIPTION "OSX on PowerPC"
#        else
#            error Unknown processor
#        endif
#        if defined(__GNUC__)
#            define FY_ASM_STYLE_ATT 1
#        else
#            define FY_ASM_STYLE_MOTOROLA 1
#        endif
#    else
#        error Unknown Apple platform
#    endif
#elif defined(FY_PLATFORM_LINUX) || defined(__linux) || defined(__linux__)
#    undef FY_PLATFORM_LINUX
#    define FY_PLATFORM_LINUX 1
#    define FY_PLATFORM_NAME "Linux"
#    define FY_PLATFORM_LINUX 1
#    define FY_PLATFORM_UNIX 1
#    define FY_PLATFORM_POSIX 1
#    if defined(__i386__) || defined(__intel__) || defined(_M_IX86)
#        define FY_PROCESSOR_X86 1
#        define FY_PLATFORM_DESCRIPTION "Linux on x86"
#        define FY_SYSTEM_LITTLE_ENDIAN 1
#        define FY_PLATFORM_DESKTOP 1
#    elif defined(__x86_64__)
#        define FY_PROCESSOR_X86_64 1
#        define FY_PLATFORM_DESCRIPTION "Linux on x64"
#        define FY_SYSTEM_LITTLE_ENDIAN 1
#        define FY_PLATFORM_DESKTOP 1
#    elif defined(__powerpc64__)
#        define FY_PROCESSOR_POWERPC64 1
#        define FY_PLATFORM_DESCRIPTION "Linux on PowerPC 64"
#        define FY_SYSTEM_BIG_ENDIAN 1
#        define FY_PLATFORM_DESKTOP 1
#    elif defined(__powerpc__)
#        define FY_PROCESSOR_POWERPC32 1
#        define FY_PLATFORM_DESCRIPTION "Linux on PowerPC"
#        define FY_SYSTEM_BIG_ENDIAN 1
#        define FY_PLATFORM_DESKTOP 1
#    elif defined(__arm__)
#        define FY_PROCESSOR_ARM 1
#        define FY_PLATFORM_DESCRIPTION "Linux on ARM"
#        define FY_SYSTEM_LITTLE_ENDIAN 1
#    else
#        error Unknown processor
#        error Unknown endianness
#    endif
#    if defined(__GNUC__)
#        define FY_ASM_STYLE_ATT 1
#    endif

#elif defined(FY_PLATFORM_BSD) || defined(__BSD__) || defined(__FreeBSD__)
#    undef FY_PLATFORM_BSD
#    define FY_PLATFORM_BSD 1
#    define FY_PLATFORM_UNIX 1
#    define FY_PLATFORM_POSIX 1
#    define FY_PLATFORM_NAME "BSD Unix"
#    define FY_PLATFORM_DESKTOP 1
#    if defined(__i386__) || defined(__intel__)
#        define FY_PROCESSOR_X86 1
#        define FY_SYSTEM_LITTLE_ENDIAN 1
#        define FY_PLATFORM_DESCRIPTION "BSD on x86"
#    elif defined(__x86_64__)
#        define FY_PROCESSOR_X86_64 1
#        define FY_SYSTEM_LITTLE_ENDIAN 1
#        define FY_PLATFORM_DESCRIPTION "BSD on x64"
#    elif defined(__powerpc64__)
#        define FY_PROCESSOR_POWERPC64 1
#        define FY_SYSTEM_BIG_ENDIAN 1
#        define FY_PLATFORM_DESCRIPTION "BSD on PowerPC 64"
#    elif defined(__powerpc__)
#        define FY_PROCESSOR_POWERPC32 1
#        define FY_SYSTEM_BIG_ENDIAN 1
#        define FY_PLATFORM_DESCRIPTION "BSD on PowerPC"
#    else
#        error Unknown processor
#        error Unknown endianness
#    endif
#    if !defined(FY_PLATFORM_FREEBSD) && defined(__FreeBSD__)
#        define FY_PLATFORM_FREEBSD 1 // This is a variation of BSD.
#    endif
#    if defined(__GNUC__)
#        define FY_ASM_STYLE_ATT 1
#    endif
#elif defined(FY_PLATFORM_WINDOWS_PHONE)
#    undef FY_PLATFORM_WINDOWS_PHONE
#    define FY_PLATFORM_WINDOWS_PHONE 1
#    define FY_PLATFORM_NAME "Windows Phone"
#    define FY_PLATFORM_MOBILE 1
#    if defined(_M_AMD64) || defined(_AMD64_) || defined(__x86_64__)
#        define FY_PROCESSOR_X86_64 1
#        define FY_SYSTEM_LITTLE_ENDIAN 1
#        define FY_PLATFORM_DESCRIPTION "Windows Phone on x64"
#    elif defined(_M_IX86) || defined(_X86_)
#        define FY_PROCESSOR_X86 1
#        define FY_SYSTEM_LITTLE_ENDIAN 1
#        define FY_PLATFORM_DESCRIPTION "Windows Phone on X86"
#    elif defined(_M_ARM)
#        define FY_ABI_ARM_WINCE 1
#        define FY_PROCESSOR_ARM32 1
#        define FY_SYSTEM_LITTLE_ENDIAN 1
#        define FY_PLATFORM_DESCRIPTION "Windows Phone on ARM"
#    else //Possibly other Windows Phone variants
#        error Unknown processor
#        error Unknown endianness
#    endif
#elif defined(FY_PLATFORM_WINDOWS) || defined(_WIN32) || defined(__WIN32__) || defined(_WIN64)
#    undef FY_PLATFORM_WINDOWS
#    define FY_PLATFORM_WINDOWS 1
#    define FY_PLATFORM_NAME "Windows"
#    define FY_PLATFORM_DESKTOP 1
#    ifdef _WIN64 //VC++ defines both _WIN32 and _WIN64 when compiling for Win64
#        define FY_PLATFORM_WIN64 1
#    else
#        define FY_PLATFORM_WIN32 1
#    endif
#    if defined(_M_AMD64) || defined(_AMD64_) || defined(__x86_64__)
#        define FY_PROCESSOR_X86_64 1
#        define FY_SYSTEM_LITTLE_ENDIAN 1
#        define FY_PLATFORM_DESCRIPTION "Windows on x64"
#    elif defined(_M_IX86) || defined(_X86_)
#        define FY_PROCESSOR_X86 1
#        define FY_SYSTEM_LITTLE_ENDIAN 1
#        define FY_PLATFORM_DESCRIPTION "Windows on x86"
#    elif defined(_M_IA64) || defined(_IA64_)
#        define FY_PROCESSOR_IA64 1
#        define FY_SYSTEM_LITTLE_ENDIAN 1
#        define FY_PLATFORM_DESCRIPTION "Windows on IA-64"
#    elif defined(_M_ARM)
#        define FY_ABI_ARM_WINCE 1
#        define FY_PROCESSOR_ARM32 1
#        define FY_SYSTEM_LITTLE_ENDIAN 1
#        define FY_PLATFORM_DESCRIPTION "Windows on ARM"
#    else
#        error Unknown processor
#        error Unknown endianness
#    endif
#    if defined(__GNUC__)
#        define FY_ASM_STYLE_ATT 1
#    elif defined(_MSC_VER) || defined(__BORLANDC__) || defined(__ICL)
#        define FY_ASM_STYLE_INTEL 1
#    endif
#else
#    error Unknown platform
#    error Unknown processor
#    error Unknown endianness
#endif

#ifndef FY_PROCESSOR_ARM
#    if defined(FY_PROCESSOR_ARM32) || defined(FY_PROCESSOR_ARM64)
#        define FY_PROCESSOR_ARM
#    endif
#endif

#ifndef FY_PROCESSOR_POWERPC
#    if defined(FY_PROCESSOR_POWERPC32) || defined(FY_PROCESSOR_POWERPC64)
#        define FY_PROCESSOR_POWERPC
#    endif
#endif

#endif