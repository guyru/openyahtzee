<?php


class Template
{
	
	var $siteTitle;

	var $content;
	


	var $meta='';
	
	function out()
	{
	?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<head>
<meta http-equiv="content-type" content="text/html; charset=utf-8" />
<meta name="description" content="The Home page of Open Yahtzee" />
<meta name="keywords" content="yahtzee,openyahtzee,open" />
<meta name="author" content="Guy Rutenberg  / Based on a  design by Andreas Viklund - http://andreasviklund.com/" />
<link rel="stylesheet" type="text/css" href="andreas01.css" media="screen" title="andreas01 (screen)" />
<?php print $this->meta; ?>
<title><?php print $this->siteTitle; ?></title>
</head>

<body>

<div id="wrap">
<div id="header">
<h1>Open Yahtzee</h1>
<p></p>
</div>

<img style="margin-left:85px;" id="frontphoto" src="top2.gif" width="600" height="175" alt="" />

<div id="avmenu">
<h2 class="hide">Menu:</h2>
<ul>
<li><a href="index.php">Home Page</a></li>
<li><a href="download.php">Download</a></li>
<li><a href="news.php">News</a></li>
<li><a href="index.php#features">Features</a></li>
<li><a href="screenshots.php">Screenshots</a></li>
<li><a href="http://sourceforge.net/projects/openyahtzee">SF Project Page</a></li>
</ul>

<div class="announce">
<strong>Latest Stable Version:</strong>
<p>Open Yahtzee 1.7</p>
<p class="textright"><a 	href="http://sourceforge.net/project/showfiles.php?group_id=175453&amp;package_id=201410&amp;release_id=485238">Download...</a></p>
</div><!--End of announce div -->

</div><!--End of avmenu div -->

<div id="extras">
<h3>Short About:</h3>
<p>This is Open Yahtzee. Open Yahtzee is an open-source cross-platform version of the classic dice game Yahtzee</p>

<h3>Links:</h3>
<p>- <a href="http://guy.sikumuna.com">Guy Rutenberg</a><br />
- <a href="http://sourceforge.net/">SourceForge</a><br />
- <a href="http://www.wxwidgets.org/">wxWidgets</a><br />
</p>

</div><!--End of extras div -->

<div id="content">

<?php print $this->content; ?>

</div><!--End of content div -->

<div id="footer">
Copyright &copy; 2006 Guy Rutenberg. based on a design by <a 
href="http://andreasviklund.com">Andreas Viklund</a>. Logo created by Seamus McGill.
<br />
<a href="http://sourceforge.net"><img 
src="http://sflogo.sourceforge.net/sflogo.php?group_id=175453&amp;type=1" 
width="88" height="31" alt="SourceForge.net Logo" /></a>
<a href="http://validator.w3.org/check?uri=referer"><img
        src="http://www.w3.org/Icons/valid-xhtml10"
        alt="Valid XHTML 1.0 Strict" height="31" width="88" /></a>
<script src="http://www.google-analytics.com/urchin.js" type="text/javascript">
</script>
<script type="text/javascript">
_uacct = "UA-1882923-1";
urchinTracker();
</script> 
</div><!--End of footer div -->

</div><!--End of wrapper div -->

</body>
</html>


<?php	
	}

}

?>
