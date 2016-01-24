<?php

function createPieChart() {
  $chart = array(
    '#type' => 'chart',
    '#title' => t('Pie simple'),
    '#chart_type' => 'pie',
    '#chart_library' => 'highcharts',
    '#legend_position' => 'right',
    '#data_labels' => TRUE,
    '#tooltips' => TRUE
  );
  $chart['pie_data'] = array(
    '#type' => 'chart_data',
    '#title' => t('Gender'),
    '#labels' => array('Male', 'Female'),
    '#data' => array(10, 20),
  );
  $chart['pie_data'][0] = array(
    '#type' => 'chart_data_item',
    '#data' => 15,
    '#color' => 'red',
    '#title' => t('Dudes'),
  );
  $chart['pie_data'][1] = array(
    '#type' => 'chart_data_item',
    '#data' => 20,
    '#title' => t('Chicks'),
  );

  return $chart;
}

function createColumnChart($arrayNumeroPartidas, $arrayXLabels) {
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

function rj_simulador_pagina_form($form, &$form_state) {
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

  if(isset($order) && isset($sort)) {
    $listadoPartidas->sortBy($order['sql'],$sort);
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
          '#empty'=> t('No tienes partidas almacenadas en el servidor.')
        ),
        array(
          '#theme' => 'pager',
          '#element' => 0,
          '#parameters' => array('filter' => isset($form_state['storage']['name'])?$form_state['storage']['name']:"")
        ),
      );
  }


  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
  );

  $sip['apellidos'] = array(
    '#type' => 'textfield',
    '#title' => t('Apellidos'),
  );

  // Adds a simple submit button that refreshes the form and clears its contents -- this is the default behavior for forms.
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );

  $form['hola'] = array(
    '#theme' => 'lista_partidas',
    '#upper_left' => array($renderArrayChart, $sip),
    '#upper_right' => array($renderArrayChart2),
    '#main_content' => array($renderArrayTablePartidas)
  );

  return $form;
}

function rj_simulador_pagina_form_submit($form,&$form_state){
  $form_state['storage']['name'] = $form_state['values']['name'];
  $form_state['rebuild'] = TRUE;
}

function partida_page($id_partida) {

  $miPartida = Partida::loadPartidaById($id_partida);

  // Creamos la tabla a sacar para mostrar las partidas
  // Headers array
  $headers = array(
    array('data' => t('Instante') ),
    array('data' => t('Velocidad')),
    array('data' => t('Revoluciones (RPM)')),
    array('data' => t('Marcha')),
    array('data' => t('Consumo Instantaneo'))
  );

  $rows = array();
  foreach ($miPartida->getListaDatos() as $dato) {
    $rows[] = array(
      array('data' => $dato->getInstante()),
      array('data' => $dato->getVelocidad()),
      array('data' => $dato->getRpm()),
      array('data' => $dato->getMarcha()),
      array('data' => $dato->getConsumoInstantaneo())
    );
  }

  $renderArrayTableDatos = null;
  if (!empty($rows)) {
    $limit = 10;
    $page = pager_default_initialize(count($rows), $limit, 0);
    $offset = $limit * $page;
    $renderArrayTableDatos =
      array(
        array(
          '#theme' => 'table',
          '#header' => $headers,
          '#rows' => array_slice($rows, $offset, $limit),
          '#empty'=> t('No hay datos para esta partida.')
        ),
        array(
          '#theme' => 'pager',
          '#element' => 0
        ),
      );
  }

  // Creamos la tabla a sacar para mostrar las partidas
  // Headers array
  $headers = array(
    array('data' => t('Instante') ),
    array('data' => t('Nombre Infracción')),
    array('data' => t('Observaciones'))
  );

  $rows = array();
  foreach ($miPartida->getListaInfracciones() as $elemento_lista) {
    $rows[] = array(
      array('data' => $elemento_lista->getInstante()),
      array('data' => $elemento_lista->getNombreInfraccion()),
      array('data' => $elemento_lista->getObservaciones())
    );
  }

  $renderArrayTableInfracciones = null;
  if (!empty($rows)) {
    $limit = 10;
    $page = pager_default_initialize(count($rows), $limit, 1);
    $offset = $limit * $page;
    $renderArrayTableInfracciones =
      array(
        array(
          '#theme' => 'table',
          '#header' => $headers,
          '#rows' => array_slice($rows, $offset, $limit),
          '#empty'=> t('No hay infracciones para esta partida.')
        ),
        array(
          '#theme' => 'pager',
          '#element' => 1
        ),
      );
  }

  $link = array(
    '#type' => 'link',
    '#title' => 'Go Back',
    '#href' => 'mis_partidas',
    '#options' => array('attributes' => array('title' => 'back',
                                              'style' => 'padding:8px;appearance:button;-webkit-appearance:button;text-decoration:none;'), 'html' => TRUE)
  );

  $a = array(
    '#type' => 'container',
    '#attributes' => array('id' => 'my-id', 'style' => array('text-align:center;','color:red;')),
    'enlace' => $link
  );

  $renderArrayFinal = array(
    '#type' => 'container',
    'content' => array($renderArrayTableDatos, $renderArrayTableInfracciones, $a)
  );

  return $renderArrayFinal;
}