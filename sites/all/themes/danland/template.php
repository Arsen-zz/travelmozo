<?php
// $Id: template.php,v 1.10.4.3 2010/12/14 03:30:39 danprobo Exp $
function danland_page_class($sidebar_first, $sidebar_second) {
	if ($sidebar_first && $sidebar_second) {
		$id = 'layout-type-2';	
	}
	else if ($sidebar_first || $sidebar_second) {
		$id = 'layout-type-1';
	}

	if(isset($id)) {
		print ' id="'. $id .'"';
	}
}

function danland_preprocess_html(&$vars) {
  // Add conditional CSS for IE6.
drupal_add_css(path_to_theme() . '/style.ie6.css', array('group' => CSS_THEME, 'browsers' => array('IE' => 'IE 6', '!IE' => FALSE), 'preprocess' => FALSE));
}

function danland_preprocess_maintenance_page(&$variables) {
  if (!$variables['db_is_active']) {
    unset($variables['site_name']);
  }
  drupal_add_css(drupal_get_path('theme', 'danland') . '/maintenance.css');
  drupal_add_js(drupal_get_path('theme', 'danland') . '/scripts/jquery.cycle.all.js');
}

if (drupal_is_front_page()) {
  drupal_add_js(drupal_get_path('theme', 'danland') . '/scripts/jquery.cycle.all.js');
}

function danland_breadcrumb($variables) {
	$breadcrumb = $variables['breadcrumb'];
	if (!empty($breadcrumb)) {
		$breadcrumb[0] = '<a href="/">Travel Guid</a>';
    	if (arg(0) == 'node' && is_numeric(arg(1))) {
			$node = node_load(arg(1));
			
			if(isset($node->field_tags['und']['0']['tid'])) {
				$node_term = $node->field_tags['und']['0']['tid'];
				
				$parents = taxonomy_get_parents_all($node_term);
        		$parents = array_reverse($parents);
        	
				foreach ($parents as $parent) {
					if ($parent->tid != $node_term) {
						$parentlowcase = strtolower($parent->name);
						$parent->name = ucfirst($parent->name);
        	  			$breadcrumb[] = l($parent->name, $parentlowcase);
					} else {
						$breadcrumb[] = $parent->namee;
					}
       			}
			}
   		}
		$output = '<div class="breadcrumb">'. implode(' › ', $breadcrumb) .'</div>';
		return $output;
   	}
}