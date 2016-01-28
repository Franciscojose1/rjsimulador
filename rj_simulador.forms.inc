<?php

function rj_simulador_simulaciones_upper_left_form($form, &$form_state, GestorSimulaciones $gestorSimulaciones)
{
  dpm($form_state);
  $listaPartidas = $gestorSimulaciones->retrieveAllSimulationsPartidas();

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

  $form['chart'] = createPieChart("Procentaje infracciones totales", $labels, $data);
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

  foreach ($gestorSimulaciones->getListaSimulaciones() as $simulacion) {
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

function createPieChart($title, array $labels, array $data)
{
  $chart = array(
    '#type' => 'chart',
    '#title' => t('Pie simple'),
    '#chart_type' => 'pie',
    '#chart_library' => 'highcharts',
    '#legend_position' => 'bottom',
    '#data_labels' => TRUE,
    '#tooltips' => TRUE
  );
  $chart['pie_data'] = array(
    '#type' => 'chart_data',
    '#title' => t($title),
    '#labels' => $labels,
    '#data' => $data,
  );

  return $chart;
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



function rj_simulador_partidas_form($form, &$form_state)
{
  // Recuperamos el usuario original
  $usuario = user_uid_optional_load();

  // LAnzamos el GestorSimulaciones y cargamos todas las Partidas de las simulaciones pasadas
  $idsSimulaciones = array(GestorSimulaciones::SIMULACION_UNO, GestorSimulaciones::SIMULACION_DOS, GestorSimulaciones::SIMULACION_TRES,
    GestorSimulaciones::SIMULACION_CUATRO, GestorSimulaciones::SIMULACION_CINCO);
  $gestorSimulaciones = new GestorSimulaciones($idsSimulaciones, $usuario->uid);


  $renderArrayChart = createPieChart();

  $arrayData = array();
  $xLabels = array();

  foreach ($gestorSimulaciones->getListaSimulaciones() as $simulacion) {
    $arrayData[] = $simulacion->getListaPartidas()->count();
    $xLabels[] = $simulacion->getNombreSimulacion();
  }

  $renderArrayChart2 = createColumnChart($arrayData, $xLabels);

  $listadoPartidas = $gestorSimulaciones->retrieveAllSimulationsPartidas();

  // Creamos la tabla a sacar para mostrar las partidas
  // Headers array
  $headers = array(
    array('data' => t('Fecha'), 'field' => 'Fecha', 'sort' => 'desc'),
    array('data' => t('Nombre Simulación'), 'field' => 'NombreSimulacion'),
    array('data' => t('Consumo Medio')),
    array('data' => t('Consumo Total')),
    array('data' => t('Tiempo Total')),
    array('data' => t('Enlaces'))
  );

  $listadoPartidas = $listadoPartidas->filterBy(new FilterByFecha(array('fecha_fin' => new DateTime("21-01-2016"))));

  // Getting the current sort and order parameters from the url
  $order = tablesort_get_order($headers);
  $sort = tablesort_get_sort($headers);

  if (isset($order) && isset($sort)) {
    $listadoPartidas->sortBy($order['sql'], $sort);
  }

  $rows = array();
  foreach ($listadoPartidas as $partida) {
    $rows[] = array(
      array('data' => $partida->getFechaAsObject()->format("Y-m-d H:i:s")),
      array('data' => $partida->getNombreSimulacion()),
      array('data' => $partida->getConsumoMedio()),
      array('data' => $partida->getConsumoTotal()),
      array('data' => $partida->getTiempoTotal()),
      array('data' => $partida->getURLToPartidaPage('html_link'))
    );
  }

  $renderArrayTablePartidas = null;
  if (!empty($rows)) {
    $limit = 2;
    $page = pager_default_initialize(count($rows), $limit, 0);
    $offset = $limit * $page;
    $renderArrayTablePartidas =
      array(
        array(
          '#theme' => 'table',
          '#header' => $headers,
          '#rows' => array_slice($rows, $offset, $limit),
          '#empty' => t('No tienes partidas almacenadas en el servidor.')
        ),
        array(
          '#theme' => 'pager',
          '#element' => 0,
          '#parameters' => array('filter' => isset($form_state['storage']['name']) ? $form_state['storage']['name'] : "")
        ),
      );
  }


  $form['upper_left'] = array(
    '#type' => 'fieldset',
    '#attributes' => array('style' => 'width:50%;')
  );

  $form['upper_left']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
  );
  $form['upper_left']['content'] = array($renderArrayChart);

  $form['upper_right'] = array(
    '#type' => 'container'
  );

  $form['upper_right']['apellidos'] = array(
    '#type' => 'textfield',
    '#title' => t('Apellidos'),
  );

  $form['upper_right']['content'] = array($renderArrayChart2);

  $form['main'] = array(
    '#type' => 'fieldset',
  );

  $form['main']['content'] = array($renderArrayTablePartidas);

  // Adds a simple submit button that refreshes the form and clears its contents -- this is the default behavior for forms.
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );

  return $form;
}