#ifndef FY_GUIDANCE_COMPILER_H
#define FY_GUIDANCE_COMPILER_H

#include "fy_platform.hpp"

// ------------------------------------------------------------------------
// FY_STRINGIFY
//
// Example usage:
//     printf("Line: %s", FY_STRINGIFY(__LINE__));
//
#ifndef FY_STRINGIFY
#    define FY_STRINGIFY(x)     FY_STRINGIFYIMPL(x)
#    define FY_STRINGIFYIMPL(x) #x
#endif

#ifndef FY_COMPILER_HAS_FEATURE
#    if defined(__clang__)
#        define FY_COMPILER_HAS_FEATURE(x) __has_feature(x)
#    else
#        define FY_COMPILER_HAS_FEATURE(x) 0
#    endif
#endif


#ifndef FY_COMPILER_HAS_BUILTIN
#    if defined(__clang__)
#        define FY_COMPILER_HAS_BUILTIN(x) __has_builtin(x)
#    else
#        define FY_COMPILER_HAS_BUILTIN(x) 0
#    endif
#endif

// EDG (EDG compiler front-end, used by other compilers such as SN)
#if defined(__EDG_VERSION__)
#    define FY_COMPILER_EDG 1
#    if defined(_MSC_VER)
#        define FY_COMPILER_EDG_VC_MODE 1
#    endif
#    if defined(__GNUC__)
#        define FY_COMPILER_EDG_GCC_MODE 1
#    endif
#endif

// ------------------------------------------------------------------------
// FY_COMPILER_CPP11_ENABLED
//
// Defined as 1 if the compiler has its available C++11 support enabled, else undefined.
// This does not mean that all of C++11 or any particular feature of C++11 is supported
// by the compiler. It means that whatever C++11 support the compiler has is enabled.
// This also includes existing and older compilers that still identify C++11 as C++0x.
//
// We cannot use (__cplusplus >= 201103L) alone because some compiler vendors have 
// decided to not define __cplusplus like thus until they have fully completed their
// C++11 support.
//
#if !defined(FY_COMPILER_CPP11_ENABLED) && defined(__cplusplus)
#    if (__cplusplus >= 201103L)    // Clang and GCC defines this like so in C++11 mode.
#        define FY_COMPILER_CPP11_ENABLED 1
#    elif defined(__GNUC__) && defined(__GXX_EXPERIMENTAL_CXX0X__)
#        define FY_COMPILER_CPP11_ENABLED 1
#    elif defined(_MSC_VER)         // Microsoft unilaterally enables its C++11 support; there is no way to disable it.
#        define FY_COMPILER_CPP11_ENABLED 1
#    elif defined(__SN_VER__) && (__SN_VER__ >= 43001)
#        if __option(cpp11)
#            define FY_COMPILER_CPP11_ENABLED 1
#        endif
#    elif defined(__EDG_VERSION__) // && ???
// To do: Is there a generic way to determine this?
#    endif
#endif

// ------------------------------------------------------------------------
// FY_COMPILER_CPP14_ENABLED
//
// Defined as 1 if the compiler has its available C++14 support enabled, else undefined.
// This does not mean that all of C++14 or any particular feature of C++14 is supported
// by the compiler. It means that whatever C++14 support the compiler has is enabled.
//
// We cannot use (__cplusplus >= 201402L) alone because some compiler vendors have 
// decided to not define __cplusplus like thus until they have fully completed their
// C++14 support.
#if !defined(FY_COMPILER_CPP14_ENABLED) && defined(__cplusplus)
#    if (__cplusplus >= 201402L)                                // Clang and GCC defines this like so in C++14 mode.
#        define FY_COMPILER_CPP14_ENABLED 1
#    elif defined(_MSC_VER) && (FY_COMPILER_VERSION >= 1900)    // VS2015+ 
#        define FY_COMPILER_CPP14_ENABLED 1
#    endif
#endif

// ------------------------------------------------------------------------
#if   defined(__ARMCC_VERSION)
// Note that this refers to the ARM RVCT compiler (armcc or armcpp), but there
// are other compilers that target ARM processors, such as GCC and Microsoft VC++.
// If you want to detect compiling for the ARM processor, check for FY_PROCESSOR_ARM
// being defined.
// This compiler is also identified by defined(__CC_ARM) || defined(__ARMCC__).
#    define FY_COMPILER_RVCT    1
#    define FY_COMPILER_ARM     1
#    define FY_COMPILER_VERSION __ARMCC_VERSION
#    define FY_COMPILER_NAME    "RVCT"
#elif defined(__clang__)
#    define FY_COMPILER_CLANG   1
#    define FY_COMPILER_VERSION (__clang_major__ * 100 + __clang_minor__)
#    define FY_COMPILER_NAME    "clang"
#    define FY_COMPILER_STRING  FY_COMPILER_NAME __clang_version__
// GCC (a.k.a. GNUC)
#elif defined(__GNUC__) // GCC compilers exist for many platforms.
#    define FY_COMPILER_GNUC    1
#    define FY_COMPILER_VERSION (__GNUC__ * 1000 + __GNUC_MINOR__)
#    define FY_COMPILER_NAME    "GCC"
#    define FY_COMPILER_STRING  FY_COMPILER_NAME " compiler, version " FY_STRINGIFY( __GNUC__ ) "." FY_STRINGIFY( __GNUC_MINOR__ )

