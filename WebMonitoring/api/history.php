<?php
include_once "../config/database.php";

// Query yang sesuai dengan struktur tabel data
$query = "SELECT waktu, 
                 CONCAT('Ketinggian Air: ', ketinggianair, ' cm, Kapasitas Sampah: ', kapasitassampah, ' cm') AS aktivitas 
          FROM data 
          ORDER BY waktu DESC 
          LIMIT 10000";

$result = mysqli_query($conn, $query);

if (!$result) {
    // Debugging: Tampilkan error langsung untuk development
    die(json_encode([
        'error' => true,
        'message' => 'Database error: ' . mysqli_error($conn)
    ]));
}

$logs = [];
while ($row = mysqli_fetch_assoc($result)) {
    $logs[] = [
        'waktu' => $row['waktu'] ?? 'N/A',
        'aktivitas' => $row['aktivitas'] ?? 'Tidak ada aktivitas'
    ];
}

header('Content-Type: application/json');
echo json_encode($logs);
?>