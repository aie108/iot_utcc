<!DOCTYPE html>
<html>
<header>
<link rel="stylesheet" type="text/css" href="css/table.css">
</header>
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

echo '<table cellpadding="0" cellspacing="0" class="db-table">';
echo '<tr><th>ID</th><th>Sensor ID</th> <th>Temperature</th><th>Humidity</th><th>Time</th></tr>';
while($row = $result->fetch_assoc()) {
	echo '<tr>';
	foreach($row as $key=>$value) {
		echo '<td>',$value,'</td>';
	}
	echo '</tr>';
}
echo '</table><br />';

$conn->close();
?> 

</body>
</html>