document.addEventListener('DOMContentLoaded', function() {
    // Get DOM elements
    const waterControl = document.getElementById('water-control');
    const waterValue = document.getElementById('water-value');
    const waterLevelDisplay = document.getElementById('water-level');
    const waterStatusLabel = document.querySelector('.data-item:nth-child(1) .status-label');
    
    const trashControl = document.getElementById('trash-control');
    const trashValue = document.getElementById('trash-value');
    const trashLevelDisplay = document.getElementById('trash-level');
    const trashStatusLabel = document.querySelector('.data-item:nth-child(2) .status-label');
    
    const activityLog = document.getElementById('activity-log');

    // Update water level and status
    waterControl.addEventListener('input', function() {
        const value = this.value;
        waterValue.textContent = `${value} cm`;
        waterLevelDisplay.textContent = `${value} cm`;
        
        // Update water status
        if (value > 250) {
            waterStatusLabel.textContent = 'Bahaya';
            waterStatusLabel.className = 'status-label warning';
            addLogEntry(`Warning: Water level critical (${value} cm)`);
        } else if (value > 200) {
            waterStatusLabel.textContent = 'Waspada';
            waterStatusLabel.className = 'status-label not-full';
            addLogEntry(`Notice: Water level rising (${value} cm)`);
        } else {
            waterStatusLabel.textContent = 'Aman';
            waterStatusLabel.className = 'status-label safe';
        }
    });

    // Update trash level and status
    trashControl.addEventListener('input', function() {
        const value = this.value;
        trashValue.textContent = `${value} cm`;
        trashLevelDisplay.textContent = `${value} cm`;
        
        // Update trash status
        if (value > 120) {
            trashStatusLabel.textContent = 'Penuh';
            trashStatusLabel.className = 'status-label warning';
            addLogEntry(`Alert: Trash container full (${value} cm)`);
        } else if (value > 80) {
            trashStatusLabel.textContent = 'Hampir Penuh';
            trashStatusLabel.className = 'status-label not-full';
            addLogEntry(`Notice: Trash level increasing (${value} cm)`);
        } else {
            trashStatusLabel.textContent = 'Belum Penuh';
            trashStatusLabel.className = 'status-label safe';
        }
    });

    // Function to add log entries
    function addLogEntry(message) {
        const logEntry = document.createElement('div');
        logEntry.className = 'log-entry';
        logEntry.textContent = message;
        activityLog.insertBefore(logEntry, activityLog.firstChild);
        
        // Keep log to a reasonable size
        if (activityLog.children.length > 20) {
            activityLog.removeChild(activityLog.lastChild);
        }
    }
});