#    if (__GNUC__ == 2) && (__GNUC_MINOR__ < 95) // If GCC < 2.95... 
#        define FY_COMPILER_NO_MEMBER_TEMPLATES 1
#    endif
#    if (__GNUC__ == 2) && (__GNUC_MINOR__ <= 97) // If GCC <= 2.97...
#        define FY_COMPILER_NO_MEMBER_TEMPLATE_FRIENDS 1
#    endif
#    if (__GNUC__ == 3) && ((__GNUC_MINOR__ == 1) || (__GNUC_MINOR__ == 2)) // If GCC 3.1 or 3.2 (but not pre 3.1 or post 3.2)...
#        define FY_COMPILER_NO_EXPLICIT_FUNCTION_TEMPLATE_ARGUMENTS 1
#    endif

// Borland C++
#elif defined(__BORLANDC__)
#    define FY_COMPILER_BORLANDC 1
#    define FY_COMPILER_VERSION  __BORLANDC__
#    define FY_COMPILER_NAME     "Borland C"
#    if (__BORLANDC__ <= 0x0550)      // If Borland C++ Builder 4 and 5...
#        define FY_COMPILER_NO_MEMBER_TEMPLATE_FRIENDS 1
#    endif
#    if (__BORLANDC__ >= 0x561) && (__BORLANDC__ < 0x600)
#        define FY_COMPILER_NO_MEMBER_FUNCTION_SPECIALIZATION 1
#    endif

// Intel C++
// The Intel Windows compiler masquerades as VC++ and defines _MSC_VER.
// The Intel compiler is based on the EDG compiler front-end.
#elif defined(__ICL) || defined(__ICC)
#    define FY_COMPILER_INTEL 1

// Should we enable the following? We probably should do so since enabling it does a lot more good than harm
// for users. The Intel Windows compiler does a pretty good job of emulating VC++ and so the user would likely
// have to handle few special cases where the Intel compiler doesn't emulate VC++ correctly.
#    if defined(_MSC_VER)
#        define FY_COMPILER_MSVC 1
#        define FY_COMPILER_MICROSOFT 1
#    endif

// Should we enable the following? This isn't as clear because as of this writing we don't know if the Intel 
// compiler truly emulates GCC well enough that enabling this does more good than harm.
#    if defined(__GNUC__)
#        define FY_COMPILER_GNUC 1
#    endif

#    if defined(__ICL)
#        define FY_COMPILER_VERSION __ICL
#    elif defined(__ICC)
#        define FY_COMPILER_VERSION __ICC
#    endif
#    define FY_COMPILER_NAME "Intel C++"
#    if defined(_MSC_VER)
#        define FY_COMPILER_STRING  FY_COMPILER_NAME " compiler, version " FY_STRINGIFY( FY_COMPILER_VERSION ) ", EDG version " FY_STRINGIFY( __EDG_VERSION__ ) ", VC++ version " FY_STRINGIFY( _MSC_VER )
#    elif defined(__GNUC__)
#        define FY_COMPILER_STRING  FY_COMPILER_NAME " compiler, version " FY_STRINGIFY( FY_COMPILER_VERSION ) ", EDG version " FY_STRINGIFY( __EDG_VERSION__ ) ", GCC version " FY_STRINGIFY( __GNUC__ )
#    else
#        define FY_COMPILER_STRING  FY_COMPILER_NAME " compiler, version " FY_STRINGIFY( FY_COMPILER_VERSION ) ", EDG version " FY_STRINGIFY( __EDG_VERSION__ )
#    endif

#elif defined(_MSC_VER)
#    define FY_COMPILER_MSVC 1
#    define FY_COMPILER_MICROSOFT 1
#    define FY_COMPILER_VERSION _MSC_VER
#    define FY_COMPILER_NAME "Microsoft Visual C++"
#    define FY_STANDARD_LIBRARY_MSVC 1
#    define FY_STANDARD_LIBRARY_MICROSOFT 1

