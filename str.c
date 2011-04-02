#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_str.h"
#include "ext/standard/php_string.h"

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
    PHP_FE(str_contains,        NULL)
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
    "0.1",
#endif
    STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_STR
ZEND_GET_MODULE(str)
#endif

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(str)
{
    return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(str)
{
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

/* {{{ proto bool str_contains(string haystack, string needle [, bool case_sensitivity])
   Binary safe optionally case sensitive check if haystack contains needle */
PHP_FUNCTION(str_contains)
{
    char *needle, *haystack, *haystack_dup, *needle_dup;
    char *found = NULL;
    int needle_len, haystack_len;
    zend_bool cs = 0;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss|b", &haystack, &haystack_len, &needle, &needle_len, &cs) == FAILURE) {
        RETURN_FALSE;
    }

    if (needle_len > haystack_len || !haystack_len) {
        RETURN_FALSE;
    }

    if (!needle_len) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "Empty delimiter");
        RETURN_FALSE;
    }

    /* TODO: mimick more of the logic from strpos, i.e. make needle
     * a zval.
     */
    if (cs) {
        found = php_memnstr(haystack, needle, needle_len, haystack + haystack_len);
    } else {
        haystack_dup = estrndup(haystack, haystack_len);
        php_strtolower(haystack_dup, haystack_len);

        needle_dup = estrndup(needle, needle_len);
        php_strtolower(needle_dup, needle_len);
        found = php_memnstr(haystack_dup, needle_dup, needle_len, haystack_dup + haystack_len);
        
        efree(haystack_dup);
        efree(needle_dup);
    }

    RETURN_BOOL(found);
}
/* }}} */

/* {{{ proto bool str_isupper(string input)
   Checks if all cased characters in the given input string are uppercase and if the 
   string contains at least one cased character. */
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
/* }}} */

/* {{{ proto bool str_islower(string input)
   Checks if all cased characters in the given input string are lowercase and if the 
   string contains at least one cased character. */
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
/* }}} */

/* {{{ proto bool str_iswhitespace(string input)
   Checks if the given string is empty or contains whitespace characters
   only. */
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
/* }}} */

/* {{{ proto bool str_swapcase(string input)
   Converts uppercase characters in the given string to their
   lowercase representations and vice versa. */
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
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
