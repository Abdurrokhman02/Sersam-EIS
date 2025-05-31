<?php
include_once "../config/database.php";

$query = "SELECT * FROM data ORDER BY waktu DESC LIMIT 1";
$result = mysqli_query($conn, $query);
$data = mysqli_fetch_assoc($result);

header('Content-Type: application/json');

echo json_encode([
    'ketinggianAir' => $data['ketinggianair'] ?? '--',
    'kapasitasSampah' => $data['kapasitassampah'] ?? '--',
    'waktu' => $data['waktu'] ?? 'N/A'
]);
?>
