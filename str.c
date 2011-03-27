#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_str.h"

/* True global resources - no need for thread safety here */
static int le_str;

/* {{{ str_functions[]
 *
 * Every user visible function must have an entry in str_functions[].
 */
const zend_function_entry str_functions[] = {
    PHP_FE(str_startswith,      NULL)
    PHP_FE(str_endswith,        NULL)
    PHP_FE(str_isupper,         NULL)
    PHP_FE(str_islower,         NULL)
    PHP_FE(str_iswhitespace,    NULL)
    PHP_FE(str_swapcase,        NULL)
    {NULL, NULL, NULL}
};
/* }}} */

/* {{{ str_module_entry
 */
zend_module_entry str_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
    STANDARD_MODULE_HEADER,
#endif
    "str",
    str_functions,
    PHP_MINIT(str),
    PHP_MSHUTDOWN(str),
    NULL,
    NULL,
    PHP_MINFO(str),
#if ZEND_MODULE_API_NO >= 20010901
    "0.1", /* Replace with version number for your extension */
#endif
    STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_STR
ZEND_GET_MODULE(str)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("str.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_str_globals, str_globals)
    STD_PHP_INI_ENTRY("str.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_str_globals, str_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ php_str_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_str_init_globals(zend_str_globals *str_globals)
{
    str_globals->global_value = 0;
    str_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(str)
{
    /* If you have INI entries, uncomment these lines 
    REGISTER_INI_ENTRIES();
    */
    return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(str)
{
    /* uncomment this line if you have INI entries
    UNREGISTER_INI_ENTRIES();
    */
    return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(str)
{
    php_info_print_table_start();
    php_info_print_table_header(2, "str support", "enabled");
    php_info_print_table_end();

    /* Remove comments if you have entries in php.ini
    DISPLAY_INI_ENTRIES();
    */
}
/* }}} */

/* {{{ proto bool str_startswith(string haystack, string needle [, bool case_sensitivity])
   Binary safe optionally case sensitive check if haystack starts with needle */
PHP_FUNCTION(str_startswith)
{
    char *needle, *haystack;
    int needle_len, haystack_len, retval;
    zend_bool cs = 0;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss|b", &haystack, &haystack_len, &needle, &needle_len, &cs) == FAILURE) {
        RETURN_FALSE;
    }

    if (needle_len > haystack_len) {
        RETURN_FALSE;
    }

    if (cs) {
        retval = zend_binary_strncmp(needle, needle_len, haystack, haystack_len, needle_len);
    } else {
        retval = zend_binary_strncasecmp(needle, needle_len, haystack, haystack_len, needle_len);
    }

    RETURN_BOOL(retval == 0);
}
/* }}} */

/* {{{ proto bool str_endswith(string haystack, string needle [, bool case_sensitivity])
   Binary safe optionally case sensitive check if haystack ends with needle */
PHP_FUNCTION(str_endswith)
{
    char *needle, *haystack;
    int needle_len, haystack_len, retval;
    zend_bool cs = 0;
    
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss|b", &haystack, &haystack_len, &needle, &needle_len, &cs) == FAILURE) {
        RETURN_FALSE;
    }

    if (needle_len > haystack_len) {
        RETURN_FALSE;
    }

    if (cs) {
        retval = zend_binary_strncmp(needle, needle_len, haystack + (haystack_len - needle_len), needle_len, needle_len);
    } else {
        retval = zend_binary_strncasecmp(needle, needle_len, haystack + (haystack_len - needle_len), needle_len, needle_len);        
    }

    RETURN_BOOL(retval == 0);
}
/* }}} */

PHP_FUNCTION(str_isupper)
{
    unsigned char *input, *end;
    unsigned char c;
    int input_len;
    int has_cased_char = 0;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &input, &input_len) == FAILURE) {
        RETURN_FALSE;
    }

    /* A string must at least contain one cased character in order to
     * yield a positive result.
     */
    if (input_len == 0) {
        RETURN_FALSE;
    }
    
    /* Shortcut for strings that consist of just a single character. */
    if (input_len == 1) {
        RETURN_BOOL(isupper(*input) != 0)
    }
    
    for (end = input + input_len; input < end; input++) {
        c = *input;
        
        if (islower(c)) {
            RETURN_FALSE;
        }

        if (!has_cased_char && isupper(c)) {
            has_cased_char = 1;
        }
    }
    
    RETURN_BOOL(has_cased_char == 1);
}

PHP_FUNCTION(str_islower)
{
    unsigned char *input, *end;
    unsigned char c;
    int input_len;
    int has_cased_char = 0;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &input, &input_len) == FAILURE) {
        RETURN_FALSE;
    }

    /* A string must at least contain one cased character in order to
     * yield a positive result.
     */
    if (input_len == 0) {
        RETURN_FALSE;
    }
    
    /* Shortcut for strings that consist of just a single character. */
    if (input_len == 1) {
        RETURN_BOOL(islower(*input) != 0)
    }
    
    for (end = input + input_len; input < end; input++) {
        c = *input;
        
        if (isupper(c)) {
            RETURN_FALSE;
        }

        if (!has_cased_char && islower(c)) {
            has_cased_char = 1;
        }
    }
    
    RETURN_BOOL(has_cased_char == 1);
}

PHP_FUNCTION(str_iswhitespace)
{
    unsigned char *input, *end;
    int input_len;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &input, &input_len) == FAILURE) {
        RETURN_FALSE;
    }

    /* Empty strings yield a positive result.  Need to think about this some
     * more.
     */
    if (input_len == 0) {
        RETURN_TRUE;
    }
    
    /* Shortcut for strings that consist of just a single character. */
    if (input_len == 1) {
        RETURN_BOOL(isspace(*input))
    }
    
    for (end = input + input_len; input < end; input++) {
        if (!isspace(*input)) {
            RETURN_FALSE;
        }
    }

    RETURN_TRUE;
}

PHP_FUNCTION(str_swapcase)
{
    char *input;
    unsigned char *c, *end;
    int input_len;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &input, &input_len) == FAILURE) {
        RETURN_FALSE;
    }

    input = estrndup(input, input_len);

    c = (unsigned char *)input;
    end = (unsigned char *)c + input_len;

    while (c < end) {
        if (isupper(*c)) {
            *c = tolower(*c);
        } else if (islower(*c)) {
            *c = toupper(*c);
        }

        c++;
    }

    RETURN_STRINGL(input, input_len, 0)
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
