<?php
function rjsimulador_configuration_admin_form($form, &$form_state) {
  $form['general_configuration'] = arraY(
    '#type' => 'fieldset',
    '#title' => t('General configuration'),
    '#description' => t('General configuration about module.')
  );

  $form['general_configuration']['select_grupo_default'] = array(
    '#type' => 'select',
    '#title' => t('Default group'),
    '#description' => t('Select default group to show info about in "Data Analysis or Infractions Analysis by group".'),
    '#options' => Grupos::getListaGrupos(),
    '#default_value' => variable_get('rjsimulador_grupo_default', 0),
    '#required' => TRUE
  );

  $form['grupo_edad'] = array(
    '#type' => 'fieldset',
    '#title' => t("Groups by Ages"),
    '#description' => t(""),
    '#collapsible' => FALSE,
    '#tree' => TRUE,
    '#prefix' => '<div id="div-config-grupo-edad" class="group">',
    '#suffix' => '</div>'
  );

  $form['grupo_edad'] += generarGruposEdadConfiguracion($form_state);

  $form['grupo_experiencia'] = array(
    '#type' => 'fieldset',
    '#title' => t("Groups by Driving Experience"),
    '#description' => t(""),
    '#collapsible' => FALSE,
    '#tree' => TRUE,
    '#prefix' => '<div id="div-config-grupo-experiencia" class="group">',
    '#suffix' => '</div>'
  );

  $form['grupo_experiencia'] += generarGruposExperienciaConfiguracion($form_state);

  $form['grupo_kilometraje'] = array(
    '#type' => 'fieldset',
    '#title' => t("Groups by Average Annual Mileage"),
    '#description' => t(""),
    '#collapsible' => FALSE,
    '#tree' => TRUE,
    '#prefix' => '<div id="div-config-grupo-kilometraje" class="group">',
    '#suffix' => '</div>'
  );

  $form['grupo_kilometraje'] += generarGruposKilometrajeConfiguracion($form_state);

  $form['actions'] = array(
    '#type' => 'actions',
    '#prefix' => '<div class="clearfix"></div>'
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t("Submit"),
  );

  $form['#attributes'] = array('class' => array('rjsimulador-config'));
  $form['#attached'] = array(
    'css' => array(drupal_get_path('module', 'rjsimulador') . '/css/rjsimulador-admin.css')
  );

  return $form;
}

