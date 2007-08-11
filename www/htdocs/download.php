<?php
require_once("template.php");
$template = new Template;
$template->siteTitle = "Open Yahtzee - Download";

$latestversion="1.8.0";

$template->content = '<h2>Open Yahtzee Download</h2>
Open Yahtzee is a cross platform game. It works on different platforms and hence is available for download fo this platforms.
All the Open Yahtzee files available for download can be found in the <a href="http://sourceforge.net/project/showfiles.php?group_id=175453">Download</a> Page on the SourceForge project page. 
<h3>Windows</h3>
Windows users should download the Windows binaries, that is the files that end with a ".exe" e.g. "OpenYahtzee-1.6.0.exe".
Windows users can also go to the <a href="http://sourceforge.net/project/platformdownload.php?group_id=175453&amp;sel_platform=15">download for windows page</a>.
<h3>Linux</h3>
Linux users can download the precompiled binaries (currently RPM, deb, tgz), an ebuild for Gentoo, or the source-code.
To compile from source-code  all you have to do is to execute a "./configure && make" (without the quotes).';
if (!defined('__INDEX'))
	$template->out();
?>
 