#    if (_MSC_VER <= 1200) // If VC6.x and earlier...
#        if (_MSC_VER < 1200)
#            define FY_COMPILER_MSVCOLD 1
#        else
#            define FY_COMPILER_MSVC6 1
#        endif

#        if (_MSC_VER < 1200) // If VC5.x or earlier...
#            define FY_COMPILER_NO_TEMPLATE_SPECIALIZATION 1
#        endif
#        define FY_COMPILER_NO_EXPLICIT_FUNCTION_TEMPLATE_ARGUMENTS 1     // The compiler compiles this OK, but executes it wrong. Fixed in VC7.0
#        define FY_COMPILER_NO_VOID_RETURNS 1                             // The compiler fails to compile such cases. Fixed in VC7.0
#        define FY_COMPILER_NO_EXCEPTION_STD_NAMESPACE 1                  // The compiler fails to compile such cases. Fixed in VC7.0
#        define FY_COMPILER_NO_DEDUCED_TYPENAME 1                         // The compiler fails to compile such cases. Fixed in VC7.0
#        define FY_COMPILER_NO_STATIC_CONSTANTS 1                         // The compiler fails to compile such cases. Fixed in VC7.0
#        define FY_COMPILER_NO_COVARIANT_RETURN_TYPE 1                    // The compiler fails to compile such cases. Fixed in VC7.1
#        define FY_COMPILER_NO_ARGUMENT_DEPENDENT_LOOKUP 1                // The compiler compiles this OK, but executes it wrong. Fixed in VC7.1
#        define FY_COMPILER_NO_TEMPLATE_TEMPLATES 1                       // The compiler fails to compile such cases. Fixed in VC7.1
#        define FY_COMPILER_NO_TEMPLATE_PARTIAL_SPECIALIZATION 1          // The compiler fails to compile such cases. Fixed in VC7.1
#        define FY_COMPILER_NO_MEMBER_TEMPLATE_FRIENDS 1                  // The compiler fails to compile such cases. Fixed in VC7.1

#    elif (_MSC_VER <= 1300) // If VC7.0 and earlier...
#        define FY_COMPILER_MSVC7 1

#        define FY_COMPILER_NO_COVARIANT_RETURN_TYPE 1                    // The compiler fails to compile such cases. Fixed in VC7.1
#        define FY_COMPILER_NO_ARGUMENT_DEPENDENT_LOOKUP 1                // The compiler compiles this OK, but executes it wrong. Fixed in VC7.1
#        define FY_COMPILER_NO_TEMPLATE_TEMPLATES 1                       // The compiler fails to compile such cases. Fixed in VC7.1
#        define FY_COMPILER_NO_TEMPLATE_PARTIAL_SPECIALIZATION 1          // The compiler fails to compile such cases. Fixed in VC7.1
#        define FY_COMPILER_NO_MEMBER_TEMPLATE_FRIENDS 1                  // The compiler fails to compile such cases. Fixed in VC7.1
#        define FY_COMPILER_NO_MEMBER_FUNCTION_SPECIALIZATION 1           // This is the case only for VC7.0 and not VC6 or VC7.1+. Fixed in VC7.1

#    elif (_MSC_VER < 1400) // VS2003       _MSC_VER of 1300 means VC7 (VS2003)
// The VC7.1 and later compiler is fairly close to the C++ standard 
// and thus has no compiler limitations that we are concerned about.
#        define FY_COMPILER_MSVC7_2003 1
#        define FY_COMPILER_MSVC7_1    1

#    elif (_MSC_VER < 1500) // VS2005       _MSC_VER of 1400 means VC8 (VS2005)
#        define FY_COMPILER_MSVC8_2005 1
#        define FY_COMPILER_MSVC8_0    1

#    elif (_MSC_VER < 1600) // VS2008.      _MSC_VER of 1500 means VC9 (VS2008)
#        define FY_COMPILER_MSVC9_2008 1
#        define FY_COMPILER_MSVC9_0    1

#    elif (_MSC_VER < 1700) // VS2010       _MSC_VER of 1600 means VC10 (VS2010)
#        define FY_COMPILER_MSVC_2010 1
#        define FY_COMPILER_MSVC10_0  1

#    elif (_MSC_VER < 1800) // VS2012       _MSC_VER of 1700 means VS2011/VS2012
#        define FY_COMPILER_MSVC_2011 1   // Microsoft changed the name to VS2012 before shipping, despite referring to it as VS2011 up to just a few weeks before shipping.
#        define FY_COMPILER_MSVC11_0  1
#        define FY_COMPILER_MSVC_2012 1
#        define FY_COMPILER_MSVC12_0  1

