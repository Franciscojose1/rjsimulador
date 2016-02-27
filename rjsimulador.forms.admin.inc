<?php
/* *************************************************************************** */
/* *                     FORMULARIO POR SIMULACIÓN                             */
/* *************************************************************************** */
function rjsim_datos_simulaciones_usuarios_admin_form($form, &$form_state, GestorSimulaciones $gestorSimulaciones) {
  // Recuperamos el provider
  $provider = FactoryDataProvider::createDataProvider();

  $form['data_analysis'] = array(
    '#type' => 'vertical_tabs',
  );

  // This is the field fieldset.
  $form['data_general_analysis'] = array(
    '#type' => 'fieldset',
    '#title' => t('General data analysis'),
    '#collapsible' => TRUE,
    '#description' => t('General data analysis for all users.'),
    '#group' => 'data_analysis'
  );

  $form['data_general_analysis'] += generate_form_data_general_analysis($form_state, $gestorSimulaciones, $provider);


  $form['data_group_analysis'] = array(
    '#type' => 'fieldset',
    '#title' => t('Data analysis by group'),
    '#collapsible' => TRUE,
    '#description' => t('Data analysis by group.'),
    '#group' => 'data_analysis',
  );

  $form['data_group_analysis'] += generate_form_data_group_analysis($form_state, $gestorSimulaciones, $provider);

  $form['data_user_analysis'] = array(
    '#type' => 'fieldset',
    '#title' => t('Data analysis by user'),
    '#collapsible' => TRUE,
    '#description' => t('Data analysis by user.'),
    '#group' => 'data_analysis'
  );

  $form['infractions_general_analysis'] = array(
    '#type' => 'fieldset',
    '#title' => t('General infraction analysis'),
    '#collapsible' => TRUE,
    '#description' => t('General infractions analysis for all users.'),
    '#group' => 'data_analysis'
  );

  $form['infractions_group_analysis'] = array(
    '#type' => 'fieldset',
    '#title' => t('Infraction analysis by group'),
    '#collapsible' => TRUE,
    '#description' => t('Infraction analysis by group.'),
    '#group' => 'data_analysis'
  );

  $form['infractions_user_analysis'] = array(
    '#type' => 'fieldset',
    '#title' => t('Data analysis by user'),
    '#collapsible' => TRUE,
    '#description' => t('Infraction analysis by user.'),
    '#group' => 'data_analysis'
  );

  return $form;
}

