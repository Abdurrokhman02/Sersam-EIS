<?php
include_once "../config/database.php";

// Query untuk mengambil beberapa data terbaru (misal 10)
$query = "SELECT waktu, aktivitas FROM log_aktivitas ORDER BY waktu DESC LIMIT 10";
$result = mysqli_query($conn, $query);

$logs = [];

if ($result) {
    while ($row = mysqli_fetch_assoc($result)) {
        // Pastikan kolom aktivitas dan waktu ada di tabel
        $logs[] = [
            'waktu' => $row['waktu'] ?? 'N/A',
            'aktivitas' => $row['aktivitas'] ?? 'Tidak ada aktivitas'
        ];
    }
}

header('Content-Type: application/json');
echo json_encode($logs);
?>