#    elif (_MSC_VER < 1900) // VS2013       _MSC_VER of 1800 means VS2013
#        define FY_COMPILER_MSVC_2013 1
#        define FY_COMPILER_MSVC13_0  1
#    elif (_MSC_VER < 2000 ) //VS2015       _MSC_VER of 1900 means VS2015
#        define FY_COMPILER_MSVC_2015 1
#        define FY_COMPILER_MSVC14_0  1
#    endif

// IBM
#elif defined(__xlC__)
#    define FY_COMPILER_IBM     1
#    define FY_COMPILER_NAME    "IBM XL C"
#    define FY_COMPILER_VERSION __xlC__
#    define FY_COMPILER_STRING "IBM XL C compiler, version " FY_STRINGIFY( __xlC__ )

// Unknown
#else // Else the compiler is unknown
#    define FY_COMPILER_VERSION 0
#    define FY_COMPILER_NAME   "Unknown"
#endif

#ifndef FY_COMPILER_STRING
#    define FY_COMPILER_STRING FY_COMPILER_NAME " compiler, version " FY_STRINGIFY(FY_COMPILER_VERSION)
#endif

// ------------------------------------------------------------------------
// FY_COMPILER_NO_RTTI
//
// If FY_COMPILER_NO_RTTI is defined, then RTTI (run-time type information)
// is not available (possibly due to being disabled by the user).
//
#if defined(__EDG_VERSION__) && !defined(__RTTI)
#    define FY_COMPILER_NO_RTTI 1
#elif defined(__clang__) && !FY_COMPILER_HAS_FEATURE(cxx_rtti)
#    define FY_COMPILER_NO_RTTI 1
#elif defined(__IBMCPP__) && !defined(__RTTI_ALL__)
#    define FY_COMPILER_NO_RTTI 1
#elif defined(__GXX_ABI_VERSION) && !defined(__GXX_RTTI)
#    define FY_COMPILER_NO_RTTI 1
#elif defined(_MSC_VER) && !defined(_CPPRTTI)
#    define FY_COMPILER_NO_RTTI 1
#elif defined(__ARMCC_VERSION) && defined(__TARGET_CPU_MPCORE) && !defined(__RTTI)
#    define FY_COMPILER_NO_RTTI 1
#endif

// ------------------------------------------------------------------------
// FY_COMPILER_NO_EXCEPTIONS / FY_COMPILER_NO_UNWIND
//
// If FY_COMPILER_NO_EXCEPTIONS is defined, then the compiler is 
// configured to not recognize C++ exception-handling statements 
// such as try/catch/throw. Thus, when FY_COMPILER_NO_EXCEPTIONS is 
// defined, code that attempts to use exception handling statements
// will usually cause a compilation error. If is often desirable
// for projects to disable exception handling because exception 
// handling causes extra code and/or data generation which might
// not be needed, especially if it is known that exceptions won't
// be happening. When writing code that is to be portable between
// systems of which some enable exception handling while others
// don't, check for FY_COMPILER_NO_EXCEPTIONS being defined. 
//
#if !defined(FY_COMPILER_NO_EXCEPTIONS) && !defined(FY_COMPILER_NO_UNWIND)
#    if defined(FY_COMPILER_GNUC) && defined(_NO_EX) // GCC on some platforms defines _NO_EX when exceptions are disabled.
#        define FY_COMPILER_NO_EXCEPTIONS 1
#    elif (defined(FY_COMPILER_CLANG) || defined(FY_COMPILER_GNUC) || defined(FY_COMPILER_INTEL) || defined(FY_COMPILER_RVCT)) && !defined(__EXCEPTIONS) // GCC and most EDG-based compilers define __EXCEPTIONS when exception handling is enabled.
#        define FY_COMPILER_NO_EXCEPTIONS 1
#    elif (defined(FY_COMPILER_BORLAND) || defined(FY_COMPILER_MSVC)) && !defined(_CPPUNWIND)
#        define FY_COMPILER_NO_UNWIND 1
#    endif
#endif

// ------------------------------------------------------------------------
// FY_DISABLE_VC_WARNING / FY_RESTORE_VC_WARNING
// 
// Disable and re-enable warning(s) within code.
// This is simply a wrapper for VC++ #pragma warning(disable: nnnn) for the
// purpose of making code easier to read due to avoiding nested compiler ifdefs
// directly in code.
//
// Example usage:
//     FY_DISABLE_VC_WARNING(4127 3244)
//     <code>
//     FY_RESTORE_VC_WARNING()
//
#ifndef FY_DISABLE_VC_WARNING
#    if defined(_MSC_VER)
#        define FY_DISABLE_VC_WARNING(w)   \
             __pragma(warning(push))       \
             __pragma(warning(disable:w))
