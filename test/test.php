<?php
$meta = new Meta(1,"a","你好",4);

print_r($meta);

$snowflake = new Snowflake($meta);

var_dump($snowflake);

$id = $snowflake->generate();

var_dump($id);

var_dump($snowflake->degenerate($id));
