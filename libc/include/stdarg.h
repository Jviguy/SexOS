# if defined(__GNUC__) && (__GNUC__ < 3)

typedef char *va_list;

#  define __UX_ROUNDED(type) \
	(((sizeof(type) + sizeof(int) -1) / sizeof(int)) * sizeof(int))

#  define va_start(ap, rest)           (ap = (va_list) __builtin_next_arg(rest))
#  define va_arg(ap, type)             (ap = (va_list) ((char *) (ap) + __UX_ROUNDED(type)), *((type *) (void *) ((char *) (ap) - __UX_ROUNDED(type))))

# elif defined(__GNUC__)

typedef __builtin_va_list va_list;

#  define va_start(ap, rest)           __builtin_va_start(ap, rest)
#  define va_copy(dest, src)           __builtin_va_copy(ap)
#  define va_arg(ap, type)             __builtin_va_arg(ap, type)
#  define va_end(ap)                   __builtin_va_end(ap)

# elif defined(_MSC_VER)

#   ifdef __cplusplus
extern "C" void __cdecl __va_start(va_list *, ...);
#   else
void __cdecl __va_start(va_list *, ...);
#   endif

#  if defined(_M_AMD64)

typedef char *va_list;

#   define va_start(ap, rest)          __va_start(&ap, rest)

/* Declare the compiler-provided __va_start() builtin */
#  elif defined(_M_ALPHA)

typedef struct
{
	char *a0;
	int offset;
} va_list;

#  elif defined(_M_IX86)

typedef char *va_list;

#  elif defined(_M_IA64)

typedef char *va_list;

#  endif

# else

#  error Unsupported compiler

# endif

# ifndef va_copy
#  define va_copy(dest, src)           (dest) = (src)
# endif

# ifndef va_end
#  define va_end(ap)                   ((void) 0)
# endif
