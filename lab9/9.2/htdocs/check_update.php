<?php
    // Определите каталог прошивки
    $firmware_dir = "firmware/";

    // Получите список всех файлов прошивки
    $files = array_diff(scandir($firmware_dir), array('..', '.'));

    // Найдите последний файл прошивки
    $latest_file = end($files);

    // Извлеките версию из названия файла
    $latest_version = pathinfo($latest_file, PATHINFO_FILENAME);

    // Определите URL файла прошивки
    $firmware_url = "http://192.168.1.108/firmware/" . $latest_file;

    // Ответьте с последней версией прошивки и ее URL
    header('Content-Type: application/json');
    echo json_encode(array("version" => $latest_version, "url" => $firmware_url));
?>
