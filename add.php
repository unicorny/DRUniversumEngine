<?php
$path = $argv[1];

$include = "include/$path.h";
$src = "src/$path.cpp";

$touch = "touch $include $src";
$git_add = "git add $include $src";

system($touch);
system($git_add);