#    else
#        define FY_DISABLE_VC_WARNING(w)
#    endif
#endif

#ifndef FY_RESTORE_VC_WARNING
#    if defined(_MSC_VER)
#        define FY_RESTORE_VC_WARNING()   \
             __pragma(warning(pop))
#    else
#        define FY_RESTORE_VC_WARNING()
#    endif
#endif


// ------------------------------------------------------------------------
// FY_DISABLE_GCC_WARNING / FY_RESTORE_GCC_WARNING
//
// Example usage:
//     // Only one warning can be ignored per statement, due to how GCC works.
//     FY_DISABLE_GCC_WARNING(-Wuninitialized)
//     FY_DISABLE_GCC_WARNING(-Wunused)
//     <code>
//     FY_RESTORE_GCC_WARNING()
//     FY_RESTORE_GCC_WARNING()
//
#ifndef FY_DISABLE_GCC_WARNING
#    if defined(FY_COMPILER_GNUC)
#        define FYGCCWHELP0(x) #x
#        define FYGCCWHELP1(x) FYGCCWHELP0(GCC diagnostic ignored x)
#        define FYGCCWHELP2(x) FYGCCWHELP1(#x)
#    endif

#    if defined(FY_COMPILER_GNUC) && (FY_COMPILER_VERSION >= 4006) // Can't test directly for __GNUC__ because some compilers lie.
#        define FY_DISABLE_GCC_WARNING(w)   \
             _Pragma("GCC diagnostic push")  \
             _Pragma(FYGCCWHELP2(w))
#    elif defined(FY_COMPILER_GNUC) && (FY_COMPILER_VERSION >= 4004)
#        define FY_DISABLE_GCC_WARNING(w)   \
             _Pragma(FYGCCWHELP2(w))
#    else
#        define FY_DISABLE_GCC_WARNING(w)
#    endif
#endif

#ifndef FY_RESTORE_GCC_WARNING
#    if defined(FY_COMPILER_GNUC) && (FY_COMPILER_VERSION >= 4006)
#        define FY_RESTORE_GCC_WARNING()    \
             _Pragma("GCC diagnostic pop")
#    else
#        define FY_RESTORE_GCC_WARNING()
#    endif
#endif

// ------------------------------------------------------------------------
// FY_DISABLE_CLANG_WARNING / FY_RESTORE_CLANG_WARNING
//
// Example usage:
//     // Only one warning can be ignored per statement, due to how clang works.
//     FY_DISABLE_CLANG_WARNING(-Wuninitialized)
//     FY_DISABLE_CLANG_WARNING(-Wunused)
//     <code>
//     FY_RESTORE_CLANG_WARNING()
//     FY_RESTORE_CLANG_WARNING()
//
#ifndef FY_DISABLE_CLANG_WARNING
#    if defined(FY_COMPILER_CLANG)
#        define FYCLANGWHELP0(x) #x
#        define FYCLANGWHELP1(x) FYCLANGWHELP0(clang diagnostic ignored x)
#        define FYCLANGWHELP2(x) FYCLANGWHELP1(#x)
#        define FY_DISABLE_CLANG_WARNING(w)   \
             _Pragma("clang diagnostic push")  \
             _Pragma(FYCLANGWHELP2(w))
#    else
#        define FY_DISABLE_CLANG_WARNING(w)
#    endif
#endif

#ifndef FY_RESTORE_CLANG_WARNING
#    if defined(FY_COMPILER_CLANG)
#        define FY_RESTORE_CLANG_WARNING()    \
             _Pragma("clang diagnostic pop")
#    else
#        define FY_RESTORE_CLANG_WARNING()
#    endif
#endif


// ------------------------------------------------------------------------
// FY_DISABLE_EDG_WARNING / FY_RESTORE_EDG_WARNING
//
// Example usage:
//     // Currently we are limited to one warning per line.
//     FY_DISABLE_EDG_WARNING(193)
//     FY_DISABLE_EDG_WARNING(236)
//     <code>
//     FY_RESTORE_EDG_WARNING()
//     FY_RESTORE_EDG_WARNING()
//
#ifndef FY_DISABLE_EDG_WARNING
// EDG-based compilers are inconsistent in how the implement warning pragmas.
#    if defined(FY_COMPILER_EDG) && !defined(FY_COMPILER_INTEL) && !defined(FY_COMPILER_RVCT)
#        define FYEDGWHELP0(x) #x
#        define FYEDGWHELP1(x) FYEDGWHELP0(diag_suppress x)

#        define FY_DISABLE_EDG_WARNING(w)   \
             _Pragma("control %push diag")   \
             _Pragma(FYEDGWHELP1(w))