function generate_form_data_general_analysis(&$form_state, GestorSimulaciones $gestorSimulaciones, DataProvider $provider) {
  // Recuperamos el idSimulacion por el que filtrar la gráfica
  $idSimulacion = isset($form_state['input']['data_general_select_simulacion']) ?
    $form_state['input']['data_general_select_simulacion'] : 1;
  $tipoDatoCalcular = isset($form_state['input']['data_general_select_type']) ?
    $form_state['input']['data_general_select_type'] : CalculateAverageData::CONSUMO_MEDIO;

  if ($tipoDatoCalcular == CalculateAverageData::CONSUMO_MEDIO) {
    $tituloGrafico = t("Average consumption of all users for Simulation ") . $idSimulacion;
  }
  else {
    if ($tipoDatoCalcular == CalculateAverageData::TIEMPO_TOTAL) {
      $tituloGrafico = t("Average Partida time length of all users for Simulation ") . $idSimulacion;
    }
    else {
      if ($tipoDatoCalcular == CalculateAverageData::VELOCIDAD) {
        $tituloGrafico = t("Average speed by Partida of all users for Simulation ") . $idSimulacion;
      }
      else {
        $tituloGrafico = t("Average RPMs by Partida of all users for Simulation ") . $idSimulacion;
      }
    }
  }

  // Eje X
  $labelAxisX[] = t("Users");

  $form['data_general_chart'] = array(
    '#type' => 'chart',
    '#chart_type' => 'column',
    '#chart_library' => 'highcharts',
    '#title' => check_plain($tituloGrafico),
    '#prefix' => '<div id="div-data-general-chart">',
    '#suffix' => '</div>',
    'xaxis' =>
      array(
        '#type' => 'chart_xaxis',
        '#labels' => $labelAxisX,
      ),
  );

  foreach ($gestorSimulaciones->getListaTodosUsuarios() as $usuario) {
    // Datos medios por simulación
    $dataUsuario = round($usuario->retrieveAllPartidasByIdSimulacion($idSimulacion)
      ->calculateData(new CalculateAverageData($tipoDatoCalcular)), 2);

    $form['data_general_chart']['data_general_user_' . $usuario->getUid()] =
      array(
        '#type' => 'chart_data',
        '#title' => t('User ') . $usuario->getName(),
        '#data' => array($dataUsuario),
      );
  }

  $form['data_general_actions'] = array(
    '#type' => 'actions',
  );

  $form['data_general_actions']['data_general_select_simulacion'] = array(
    '#type' => 'select',
    '#title' => t('Select Simulation'),
    '#options' => $provider->loadAllIdsSimulaciones(),
    '#default_value' => 1,
    '#description' => t('Select the simulation about you want information.'),
    '#ajax' => array(
      'callback' => 'ajax_generate_form_data_general_analysis_callback',
      'wrapper' => 'div-data-general-chart',
    ),
  );

  $form['data_general_actions']['data_general_select_type'] = array(
    '#type' => 'select',
    '#title' => t('Select data to show'),
    '#options' =>
      array(
        CalculateAverageData::CONSUMO_MEDIO => t('Average consumption'),
        CalculateAverageData::TIEMPO_TOTAL => t('Average time'),
        CalculateAverageData::VELOCIDAD => t('Average speed'),
        CalculateAverageData::RPM => t('Average RPMs')
      ),
    '#default_value' => CalculateAverageData::CONSUMO_MEDIO,
    '#description' => t('Select the simulation about you want information.'),
    '#ajax' => array(
      'callback' => 'ajax_generate_form_data_general_analysis_callback',
      'wrapper' => 'div-data-general-chart',
    ),
  );

  return $form;
}

function ajax_generate_form_data_general_analysis_callback($form, &$form_state) {
  return $form['data_general_analysis']['data_general_chart'];
}

