<?php
// SERSAM/layout/header.php
if (session_status() == PHP_SESSION_NONE) {
    session_start(); // Mulai sesi jika belum dimulai
}

// Definisikan BASE_URL jika belum ada (berguna untuk path aset)
if (!defined('BASE_URL')) {
    $protocol = (!empty($_SERVER['HTTPS']) && $_SERVER['HTTPS'] !== 'off' || $_SERVER['SERVER_PORT'] == 443) ? "https://" : "http://";
    $host = $_SERVER['HTTP_HOST'];
    $script_name = $_SERVER['SCRIPT_NAME']; 
    
    $path_parts = pathinfo($script_name);
    $directory = $path_parts['dirname']; 
    
    if ($directory === '/' || $directory === '\\') {
        $directory = '';
    }
    
    define('BASE_URL', rtrim($protocol . $host . $directory, '/') . '/');
}

$page_title = isset($page_title) ? htmlspecialchars($page_title) : 'SERSAM Dashboard Monitoring';
?>
<!DOCTYPE html>
<html lang="id">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title><?php echo $page_title; ?></title>
    <link href="https://fonts.googleapis.com/css2?family=Roboto:wght@300;400;500;700&display=swap" rel="stylesheet">
    
    <link rel="stylesheet" href="<?php echo BASE_URL; ?>css/style.css?v=<?php echo time(); // Cache busting ?>">
    
</head>
<body>
    <div class="page-container">
        <div class="dashboard-container">
            <?php
                if (file_exists(__DIR__ . '/navbar.php')) {
                    require_once __DIR__ . '/navbar.php';
                } else {
                    echo "<p>Error: Navbar tidak ditemukan.</p>";
                }
            ?>
            