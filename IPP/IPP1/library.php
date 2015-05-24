<?php

#SYN:xpiste04

// GLOBALNI PROMENNE
$FILE_FORMAT    = "";
$FILE_INPUT     = "";
$FILE_OUTPUT    = "";

/**
 *  Funkce pro zpracovani parametru
 */
function getParams() {

    if ($_SERVER[ "argc" ] ==  1) {                                     // skript nelze spoustet bez zadani parametru
        Error();
    }

    $params = array();

    array_shift($_SERVER[ "argv" ]);                                    // shift (kvuli ceste k skriptu v argv)

    foreach( $_SERVER[ "argv" ] as $key => $arg ) {

        if (preg_match( '@\-\-(format|input|output)=(.+)@', $arg, $matches)) {    // odchyti parametry --(format|output|input)=file
            $key   = $matches[ 1 ];
            $value = $matches[ 2 ];

            if (array_key_exists($key, $params)) {                      // pokud uz takovy parametr existuje je to chyba
               Error();
            }

            $params[ $key ] = $value;
        }
        else if ( $arg === "--help" || $arg === "--br") {               // parametry --help || --br
            preg_match( '@\-\-(.+)@', $arg, $matches);                  // zjistim jestli se jedna o --br neb --help
            $key = $matches[ 1 ];

            if (array_key_exists($key, $params)) {                      // pokud uz existuje je to chyba...
                Error();
            }

            $params [ $key ] = true;
        }
        else if ( preg_match( '@(.*)@', $arg, $matches)) {              // vse ostatni taky chyba
            Error();
        }
    }

    return $params;
}



/**
 * Velmi kratka napoveda...
 */
function printHelp() {
    print ("Autor: Radek Pistelak, xpiste04@stud.fit.vutbr.cz"); 
    print ("Pouziti: viz zadani.\n");
}


/**
 * Fce vypise chybu podle toho ze ktere funkce byla volana...
 */
function Error() {

   $callers = debug_backtrace();

   if ($callers[1]["function"] ==  "getParams") {
       fwrite(STDERR, "ERR: Chyba pri zpracovani parametru!!\n");
       exit(1);
   }
   else if ($callers[1]["function"] == "format_file") {
       fwrite(STDERR, "ERR: Chyba pri zpracovani formatovaciho souboru!!\n");
       exit(4);
   }
   else if ($callers[1]["function"] == "myFileGetContents") {
       fwrite(STDERR, "ERR: Chyba pri cteni souboru!!\n");
       exit(2); 
   }
   else if ($callers[1]["function"] == "myFileOutput") {
        fwrite(STDERR, "ERR: Chyba pri zapisovani vystupniho souboru!!\n"); 
        exit(3); 
   }
   else if ($callers[1]["function"] == "insert_tags") {
        fwrite(STDERR, "ERR: 4!!\n"); 
        exit(4); 
   }
   else if ($callers[1]["function"] == "parseFormatFile") {
        fwrite(STDERR, "ERR: Chyba pri zpracovani formatovaciho souboru!!\n"); 
        exit(4);
   }
}

/**
 * Fce vraci ve vystupni promenne obsah souboru 
 *
 * @param $file_name       Nazev souboru, ktery se ma nacist
 * @return $content        Obsah souboru 
 */
function myFileGetContents($file_name) {

    if (!is_readable($file_name) && $file_name != "php://stdin") {
        if ($file_name == $GLOBALS["FILE_FORMAT"]) {
            return NULL; 
        }
        Error();
    }

    $content = file_get_contents($file_name);

    if (mb_detect_encoding($content, 'UTF-8', true) == false) {
        Error("Spatne kodovani souboru!");
    }

    return $content; 
}

/**
 * Fce vytiskne paramentr $content do prislusneho souboru $file_name
 *
 * @param $file_name       Nazev souboru, kam se ma tisknout
 * @return $content        Obsah souboru 
 */
