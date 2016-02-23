<?php
/* *************************************************************************** */
/* *                     FORMULARIO POR SIMULACIÓN                             */
/* *************************************************************************** */
function rjsim_datos_simulaciones_usuarios_admin_form($form, &$form_state, GestorSimulaciones $gestorSimulaciones) {
  // Recuperamos el provider
  $provider = FactoryDataProvider::createDataProvider();

  $form['data_analysis']  = array(
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
    '#group' => 'data_analysis'
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

  $form['infractions_group_analysi'] = array(
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

function generate_form_data_general_analysis (&$form_state, GestorSimulaciones $gestorSimulaciones, DataProvider $provider) {
   // Recuperamos el idSimulacion por el que filtrar la gráfica
  $idSimulacion = isset($form_state['values']['data_general_select_simulacion']) ?
    $form_state['values']['data_general_select_simulacion'] : 1;
  $tipoDatoCalcular = isset($form_state['values']['data_general_select_type']) ?
    $form_state['values']['data_general_select_type'] : CalculateAverageData::CONSUMO_MEDIO;

  if ($tipoDatoCalcular == CalculateAverageData::CONSUMO_MEDIO) {
    $tituloGrafico = t("Average consumption of all users for Simulation ") . $idSimulacion;
  }
  else if ($tipoDatoCalcular == CalculateAverageData::TIEMPO_TOTAL) {
    $tituloGrafico = t("Average Partida time length of all users for Simulation ") . $idSimulacion;
  }
  else if ($tipoDatoCalcular == CalculateAverageData::VELOCIDAD) {
    $tituloGrafico = t("Average speed by Partida of all users for Simulation ") . $idSimulacion;
  }
  else {
    $tituloGrafico = t("Average RPMs by Partida of all users for Simulation ") . $idSimulacion;
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
  $idSimulacion = isset($form_state['values']['select_simulacion']) ? $form_state['values']['select_simulacion'] : 1;
  $tipoDatoCalcular = isset($form_state['values']['select_data']) ? $form_state['values']['select_data'] : CalculateAverageData::CONSUMO_MEDIO;

  if ($tipoDatoCalcular == CalculateAverageData::CONSUMO_MEDIO) {
    $tituloGrafico = t("Average consumption of all users for Simulation @idSimulacion", array('@idSimulacion' => $idSimulacion));
  }
  else if ($tipoDatoCalcular == CalculateAverageData::TIEMPO_TOTAL) {
    $tituloGrafico = t("Average Partida time length of all users for Simulation ") . $idSimulacion;
  }
  else if ($tipoDatoCalcular == CalculateAverageData::VELOCIDAD) {
    $tituloGrafico = t("Average speed by Partida of all users for Simulation ") . $idSimulacion;
  }
  else {
    $tituloGrafico = t("Average RPMs by Partida of all users for Simulation ") . $idSimulacion;
  }

  $form['data_analysis_by_group']['actions'] = array(
    '#type' => 'actions',
    '#weight' => -50,
  );

  $form['data_analysis_by_group']['actions']['select_simulacion'] = array(
    '#type' => 'select',
    '#title' => t('Select Simulation'),
    '#options' => $provider->loadAllIdsSimulaciones(),
    '#default_value' => 1,
    '#description' => t('Select the simulation about you want information.'),
    '#ajax' => array(
      'callback' => 'ajax_generate_form_data_analysis_by_group_callback',
      'wrapper' => 'div-chart-datos-medios-group',
    ),
  );

  $form['data_analysis_by_group']['actions']['select_data'] = array(
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
      'callback' => 'ajax_generate_form_data_analysis_by_group_callback',
      'wrapper' => 'div-chart-datos-medios-group',
    ),
  );

  // Eje X
  $labelAxisX[] = t("Groups of users");

  $form['data_analysis_by_group']['chart_datos_medios_group'] = array(
    '#type' => 'chart',
    '#chart_type' => 'column',
    '#chart_library' => 'highcharts',
    '#title' => check_plain($tituloGrafico),
    '#weight' => 0,
    '#prefix' => '<div id="div-chart-datos-medios-group">',
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

    $form['data_analysis_by_group']['chart_datos_medios_group']['data_group_' . $usuario->getUid()] =
      array(
        '#type' => 'chart_data',
        '#title' => t('User ') . $usuario->getName(),
        '#data' => array($dataUsuario),
      );
  }

  // This is the field fieldset.
  $form['data_analysis_by_group']['groups'] = array(
    '#type' => 'fieldset',
    '#title' => t('Group'),
    '#collapsible' => TRUE,
    '#description' => t('Filter group'),
  );
  $form['data_analysis_by_group']['groups']['check_age'] = array(
    '#type' => 'checkbox',
    '#title' => t('Filter by age.'),
  );
  $form['data_analysis_by_group']['groups']['age']  = array(
    '#type' => 'fieldset',
    '#states' => array(
      // Only show this field when the 'toggle_me' checkbox is enabled.
      'visible' => array(
        ':input[name="check_age"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['data_analysis_by_group']['groups']['age']['age_from'] = array(
    '#type' => 'textfield',
    '#field_prefix' => t('From'),
    '#size' => 3,
    '#maxlength' => 3,
    '#required' => TRUE,
  );
  $form['data_analysis_by_group']['groups']['age']['age_to'] = array(
    '#type' => 'textfield',
    '#field_prefix' => t('to'),
    '#field_suffix' => t(' years'),
    '#size' => 3,
    '#maxlength' => 3,
    '#required' => TRUE,
  );

  return $form;
}

function ajax_generate_form_data_group_analysis_callback($form, &$form_state) {
  return $form['data_analysis_by_group']['chart_datos_medios_group'];
}


