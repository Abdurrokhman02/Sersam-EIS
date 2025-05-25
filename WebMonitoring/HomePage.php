<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>SERSAM Dashboard</title>
    <link rel="stylesheet" href="style.css">
</head>
<body>
    <header class="sersam-header">
    <div class="header-content">
        <div class="title-section">
            <h1>SERSAM</h1>
            <p>SERSAM (Serok Sampah Pintar untuk Membersihkan Aliran Sungai) jadilah sistem berbasis IoT yang difancang untuk mengelola sampah sungai secara otomatis guna mencegah banjir dan menjaga kebersihan lingkungan. Sistem ini mengintegrasikan perangkat keras dan lunak untuk meningkatkan efektivitas pembersihan sungai serta mendukung konsep smart device dalam pengelolaan sampah</p>
        </div>
        <div class="location-section">
            <div class="location-border">
                <svg class="location-icon" xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" fill="currentColor">
                    <path d="M12 2C8.13 2 5 5.13 5 9c0 5.25 7 13 7 13s7-7.75 7-13c0-3.87-3.13-7-7-7zm0 9.5c-1.38 0-2.5-1.12-2.5-2.5s1.12-2.5 2.5-2.5 2.5 1.12 2.5 2.5-1.12 2.5-2.5 2.5z"/>
                </svg>
                <span class="location-value">Sungai Ciliwung, Jakarta Pusat</span>
            </div>
        </div>
    </div>
</header>

        <div class="sersam-content">
            <div class="sensor-section">
                <h2>Status Sensor</h2>
                <div class="sensor-list">
                    <div class="sensor-item">
                        <div class="sensor-indicator active"></div>
                        <span>Sensor ketinggian sampah</span>
                    </div>
                    <div class="sensor-item">
                        <div class="sensor-indicator active"></div>
                        <span>Sensor pendeteksi sampah</span>
                    </div>
                    <div class="sensor-item">
                        <div class="sensor-indicator active"></div>
                        <span>Sensor ketinggian air</span>
                    </div>
                </div>

                <div class="sensor-data">
                    <div class="data-item">
                        <h3>Ketinggian Air</h3>
                        <div class="data-value" id="water-level">200 cm</div>
                        <div class="status-label safe">Aman</div>
                    </div>
                    <div class="data-item">
                        <h3>Ketinggian Sampah</h3>
                        <div class="data-value" id="trash-level">80 cm</div>
                        <div class="status-label not-full">Belum Penuh</div>
                    </div>
                </div>
                
                <div class="control-section">
                    <h3>Kontrol Ketinggian</h3>
                    <div class="control-item">
                        <label for="water-control">Ketinggian Air:</label>
                        <input type="range" id="water-control" min="0" max="300" value="200" step="10">
                        <span id="water-value">200 cm</span>
                    </div>
                    <div class="control-item">
                        <label for="trash-control">Ketinggian Sampah:</label>
                        <input type="range" id="trash-control" min="0" max="150" value="80" step="5">
                        <span id="trash-value">80 cm</span>
                    </div>
                </div>
            </div>

            <div class="log-section">
                <h2>Log Aktivitas</h2>
                <div class="log-container" id="activity-log">
                    <div class="log-entry">System initialized</div>
                    <div class="log-entry">Sensors activated</div>
                    <div class="log-entry">Water level: 200 cm</div>
                    <div class="log-entry">Trash level: 80 cm</div>
                    <div class="log-entry">All systems normal</div>
                </div>
            </div>
        </div>
    </div>

    <script src="script.js"></script>
</body>
</html>