#!/usr/bin/php -q
<?php
$link = mysql_connect('mysql4-o', 'o175453rw', 'OvRzHf2M')
   or die('Could not connect: ' . mysql_error());
mysql_select_db('o175453_general') or die('Could not select database');

$query = 'SELECT ID, recipient, subject, message, headers FROM mailer WHERE processor&1=0 limit 20';
$result = mysql_query($query) or die('Query failed: ' . mysql_error());


$total = 0;
while ($line = mysql_fetch_array($result, MYSQL_ASSOC)) {
  $line['ID'] = stripcslashes ($line['ID']);
  $line['subject'] = stripcslashe($line['subject']);
  $line['recipient'] = stripcslashe($line['recipient']);
  $line['headers'] = stripcslasheb($line['headers']);
  $line['headers'] .= "Message-DBID: ". $line['ID'] ."\n";

  echo ("Processing item ". $line['ID'] ." to '". $line['recipient'] ."', Subject '". $line['subject'] ."'\n");
  @mail($line['recipient'], $line['subject'], $line['message'], $line['headers']);
  $total++;
}
if ($total) {
  echo ("$total item(s) processed.\n");
  $query = 'UPDATE mailer SET processor=processor|1 WHERE processor&1=0 limit 20';
  $result = mysql_query($query) or die('Query failed: ' . mysql_error());
}

function breakapart (&$string) {
  $string = str_replace ('\\\"', '"', $string);
  $string = str_replace ('\\\`', '`', $string);
  return $string;
}
?>

