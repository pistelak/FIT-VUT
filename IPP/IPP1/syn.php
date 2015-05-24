<?php

#SYN:xpiste04

error_reporting(E_ERROR);

require 'library.php';

// zpracujeme parametry:
$params = getParams();

if (array_key_exists("help", $params) && $_SERVER[ "argc" ] != 2) {   // help muze byt jako jediny parametr...
    fwrite(STDERR, "ERR: Chyba pri zpracovani parametru!!\n");
    exit(1);
}
else if (array_key_exists("help", $params)) {
    printHelp();
    exit();
}

if (array_key_exists("input", $params )) {
    $GLOBALS[ "FILE_INPUT" ] = $params[ "input" ];
}
else {
    $GLOBALS[ "FILE_INPUT" ] = "php://stdin";
}

if (array_key_exists("output" , $params )) {
    $GLOBALS[ "FILE_OUTPUT" ] = $params[ "output" ];
}
else {
    $GLOBALS[ "FILE_OUTPUT" ] = "php://stdout";
}

if (array_key_exists("format", $params )) {                  
    $GLOBALS["FILE_FORMAT"] = $params[ "format" ];
}

// nacteni formatovaciho souboru 
$format_string 	= myFileGetContents($GLOBALS[ "FILE_FORMAT" ]);	

// nacteni vstupniho souboru
$input_string 	= myFileGetContents($GLOBALS[ "FILE_INPUT" ]);				


if (empty($format_string)) {
    $output_str = $input_string; 
}
else {
    // rozparsovani formatovaciho souboru
    $format_list    = parseFormatFile($format_string);                      

    $TagList        = makeTagList($input_string, $format_list);            

    $output_str     = insert_tags($input_string, $TagList);
}

if (array_key_exists("br", $params)) {
	$output_str = preg_replace("/\n/", "<br />\n", $output_str); 
}

// vystup do souboru...  
myFileOutput($GLOBALS[ "FILE_OUTPUT" ], $output_str);		