#    else
#        define FY_DISABLE_EDG_WARNING(w)
#    endif
#endif

#ifndef FY_RESTORE_EDG_WARNING
#    if defined(FY_COMPILER_EDG) && !defined(FY_COMPILER_INTEL) && !defined(FY_COMPILER_RVCT)
#        define FY_RESTORE_EDG_WARNING()   \
             _Pragma("control %pop diag")
#    else
#        define FY_RESTORE_EDG_WARNING()
#    endif
#endif

// ------------------------------------------------------------------------
// FY_OPTIMIZE_OFF / FY_OPTIMIZE_ON
//
// Implements portable inline optimization enabling/disabling.
// Usage of these macros must be in order OFF then ON. This is 
// because the OFF macro pushes a set of settings and the ON
// macro pops them. The nesting of OFF/ON sets (e.g. OFF, OFF, ON, ON)
// is not guaranteed to work on all platforms.
//
// This is often used to allow debugging of some code that's 
// otherwise compiled with undebuggable optimizations. It's also
// useful for working around compiler code generation problems
// that occur in optimized builds.
//
// Some compilers (e.g. VC++) don't allow doing this within a function and 
// so the usage must be outside a function, as with the example below.
// GCC on x86 appears to have some problem with argument passing when 
// using FY_OPTIMIZE_OFF in optimized builds.
//
// Example usage:
//     // Disable optimizations for SomeFunction.
//     FY_OPTIMIZE_OFF()
//     void SomeFunction()
//     {
//         ...
//     }
//     FY_OPTIMIZE_ON()
//
#if !defined(FY_OPTIMIZE_OFF)
#    if   defined(FY_COMPILER_MSVC)
#        define FY_OPTIMIZE_OFF() __pragma(optimize("", off))
#    elif defined(FY_COMPILER_GNUC) && (FY_COMPILER_VERSION > 4004) && (defined(__i386__) || defined(__x86_64__)) // GCC 4.4+ - Seems to work only on x86/Linux so far. However, GCC 4.4 itself appears broken and screws up parameter passing conventions.
#        define FY_OPTIMIZE_OFF()            \
             _Pragma("GCC push_options")      \
             _Pragma("GCC optimize 0")
#    elif defined(FY_COMPILER_CLANG) &&  !defined(FY_PLATFORM_ANDROID) // android clang 305 compiler crashes when this pragma is used
#        define FY_OPTIMIZE_OFF() \
             FY_DISABLE_CLANG_WARNING(-Wunknown-pragmas) \
             _Pragma("clang optimize off") \
             FY_RESTORE_CLANG_WARNING()
#    else
#        define FY_OPTIMIZE_OFF()
#    endif
#endif

#if !defined(FY_OPTIMIZE_ON)
#    if defined(FY_COMPILER_MSVC)
#        define FY_OPTIMIZE_ON() __pragma(optimize("", on))
#    elif defined(FY_COMPILER_GNUC) && (FY_COMPILER_VERSION > 4004) && (defined(__i386__) || defined(__x86_64__)) // GCC 4.4+ - Seems to work only on x86/Linux so far. However, GCC 4.4 itself appears broken and screws up parameter passing conventions.
#        define FY_OPTIMIZE_ON() _Pragma("GCC pop_options")
#    elif defined(FY_COMPILER_CLANG) && !defined(FY_PLATFORM_ANDROID) // android clang 305 compiler crashes when this pragma is used
#        define FY_OPTIMIZE_ON() \
             FY_DISABLE_CLANG_WARNING(-Wunknown-pragmas) \
             _Pragma("clang optimize on") \
             FY_RESTORE_CLANG_WARNING()
#    else
#        define FY_OPTIMIZE_ON()
#    endif
#endif

// ------------------------------------------------------------------------
// FY_FORCE_INLINE              // Used as a prefix.
// FY_PREFIX_FORCE_INLINE       // You should need this only for unusual compilers.
// FY_POSTFIX_FORCE_INLINE      // You should need this only for unusual compilers.
//
// Example usage:
//     FY_FORCE_INLINE void Foo();                                // Implementation elsewhere.
//     FY_PREFIX_FORCE_INLINE void Foo() FY_POSTFIX_FORCE_INLINE; // Implementation elsewhere.
//
// Note that when the prefix version of this function is used, it replaces
// the regular C++ 'inline' statement. Thus you should not use both the 
// C++ inline statement and this macro with the same function declaration.
//
// To force inline usage under GCC 3.1+, you use this:
//    inline void Foo() __attribute__((always_inline));
//       or
//    inline __attribute__((always_inline)) void Foo();
//
// The CodeWarrior compiler doesn't have the concept of forcing inlining per function.
// 
#ifndef FY_FORCE_INLINE
#    if defined(FY_COMPILER_MSVC)
#        define FY_FORCE_INLINE __forceinline
#    elif defined(FY_COMPILER_GNUC) && (((__GNUC__ * 100) + __GNUC_MINOR__) >= 301) || defined(FY_COMPILER_CLANG)
#        if defined(__cplusplus)
#            define FY_FORCE_INLINE inline __attribute__((always_inline))
#        else
#            define FY_FORCE_INLINE __inline__ __attribute__((always_inline))
#        endif
#    else
#        if defined(__cplusplus)
#            define FY_FORCE_INLINE inline
#        else
#            define FY_FORCE_INLINE __inline
#        endif
#    endif
#endif