function myFileOutput($file_name, $content) {

    $handle = @fopen($file_name, 'w');
    if (!is_resource($handle)) {
        Error();
    }
    else {
        fclose($handle);
        $err = file_put_contents($file_name, $content);
    }
}

function checkRegExp($string) {
       
    $check1 = "(%[^s|a|d|l|L|e|w|W|t|n|\.|\||!|\*|\+|\(|\)|%])|";
    $check2 = "(\.\.)|(!!+)|(\*\*)|(\+\+)|";
    $check3 = "(\.\+)|(\.\*)|(\.\|)|";
    $check4 = "(\+\*)|(\*\+)|(\*\|)|";
    $check5 = "(!\))|(\|\))|(\.\))|(\(\))|";
    $check6 = "(([^%]|^)!\()|";
    $check7 = "(!\.)|(!\|)|(!\*)|(!\+)";

    $validator = "/" . $check1 . $check2 . $check3 . $check4 . $check5 . $check6 . $check7 . "/";

    preg_match_all($validator, $string, $matches); 
    if (!empty($matches[0])) {
        return false;
    }

    return true; 
}

/**
 * Kriticka funkce, ktera ma za ukol vratit prevedeny reg. vyraz... 
 *
 * @param $RegExp   regularni vyraz odpovidajici zadani ze seznamu FormatList
 * @return mixed    odpovidajici reg. vyraz v jazyce PHP
 */
function convertRegExp($string) {
   
    $phpRegExp = $string; 

    $phpRegExp  = preg_replace("/\[/", "\[", $phpRegExp);
    $phpRegExp  = preg_replace("/\]/", "\]", $phpRegExp);
    $phpRegExp  = preg_replace("/\^/", "\^", $phpRegExp);
    $phpRegExp  = preg_replace("/\{/", "\{", $phpRegExp);
    $phpRegExp  = preg_replace("/\}/", "\}", $phpRegExp);
    $phpRegExp  = preg_replace("/\?/", "\?", $phpRegExp);
    $phpRegExp  = preg_replace("/\\\([[]^{}?])/", "\\\\\[$1]", $phpRegExp);
    $phpRegExp  = preg_replace("/\\$/", "\\\\$", $phpRegExp);
    $phpRegExp  = preg_replace("/\\\\([AzsSdDwWbntrfv])/", "\\\\\[$1]", $phpRegExp);

    $phpRegExp  = preg_replace("/%([.|!*+()%])/", "\\\\$1", $phpRegExp);
    $phpRegExp  = preg_replace("/!%(.)/", "[^\\\\$1]", $phpRegExp); 
    $phpRegExp  = preg_replace("/!(.)/", "[^$1]", $phpRegExp);
    $phpRegExp  = preg_replace("/(.)\.(.)/", "$1$2", $phpRegExp);
    $phpRegExp  = preg_replace("/%s/", "\s", $phpRegExp);
    $phpRegExp  = preg_replace("/%a/", "(.|\s)", $phpRegExp);
    $phpRegExp  = preg_replace("/%d/", "[0-9]", $phpRegExp);
    $phpRegExp  = preg_replace("/%l/", "[a-z]", $phpRegExp);
    $phpRegExp  = preg_replace("/%L/", "[A-Z]", $phpRegExp);
    $phpRegExp  = preg_replace("/%w/", "[a-zA-Z]", $phpRegExp);
    $phpRegExp  = preg_replace("/%W/", "[a-zA-Z0-9]", $phpRegExp);
    $phpRegExp  = preg_replace("/%t/", "[\t]", $phpRegExp);  
    $phpRegExp  = preg_replace("/%n/", "[\n]", $phpRegExp); 
    $phpRegExp  = preg_replace("/\//", "\/", $phpRegExp);

    $phpRegExp  = "/" . $phpRegExp . "/";

    return $phpRegExp;
}


/**
 * Funkce vymaze pripadne vyskyty prazdnch retezcu (odpovidajich reg. vyrazu)
 * 
 * @param $matches  pole se shodami po pouziti reg. vyrazu // Promenna se modifikuje!
 */
