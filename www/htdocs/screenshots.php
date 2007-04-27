<?php
require_once("template.php");
$template = new Template;
$template->siteTitle = "Open Yahtzee - Screenshots";


$template->content = <<<EOF
<h2>Open Yahtzee - Screenshots</h2>
<h4>Open Yahtzee 1.7</h4>
<table class="screenshots">
	<tr>
		<td><div class="center">
			<a href="images/openyahtzee1.7_linux.jpg"><img src="images/openyahtzee1.7_linux_thumb.jpg" alt="screenshot" /></a></div>
			<div class="center">linux</div></td>
		<td><div class="center">
			<a href="images/openyahtzee1.7_linux2.png"><img src="images/openyahtzee1.7_linux2_thumb.png" alt="screenshot"/></a></div>
			<div class="center">linux</div></td>
	</tr>
</table>
<h4>Open Yahtzee 1.6</h4>
<table class="screenshots">
	<tr>
		<td><div class="center">
			<a href="images/openyahtzee1.6_linux.png"><img src="images/openyahtzee1.6_linux_thumb.png" alt="screenshot" /></a></div>
			<div class="center">linux</div></td>
	</tr>
</table>
<h4>Open Yahtzee 1.5.1</h4>
<table class="screenshots">
	<tr>
		<td><div class="center">
			<a href="images/openyahtzee1.5.1_linux.png"><img src="images/openyahtzee1.5.1_linux_thumb.png" alt="screenshot" /></a></div>
			<div class="center">linux</div></td>
		
	</tr>
</table>
<h4>Open Yahtzee 1.0.1</h4>
<table class="screenshots">
	<tr>
		<td><div class="center">
			<a href="images/openyahtzee1.0.1_linux.png"><img src="images/openyahtzee1.0.1_linux_thumb.png" alt="screenshot" /></a></div>
			<div class="center">linux</div></td>
	</tr>
</table>
EOF;

if (!defined('__INDEX'))
	$template->out();
?> 
