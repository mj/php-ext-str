--TEST--
str_endswith() function
--FILE--
<?php
var_dump(str_endswith("a", "a"));
var_dump(str_endswith("", ""));
var_dump(str_endswith("ab", "b"));
var_dump(str_endswith("a", "ab"));
var_dump(str_endswith(-1, 1));
var_dump(str_endswith("abc", "abc"));
var_dump(str_endswith("hello", "la"));
var_dump(str_endswith("ABCd", "D"));
var_dump(str_endswith("ABCd", "D", true));
var_dump(str_endswith("Hellö", "ö"));
var_dump(str_endswith("abc", ""));
--EXPECT--
bool(true)
bool(true)
bool(true)
bool(false)
bool(true)
bool(true)
bool(false)
bool(true)
bool(false)
bool(true)
bool(true)
