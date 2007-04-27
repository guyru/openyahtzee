<?php
require_once("template.php");
$template = new Template;
$template->siteTitle = "Open Yahtzee - Feedback";


$template->content = '<h2>Feedbacks and Comments</h2>
<div>Here you can send feedback to the developers of Open Yahtzee. Any suggestion, comments, questions and bug reports will be welcomed. Your feedback helps up make Open Yahtzee better!
</div>
<form action="feedout.php" method="post">
<div>
Your Name:<br /><input type="text" name="name" /><br />
E-mail:<br /><input type="text" name = "email" /><br /><br />
Comments
<textarea rows="10" cols="50" style="width:100%;height:200px" name="comments"></textarea><br /><br />
<input type="submit" value="Submit" />
</div>
</form>
';

$template->out();
?> 
