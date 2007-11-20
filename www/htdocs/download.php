<?php
require_once("template.php");
$template = new Template;
$template->siteTitle = "Open Yahtzee - Download";

$latestversion="1.8.0";

$template->content =<<<EndHereDoc
<h2>Open Yahtzee Download</h2>
Open Yahtzee is a cross platform game. It works on different platforms and hence is available for download fo this platforms.
All the Open Yahtzee files available for download can be found in the <a href="http://sourceforge.net/project/showfiles.php?group_id=175453">download page</a> on the SourceForge project page. 
<h3>Windows</h3>
<p>
Windows users should download the Windows binaries, that is the files that end with a ".exe" e.g. "OpenYahtzee-1.6.0.exe".
Windows users can also go to the <a href="http://sourceforge.net/project/platformdownload.php?group_id=175453&amp;sel_platform=15">download for windows page</a>.
</p><p>
	Windows users also have the option of downloading Open Yahtzee PE, 
	the portable edition of Open Yahtzee specialy built to be used on 
	Disk On Keys.
</p>
<h3>Linux</h3>
Linux users can download the precompiled binaries (currently RPM, deb, tgz), an ebuild for Gentoo, or the source-code.
To compile from source-code  all you have to do is to execute a "./configure && make" (without the quotes).
EndHereDoc;
if (!defined('__INDEX'))
	$template->out();
?>
 
