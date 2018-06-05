<!DOCTYPE html>
<html>
  <body>
      <?php
        $servername = "awlay@cs-sql2014.ua-net.ua.edu";
        $username = "awlay";
        $password = "11719263";
        $dbname = "awlay";

        // Create connection
        $conn = new mysqli($servername, $username, $password, $dbname);
        // Check connection
        if ($conn->connect_error) {
            die("Connection failed: " . $conn->connect_error);
        }

        $sql = "SELECT * FROM $_POST["db_name"]";
        $result = $conn->query($sql);

        //echo "<div> Hello World </div>";
        /*if ($result->num_rows > 0) {
          echo "<table";
            // output data of each row
            while($row = $result->fetch_assoc()) {
                echo "<tr>";
                foreach($row as $value) {
                  echo "<td>".$value."</td>";
                }
                echo "<tr>";
            }
          echo "</table>";
        } else {
            echo "0 results";
        }*/

        $conn->close();
      ?>
    </div> <!-- task3 -->
  </body>
</html>
