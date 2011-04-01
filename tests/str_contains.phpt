--TEST--
str_contains() function
--FILE--
<?php
var_dump(str_contains("abc", "a"));
var_dump(str_contains("abc", "b"));
var_dump(str_contains("abc", "c"));
var_dump(str_contains("abc", "ab"));
var_dump(str_contains("abc", "bc"));
var_dump(str_contains("abc", "abc"));
var_dump(str_contains("abc", "A"));
var_dump(str_contains("abc", "B"));
var_dump(str_contains("abc", "C"));
var_dump(str_contains("abc", "ABC"));
var_dump(str_contains("abc", "ABC", true));
var_dump(str_contains("abc", "XYZ", true));
var_dump(str_contains("123", 1));
var_dump(str_contains("123", 2));
--EXPECT--
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(false)
bool(false)
bool(true)
bool(true)
