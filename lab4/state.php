<?php
header('Content-Type: application/json');

$matrix_state = array(
    array(0, 0, 1), // Загорелся зелёный
    array(0, 0, 1),
    array(0, 0, 1),
    array(0, 0, 1),
    array(0, 0, 1),
    array(0, 0, 1),
    array(0, 0, 0), // Мигание зелёного
    array(0, 0, 1),
    array(0, 0, 0),
    array(0, 0, 1),
    array(0, 0, 0),
    array(0, 0, 1),
    array(0, 1, 0), // Загорелся жёлтый
    array(0, 1, 0),
    array(0, 1, 0),
    array(1, 0, 0), // Загорелся красный
    array(1, 0, 0),
    array(1, 0, 0),
    array(1, 0, 0),
    array(1, 0, 0),
    array(1, 0, 0),
    array(1, 0, 0),
    array(1, 0, 0),
    array(1, 0, 0),
    array(1, 0, 0),
    array(1, 0, 0),
    array(1, 1, 0), // Горит красный и жёлтый
    array(1, 1, 0),
    array(1, 1, 0)
);

$stateIndex = time() % 30; // Переключение состояний каждую секунду
$clientStateIndex = ($stateIndex + 15) % 30; // Сдвиг на 15 состояний для клиента

$serverState = array(
    "red" => $matrix_state[$stateIndex][0],
    "yellow" => $matrix_state[$stateIndex][1],
    "green" => $matrix_state[$stateIndex][2]
);

$clientState = array(
    "red" => $matrix_state[$clientStateIndex][0],
    "yellow" => $matrix_state[$clientStateIndex][1],
    "green" => $matrix_state[$clientStateIndex][2]
);

// Запись текущего состояния сервера в файл
file_put_contents('state.txt', json_encode($serverState));

// Отправка состояния клиента
echo json_encode($clientState);

?>
