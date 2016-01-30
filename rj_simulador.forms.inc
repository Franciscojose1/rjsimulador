<?php

function rj_simulador_simulaciones_upper_left_form($form, &$form_state, GestorSimulaciones $gestorSimulaciones)
{
  $listaPartidas = $gestorSimulaciones->retrieveAllPartidas();

  $provider = FactoryDataProvider::createDataProvider();
  $idsInfracciones = $provider->getAllIdsInfracciones();

  $listaResultados = new ListaInfracciones();
  foreach ($listaPartidas as $partida){
    $listaResultados->mergeList($partida->getListaInfracciones());
  }

  $labels = array();
  $data = array();
  foreach($idsInfracciones as $id=>$nombre_infraccion){
    $labels[] = check_plain($nombre_infraccion);
    $data[] = $listaResultados->filterBy(new FilterByID(array($id)))->count();
  }

  $form['chart'] = array(
    '#type' => 'chart',
    '#title' => t("Porcentaje infracciones totales"),
    '#chart_type' => 'pie',
    '#chart_library' => 'highcharts',
    '#legend_position' => 'bottom',
    '#data_labels' => TRUE,
    '#tooltips' => TRUE,
    'pie_data' =>
      array(
        '#type' => 'chart_data',
        '#title' => t('Infracciones'),
        '#labels' => $labels,
        '#data' => $data,
      )
  );
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
  );
  // Adds a simple submit button that refreshes the form and clears its contents -- this is the default behavior for forms.
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );

  return $form;
}

function rj_simulador_simulaciones_upper_left_form_submit($form, &$form_state)
{
  $form_state['rebuild'] = TRUE;
}

function rj_simulador_simulaciones_upper_right_form ($form, $form_state, GestorSimulaciones $gestorSimulaciones) {
  $arrayData = array();
  $xLabels = array();

  foreach ($gestorSimulaciones->getListaSimulacionesUsuarioActual() as $simulacion) {
    $arrayData[] = $simulacion->getListaPartidas()->count();
    $xLabels[] = $simulacion->getNombreSimulacion();
  }

  $form['chart'] = createColumnChart($arrayData, $xLabels);
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name')
  );

  return $form;
}

function createColumnChart($arrayNumeroPartidas, $arrayXLabels)
{
  $chart = array(
    '#type' => 'chart',
    '#chart_type' => 'column',
    '#chart_library' => 'highcharts',
    '#title' => t('Número de partidas jugadas a cada simulación'),
  );
  $chart['simulation'] = array(
    '#type' => 'chart_data',
    '#title' => t('Nº'),
    '#data' => $arrayNumeroPartidas,
    '#suffix' => 'Nº partidas',
  );
  $chart['xaxis'] = array(
    '#type' => 'chart_xaxis',
    '#labels' => $arrayXLabels,
  );

  return $chart;
}