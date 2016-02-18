<?php
/* *************************************************************************** */
/* *                     FORMULARIO POR SIMULACIÓN                             */
/* *************************************************************************** */
function rjsim_datos_simulaciones_usuarios_admin_form($form, &$form_state, GestorSimulaciones $gestorSimulaciones) {
  // Recuperamos el provider
  $provider = FactoryDataProvider::createDataProvider();

  // Recuperamos el idSimulacion por el que filtrar la gráfica
  $idSimulacion = isset($form_state['values']['select_simulacion']) ? $form_state['values']['select_simulacion'] : 1;
  $tipoDatoCalcular = isset($form_state['values']['select_data']) ? $form_state['values']['select_data'] : CalculateAverageData::CONSUMO_MEDIO;

  if ($tipoDatoCalcular == CalculateAverageData::CONSUMO_MEDIO) {
    $tituloGrafico = t("Consumo medio de los usuarios para la Simulación ") . $idSimulacion;
  }
  else {
    $tituloGrafico = t("Duración media por partida de los usuarios para la Simulación ") . $idSimulacion;
  }

  // Eje X
  $labelAxisX[] = t("Users");

  $form['chart_datos_medios'] = array(
    '#type' => 'chart',
    '#chart_type' => 'column',
    '#chart_library' => 'highcharts',
    '#title' => $tituloGrafico,
    '#prefix' => '<div id="div-chart-datos-medios">',
    '#suffix' => '</div>',
    'xaxis' =>
      array(
        '#type' => 'chart_xaxis',
        '#labels' => $labelAxisX,
      ),
  );

  foreach ($gestorSimulaciones->getListaTodosUsuarios() as $usuario) {
    // Datos medios por simulación
    $dataUsuario = $usuario->retrieveAllPartidasByIdSimulacion($idSimulacion)
        ->calculateData(new CalculateAverageData($tipoDatoCalcular));

    $form['chart_datos_medios']['usuario_' . $usuario->getUid()] =
      array(
        '#type' => 'chart_data',
        '#title' => t('User ') . $usuario->getName(),
        '#data' => array($dataUsuario),
      );
  }

  $form['actions'] = array(
    '#type' => 'actions',
  );

  $form['actions']['select_simulacion'] = array(
    '#type' => 'select',
    '#title' => t('Select Simulation'),
    '#options' => $provider->getAllIdsSimulaciones(),
    '#default_value' => 1,
    '#description' => t('Select the simulation about you want information.'),
    '#ajax' => array(
      'callback' => 'ajax_chart_datos_medios_callback',
      'wrapper' => 'div-chart-datos-medios',
    ),
  );

  $form['actions']['select_data'] = array(
    '#type' => 'select',
    '#title' => t('Select data to show'),
    '#options' =>
      array(
        CalculateAverageData::CONSUMO_MEDIO => t('Consumo medio'),
        CalculateAverageData::TIEMPO_TOTAL => t('Tiempo medio'),
      ),
    '#default_value' => CalculateAverageData::CONSUMO_MEDIO,
    '#description' => t('Select the simulation about you want information.'),
    '#ajax' => array(
      'callback' => 'ajax_chart_datos_medios_callback',
      'wrapper' => 'div-chart-datos-medios',
    ),
  );

  return $form;
}

function ajax_chart_datos_medios_callback($form, &$form_state) {
  return $form['chart_datos_medios'];
}