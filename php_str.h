#ifndef PHP_STR_H
#define PHP_STR_H

extern zend_module_entry str_module_entry;
#define phpext_str_ptr &str_module_entry

#ifdef PHP_WIN32
#   define PHP_STR_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#   define PHP_STR_API __attribute__ ((visibility("default")))
#else
#   define PHP_STR_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_MINIT_FUNCTION(str);
PHP_MSHUTDOWN_FUNCTION(str);
PHP_RINIT_FUNCTION(str);
PHP_RSHUTDOWN_FUNCTION(str);
PHP_MINFO_FUNCTION(str);

PHP_FUNCTION(str_startswith);
PHP_FUNCTION(str_endswith);
PHP_FUNCTION(str_isupper);
PHP_FUNCTION(str_islower);
PHP_FUNCTION(str_iswhitespace);
PHP_FUNCTION(str_swapcase);

/* 
    Declare any global variables you may need between the BEGIN
    and END macros here:     

ZEND_BEGIN_MODULE_GLOBALS(str)
    long  global_value;
    char *global_string;
ZEND_END_MODULE_GLOBALS(str)
*/

/* In every utility function you add that needs to use variables 
   in php_str_globals, call TSRMLS_FETCH(); after declaring other 
   variables used by that function, or better yet, pass in TSRMLS_CC
   after the last function argument and declare your utility function
   with TSRMLS_DC after the last declared argument.  Always refer to
   the globals in your function as STR_G(variable).  You are 
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

#ifdef ZTS
#define STR_G(v) TSRMG(str_globals_id, zend_str_globals *, v)
#else
#define STR_G(v) (str_globals.v)
#endif

#endif  /* PHP_STR_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
