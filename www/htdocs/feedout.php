<?php
require_once("template.php");
include_once("/home/groups/o/op/openyahtzee/scripts/mailer.php");
$template = new Template;
$template->siteTitle = "Open Yahtzee - Feedback";

function checkOK($field)
{
if (eregi("\r",$field) || eregi("\n",$field)){
die("Invalid Input!");
}
}

$name=$_POST['name'];
checkOK($name);
$email=$_POST['email'];
checkOK($email);
$comments=$_POST['comments'];
//the checking of the comments is completly uneeded
//checkOK($comments);

$to="openyahtzee-users@lists.sourceforge.net";

$message="The following feedback was sent to the list by $name <$email>.\n\n$comments";
//$message = wordwrap($message, 70);

$template->content = '<h2>Feedback and Comments - sent</h2>';

sf_mail($to,"User Feedback",$message,"From:$name <$email>\n");
$template->content .= "Thanks for your comments.";

$template->out();
?> 
