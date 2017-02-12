<?php
                                                                          /* HALLWAY 1 */
		/* Define updatable file */
		    define("LOG_FILE", "./NewPinValues.csv");        
        /* Get Device and Confirm*/
            $temp = $_GET["temp"];
            if(!is_numeric($temp)) echo"Received Number<br>";
		/* Copy Original csv file into an arrray */ 
			/* Open the File. */
			if (($handle = fopen("NewPinValues.csv", "r")) !== FALSE) 
			{
			/* Set the parent multidimensional array key to 0 */
			$nn = 0;
			/* Check if file is empty */
			while (($data = fgetcsv($handle, 1000, ",")) !== FALSE) 
			{
            /* Count the total keys in the row */
            $c = count($data);
            /* Populate the multidimensional array */
            for ($x=0;$x<$c;$x++)
            {
                $csvarray[$nn][$x] = $data[$x];
            }
            $nn++;
            }
            /* Close the File */
            fclose($handle);
			}
		/* Update Array based on Number of devices detected */
		    /* if Device 1 is detected */
			if($temp == 1)
			{
			    $csvarray[0][0] = "H1";
			    echo"Device 1 detected in Room2 <br>";
			}
			/* if Device 2 is detected */
			else if($temp == "2")
			{
			    $csvarray[1][0] = "H1";
			    echo"Device 2 detected in Room2 <br>";
			}
			
	    /* Update CSV with detected devices */
		    /* Open updatable file */
			$file_handler = fopen(LOG_FILE, "w");
			/* Copy updated array to the updatable file */
			foreach ($csvarray as $line)
			{
				fputcsv($file_handler, $line);
			}
			/* Close the file */
			fclose($file_handler);
?>