function generarGruposEdadConfiguracion(&$form_state) {
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

  $form = array();

  for ($index = 1; $index <= $totalGrupos; $index++) {
    $form[$index]['desde'] = array(
      '#type' => 'textfield',
      '#title' => t("Age group no. @number", array('@number' => $index)),
      '#field_prefix' => t('From'),
      '#default_value' => isset($gruposEdad[$index][FilterByInterval::DESDE]) ? $gruposEdad[$index][FilterByInterval::DESDE] : 0,
      '#size' => 3,
      '#maxlength' => 3,
      '#required' => TRUE,
    );

    $form[$index]['hasta'] = array(
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
    $form['grupo_edad_add_group_button'] = array(
      '#type' => 'button',
      '#value' => t('Add new group'),
      '#name' => 'grupo_edad_add_group_button',
      '#limit_validation_errors' => array(),
      '#ajax' => array(
        'callback' => 'ajax_grupos_edad_config_callback',
        'wrapper' => 'div-config-grupo-edad',
      ),
    );
  }

  if ($totalGrupos > $numeroMinimoGrupos) {
    $form['grupo_edad_delete_group_button'] = array(
      '#type' => 'button',
      '#value' => t('Delete last group'),
      '#name' => 'grupo_edad_delete_group_button',
      '#limit_validation_errors' => array(),
      '#ajax' => array(
        'callback' => 'ajax_grupos_edad_config_callback',
        'wrapper' => 'div-config-grupo-edad',
      ),
    );
  }

  return $form;
}

function ajax_grupos_edad_config_callback($form, &$form_state) {
  return $form['grupo_edad'];
}

function generarGruposExperienciaConfiguracion(&$form_state) {
  $gruposEdad = Grupos::getGruposExperiencia();
  $totalGrupos = isset($form_state['grupo_experiencia_number']['#value']) ?
    $form_state['grupo_experiencia_number']['#value'] : count($gruposEdad);

  // Número de grupos
  $numeroMinimoGrupos = Grupos::getDataMinGroups();
  $numeroMaximoGrupos = Grupos::getDataMaxGroups();

  if (isset($form_state['clicked_button'])) {
    if ($form_state['clicked_button']['#name'] == 'grupo_experiencia_add_group_button') {
      $totalGrupos++;
    }
    if ($form_state['clicked_button']['#name'] == 'grupo_experiencia_delete_group_button') {
      $totalGrupos--;
    }
  }

  $form_state['grupo_experiencia_number'] = array(
    '#type' => 'value',
    '#value' => $totalGrupos,
  );

  $form = array();

  for ($index = 1; $index <= $totalGrupos; $index++) {
    $form[$index]['desde'] = array(
      '#type' => 'textfield',
      '#title' => t("Driving Experience group no. @number", array('@number' => $index)),
      '#field_prefix' => t('From'),
      '#default_value' => isset($gruposEdad[$index][FilterByInterval::DESDE]) ? $gruposEdad[$index][FilterByInterval::DESDE] : 0,
      '#size' => 3,
      '#maxlength' => 3,
      '#required' => TRUE,
    );

    $form[$index]['hasta'] = array(
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
    $form['grupo_experiencia_add_group_button'] = array(
      '#type' => 'button',
      '#value' => t('Add new group'),
      '#name' => 'grupo_experiencia_add_group_button',
      '#limit_validation_errors' => array(),
      '#ajax' => array(
        'callback' => 'ajax_grupos_experiencia_config_callback',
        'wrapper' => 'div-config-grupo-experiencia',
      ),
    );
  }

  if ($totalGrupos > $numeroMinimoGrupos) {
    $form['grupo_experiencia_delete_group_button'] = array(
      '#type' => 'button',
      '#value' => t('Delete last group'),
      '#name' => 'grupo_experiencia_delete_group_button',
      '#limit_validation_errors' => array(),
      '#ajax' => array(
        'callback' => 'ajax_grupos_experiencia_config_callback',
        'wrapper' => 'div-config-grupo-experiencia',
      ),
    );
  }

  return $form;
}

function ajax_grupos_experiencia_config_callback($form, &$form_state) {
  return $form['grupo_experiencia'];
}

function generarGruposKilometrajeConfiguracion(&$form_state) {
  $gruposEdad = Grupos::getGruposKmMedioAnual();
  $totalGrupos = isset($form_state['grupo_kilometraje_number']['#value']) ?
    $form_state['grupo_kilometraje_number']['#value'] : count($gruposEdad);

  // Número de grupos
  $numeroMinimoGrupos = Grupos::getDataMinGroups();
  $numeroMaximoGrupos = Grupos::getDataMaxGroups();

  if (isset($form_state['clicked_button'])) {
    if ($form_state['clicked_button']['#name'] == 'grupo_kilometraje_add_group_button') {
      $totalGrupos++;
    }
    if ($form_state['clicked_button']['#name'] == 'grupo_kilometraje_delete_group_button') {
      $totalGrupos--;
    }
  }

  $form_state['grupo_kilometraje_number'] = array(
    '#type' => 'value',
    '#value' => $totalGrupos,
  );

  $form = array();

  for ($index = 1; $index <= $totalGrupos; $index++) {
    $form[$index]['desde'] = array(
      '#type' => 'textfield',
      '#title' => t("Age group no. @number", array('@number' => $index)),
      '#field_prefix' => t('From'),
      '#default_value' => isset($gruposEdad[$index][FilterByInterval::DESDE]) ? $gruposEdad[$index][FilterByInterval::DESDE] : 0,
      '#size' => 6,
      '#maxlength' => 6,
      '#required' => TRUE,
    );

    $form[$index]['hasta'] = array(
      '#type' => 'textfield',
      '#field_prefix' => t('to'),
      '#field_suffix' => t('years'),
      '#default_value' => isset($gruposEdad[$index][FilterByInterval::HASTA]) ? $gruposEdad[$index][FilterByInterval::HASTA] : 0,
      '#size' => 6,
      '#maxlength' => 6,
      '#required' => TRUE,
    );
  }

  if ($totalGrupos < $numeroMaximoGrupos) {
    $form['grupo_kilometraje_add_group_button'] = array(
      '#type' => 'button',
      '#value' => t('Add new group'),
      '#name' => 'grupo_kilometraje_add_group_button',
      '#limit_validation_errors' => array(),
      '#ajax' => array(
        'callback' => 'ajax_grupos_kilometraje_config_callback',
        'wrapper' => 'div-config-grupo-kilometraje',
      ),
    );
  }

  if ($totalGrupos > $numeroMinimoGrupos) {
    $form['grupo_kilometraje_delete_group_button'] = array(
      '#type' => 'button',
      '#value' => t('Delete last group'),
      '#name' => 'grupo_kilometraje_delete_group_button',
      '#limit_validation_errors' => array(),
      '#ajax' => array(
        'callback' => 'ajax_grupos_kilometraje_config_callback',
        'wrapper' => 'div-config-grupo-kilometraje',
      ),
    );
  }

  return $form;
}

function ajax_grupos_kilometraje_config_callback($form, &$form_state) {
  return $form['grupo_kilometraje'];
}

function rjsimulador_configuration_admin_form_validate($form, &$form_state) {
  if (!isset($form_state['values']['select_grupo_default']) || !is_numeric($form_state['values']['select_grupo_default'])) {
    form_set_error('select_grupo_default', t('Group default is not correct.'));
  }

  $groupNameArray = array(
    'grupo_edad' => t('Age'),
    'grupo_experiencia' => t('Driving Experience'),
    'grupo_kilometraje' => t('Average Annual Mileage')
  );

  foreach ($groupNameArray as $grupoName => $description) {
    if (isset($form_state[$grupoName . '_number']['#value']) && is_numeric($form_state[$grupoName . '_number']['#value'])) {
      // Comprobamos que todos los datos pasados son enteros
      for ($index = 1; $index <= $form_state[$grupoName . '_number']['#value']; $index++) {
        $grupos[$index] = $form_state['values'][$grupoName][$index];

        if (!is_numeric($grupos[$index]['desde'])) {
          form_set_error($grupoName . '][' . $index . '][desde', t('Group @description @index "From" field must be an integer.',
            array(
              '@name' => $description,
              '@index' => $index
            )));
        }

        if (!is_numeric($grupos[$index]['hasta'])) {
          form_set_error($grupoName . '][' . $index . '][hasta', t('Group @description @index "To" field must be an integer.',
            array(
              '@name' => $description,
              '@index' => $index
            )));
        }
      }

      if (isset($grupos)) {
        for ($index = 1; $index < $form_state[$grupoName . '_number']['#value']; $index++) {
          if (is_numeric($grupos[$index]['hasta']) && is_numeric($grupos[$index]['desde'])) {
            if ($grupos[$index]['desde'] >= $grupos[$index]['hasta']) {
              form_set_error($grupoName . '][' . $index, t('Group @description @index "To" must be greater than "From".',
                array('@description' => $description, '@index' => $index)));
            }
          }

          if (is_numeric($grupos[$index]['hasta']) && is_numeric($grupos[$index + 1]['desde'])) {
            if ($grupos[$index]['hasta'] != $grupos[$index + 1]['desde']) {
              form_set_error($grupoName . '][' . ($index + 1) . '][desde',
                t('Group @description @index1 "From" must be equal to Group @index "To".', array(
                  '@description' => $description,
                  '@index1' => $index + 1,
                  '@index' => $index
                )));
            }
          }
        }
      }
    }
  }
}

function rjsimulador_configuration_admin_form_submit($form, &$form_state) {
  if (isset($form_state['values']['select_grupo_default']) && is_numeric($form_state['values']['select_grupo_default'])) {
    variable_set('rjsimulador_grupo_default', $form_state['values']['select_grupo_default']);
  }

  if (isset($form_state['grupo_edad_number']['#value']) && is_numeric($form_state['grupo_edad_number']['#value'])) {
    for ($index = 1; $index <= $form_state['grupo_edad_number']['#value']; $index++) {
      $gruposEdad[$index] = $form_state['values']['grupo_edad'][$index];
    }

    if (isset($gruposEdad)) {
      variable_set('rjsimulador_grupos_edad', $gruposEdad);
    }
  }

  if (isset($form_state['grupo_experiencia_number']['#value']) && is_numeric($form_state['grupo_experiencia_number']['#value'])) {
    for ($index = 1; $index <= $form_state['grupo_experiencia_number']['#value']; $index++) {
      $gruposExperiencia[$index] = $form_state['values']['grupo_experiencia'][$index];
    }

    if (isset($gruposExperiencia)) {
      variable_set('rjsimulador_grupos_experiencia', $gruposExperiencia);
    }
  }

  if (isset($form_state['grupo_kilometraje_number']['#value']) && is_numeric($form_state['grupo_kilometraje_number']['#value'])) {
    for ($index = 1; $index <= $form_state['grupo_kilometraje_number']['#value']; $index++) {
      $gruposKilometraje[$index] = $form_state['values']['grupo_kilometraje'][$index];
    }

    if (isset($gruposKilometraje)) {
      variable_set('rjsimulador_grupos_kilometraje', $gruposKilometraje);
    }
  }

  drupal_set_message(t('Groups correctly saved.'), 'status');
}