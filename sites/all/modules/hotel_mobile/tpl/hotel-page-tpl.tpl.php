<?php

/**
  * @file
  * Default theme template with bootstrap page.
  *
  * Variables:
  * - $head_title: A modified version of the page title, for use in the TITLE
  * - $css: An array of CSS files for the current page.
  * - $scripts: Script tags necessary to load the JavaScript files and settings
  * - $page: The rendered page content.
  *
  */
?>

<!DOCTYPE html>
<html>

<head>
    <title><?php print $page['title']; ?></title>
    <meta http-equiv="Content-Type" content="text/html; charset=UTF-8"/>
    <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=0"/>
    <?php foreach($styles as $css): ?>
        <link rel="stylesheet" type="text/css" href="<?php print $css; ?>">
    <?php endforeach; ?>

    <script type="text/javascript">
        <?php print $page['inline_js']; ?>
        <?php if(!empty($page['js_var'])): ?>
            var jsObj = <?php print json_encode($page['js_var'], JSON_UNESCAPED_UNICODE); ?>;
        <?php endif; ?>
    </script>
</head>

<body class="wm-page" id="<?php print $page['body_id']?>">

    <div class="wm-content container"> 
    </div><!--/.container-->

    
<!--**************javascript file load*************************-->
    <?php foreach($scripts as $js): ?>
        <script type="text/javascript" src="<?php print $js; ?>"></script>
    <?php endforeach; ?>
<!--**************javascript file load*************************-->

</body>

</html>