#if defined(FY_COMPILER_GNUC) && (((__GNUC__ * 100) + __GNUC_MINOR__) >= 301) || defined(FY_COMPILER_CLANG)
#    define FY_PREFIX_FORCE_INLINE  inline
#    define FY_POSTFIX_FORCE_INLINE __attribute__((always_inline))
#else
#    define FY_PREFIX_FORCE_INLINE  inline
#    define FY_POSTFIX_FORCE_INLINE 
#endif


// ------------------------------------------------------------------------
// FY_NO_INLINE             // Used as a prefix. 
// FY_PREFIX_NO_INLINE      // You should need this only for unusual compilers.
// FY_POSTFIX_NO_INLINE     // You should need this only for unusual compilers.
//
// Example usage:
//     FY_NO_INLINE        void Foo();                       // Implementation elsewhere.
//     FY_PREFIX_NO_INLINE void Foo() FY_POSTFIX_NO_INLINE;  // Implementation elsewhere.
//
// That this declaration is incompatbile with C++ 'inline' and any
// variant of FY_FORCE_INLINE.
//
// To disable inline usage under VC++ priof to VS2005, you need to use this:
//    #pragma inline_depth(0) // Disable inlining.
//    void Foo() { ... }
//    #pragma inline_depth()  // Restore to default.
//
// Since there is no easy way to disable inlining on a function-by-function
// basis in VC++ prior to VS2005, the best strategy is to write platform-specific 
// #ifdefs in the code or to disable inlining for a given module and enable 
// functions individually with FY_FORCE_INLINE. 
// 
#ifndef FY_NO_INLINE
#    if defined(FY_COMPILER_MSVC) && (FY_COMPILER_VERSION >= 1400) // If VC8 (VS2005) or later...
#        define FY_NO_INLINE __declspec(noinline)
#    elif defined(FY_COMPILER_MSVC)
#        define FY_NO_INLINE
#    else
#        define FY_NO_INLINE __attribute__((noinline))
#    endif
#endif

#if defined(FY_COMPILER_MSVC) && (FY_COMPILER_VERSION >= 1400) // If VC8 (VS2005) or later...
#    define FY_PREFIX_NO_INLINE  __declspec(noinline)
#    define FY_POSTFIX_NO_INLINE
#elif defined(FY_COMPILER_MSVC)
#   define FY_PREFIX_NO_INLINE
#    define FY_POSTFIX_NO_INLINE
#else
#    define FY_PREFIX_NO_INLINE
#    define FY_POSTFIX_NO_INLINE __attribute__((noinline))
#endif


// ------------------------------------------------------------------------
// FY_NO_VTABLE
//
// Example usage:
//     class FY_NO_VTABLE X {
//        virtual void InterfaceFunction();
//     };
// 
//     FY_CLASS_NO_VTABLE(X) {
//        virtual void InterfaceFunction();
//     };
//
#ifdef FY_COMPILER_MSVC
#    define FY_NO_VTABLE           __declspec(novtable)
#    define FY_CLASS_NO_VTABLE(x)  class __declspec(novtable) x
#    define FY_STRUCT_NO_VTABLE(x) struct __declspec(novtable) x
#else
#    define FY_NO_VTABLE
#    define FY_CLASS_NO_VTABLE(x)  class x
#    define FY_STRUCT_NO_VTABLE(x) struct x
#endif


