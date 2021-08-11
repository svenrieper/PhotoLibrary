#include <gio/gio.h>

#if defined (__ELF__) && ( __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 6))
# define SECTION __attribute__ ((section (".gresource.grey_pixel"), aligned (8)))
#else
# define SECTION
#endif

#ifdef _MSC_VER
static const SECTION union { const guint8 data[677]; const double alignment; void * const ptr;}  grey_pixel_resource_data = { {
  0107, 0126, 0141, 0162, 0151, 0141, 0156, 0164, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 
  0030, 0000, 0000, 0000, 0164, 0000, 0000, 0000, 0000, 0000, 0000, 0050, 0003, 0000, 0000, 0000, 
  0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0001, 0000, 0000, 0000, 0346, 0321, 0211, 0042, 
  0002, 0000, 0000, 0000, 0164, 0000, 0000, 0000, 0006, 0000, 0114, 0000, 0174, 0000, 0000, 0000, 
  0200, 0000, 0000, 0000, 0161, 0263, 0017, 0117, 0000, 0000, 0000, 0000, 0200, 0000, 0000, 0000, 
  0016, 0000, 0166, 0000, 0220, 0000, 0000, 0000, 0235, 0002, 0000, 0000, 0324, 0265, 0002, 0000, 
  0377, 0377, 0377, 0377, 0235, 0002, 0000, 0000, 0001, 0000, 0114, 0000, 0240, 0002, 0000, 0000, 
  0244, 0002, 0000, 0000, 0151, 0155, 0141, 0147, 0145, 0057, 0000, 0000, 0001, 0000, 0000, 0000, 
  0147, 0162, 0145, 0171, 0137, 0160, 0151, 0170, 0145, 0154, 0056, 0160, 0156, 0147, 0000, 0000, 
  0375, 0001, 0000, 0000, 0000, 0000, 0000, 0000, 0211, 0120, 0116, 0107, 0015, 0012, 0032, 0012, 
  0000, 0000, 0000, 0015, 0111, 0110, 0104, 0122, 0000, 0000, 0000, 0001, 0000, 0000, 0000, 0001, 
  0010, 0002, 0000, 0000, 0000, 0220, 0167, 0123, 0336, 0000, 0000, 0001, 0204, 0151, 0103, 0103, 
  0120, 0111, 0103, 0103, 0040, 0160, 0162, 0157, 0146, 0151, 0154, 0145, 0000, 0000, 0050, 0221, 
  0175, 0221, 0075, 0110, 0303, 0100, 0034, 0305, 0137, 0133, 0155, 0105, 0053, 0016, 0126, 0020, 
  0161, 0310, 0120, 0235, 0054, 0210, 0212, 0070, 0152, 0025, 0212, 0120, 0041, 0324, 0012, 0255, 
  0072, 0230, 0134, 0372, 0005, 0115, 0032, 0222, 0024, 0027, 0107, 0301, 0265, 0340, 0340, 0307, 
  0142, 0325, 0301, 0305, 0131, 0127, 0007, 0127, 0101, 0020, 0374, 0000, 0161, 0162, 0164, 0122, 
  0164, 0221, 0022, 0377, 0227, 0024, 0132, 0304, 0170, 0160, 0334, 0217, 0167, 0367, 0036, 0167, 
  0357, 0000, 0177, 0275, 0314, 0124, 0263, 0143, 0034, 0120, 0065, 0313, 0110, 0045, 0342, 0102, 
  0046, 0273, 0052, 0204, 0136, 0321, 0203, 0020, 0006, 0020, 0104, 0120, 0142, 0246, 0076, 0047, 
  0212, 0111, 0170, 0216, 0257, 0173, 0370, 0370, 0172, 0027, 0343, 0131, 0336, 0347, 0376, 0034, 
  0275, 0112, 0316, 0144, 0200, 0117, 0040, 0236, 0145, 0272, 0141, 0021, 0157, 0020, 0117, 0157, 
  0132, 0072, 0347, 0175, 0342, 0010, 0053, 0112, 0012, 0361, 0071, 0361, 0230, 0101, 0027, 0044, 
  0176, 0344, 0272, 0354, 0362, 0033, 0347, 0202, 0303, 0176, 0236, 0031, 0061, 0322, 0251, 0171, 
  0342, 0010, 0261, 0120, 0150, 0143, 0271, 0215, 0131, 0321, 0120, 0211, 0247, 0210, 0243, 0212, 
  0252, 0121, 0276, 0077, 0343, 0262, 0302, 0171, 0213, 0263, 0132, 0256, 0262, 0346, 0075, 0371, 
  0013, 0303, 0071, 0155, 0145, 0231, 0353, 0064, 0207, 0221, 0300, 0042, 0226, 0040, 0102, 0200, 
  0214, 0052, 0112, 0050, 0303, 0102, 0214, 0126, 0215, 0024, 0023, 0051, 0332, 0217, 0173, 0370, 
  0207, 0034, 0277, 0110, 0056, 0231, 0134, 0045, 0060, 0162, 0054, 0240, 0002, 0025, 0222, 0343, 
  0007, 0377, 0203, 0337, 0335, 0232, 0371, 0311, 0011, 0067, 0051, 0034, 0007, 0072, 0137, 0154, 
  0373, 0143, 0004, 0010, 0355, 0002, 0215, 0232, 0155, 0177, 0037, 0333, 0166, 0343, 0004, 0010, 
  0074, 0003, 0127, 0132, 0313, 0137, 0251, 0003, 0063, 0237, 0244, 0327, 0132, 0132, 0364, 0010, 
  0350, 0333, 0006, 0056, 0256, 0133, 0232, 0274, 0007, 0134, 0356, 0000, 0203, 0117, 0272, 0144, 
  0110, 0216, 0024, 0240, 0351, 0317, 0347, 0201, 0367, 0063, 0372, 0246, 0054, 0320, 0177, 0013, 
  0164, 0257, 0271, 0275, 0065, 0367, 0161, 0372, 0000, 0244, 0251, 0253, 0344, 0015, 0160, 0160, 
  0010, 0214, 0026, 0050, 0173, 0335, 0343, 0335, 0135, 0355, 0275, 0375, 0173, 0246, 0331, 0337, 
  0017, 0351, 0331, 0162, 0160, 0311, 0103, 0357, 0026, 0000, 0000, 0000, 0011, 0160, 0110, 0131, 
  0163, 0000, 0000, 0056, 0043, 0000, 0000, 0056, 0043, 0001, 0170, 0245, 0077, 0166, 0000, 0000, 
  0000, 0007, 0164, 0111, 0115, 0105, 0007, 0344, 0013, 0007, 0025, 0011, 0057, 0313, 0370, 0035, 
  0027, 0000, 0000, 0000, 0014, 0111, 0104, 0101, 0124, 0010, 0327, 0143, 0010, 0015, 0015, 0005, 
  0000, 0002, 0002, 0001, 0000, 0361, 0125, 0270, 0244, 0000, 0000, 0000, 0000, 0111, 0105, 0116, 
  0104, 0256, 0102, 0140, 0202, 0000, 0000, 0050, 0165, 0165, 0141, 0171, 0051, 0057, 0000, 0000, 
  0000, 0000, 0000, 0000
} };
#else /* _MSC_VER */
static const SECTION union { const guint8 data[677]; const double alignment; void * const ptr;}  grey_pixel_resource_data = {
  "\107\126\141\162\151\141\156\164\000\000\000\000\000\000\000\000"
  "\030\000\000\000\164\000\000\000\000\000\000\050\003\000\000\000"
  "\000\000\000\000\000\000\000\000\001\000\000\000\346\321\211\042"
  "\002\000\000\000\164\000\000\000\006\000\114\000\174\000\000\000"
  "\200\000\000\000\161\263\017\117\000\000\000\000\200\000\000\000"
  "\016\000\166\000\220\000\000\000\235\002\000\000\324\265\002\000"
  "\377\377\377\377\235\002\000\000\001\000\114\000\240\002\000\000"
  "\244\002\000\000\151\155\141\147\145\057\000\000\001\000\000\000"
  "\147\162\145\171\137\160\151\170\145\154\056\160\156\147\000\000"
  "\375\001\000\000\000\000\000\000\211\120\116\107\015\012\032\012"
  "\000\000\000\015\111\110\104\122\000\000\000\001\000\000\000\001"
  "\010\002\000\000\000\220\167\123\336\000\000\001\204\151\103\103"
  "\120\111\103\103\040\160\162\157\146\151\154\145\000\000\050\221"
  "\175\221\075\110\303\100\034\305\137\133\155\105\053\016\126\020"
  "\161\310\120\235\054\210\212\070\152\025\212\120\041\324\012\255"
  "\072\230\134\372\005\115\032\222\024\027\107\301\265\340\340\307"
  "\142\325\301\305\131\127\007\127\101\020\374\000\161\162\164\122"
  "\164\221\022\377\227\024\132\304\170\160\334\217\167\367\036\167"
  "\357\000\177\275\314\124\263\143\034\120\065\313\110\045\342\102"
  "\046\273\052\204\136\321\203\020\006\020\104\120\142\246\076\047"
  "\212\111\170\216\257\173\370\370\172\027\343\131\336\347\376\034"
  "\275\112\316\144\200\117\040\236\145\272\141\021\157\020\117\157"
  "\132\072\347\175\342\010\053\112\012\361\071\361\230\101\027\044"
  "\176\344\272\354\362\033\347\202\303\176\236\031\061\322\251\171"
  "\342\010\261\120\150\143\271\215\131\321\120\211\247\210\243\212"
  "\252\121\276\077\343\262\302\171\213\263\132\256\262\346\075\371"
  "\013\303\071\155\145\231\353\064\207\221\300\042\226\040\102\200"
  "\214\052\112\050\303\102\214\126\215\024\023\051\332\217\173\370"
  "\207\034\277\110\056\231\134\045\060\162\054\240\002\025\222\343"
  "\007\377\203\337\335\232\371\311\011\067\051\034\007\072\137\154"
  "\373\143\004\010\355\002\215\232\155\177\037\333\166\343\004\010"
  "\074\003\127\132\313\137\251\003\063\237\244\327\132\132\364\010"
  "\350\333\006\056\256\133\232\274\007\134\356\000\203\117\272\144"
  "\110\216\024\240\351\317\347\201\367\063\372\246\054\320\177\013"
  "\164\257\271\275\065\367\161\372\000\244\251\253\344\015\160\160"
  "\010\214\026\050\173\335\343\335\135\355\275\375\173\246\331\337"
  "\017\351\331\162\160\311\103\357\026\000\000\000\011\160\110\131"
  "\163\000\000\056\043\000\000\056\043\001\170\245\077\166\000\000"
  "\000\007\164\111\115\105\007\344\013\007\025\011\057\313\370\035"
  "\027\000\000\000\014\111\104\101\124\010\327\143\010\015\015\005"
  "\000\002\002\001\000\361\125\270\244\000\000\000\000\111\105\116"
  "\104\256\102\140\202\000\000\050\165\165\141\171\051\057\000\000"
  "\000\000\000\000" };
