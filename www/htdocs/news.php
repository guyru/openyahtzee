<?php
require_once("template.php");
$template = new Template;
$template->siteTitle = "Open Yahtzee - News";

$latestnews = '<div><strong>11 Aug. 2007</strong> Open Yahtzee 1.8 was released today. ou can read more about the release and the new features in:<br/><a href="http://sourceforge.net/forum/forum.php?forum_id=724736">http://sourceforge.net/forum/forum.php?forum_id=724736</a>
</div>
<div><strong>12 May 2007</strong> Open Yahtzee PE 
(Portable Edition) was anounced today. for further details see:</br>
<a href="http://sourceforge.net/forum/forum.php?forum_id=694892">http://sourceforge.net/forum/forum.php?forum_id=694892</a>
</div>
<div><strong>09 Feb. 2007</strong> Open Yahtzee 1.7 was released today. You can read more about the release and the new features in:<br/><a href="https://sourceforge.net/forum/forum.php?forum_id=663217">https://sourceforge.net/forum/forum.php?forum_id=663217</a>
</div>
';

$template->content = '<h2>Open Yahtzee News Archive</h2>';

$template->content .= $latestnews;

$template->content .= '
<div><strong>29 Jan. 2007</strong> The development of Open Yahtzee 1.7 is almost complete and it should be released as planned (second week of February). Meanwhile Seamus McGill designed new logo for Open Yahtzee. He also designed new dice graphics which will be incorprated in the upcoming version.
</div>
<div><strong>10 Jan. 2007</strong> Open Yahtzee 1.6 was released today. You can read more about the release and the new features in:<br/><a href="http://sourceforge.net/forum/forum.php?forum_id=653011">http://sourceforge.net/forum/forum.php?forum_id=653011</a>
</div>
<div><strong>19 Dec. 2006</strong> I released today the first package of Open Yahtzee for windows. You can find more details in:<br/><a href="http://sourceforge.net/forum/forum.php?forum_id=646283">http://sourceforge.net/forum/forum.php?forum_id=646283</a>
</div>
<div><strong>12 Dec. 2006</strong> I released today a bug fix version for Open Yahtzee, Open Yahtzee 1.5.1 fixes several bugs found in the way the high score table was handled.<br />This release also contains ebuild for gentoo users.
</div>
<div><strong>18 Oct. 2006</strong> I released today the first beta for Open Yahtzee 1.5. The biggest change interduced in this version is the new high-score table, which is easy customizable.
<br />You can read the full announcement about this release in:<br />
<a href="http://sourceforge.net/forum/forum.php?forum_id=624679" >http://sourceforge.net/forum/forum.php?forum_id=624679</a>
<br />You can download this release via sourceforge.<br />
<a href="http://sourceforge.net/project/showfiles.php?group_id=175453&amp;package_id=201410&amp;release_id=456656" >Click here to download</a>
</div>
<div><strong>10 Oct. 2006</strong> I finished working on the basic functionality of the highscore table. Everything is ready except the UI for dynamicly setting the table\'s length. You can try it out by checking out from the CVS under module "OpenYahtzee" and tag "alpha_1-5".
</div>
<br />';
if (!defined('__INDEX'))
	$template->out();
?>