function removeEmptyMatch(&$matches) {

    foreach ($matches as $match) {
        foreach ($match as $key => $value) {
            if (isset($value[0]) && $value[0]== "") {
                unset($matches[0][$key]);
            }
        }
    }
}

/**
 * Aplikuje prevedeny RegExp na vstupni retezec a hleda vsechny vyskyty
 *
 * @param $INPUT    Vstupni retezec
 * @param $RegExp   ...
 * @return array    Pole indexu oteviracich a uzaviracich znacek pro danny RegExp
 */
function applyRegExp($INPUT, $RegExp) {

    if (@preg_match_all($RegExp, $INPUT, $matches, PREG_OFFSET_CAPTURE));

    removeEmptyMatch($matches);

    $i = 0;
    
    foreach ($matches[0] as $key => $index) {

        $first_flag[$i]     = $index[1];
        $second_flag[$i]    = $first_flag[$i] + strlen($index[0]);

        $i = $i + 1;
    }

    if (@$first_flag == NULL && @$second_flag == NULL) {    // nemusi dojit k "matchnuti" reg. vyrazem
        return NULL;
    }
    else
        return array ( $first_flag, $second_flag ); // indexy kam se umisti formatovaci znacky
}


/**
 * Fce dostane na vstup radek formatovaciho souboru a ten rozdeli do pole 
 * indexovaneho reg. vyrazem s polozkou typu pole obsahujiciho retezce
 * -> formatovaci klicova slova... 
 *
 * V pripade prazdneho radku vraci NULL.
 * V pripade chyby ukonci skript s prislusnym chybovym kodem. (4)
 *
 * @param $line         Radek formatovaciho souboru 
 * @param $list         Pole indexovane reg. vyrazem a na indexu obsahuje fmt. retezec
 * @return true|false   
 */
function parseFormatLine($line, &$list) {

    preg_match('@([^\t]*)\t+(.*)@', $line, $matches);

    $regexp = $matches [1];                                      // regexp
    $tmp    = $matches [2];                                      // "radek" format. prikazu (temp - carky...)

    if (checkRegExp($regexp) != true) {
        return false; 
    } 

    $format = preg_split('@[\s,]+@', $tmp);                      // pole format. prikazu co index to prikaz

    foreach ($format as $key => $tmp) {                          // najde pripadny chybny form. prikaz....

        preg_match('@(^bold$|^italic$|^underline$|^teletype$|^color:[a-fA-F0-9]{6}$|^size:[1-7]$)@', $tmp, $matches);
        if (empty($matches)) {
            return false;
        }

        $list[$regexp][] = $tmp;
    }

    return true; 
}

/**
 * Rozdeli formatovaci souboru na retezec reg. vyrazu a k nemu prislusne
 * formatovani 
 *
 * @param $content      Obsah souboru --format=();
 * @return $list        Vraci (pole!), kde klic je regexp a obsah "bunky" formatovaci retezec
 */
function parseFormatFile($content) {

    $list = array(); 

    $content = explode(PHP_EOL, $content);

    foreach ($content as $line) {

        if (empty($line)) continue; 

        if (parseFormatLine($line, $list) != true) 
            Error(); 
    }
    return $list;
}

/**
 * Vytvori z formatovaciho retezce prislusne oteviraci a zaviraci tagy...
 *
 * @param $format       Formatovaci retezec (druhy sloupec souboru --format=......)
 * @return array        Pole oteviracich a uzaviracich tagu
 */
