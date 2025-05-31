<?php
$host   = "localhost";
$user   = "root";
$pass   = "";
$db     = "sersam";

$conn = mysqli_connect($host,$user,$pass,$db);

if(!$conn){
    echo "Gagal Terhubung ke Database";
}