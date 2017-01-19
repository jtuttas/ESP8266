<?php

if (isset($_GET["ip"])) {
    $r = file_get_contents($_GET["ip"]);
    if ($r) {
        header($http_response_header[1]);
        echo $r;
    }
    else {
        header("HTTP/1.1 404 Not Found");
    }
}
else {
    echo "no ip paramater";
}
?>
