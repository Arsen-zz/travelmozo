<?php

/**
 * @file
 * Advanced forum settings / configuration page.
 */

/**
 * Defines the Advanced Forum settings form.
 */
function advanced_forum_settings_page() {
  /* General settings */
  $form['advanced_forum_general'] = array(
    '#type' => 'fieldset',
    '#title' => t('General'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  // Choose style
  $options = array();
  $available_styles = advanced_forum_get_all_styles();
  foreach ($available_styles as $style_machine_name => $style) {
    $options[$style_machine_name] = $style['name'];
  }
  asort($options);

  $form['advanced_forum_general']['advanced_forum_style'] = array(
    '#type' => 'select',
    '#title' => t('Advanced forum style'),
    '#options' => $options,
    '#description' => t('Choose which style to use for your forums. This will apply independent of site theme.'),
    '#default_value' => variable_get('advanced_forum_style', 'stacked_bells'),
  );

  // Choose node types that are styled.
  $node_types = _node_types_build()->types;
  $options = array();
  foreach ($node_types as $node_machine_name => $node_type) {
    $options[$node_machine_name] = $node_type->name;
  }

  $form['advanced_forum_general']['advanced_forum_styled_node_types'] = array(
    '#type' => 'select',
    '#title' => t('Node types to style'),
    '#options' => $options,
    '#multiple' => TRUE,
    '#description' => t('Choose which node types will have the forum style applied.'),
    '#default_value' => variable_get('advanced_forum_styled_node_types', array('forum')),
  );

  // Style nodes presented in teaser form.
  $form['advanced_forum_general']['advanced_forum_style_teasers'] = array(
    '#type' => 'checkbox',
    '#title' => t('Style nodes when being displayed as teasers.'),
    '#default_value' => variable_get('advanced_forum_style_teasers', 0),
    '#description' => t('If checked, selected node types will be styled even when they are in a teaser list.'),
  );

  // Style nodes only if tagged for the forum.
  $form['advanced_forum_general']['advanced_forum_style_only_forum_tagged'] = array(
    '#type' => 'checkbox',
    '#title' => t('Style nodes only if they have a forum term attached.'),
    '#default_value' => variable_get('advanced_forum_style_only_forum_tagged', 1),
    '#description' => t('If checked, selected node types will be only styled if they are associated with a forum term.'),
  );

  // Style all site comments as forums
  $form['advanced_forum_general']['advanced_forum_style_all_comments'] = array(
    '#type' => 'checkbox',
    '#title' => t('Style all comments like forum replies.'),
    '#default_value' => variable_get('advanced_forum_style_all_comments', 0),
    '#description' => t('If checked, every comment will be styled as if it were a forum reply.'),
  );

  $form['advanced_forum_general']['advanced_forum_add_local_task'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add a tab for forum view page.'),
    '#default_value' => variable_get('advanced_forum_add_local_task', TRUE),
    '#description' => t('If checked, this will add a local task tab for "View forums". Use this in conjunction with making the included views have local tasks. If you don\'t know what this means, leave it unchecked. You must clear the cache before this will take effect.'),
  );

  $form['advanced_forum_general']['advanced_forum_views_as_tabs'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add a tab for included views that have their own pages.'),
    '#default_value' => variable_get('advanced_forum_views_as_tabs', TRUE),
    '#description' => t('If checked, this will add a local task tab for "Active topics," "New posts," "My posts," and "Unanswered topics." If you don\'t know what this means, leave it unchecked. You must clear the cache before this will take effect.'),
  );

  /* Forum / topic list settings */
  $form['advanced_forum_lists'] = array(
    '#type' => 'fieldset',
    '#title' => t('Forum and topic lists'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  // Disable breadcrumbs
  $form['advanced_forum_lists']['advanced_forum_disable_breadcrumbs'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disable breadcrumbs'),
    '#default_value' => variable_get('advanced_forum_disable_breadcrumbs', 0),
    '#description' => t('Check this to disable breadcrumbs in the forum if you are using another module to customize them. Does not affect node pages and does not work when Page Manager is overriding forum pages.'),
  );

  $form['advanced_forum_general']['advanced_forum_collapsible_containers'] = array(
    '#type' => 'select',
    '#title' => t('Collapsible forum containers'),
    '#options' => array(
      'none' => t("None"),
      'toggle' => t("Toggle"),
      'fade' => t("Fade"),
      'slide' => t("Slide"),
    ),
    '#description' => t('Select whether or not to enable collapsible forum containers and what type of animation to use.'),
    '#default_value' => variable_get('advanced_forum_collapsible_containers', 'toggle'),
  );

  if (module_exists('image')) {

    /* Forum image settings */
    $form['advanced_forum_forum_image'] = array(
      '#type' => 'fieldset',
      '#title' => t('Forum image settings'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );

    $forum_vocabulary = taxonomy_vocabulary_load(variable_get('forum_nav_vocabulary', 0));
    $field_info = field_info_instances('taxonomy_term', $forum_vocabulary->machine_name);
    $image_fields = array();
    foreach ($field_info as $bundle => $field) {
      if ($field['widget']['type'] == 'image_image') {
        $image_fields[$bundle] = $bundle;
      }
    }
    $form['advanced_forum_forum_image']['advanced_forum_forum_image_field'] = array(
      '#title' => t('Image field'),
      '#description' => t('The image field to use to display forum images.'),
      '#type' => 'select',
      '#default_value' => variable_get('advanced_forum_forum_image_field', ''),
      '#empty_option' => t('None'),
      '#options' => $image_fields,
    );

    $form['advanced_forum_forum_image']['advanced_forum_forum_image_preset'] = array(
      '#title' => t('Forum image style'),
      '#description' => t('The image style to apply to the images.'),
      '#type' => 'select',
      '#default_value' => variable_get('advanced_forum_forum_image_preset', ''),
      '#empty_option' => t('None (original image)'),
      '#options' => image_style_options(FALSE),
    );
  }
  else {
    variable_set('advanced_forum_forum_image_field', '');
  }
  // Picture preset
  if (module_exists('image') && module_exists('author_pane')) {
    $options = array('' => '');
    $styles = image_styles();
    foreach ($styles AS $style) {
      $options[$style['name']] = $style['name'];
    }

    $form['advanced_forum_general']['advanced_forum_user_picture_preset'] = array(
      '#type' => 'select',
      '#title' => t('User picture preset'),
      '#options' => $options,
      '#description' => t('Image preset to use for forum avatars. Leave blank to not use this feature.'),
      '#default_value' => variable_get('advanced_forum_user_picture_preset', ''),
    );
  }
  else {
    variable_set('advanced_forum_user_picture_preset', '');
  }

  // Retrieve new comments on forum listing
  $form['advanced_forum_lists']['advanced_forum_get_new_comments'] = array(
    '#type' => 'checkbox',
    '#title' => t('Get the number of new comments per forum on the forum list'),
    '#default_value' => variable_get('advanced_forum_get_new_comments', 0),
    '#description' => t('Core forum shows the number of new topics. If checked, Advanced Forum will get the number of new comments as well and show it under "posts" on the forum overview. Slow query not recommended on large forums.'),
  );

  // Title length max
  $form['advanced_forum_lists']['advanced_forum_topic_title_length'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of characters to display for the topic title'),
    '#size' => 5,
    '#description' => t('Used on main forum page. Enter 0 to use the full title.'),
    '#default_value' => variable_get('advanced_forum_topic_title_length', 20),
  );

  // Send our form to Drupal to make a settings page
  return system_settings_form($form);
}