function generate_form_data_group_analysis(&$form_state, GestorSimulaciones $gestorSimulaciones, DataProvider $provider) {
  // Recuperamos el idSimulacion por el que filtrar la gráfica
  $idSimulacion = isset($form_state['input']['data_group_select_simulacion']) ?
    $form_state['input']['data_group_select_simulacion'] : 1;
  $tipoDatoCalcular = isset($form_state['input']['data_group_select_data']) ?
    $form_state['input']['data_group_select_data'] : CalculateAverageData::CONSUMO_MEDIO;

  if ($tipoDatoCalcular == CalculateAverageData::CONSUMO_MEDIO) {
    $tituloGrafico = t("Average consumption of all users for Simulation @idSimulacion", array('@idSimulacion' => $idSimulacion));
  }
  else {
    if ($tipoDatoCalcular == CalculateAverageData::TIEMPO_TOTAL) {
      $tituloGrafico = t("Average Partida time length of all users for Simulation ") . $idSimulacion;
    }
    else {
      if ($tipoDatoCalcular == CalculateAverageData::VELOCIDAD) {
        $tituloGrafico = t("Average speed by Partida of all users for Simulation ") . $idSimulacion;
      }
      else {
        $tituloGrafico = t("Average RPMs by Partida of all users for Simulation ") . $idSimulacion;
      }
    }
  }

  //dpm($form_state);

  // Eje X
  $labelAxisX[] = t("Groups of users");

  $form['data_group_container']= array(
    '#prefix' => '<div id="div-data-group">',
    '#suffix' => '</div>',
  );

  $form['data_group_container']['data_group_chart'] = array(
    '#type' => 'chart',
    '#chart_type' => 'column',
    '#chart_library' => 'highcharts',
    '#title' => check_plain($tituloGrafico),
    '#weight' => 0,
    'xaxis' =>
      array(
        '#type' => 'chart_xaxis',
        '#labels' => $labelAxisX,
      ),
  );

  $form['data_group_container']['data_group_actions'] = array(
    '#type' => 'actions',
    '#weight' => 25,
  );
  $form['data_group_container']['data_group_actions']['data_group_select_simulacion'] = array(
    '#type' => 'select',
    '#title' => t('Select Simulation'),
    '#options' => $provider->loadAllIdsSimulaciones(),
    '#default_value' => 1,
    '#description' => t('Select the simulation about you want information.'),
    '#ajax' => array(
      'callback' => 'ajax_generate_form_data_group_analysis_callback',
      'wrapper' => 'div-data-group',
    ),
  );
  $form['data_group_container']['data_group_actions']['data_group_select_data'] = array(
    '#type' => 'select',
    '#title' => t('Select data to show'),
    '#options' =>
      array(
        CalculateAverageData::CONSUMO_MEDIO => t('Average consumption'),
        CalculateAverageData::TIEMPO_TOTAL => t('Average time'),
        CalculateAverageData::VELOCIDAD => t('Average speed'),
        CalculateAverageData::RPM => t('Average RPMs')
      ),
    '#default_value' => CalculateAverageData::CONSUMO_MEDIO,
    '#description' => t('Select the simulation about you want information.'),
    '#ajax' => array(
      'callback' => 'ajax_generate_form_data_group_analysis_callback',
      'wrapper' => 'div-data-group',
    ),
  );

  $form['data_group_container']['data_group_actions']['data_group_filter_button'] = array(
    '#type' => 'button',
    '#value' => t('Filter'),
    '#name' => 'data_group_filter_button',
    '#limit_validation_errors' => array(
      array('data_group_groups'),
    ),
    '#validate' => array('generate_form_data_group_analysis_validate'),
    '#ajax' => array(
      'callback' => 'ajax_generate_form_data_group_analysis_callback',
      'wrapper' => 'div-data-group',
    ),
  );

  //dpm($form_state['complete form']['data_group_analysis']['data_group_groups']);

  $form_state['data_group_number_groups'] = isset($form_state['data_group_number_groups']) ? $form_state['data_group_number_groups'] : 1;

  if (isset($form_state['clicked_button'])) {
    if ($form_state['clicked_button']['#name'] == 'data_group_add_group_button') {
      $form_state['data_group_number_groups']++;
    }
    if ($form_state['clicked_button']['#name'] == 'data_group_delete_group_button') {
      $form_state['data_group_number_groups']--;
    }
  }

  if ($form_state['data_group_number_groups'] < 6) {
    $form['data_group_container']['data_group_actions']['data_group_add_group_button'] = array(
      '#type' => 'button',
      '#value' => t('Add new group'),
      '#name' => 'data_group_add_group_button',
      '#limit_validation_errors' => array(),
      '#ajax' => array(
        'callback' => 'ajax_generate_form_data_group_analysis_callback',
        'wrapper' => 'div-data-group',
      ),
    );
  }

  if ($form_state['data_group_number_groups'] > 1) {
    $form['data_group_container']['data_group_actions']['data_group_delete_group_button'] = array(
      '#type' => 'button',
      '#value' => t('Delete last group'),
      '#name' => 'data_group_delete_group_button',
      '#limit_validation_errors' => array(),
      '#ajax' => array(
        'callback' => 'ajax_generate_form_data_group_analysis_callback',
        'wrapper' => 'div-data-group',
      ),
    );
  }

  $form['data_group_container']['data_group_groups'] = array(
    '#type' => 'fieldset',
    '#title' => t('Groups'),
    '#collapsible' => TRUE,
    '#description' => t('All groups'),
    '#tree' => TRUE,
    '#weight' => '50'
  );

  for ($index = 1; $index <= $form_state['data_group_number_groups']; $index++) {
    $form['data_group_container']['data_group_groups'][$index] = generate_group_array('data_group', $index);

    $listaUsuarios = $gestorSimulaciones->getListaTodosUsuarios();
    $retriever = new ListaUsuariosDataRetriever($listaUsuarios);

    // Datos medios por simulación
    $dataGroup = round($retriever->retrieveAllPartidasByIdSimulacion($idSimulacion)
      ->calculateData(new CalculateAverageData($tipoDatoCalcular)), 2);

    $form['data_group_container']['data_group_chart']['data_group_' . $index] =
      array(
        '#type' => 'chart_data',
        '#title' => t('Group @index', array('@index' => $index)),
        '#data' => array($dataGroup),
      );
  }

  return $form;
}

