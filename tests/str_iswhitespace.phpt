--TEST--
str_iswhitespace() function
--FILE--
<?php
var_dump(str_iswhitespace(""));
var_dump(str_iswhitespace(" "));
var_dump(str_iswhitespace("\t"));
var_dump(str_iswhitespace("\t \t"));
var_dump(str_iswhitespace("  "));
var_dump(str_iswhitespace("   "));
var_dump(str_iswhitespace("    "));
var_dump(str_iswhitespace(" a"));
var_dump(str_iswhitespace(" a "));
var_dump(str_iswhitespace("  a "));
var_dump(str_iswhitespace(" a\t"));
--EXPECT--
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(false)
bool(false)
bool(false)
bool(false)
