<?php
// SERSAM/index.php

// Set judul halaman spesifik untuk halaman ini
$page_title = "Dashboard Monitoring SERSAM";

// Memuat header (yang juga akan memuat navbar)
// __DIR__ adalah konstanta magic PHP yang mengembalikan direktori file saat ini.
if (file_exists(__DIR__ . '/layout/header.php')) {
    require_once __DIR__ . '/layout/header.php';
} else {
    die("Error: File header.php tidak ditemukan.");
}

// Tidak perlu require navbar.php lagi di sini karena sudah di-handle oleh header.php
?>

<main class="sersam-content">
    <div class="left-column">
        <div class="card status-sensor-card">
            <h2>Status Sensor</h2>
            <div class="sensor-list">
                <div class="sensor-item">
                    <span>Sensor ketinggian sampah</span>
                    <div class="sensor-icon inactive" id="sensor_ketinggian_sampah_status_icon">-</div>
                </div>
                <div class="sensor-item">
                    <span>Sensor pendeteksi sampah</span>
                    <div class="sensor-icon inactive" id="sensor_pendeteksi_sampah_status_icon">-</div>
                </div>
                <div class="sensor-item">
                    <span>Sensor ketinggian air</span>
                    <div class="sensor-icon inactive" id="sensor_ketinggian_air_status_icon">-</div>
                </div>
            </div>
        </div>

        <div class="card data-display-card">
            <div class="card-header">
                <h3>Ketinggian Air</h3>
                <span id="water-level-value-display">0 cm</span>
            </div>
            <input type="range" id="water-level-slider" min="0" max="300" value="0" class="level-slider" style="pointer-events: none;">
            <div class="progress-bar-container">
                <div class="progress-bar water-level-bar" id="water-progress-bar" style="width: 0%;">
                    <span class="progress-value" id="water-progress-value">0 cm</span>
                </div>
            </div>
            <span class="status-label" id="water-status-label">Memuat...</span>
        </div>

        <div class="card data-display-card">
            <div class="card-header">
                <h3>Ketinggian Sampah</h3>
                <span id="trash-level-value-display">0 cm</span>
            </div>
            <input type="range" id="trash-level-slider" min="0" max="150" value="0" class="level-slider" style="pointer-events: none;">
            <div class="progress-bar-container">
                <div class="progress-bar trash-level-bar" id="trash-progress-bar" style="width: 0%;">
                     <span class="progress-value" id="trash-progress-value">0 cm</span>
                </div>
            </div>
            <span class="status-label" id="trash-status-label">Memuat...</span>
        </div>
    </div>

    <div class="right-column">
        <div class="card log-activity-card">
            <h2>Log Aktivitas</h2>
            <div class="log-container" id="log_activity_container">
                <div class="log-entry-item">Memuat log aktivitas...</div>
            </div>
        </div>

        <?php
        // Memuat footer
        if (file_exists(__DIR__ . '/layout/footer.php')) {
            require_once __DIR__ . '/layout/footer.php';
        } else {
            die("Error: File footer.php tidak ditemukan.");
        }
        ?>
        
    </div>
</main>

