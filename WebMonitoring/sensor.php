<?php
// Koneksi database
$servername = "localhost";
$username = "username_db";
$password = "password_db";
$dbname = "nama_db";

$conn = new mysqli($servername, $username, $password, $dbname);

// Terima data dari Arduino
$air = $_POST['air'];
$sampah = $_POST['sampah'];

// Simpan ke database
$sql = "INSERT INTO sensor_data (air, sampah, waktu) VALUES ($air, $sampah, NOW())";
$conn->query($sql);

// Simpan juga ke file JSON untuk dibaca dashboard
$data = array(
    'air' => $air,
    'sampah' => $sampah,
    'waktu' => date('Y-m-d H:i:s')
);
file_put_contents('sensor-data.json', json_encode($data));

$conn->close();
echo "Data received";
?>