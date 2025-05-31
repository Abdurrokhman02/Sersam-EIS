document.addEventListener('DOMContentLoaded', function () {
    // Selektor Elemen untuk Ketinggian Air
    const waterLevelSlider = document.getElementById('water-level-slider');
    const waterLevelValueDisplay = document.getElementById('water-level-value-display');
    const waterProgressBar = document.getElementById('water-progress-bar');
    const waterProgressValue = document.getElementById('water-progress-value');
    const waterStatusLabel = document.getElementById('water-status-label');

    // Selektor Elemen untuk Ketinggian Sampah
    const trashLevelSlider = document.getElementById('trash-level-slider');
    const trashLevelValueDisplay = document.getElementById('trash-level-value-display');
    const trashProgressBar = document.getElementById('trash-progress-bar');
    const trashProgressValue = document.getElementById('trash-progress-value');
    const trashStatusLabel = document.getElementById('trash-status-label');

    // Selektor untuk ikon status sensor
    const sensorKetinggianSampahIcon = document.getElementById('sensor_ketinggian_sampah_status_icon');
    const sensorPendeteksiSampahIcon = document.getElementById('sensor_pendeteksi_sampah_status_icon');
    const sensorKetinggianAirIcon = document.getElementById('sensor_ketinggian_air_status_icon');


    const MAX_WATER_LEVEL = 300; // cm, sesuaikan dengan max slider
    const MAX_TRASH_LEVEL = 150; // cm, sesuaikan dengan max slider

    function updateLevelUI(currentValue, maxValue, unit, type) {
        const percentage = Math.max(0, Math.min((currentValue / maxValue) * 100, 100)); // Pastikan persentase antara 0 dan 100
        let sliderElement, valueDisplayElement, progressBarElement, progressValueElement, statusLabelElement;

        if (type === 'water') {
            sliderElement = waterLevelSlider;
            valueDisplayElement = waterLevelValueDisplay;
            progressBarElement = waterProgressBar;
            progressValueElement = waterProgressValue;
            statusLabelElement = waterStatusLabel;

            if (currentValue < 100) {
                statusLabelElement.textContent = 'Rendah';
                statusLabelElement.className = 'status-label warning';
            } else if (currentValue < 250) {
                statusLabelElement.textContent = 'Aman';
                statusLabelElement.className = 'status-label safe';
            } else {
                statusLabelElement.textContent = 'Bahaya';
                statusLabelElement.className = 'status-label warning';
            }
        } else if (type === 'trash') {
            sliderElement = trashLevelSlider;
            valueDisplayElement = trashLevelValueDisplay;
            progressBarElement = trashProgressBar;
            progressValueElement = trashProgressValue;
            statusLabelElement = trashStatusLabel;

            if (currentValue < 75) {
                statusLabelElement.textContent = 'Belum Penuh';
                statusLabelElement.className = 'status-label not-full';
            } else if (currentValue < 130) {
                statusLabelElement.textContent = 'Hampir Penuh';
                statusLabelElement.className = 'status-label warning';
            } else {
                statusLabelElement.textContent = 'Penuh';
                statusLabelElement.className = 'status-label warning';
            }
        } else {
            return; 
        }

        if (sliderElement) sliderElement.value = currentValue;
        if (valueDisplayElement) valueDisplayElement.textContent = `${currentValue} ${unit}`;
        if (progressBarElement) progressBarElement.style.width = `${percentage}%`;
        if (progressValueElement) progressValueElement.textContent = `${currentValue} ${unit}`;
    }

    function updateSensorStatusIcons(ketinggianAir, ketinggianSampah, deteksiSampah) {
        // Logika untuk ikon sensor ketinggian air
        if (ketinggianAir > 250) { // Contoh ambang batas bahaya
            sensorKetinggianAirIcon.className = 'sensor-icon alert';
            sensorKetinggianAirIcon.textContent = '!';
        } else if (ketinggianAir > 0) {
             sensorKetinggianAirIcon.className = 'sensor-icon check';
             sensorKetinggianAirIcon.textContent = '✓';
        } else {
            sensorKetinggianAirIcon.className = 'sensor-icon inactive'; // Jika tidak ada data atau 0
            sensorKetinggianAirIcon.textContent = '-';
        }

        // Logika untuk ikon sensor ketinggian sampah
        if (ketinggianSampah > 130) { // Contoh ambang batas penuh
            sensorKetinggianSampahIcon.className = 'sensor-icon alert';
            sensorKetinggianSampahIcon.textContent = '!';
        } else if (ketinggianSampah >= 0) { // >=0 untuk menangani kasus 0
             sensorKetinggianSampahIcon.className = 'sensor-icon check';
             sensorKetinggianSampahIcon.textContent = '✓';
        } else {
            sensorKetinggianSampahIcon.className = 'sensor-icon inactive';
            sensorKetinggianSampahIcon.textContent = '-';
        }
        
        // Logika untuk ikon sensor pendeteksi sampah
        // Asumsi deteksiSampah adalah 1 jika terdeteksi, 0 jika tidak
        if (deteksiSampah === 1 || deteksiSampah === "1") {
            sensorPendeteksiSampahIcon.className = 'sensor-icon alert'; // Atau 'check' tergantung makna "terdeteksi"
            sensorPendeteksiSampahIcon.textContent = '!'; // Atau '✓'
        } else {
            sensorPendeteksiSampahIcon.className = 'sensor-icon check';
            sensorPendeteksiSampahIcon.textContent = '✓';
        }
    }


    function fetchSensorData() {
        fetch('api/read.php')
            .then(response => {
                if (!response.ok) throw new Error(`HTTP error! status: ${response.status}`);
                return response.json();
            })
            .then(data => {
                console.log("Data dari API:", data); // debug log

                const airValue = parseInt(data.ketinggianAir, 10);
                const sampahValue = parseInt(data.kapasitasSampah, 10);
                const deteksiValue = data.deteksiSampah || 0; // kalau tersedia

                if (!isNaN(airValue)) {
                    updateLevelUI(airValue, MAX_WATER_LEVEL, 'cm', 'water');
                } else {
                    console.warn('Data Ketinggian Air tidak valid:', data.ketinggianAir);
                    updateLevelUI(0, MAX_WATER_LEVEL, 'cm', 'water');
                }

                if (!isNaN(sampahValue)) {
                    updateLevelUI(sampahValue, MAX_TRASH_LEVEL, 'cm', 'trash');
                } else {
                    console.warn('Data Ketinggian Sampah tidak valid:', data.kapasitasSampah);
                    updateLevelUI(0, MAX_TRASH_LEVEL, 'cm', 'trash');
                }

                updateSensorStatusIcons(airValue, sampahValue, deteksiValue);
            })
            .catch(error => {
                console.error("Gagal mengambil data sensor:", error);
                // Set nilai default atau tampilkan pesan error di UI jika perlu
                updateLevelUI(0, MAX_WATER_LEVEL, 'cm', 'water');
                updateLevelUI(0, MAX_TRASH_LEVEL, 'cm', 'trash');
                updateSensorStatusIcons(NaN, NaN, NaN); // Set ikon ke status tidak aktif/error
            });
    }

    function fetchLogAktivitas() {
        fetch('api/history.php')
            .then(response => {
                if (!response.ok) throw new Error(`HTTP error! status: ${response.status}`);
                return response.json();
            })
            .then(data => {
                const logContainer = document.getElementById('log_activity_container');
                if (logContainer) {
                    logContainer.innerHTML = ''; // Bersihkan log sebelumnya
                    if (Array.isArray(data) && data.length > 0) {
                        data.forEach(entry => {
                            const logEntryDiv = document.createElement('div');
                            logEntryDiv.classList.add('log-entry-item');
                            logEntryDiv.textContent = `${entry.waktu || 'No timestamp'} - ${entry.aktivitas || 'No activity'}`;
                            logContainer.appendChild(logEntryDiv);
                        });
                    } else {
                        const noLogEntry = document.createElement('div');
                        noLogEntry.classList.add('log-entry-item');
                        noLogEntry.textContent = 'Tidak ada aktivitas tercatat.';
                        logContainer.appendChild(noLogEntry);
                    }
                }
            })
            .catch(error => {
                console.error("Gagal mengambil log aktivitas:", error);
                const logContainer = document.getElementById('log_activity_container');
                if (logContainer) {
                    logContainer.innerHTML = '<div class="log-entry-item">Gagal memuat log aktivitas.</div>';
                }
            });
    }

    // Listener untuk slider (jika interaksi manual diizinkan)
    if (waterLevelSlider) {
        waterLevelSlider.addEventListener('input', function () {
            updateLevelUI(parseInt(this.value), MAX_WATER_LEVEL, 'cm', 'water');
        });
    }
    if (trashLevelSlider) {
        trashLevelSlider.addEventListener('input', function () {
            updateLevelUI(parseInt(this.value), MAX_TRASH_LEVEL, 'cm', 'trash');
        });
    }

    // Pengambilan data awal dan pembaruan berkala
    fetchSensorData();
    fetchLogAktivitas();

    setInterval(() => {
        fetchSensorData();
        fetchLogAktivitas();
    }, 1000); // Ambil data setiap 5 detik
});
