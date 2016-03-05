<?php
function rjsimulador_configuration_admin_form($form, &$form_state) {
  $form['grupo_edad'] = array(
    '#type' => 'fieldset',
    '#title' => t("Groups by Ages"),
    '#description' => t(""),
    '#collapsible' => FALSE,
    '#tree' => TRUE,
    '#prefix' => '<div id="div-config-grupo-edad">',
    '#suffix' => '</div>'
  );

  $gruposEdad = Grupos::getGruposEdad();
  $totalGrupos = isset($form_state['grupo_edad_number']['#value']) ? $form_state['grupo_edad_number']['#value'] : count($gruposEdad);

  // Número de grupos
  $numeroMinimoGrupos = Grupos::getDataMinGroups();
  $numeroMaximoGrupos = Grupos::getDataMaxGroups();

  if (isset($form_state['clicked_button'])) {
    if ($form_state['clicked_button']['#name'] == 'grupo_edad_add_group_button') {
      $totalGrupos++;
    }
    if ($form_state['clicked_button']['#name'] == 'grupo_edad_delete_group_button') {
      $totalGrupos--;
    }
  }

  $form_state['grupo_edad_number'] = array(
    '#type' => 'value',
    '#value' => $totalGrupos,
  );

  for ($index = 1; $index <= $totalGrupos; $index++) {
    $form['grupo_edad'][$index]['desde'] = array(
      '#type' => 'textfield',
      '#title' => t("Age group no. @number", array('@number' => $index)),
      '#field_prefix' => t('From'),
      '#default_value' => isset($gruposEdad[$index][FilterByInterval::DESDE]) ? $gruposEdad[$index][FilterByInterval::DESDE] : 0,
      '#size' => 3,
      '#maxlength' => 3,
      '#required' => TRUE,
    );

    $form['grupo_edad'][$index]['hasta'] = array(
      '#type' => 'textfield',
      '#field_prefix' => t('to'),
      '#field_suffix' => t('years'),
      '#default_value' => isset($gruposEdad[$index][FilterByInterval::HASTA]) ? $gruposEdad[$index][FilterByInterval::HASTA] : 0,
      '#size' => 3,
      '#maxlength' => 3,
      '#required' => TRUE,
    );
  }

  if ($totalGrupos < $numeroMaximoGrupos) {
    $form['grupo_edad']['grupo_edad_add_group_button'] = array(
      '#type' => 'button',
      '#value' => t('Add new group'),
      '#name' => 'grupo_edad_add_group_button',
      '#ajax' => array(
        'callback' => 'ajax_config_callback',
        'wrapper' => 'div-config-grupo-edad',
      ),
    );
  }

  if ($totalGrupos > $numeroMinimoGrupos) {
    $form['grupo_edad']['grupo_edad_delete_group_button'] = array(
      '#type' => 'button',
      '#value' => t('Delete last group'),
      '#name' => 'grupo_edad_delete_group_button',
      '#ajax' => array(
        'callback' => 'ajax_config_callback',
        'wrapper' => 'div-config-grupo-edad',
      ),
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t("Submit"),
  );

  return $form;
}

function ajax_config_callback($form, &$form_state) {
  return $form['grupo_edad'];
}

function rjsimulador_configuration_admin_form_validate($form, &$form_state) {
  dpm("Validate");
}

function rjsimulador_configuration_admin_form_submit($form, &$form_state) {
  dpm("Submit");

  if (isset($form_state['grupo_edad_number']['#value']) && is_numeric($form_state['grupo_edad_number']['#value'])) {
    for ($index = 1; $index <= $form_state['grupo_edad_number']['#value']; $index++) {
      $gruposEdad[$index] = $form_state['values']['grupo_edad'][$index];
    }

    if (isset($gruposEdad)) {
      variable_set('rjsimulador_grupos_edad', $gruposEdad);
    }
  }
}