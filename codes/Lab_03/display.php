<!DOCTYPE html>
<html>
<body>

<?php
$servername = "localhost";
$username = "iot";
$password = "P@ssw0rd";
$dbname = "mysimpleiot";

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);
// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
} 

$sql = "SELECT * FROM my_data";
$result = $conn->query($sql);

if ($result->num_rows > 0) {
    // output data of each row
    while($row = $result->fetch_assoc()) {
        echo "<br> id: ". $row["id"]. " - Sensor ID: ". $row["sensor_id"]. " temperature: " . $row["temperature"] . " humidity: " . $row["humidity"] . "<br>";
    }
} else {
    echo "0 results";
}

$conn->close();
?> 

</body>
</html>