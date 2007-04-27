<?php

//VERSION NUMBER set here the latest stable version
$oy_version = "1.7.0";
$stableversion = explode (".",$oy_version);


require_once("template.php");
$template = new Template;
$template->siteTitle = "Open Yahtzee - Check for Updates";

$version='';
if(isset($_GET['version']))
{
	$version = $_GET['version'];
}
$versionnumbers = array("0","0","0","0");
$versionnumbers = explode(".",$version,4);

$needsupdate = false;

if((int)$stableversion[0]>(int)$versionnumbers[0]){
	$needsupdate=true;
} else if((int)$stableversion[1]>(int)$versionnumbers[1]){
	$needsupdate=true;
} else if((int)$stableversion[2]>(int)$versionnumbers[2]){
	$needsupdate=true;
}


$template->content='
<h2>Check for Updates</h2>
<p>You are using OpenYahtzee-'.$version.'. <br />
The latest version is OpenYahtzee-'.$oy_version.'.</p>';
if ($needsupdate) {
	$template->content .= '<p>Open Yahtzee 1.7 was released! The full release announcement can be found <a href= http://sourceforge.net/forum/forum.php?forum_id=663217> here.</a></p><p>The latest version of OpenYahtzee is newer than the version you use.<br/>
<a href="http://sourceforge.net/project/showfiles.php?group_id=175453">Download the latest version of OpenYahtzee</a>.</p>';
} else {
	$template->content .= '<p>You are using the latest version of OpenYahtzee. Stay tuned for updates.</p>';
}


$template->out();
?>