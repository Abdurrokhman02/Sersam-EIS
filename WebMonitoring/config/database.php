<?php
require_once __DIR__ . '../api/api.php';

$conn = mysqli_connect($host,$user,$pass,$db);

if(!$conn){
     die("Koneksi database gagal: " . mysqli_connect_error());
}
?>