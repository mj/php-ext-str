--TEST--
str_islower() function
--FILE--
<?php
var_dump(str_islower(""));
var_dump(str_islower(" "));
var_dump(str_islower(null));
var_dump(str_islower("a"));
var_dump(str_islower("A"));
var_dump(str_islower("AB"));
var_dump(str_islower("aB"));
var_dump(str_islower("ab"));
var_dump(str_islower("abC"));
var_dump(str_islower("abc"));
var_dump(str_islower(1));
--EXPECT--
bool(false)
bool(false)
bool(false)
bool(true)
bool(false)
bool(false)
bool(false)
bool(true)
bool(false)
bool(true)
bool(false)