function generate_form_data_group_analysis_validate($form, &$form_state) {
  dpm($form_state);
  $prefix = "data_group";
  foreach ($form_state['values'][$prefix . '_groups'] as $index => $group) {
    $field = "age";
    if($group[$prefix . '_check_' . $field] == 1) {
      $from = trim($group[$prefix . '_' . $field]['from']);
      $to = trim($group[$prefix . '_' . $field]['to']);
      if ($from == "" || (int)$from != $from || $to == "" || (int)$to != $to) {

        if ($from == "" || (int)$from != $from) {
          form_set_error($prefix . '_groups]['.$index.'][' . $prefix . '_' . $field . '][from',
            t('Age from field must be an integer.'));
        }

        if ($to == "" || (int)$to != $to) {
          form_set_error($prefix . '_groups]['.$index.'][' . $prefix . '_' . $field . '][to',
            t('Age to field must be an integer.'));
        }

      } else {
        if ($from < 0 || $from > 120) {
          form_set_error($prefix . '_groups][' . $index . '][' . $prefix . '_' . $field .'][from',
            t('Age from field must be between 0 and 120.'));
        }
        if ($to < 0 || $to > 120) {
          form_set_error($prefix . '_groups][' . $index . '][' . $prefix . '_' . $field . '][to',
            t('Age to field must be between 0 and 120.'));
        }
        if ($from >= $to) {
          form_set_error($prefix . '_groups][' . $index . '][' . $prefix . '_' . $field,
            t('Age from field must be smaller than to field.'));
        }
      }
    }

    $field = "driving_experience";
    if($group[$prefix . '_check_' . $field] == 1) {
      $from = trim($group[$prefix . '_' . $field]['from']);
      $to = trim($group[$prefix . '_' . $field]['to']);
      if ($from == "" || (int)$from != $from || $to == "" || (int)$to != $to) {

        if ($from == "" || (int)$from != $from) {
          form_set_error($prefix . '_groups]['.$index.'][' . $prefix . '_' . $field . '][from',
            t('Driving experience from field must be an integer.'));
        }

        if ($to == "" || (int)$to != $to) {
          form_set_error($prefix . '_groups]['.$index.'][' . $prefix . '_' . $field .'][to',
            t('Driving experience to field must be an integer.'));
        }

      } else {
        if ($from < 0 || $from > 120) {
          form_set_error($prefix . '_groups][' . $index . '][' . $prefix . '_' . $field .'][from',
            t('Driving experience from field must be between 0 and 120.'));
        }
        if ($to < 0 || $to > 120) {
          form_set_error($prefix . '_groups][' . $index . '][' . $prefix . '_' . $field . '][to',
            t('Driving experience to field must be between 0 and 120.'));
        }
        if ($from >= $to) {
          form_set_error($prefix . '_groups][' . $index . '][' . $prefix . '_' . $field,
            t('Driving experience from field must be smaller than to field.'));
        }
      }
    }

    $field = "average_annual_mileage";
    if($group[$prefix . '_check_' . $field] == 1) {
      $from = trim($group[$prefix . '_' . $field]['from']);
      $to = trim($group[$prefix . '_' . $field]['to']);
      if ($from == "" || (int)$from != $from || $to == "" || (int)$to != $to) {

        if ($from == "" || (int)$from != $from) {
          form_set_error($prefix . '_groups]['.$index.'][' . $prefix . '_' . $field . '][from',
            t('Average annual mileage from field must be an integer.'));
        }

        if ($to == "" || (int)$to != $to) {
          form_set_error($prefix . '_groups]['.$index.'][' . $prefix . '_' . $field . '][to',
            t('Average annual mileage to field must be an integer.'));
        }

      } else {
        if ($from < 0 || $from > 500000) {
          form_set_error($prefix . '_groups][' . $index . '][' . $prefix . '_' . $field .'][from',
            t('Average annual mileage from field must be between 0 and 120.'));
        }
        if ($to < 0 || $to > 500000) {
          form_set_error($prefix . '_groups][' . $index . '][' . $prefix . '_' . $field . '][to',
            t('Average annual mileage to field must be between 0 and 120.'));
        }
        if ($from >= $to) {
          form_set_error($prefix . '_groups][' . $index . '][' . $prefix . '_' . $field,
            t('Average annual mileage from field must be smaller than to field.'));
        }
      }
    }

    $field = "usual_videogame_player";
    if($group[$prefix . '_check_' . $field] == 1) {
      $radios = $group[$prefix . '_' . $field]['radios'];
      if ($radios != 0 && $radios != 1) {
        form_set_error($prefix . '_groups][' . $index . '][' . $prefix . '_' . $field . '][radios',
          t('Must select one radio button.'));
      }
    }
  }
}

