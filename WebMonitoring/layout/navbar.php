<?php
// SERSAM/layout/navbar.php
// BASE_URL seharusnya sudah didefinisikan di header.php
?>
<header class="sersam-header-main">
    <div class="header-content">
        <div class="title-section">
            <h1><a href="<?php echo BASE_URL; ?>">SERSAM</a></h1>
            <p>SERSAM (Serok Sampah Pintar untuk Membersihkan Aliran Sungai) adalah sistem berbasis IoT yang dirancang untuk mengelola sampah sungai secara otomatis guna mencegah banjir dan menjaga kebersihan lingkungan. Sistem ini mengintegrasikan perangkat keras dan lunak untuk meningkatkan efektivitas pembersihan sungai serta mendukung konsep smart device dalam pengelolaan sampah.</p>
        </div>
        <div class="location-section">
            <div class="location-border">
                <svg class="location-icon" xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" fill="currentColor">
                    <path d="M12 2C8.13 2 5 5.13 5 9c0 5.25 7 13 7 13s7-7.75 7-13c0-3.87-3.13-7-7-7zm0 9.5a2.5 2.5 0 0 1 0-5 2.5 2.5 0 0 1 0 5z"/>
                </svg>
                <span class="location-value" id="dynamic_location_value">Telukjambe, Karawang</span> </div>
        </div>
    </div>
</header>