// ------------------------------------------------------------------------
// FY_SSE
// Visual C Processor Packs define _MSC_FULL_VER and are needed for SSE
// Intel C also has SSE support.
// FY_SSE is used to select FPU or SSE versions in hw_select.inl
//
// FY_SSE defines the level of SSE support:
//  0 indicates no SSE support
//  1 indicates SSE1 is supported
//  2 indicates SSE2 is supported
//  3 indicates SSE3 (or greater) is supported
//
// Note: SSE support beyond SSE3 can't be properly represented as a single
// version number.  Instead users should use specific SSE defines (e.g.
// FY_SSE4_2) to detect what specific support is available.  FY_SSE being
// equal to 3 really only indicates that SSE3 or greater is supported.
#ifndef FY_SSE
#    if defined(FY_COMPILER_GNUC) || defined(FY_COMPILER_CLANG)
#        if defined(__SSE3__)
#            define FY_SSE 3
#        elif defined(__SSE2__)
#            define FY_SSE 2
#        elif defined(__SSE__) && __SSE__
#            define FY_SSE 1
#        else
#            define FY_SSE 0
#        endif
#    elif (defined(FY_SSE3) && FY_SSE3)
#        define FY_SSE 3
#    elif defined(FY_SSE2) && FY_SSE2
#        define FY_SSE 2
#    elif defined(FY_PROCESSOR_X86) && defined(_MSC_FULL_VER) && !defined(__NOSSE__) && defined(_M_IX86_FP)
#        define FY_SSE _M_IX86_FP 
#    elif defined(FY_PROCESSOR_X86) && defined(FY_COMPILER_INTEL) && !defined(__NOSSE__)
#        define FY_SSE 1
#    elif defined(FY_PROCESSOR_X86_64)
// All x64 processors support SSE2 or higher
#        define FY_SSE 2
#    else
#        define FY_SSE 0
#    endif
#endif


// ------------------------------------------------------------------------
// We define separate defines for SSE support beyond SSE1.  These defines
// are particularly useful for detecting SSE4.x features since there isn't
// a single concept of SSE4.
//
// The following SSE defines are always defined.  0 indicates the
// feature/level of SSE is not supported, and 1 indicates support is
// available.
#ifndef FY_SSE2
#    if FY_SSE >= 2
#        define FY_SSE2 1
#    else
#        define FY_SSE2 0
#    endif
#endif

#ifndef FY_SSE3
#    if FY_SSE >= 3
#        define FY_SSE3 1
#    else
#        define FY_SSE3 0
#    endif
#endif

#ifndef FY_SSSE3
#    if defined __SSSE3__
#        define FY_SSSE3 1
#    else
#        define FY_SSSE3 0
#    endif
#endif

#ifndef FY_SSE4_1
#    if defined __SSE4_1__
#        define FY_SSE4_1 1
#    else
#        define FY_SSE4_1 0
#    endif
#endif

#ifndef FY_SSE4_2
#    if defined __SSE4_2__
#        define FY_SSE4_2 1
#    else
#        define FY_SSE4_2 0
#    endif
#endif

#ifndef FY_SSE4A
#    if defined __SSE4A__
#        define FY_SSE4A 1
#    else
#        define FY_SSE4A 0
#    endif
#endif

#ifndef FY_AVX
#    if defined __AVX__ 
#        define FY_AVX 1
#    else
#        define FY_AVX 0
#    endif
#endif

// ------------------------------------------------------------------------
// FY_FP16C may be used to determine the existence of float <-> half conversion operations on an x86 CPU.
// (For example to determine if _mm_cvtph_ps or _mm_cvtps_ph could be used.)
#ifndef FY_FP16C
#    if defined __F16C__
#        define FY_FP16C 1
#    else
#        define FY_FP16C 0
#    endif
#endif

#ifndef THREAD_LOCAL
#    if __STDC_VERSION__ >= 201112 && !defined (__STDC_NO_THREADS__)
#         define THREAD_LOCAL _Thread_local
#    elif defined(_WIN32) && ( defined(_MSC_VER) ||  defined (__ICL) ||  defined (__DMC__) || defined (__BORLANDC__) )
#        define THREAD_LOCAL __declspec(thread) 
#    elif defined (__GNUC__) || defined (__SUNPRO_C) || defined (__xlC__)
#        define THREAD_LOCAL __thread
#    else
#        error "Cannot define THREAD_LOCAL"
#    endif
#endif

#ifndef FY_NOEXCEPT
#    if defined(__GNUC__) || _MSC_VER >= 1900
#        define FY_NOEXCEPT noexcept
#    else
#        define FY_NOEXCEPT throw()
#    endif
#endif

#ifndef FY_CONSTEXPR
#    if (((__GNUC__ * 100) + __GNUC_MINOR__) >= 407) || _MSC_VER >= 1900
#        define FY_CONSTEXPR constexpr
#    else
#        error "constexpr is not supported by your compiler, please upgrade!"
#    endif
#endif

#ifndef FY_FUNC_NAME
#    if defined(_WIN32) && defined (_MSC_VER)
#        define FY_FUNC_NAME __FUNCTION__
#    else
#        define FY_FUNC_NAME __func__
#    endif
#endif

#endif