#endif /* !_MSC_VER */

static GStaticResource static_resource = { grey_pixel_resource_data.data, sizeof (grey_pixel_resource_data.data) - 1 /* nul terminator */, NULL, NULL, NULL };

G_MODULE_EXPORT
GResource *grey_pixel_get_resource (void);
GResource *grey_pixel_get_resource (void)
{
  return g_static_resource_get_resource (&static_resource);
}
/*
  If G_HAS_CONSTRUCTORS is true then the compiler support *both* constructors and
  destructors, in a usable way, including e.g. on library unload. If not you're on
  your own.

  Some compilers need #pragma to handle this, which does not work with macros,
  so the way you need to use this is (for constructors):

  #ifdef G_DEFINE_CONSTRUCTOR_NEEDS_PRAGMA
  #pragma G_DEFINE_CONSTRUCTOR_PRAGMA_ARGS(my_constructor)
  #endif
  G_DEFINE_CONSTRUCTOR(my_constructor)
  static void my_constructor(void) {
   ...
  }

*/

#ifndef __GTK_DOC_IGNORE__

#if  __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 7)

#define G_HAS_CONSTRUCTORS 1

#define G_DEFINE_CONSTRUCTOR(_func) static void __attribute__((constructor)) _func (void);
#define G_DEFINE_DESTRUCTOR(_func) static void __attribute__((destructor)) _func (void);

