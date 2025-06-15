<?php
include_once "../config/database.php";

if (isset($_POST['ketinggianair']) && isset($_POST['kapasitassampah'])) {
    $ketinggianAir = mysqli_real_escape_string($conn, $_POST['ketinggianair']);
    $kapasitasSampah = mysqli_real_escape_string($conn, $_POST['kapasitassampah']);

    $query = "INSERT INTO data (ketinggianair, kapasitassampah) VALUES ('$ketinggianAir', '$kapasitasSampah')";
    $result = mysqli_query($conn, $query);

    if ($result){
        echo "Berhasil Tersimpan ke Database";
    } else {
        echo "Gagal tersimpan ke Database: " . mysqli_error($conn);
    }
} else {
    echo "Data tidak lengkap.";
}
?>