function ajax_generate_form_data_group_analysis_callback($form, &$form_state) {
  return $form['data_group_analysis']['data_group_container'];
}

function generate_group_array($prefix, $index) {
  // This is the field fieldset.
  $form = array(
    '#type' => 'fieldset',
    '#title' => t('Group @index', array('@index' => $index)),
    '#collapsible' => TRUE,
    '#description' => t('Filter group'),
  );
  $form[$prefix . '_check_age'] = array(
    '#type' => 'checkbox',
    '#title' => t('Filter by age.'),
  );
  $form[$prefix . '_age'] = array(
    '#type' => 'fieldset',
    '#states' => array(
      // Only show this field when the 'toggle_me' checkbox is enabled.
      'visible' => array(
        ':input[name="' . $prefix . '_groups[' . $index . '][' . $prefix . '_check_age]"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form[$prefix . '_age']['from'] = array(
    '#type' => 'textfield',
    '#field_prefix' => t('From'),
    '#size' => 3,
    '#maxlength' => 3,
    '#required' => FALSE,
    '#states' => array(
      // Only show this field when the 'toggle_me' checkbox is enabled.
      'disabled' => array(
        ':input[name="' . $prefix . '_groups[' . $index . '][' . $prefix . '_check_age]"]' => array('checked' => FALSE),
      ),
    ),
  );
  $form[$prefix . '_age']['to'] = array(
    '#type' => 'textfield',
    '#field_prefix' => t('to'),
    '#field_suffix' => t(' years'),
    '#size' => 3,
    '#maxlength' => 3,
    '#required' => FALSE,
    '#states' => array(
      // Only show this field when the 'toggle_me' checkbox is enabled.
      'disabled' => array(
        ':input[name="' . $prefix . '_groups[' . $index . '][' . $prefix . '_check_age]"]' => array('checked' => FALSE),
      ),
    ),
  );

  $form[$prefix . '_check_driving_experience'] = array(
    '#type' => 'checkbox',
    '#title' => t('Filter by driving experience.'),
  );
  $form[$prefix . '_driving_experience'] = array(
    '#type' => 'fieldset',
    '#states' => array(
      // Only show this field when the 'toggle_me' checkbox is enabled.
      'visible' => array(
        ':input[name="' . $prefix . '_groups[' . $index . '][' . $prefix . '_check_driving_experience]"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form[$prefix . '_driving_experience']['from'] = array(
    '#type' => 'textfield',
    '#field_prefix' => t('From'),
    '#size' => 3,
    '#maxlength' => 3,
    '#required' => FALSE,
    '#states' => array(
      // Only show this field when the 'toggle_me' checkbox is enabled.
      'disabled' => array(
        ':input[name="' . $prefix . '_groups[' . $index . '][' . $prefix . '_check_driving_experience]"]' => array('checked' => FALSE),
      ),
    ),
  );
  $form[$prefix . '_driving_experience']['to'] = array(
    '#type' => 'textfield',
    '#field_prefix' => t('to'),
    '#field_suffix' => t(' years'),
    '#size' => 3,
    '#maxlength' => 3,
    '#required' => FALSE,
    '#states' => array(
      // Only show this field when the 'toggle_me' checkbox is enabled.
      'disabled' => array(
        ':input[name="' . $prefix . '_groups[' . $index . '][' . $prefix . '_check_driving_experience]"]' => array('checked' => FALSE),
      ),
    ),
  );

  $form[$prefix . '_check_average_annual_mileage'] = array(
    '#type' => 'checkbox',
    '#title' => t('Filter by average annual mileage.'),
  );
  $form[$prefix . '_average_annual_mileage'] = array(
    '#type' => 'fieldset',
    '#states' => array(
      // Only show this field when the 'toggle_me' checkbox is enabled.
      'visible' => array(
        ':input[name="' . $prefix . '_groups[' . $index . '][' . $prefix . '_check_average_annual_mileage]"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form[$prefix . '_average_annual_mileage']['from'] = array(
    '#type' => 'textfield',
    '#field_prefix' => t('From'),
    '#size' => 3,
    '#maxlength' => 3,
    '#required' => FALSE,
    '#states' => array(
      // Only show this field when the 'toggle_me' checkbox is enabled.
      'disabled' => array(
        ':input[name="' . $prefix . '_groups[' . $index . '][' . $prefix . '_check_average_annual_mileage]"]' => array('checked' => FALSE),
      ),
    ),
  );
  $form[$prefix . '_average_annual_mileage']['to'] = array(
    '#type' => 'textfield',
    '#field_prefix' => t('to'),
    '#field_suffix' => t(' kilometers'),
    '#size' => 6,
    '#maxlength' => 6,
    '#required' => FALSE,
    '#states' => array(
      // Only show this field when the 'toggle_me' checkbox is enabled.
      'disabled' => array(
        ':input[name="' . $prefix . '_groups[' . $index . '][' . $prefix . '_check_average_annual_mileage]"]' => array('checked' => FALSE),
      ),
    ),
  );

  $form[$prefix . '_check_usual_videogame_player'] = array(
    '#type' => 'checkbox',
    '#title' => t('Filter by usual videogame player.'),
  );
  $form[$prefix . '_usual_videogame_player'] = array(
    '#type' => 'fieldset',
    '#states' => array(
      // Only show this field when the 'toggle_me' checkbox is enabled.
      'visible' => array(
        ':input[name="' . $prefix . '_groups[' . $index . '][' . $prefix . '_check_usual_videogame_player]"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form[$prefix . '_usual_videogame_player']['radios'] = array(
    '#type' => 'radios',
    '#title' => t('Usual videogame players?'),
    '#default_value' => 0,
    '#options' => array(0 => t('No'), 1 => t('Yes')),
    '#states' => array(
      // Only show this field when the 'toggle_me' checkbox is enabled.
      'disabled' => array(
        ':input[name="' . $prefix . '_groups[' . $index . '][' . $prefix . '_check_usual_videogame_player]"]' => array('checked' => FALSE),
      ),
    ),
  );

  return $form;
}