#elif defined (_MSC_VER) && (_MSC_VER >= 1500)
/* Visual studio 2008 and later has _Pragma */

#include <stdlib.h>

#define G_HAS_CONSTRUCTORS 1

/* We do some weird things to avoid the constructors being optimized
 * away on VS2015 if WholeProgramOptimization is enabled. First we
 * make a reference to the array from the wrapper to make sure its
 * references. Then we use a pragma to make sure the wrapper function
 * symbol is always included at the link stage. Also, the symbols
 * need to be extern (but not dllexport), even though they are not
 * really used from another object file.
 */

/* We need to account for differences between the mangling of symbols
 * for x86 and x64/ARM/ARM64 programs, as symbols on x86 are prefixed
 * with an underscore but symbols on x64/ARM/ARM64 are not.
 */
#ifdef _M_IX86
#define G_MSVC_SYMBOL_PREFIX "_"
#else
#define G_MSVC_SYMBOL_PREFIX ""
#endif

#define G_DEFINE_CONSTRUCTOR(_func) G_MSVC_CTOR (_func, G_MSVC_SYMBOL_PREFIX)
#define G_DEFINE_DESTRUCTOR(_func) G_MSVC_DTOR (_func, G_MSVC_SYMBOL_PREFIX)

#define G_MSVC_CTOR(_func,_sym_prefix) \
  static void _func(void); \
  extern int (* _array ## _func)(void);              \
  int _func ## _wrapper(void) { _func(); g_slist_find (NULL,  _array ## _func); return 0; } \
  __pragma(comment(linker,"/include:" _sym_prefix # _func "_wrapper")) \
  __pragma(section(".CRT$XCU",read)) \
  __declspec(allocate(".CRT$XCU")) int (* _array ## _func)(void) = _func ## _wrapper;

#define G_MSVC_DTOR(_func,_sym_prefix) \
  static void _func(void); \
  extern int (* _array ## _func)(void);              \
  int _func ## _constructor(void) { atexit (_func); g_slist_find (NULL,  _array ## _func); return 0; } \
   __pragma(comment(linker,"/include:" _sym_prefix # _func "_constructor")) \
  __pragma(section(".CRT$XCU",read)) \
  __declspec(allocate(".CRT$XCU")) int (* _array ## _func)(void) = _func ## _constructor;

#elif defined (_MSC_VER)

#define G_HAS_CONSTRUCTORS 1

/* Pre Visual studio 2008 must use #pragma section */
#define G_DEFINE_CONSTRUCTOR_NEEDS_PRAGMA 1
#define G_DEFINE_DESTRUCTOR_NEEDS_PRAGMA 1

#define G_DEFINE_CONSTRUCTOR_PRAGMA_ARGS(_func) \
  section(".CRT$XCU",read)
#define G_DEFINE_CONSTRUCTOR(_func) \
  static void _func(void); \
  static int _func ## _wrapper(void) { _func(); return 0; } \
  __declspec(allocate(".CRT$XCU")) static int (*p)(void) = _func ## _wrapper;

#define G_DEFINE_DESTRUCTOR_PRAGMA_ARGS(_func) \
  section(".CRT$XCU",read)
#define G_DEFINE_DESTRUCTOR(_func) \
  static void _func(void); \
  static int _func ## _constructor(void) { atexit (_func); return 0; } \
  __declspec(allocate(".CRT$XCU")) static int (* _array ## _func)(void) = _func ## _constructor;

#elif defined(__SUNPRO_C)

/* This is not tested, but i believe it should work, based on:
 * http://opensource.apple.com/source/OpenSSL098/OpenSSL098-35/src/fips/fips_premain.c
 */

#define G_HAS_CONSTRUCTORS 1

#define G_DEFINE_CONSTRUCTOR_NEEDS_PRAGMA 1
#define G_DEFINE_DESTRUCTOR_NEEDS_PRAGMA 1

#define G_DEFINE_CONSTRUCTOR_PRAGMA_ARGS(_func) \
  init(_func)
#define G_DEFINE_CONSTRUCTOR(_func) \
  static void _func(void);

#define G_DEFINE_DESTRUCTOR_PRAGMA_ARGS(_func) \
  fini(_func)
#define G_DEFINE_DESTRUCTOR(_func) \
  static void _func(void);

#else

/* constructors not supported for this compiler */

#endif

#endif /* __GTK_DOC_IGNORE__ */

#ifdef G_HAS_CONSTRUCTORS

#ifdef G_DEFINE_CONSTRUCTOR_NEEDS_PRAGMA
#pragma G_DEFINE_CONSTRUCTOR_PRAGMA_ARGS(resource_constructor)
#endif
G_DEFINE_CONSTRUCTOR(resource_constructor)
#ifdef G_DEFINE_DESTRUCTOR_NEEDS_PRAGMA
#pragma G_DEFINE_DESTRUCTOR_PRAGMA_ARGS(resource_destructor)
#endif
G_DEFINE_DESTRUCTOR(resource_destructor)

#else
#warning "Constructor not supported on this compiler, linking in resources will not work"
#endif

static void resource_constructor (void)
{
  g_static_resource_init (&static_resource);
}

static void resource_destructor (void)
{
  g_static_resource_fini (&static_resource);
}
