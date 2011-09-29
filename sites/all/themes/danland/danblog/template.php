<?php
// $Id: template.php,v 1.1.2.1 2010/11/11 14:08:01 danprobo Exp $

function danblog_preprocess_html(&$variables) {
  drupal_add_css(path_to_theme() . '/style.ie6.css', array('group' => CSS_THEME, 'browsers' => array('IE' => 'IE 6', '!IE' => FALSE), 'preprocess' => FALSE));
}

function danblog_breadcrumb($variables) {
	$breadcrumb = $variables['breadcrumb'];
	if (!empty($breadcrumb)) {
		$breadcrumb[0] = '<a href="/">Travel Guides</a>';
    	if (arg(0) == 'node' && is_numeric(arg(1))) {
			$node = node_load(arg(1));
			
			if(isset($node->field_term_dest['und']['0']['tid'])) {
				$node_term = $node->field_term_dest['und']['0']['tid'];
				
				$parents = taxonomy_get_parents_all($node_term);
        		$parents = array_reverse($parents);
				
        	
				foreach ($parents as $parent) {
					if ($parent->tid == $node_term && $node->type == 'destinations') {
						$breadcrumb[] = $parent->name;
					} else {
						$parentlowcase = str_replace(" ", "-", strtolower($parent->name));
        	  			$breadcrumb[] = l($parent->name, $parentlowcase);
					}
       			}
				if($node->type != 'destinations' && $node->type != 'page') {
					$breadcrumb[] = l(ucfirst($node->type), $parentlowcase . '/' . $node->type);
					$breadcrumb[] = $node->title;
				} else if ($node->type == 'page') {
					$breadcrumb[] = $node->field_content_type['und']['0']['value'];
				}
			}
   		}
		$output = '<div class="breadcrumb">'. implode(' › ', $breadcrumb) .'</div>';
		return $output;
   	}
}
