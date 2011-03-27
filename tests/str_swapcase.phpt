--TEST--
str_swapcase() function
--FILE--
<?php
var_dump(str_swapcase("a"));
var_dump(str_swapcase("A"));
var_dump(str_swapcase("Ab"));
var_dump(str_swapcase("123"));
var_dump(str_swapcase("1a2A"));
--EXPECT--
string(1) "A"
string(1) "a"
string(2) "aB"
string(3) "123"
string(4) "1A2a"
