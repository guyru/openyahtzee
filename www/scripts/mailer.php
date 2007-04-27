<?php
function sf_mail($to = "" , $subject = "", $message = "", $headers = "")
{
  $mail_link = mysql_connect('mysql4-o.sourceforge.net', 'o175453rw', 'OvRzHf2M')
   or die('Could not connect: ' . mysql_error());
  $to = addslashes($to);
  $subject = addslashes($subject);
  $message = addslashes($message);
  $headers = addslashes($headers);  

  $query = "INSERT INTO o175453_general.mailer(recipient, subject, message, headers) VALUES('$to', '$subject', '$message', '$headers')";
/*  $result = */mysql_query($query, $mail_link) or die('Query failed: ' . mysql_error());
  return true;
}

//EXAMPLE: sf_mail("johndoe@users.sf.net", "TEST FROM SFMAILER", "THIS IS A TEST", "CC: johndoe@myrealbox.com");
?>

