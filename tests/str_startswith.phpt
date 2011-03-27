--TEST--
str_startswith() function
--FILE--
<?php
var_dump(str_startswith("a", "a"));
var_dump(str_startswith("", ""));
var_dump(str_startswith("ab", "b"));
var_dump(str_startswith("a", "ab"));
var_dump(str_startswith(-1, 1));
var_dump(str_startswith("a", null));
var_dump(str_startswith("abc", "abc"));
var_dump(str_startswith("Ä", "Ä"));
var_dump(str_startswith("A", "a"));
var_dump(str_startswith("A", "a", true));
var_dump(str_startswith("a", "A"));
--EXPECT--
bool(true)
bool(true)
bool(false)
bool(false)
bool(false)
bool(true)
bool(true)
bool(true)
bool(true)
bool(false)
bool(true)