function makeTags($format) {

    $openTags   = "";
    $closeTags  = "";

    foreach ($format as $str) {
        
        if ($str === "bold") {
            $openTags   .= "<b>";
            $closeTags   = "</b>" . $closeTags;
        }
        elseif ($str === "italic") {
            $openTags   .= "<i>";
            $closeTags   = "</i>" . $closeTags;
        }
        elseif ($str === "underline") {
            $openTags   .= "<u>";
            $closeTags   = "</u>" . $closeTags;
        }
        elseif ($str === "teletype") {
            $openTags   .= "<tt>";
            $closeTags   = "</tt>" . $closeTags;
        }
        elseif (preg_match('@^size:([1-7])@', $str, $matches)) {
            $openTags   .= "<font size=$matches[1]>";
            $closeTags   = "</font>" . $closeTags;
        }
        elseif (preg_match('@^color:([A-F0-9]{6})@', $str, $matches)) {
            $openTags   .= "<font color=#$matches[1]>";
            $closeTags   = "</font>" . $closeTags;
        }
    }

    return array ($openTags, $closeTags);
}

/**
 * Funkce vytvari "TagList" seznam pozic oteviracich a uzaviracich tagu...
 *
 * @param $INPUT            vstupni text...
 * @param $list             vystup fce format_file()
 * @return mixed            vracime "TagList"...
 */
function makeTagList($INPUT, $list) {

    $TagList["OPEN"] = array();
    $TagList["CLOSEOPEN"] = array();


    foreach ($list as $regexp => $format) {
        $phpRegExp  = convertRegExp($regexp);
        $indexs     = applyRegExp($INPUT, $phpRegExp);
        $tags       = makeTags($format);

        if (!$indexs[0] || !$indexs[1]) continue;       // pro kazdy radek nemusi dojit k nalazeni vrozu v textu...
        
        foreach ($indexs[0] as $open) {
            @$TagList["OPEN"][$open] .= $tags[0];
        }

        foreach ($indexs[1] as $close) {
            @$TagList["CLOSE"][$close] = $tags[1] . $TagList["CLOSE"][$close];
        }
    }

    return $TagList;
}

/**
 * Nalezeni tagu na minimalnim indexu (oteviraciho nebo uzaviraciho)...
 * TagList je zde modifikovan --> pri kazdem volani smazu znacku, kterou vracim...
 *
 * @param $TagList
 * @return array        Dvojice: index + prislusny tag
 */
function findMin(&$TagList) {

    $min = array_search(max($TagList["CLOSE"]), $TagList["CLOSE"]); // min inicializuji na nejvyssi index, ktery najdu
                                                                    // u zaviraci znacky...
    foreach ($TagList["OPEN"] as $index => $tmp) {
        if ($min > $index) {
            $min = $index;
        }
    }

    $close = false;

    foreach ($TagList["CLOSE"] as $index => $tmp) {
        if ($min >= $index) {
            $min = $index;
            $close = true;
        }
    }

    if ($close === true) { //
        $ret = array ($min, $TagList["CLOSE"][$min]);
        unset($TagList["CLOSE"][$min]);
        return $ret;
    }
    else {
        $ret = array ($min, $TagList["OPEN"][$min]);
        unset ($TagList["OPEN"][$min]);
        return $ret;
    }
}

/**
 * Funkce vklada tagy na urcene pozice (+ OFFSET)
 * Format vstupnich dat:
 *
 * @param $TagList      Seznam pozic a tagu na techto pozicich
 * @param $INPUT        Vstupni retezec
 * @return mixed        Vystupni retezec
 */
function insert_tags($INPUT, $TagList) {

    if ($TagList == NULL) {
        return $INPUT;
    }

    $OFFSET = 0;                // pokud vlozim nejaky tag ze seznamu dalsi se bude vkladat na svuj index + offset
                                // (delku predchozich vlozenych tagu)

    $OUTPUT = $INPUT;           // nechci modifikovat vstupni prom...

    while (!empty($TagList["CLOSE"])) {
        $tmp        = findMin($TagList);                                    // najdu prvni tag, ktery se ma vkladat
        $position   = $tmp[0];
        $tag        = $tmp[1];
        $OUTPUT     = substr_replace($OUTPUT, $tag, $position+$OFFSET, 0);
        $OFFSET     = $OFFSET + strlen($tag);
    }

    return $OUTPUT;
}
