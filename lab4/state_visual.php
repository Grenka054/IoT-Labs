<!DOCTYPE html>
<html>
<head>
    <style>
        #green, #yellow, #red {
            width: 100px;
            height: 100px;
            margin-bottom: 10px;
        }
    </style>
</head>
<body>

    <div id="red"></div>
    <div id="yellow"></div>
    <div id="green"></div>

<script>
    function updateTrafficLight() {
        fetch('state.txt')
            .then(response => response.json())
            .then(data => {
                document.getElementById('red').style.backgroundColor = data.red ? 'red' : 'black';
                document.getElementById('yellow').style.backgroundColor = data.yellow ? 'yellow' : 'black';
                document.getElementById('green').style.backgroundColor = data.green ? 'green' : 'black';
            });
    }

    setInterval(updateTrafficLight, 500);
</script>

</body>
</html>
