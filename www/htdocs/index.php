<?php
define ('__INDEX',true);

require_once("template.php");
include("news.php");
$template = new Template;
$template->siteTitle = "Open Yahtzee - Home Page";


$template->content='
<h2>Welcome to Open Yahtzee!</h2>
Open Yahtzee is an open-source (free) version of the classic dice game Yahtzee. Open Yahtzee is built to be OS portable, that means you can run it on many kinds of different operating systems and platforms. The portability is mainly achived via wxWidgets which also gives Open Yahtzee a naitve look on each platform.
Open Yahtzee is being developed by Guy Rutenberg. You can <a href="mailto://guyrutenberg@gmail.com">email me</a> or the <a href="mailto://openyahtzee-users@lists.sourceforge.net"> Open Yahtzee mailing list</a> if you have any questions.<br /><br />
<h3>News</h3>';
$template->content.= $latestnews;
$template->content.= '<br /><a href="news.php">News Archive</a><br />';
$template->content.='
<br />
<a name="features"></a>
<h3>Features</h3>
<ul>
<li>OS portable</li>
<li>Full-featured yahtzee implementation</li>
<li>Customized high score list</li>
<li>User-friendly interface</li>
</ul>
<br />

If you have any suggestions of feature-requests feel free to <a 
href="mailto://guyrutenberg@gmail.com">contact me</a> or post a 
feature-request in the 
<a 
href="http://sourceforge.net/tracker/?group_id=175453&amp;atid=873298">feature-requests page</a> in the project\'s SourceForge page. ';

$template->out();
?>
