<?php
function listado_partidas_page()
{
  // Recuperamos el usuario original
  $usuario = user_uid_optional_load();

  // LAnzamos el GestorSimulaciones y cargamos todas las Partidas de las simulaciones pasadas
  $idsSimulaciones = array(GestorSimulaciones::SIMULACION_UNO, GestorSimulaciones::SIMULACION_DOS, GestorSimulaciones::SIMULACION_TRES,
    GestorSimulaciones::SIMULACION_CUATRO, GestorSimulaciones::SIMULACION_CINCO);
  $gestorSimulaciones = new GestorSimulaciones($idsSimulaciones, $usuario->uid);

  $listadoPartidas = $gestorSimulaciones->retrieveSimulationsPartidas();

  // Creamos la tabla a sacar para mostrar las partidas
  // Headers array
  $headers = array(
    array('data' => t('Fecha'), 'field' => 'fecha', 'sort' => 'desc'),
    array('data' => t('Nombre Simulación'), 'field' => 'nombre_simulacion'),
    array('data' => t('Consumo Medio')),
    array('data' => t('Consumo Total')),
    array('data' => t('Tiempo Total'))
  );

  // Getting the current sort and order parameters from the url
  $order = tablesort_get_order($headers);
  $sort = tablesort_get_sort($headers);

  if (isset($order) && isset($sort)) {
    usort($listadoPartidas, array("Partida", "sortBy" . $order['sql'] . strtoupper($sort)));
  }

  foreach ($listadoPartidas as $partida) {
    $rows[] = array(
      array('data' => $partida->getFechaAsObject()->format("Y-m-d")),
      array('data' => $partida->getNombreSimulacion()),
      array('data' => $partida->getConsumoMedio()),
      array('data' => $partida->getConsumoTotal()),
      array('data' => $partida->getTiempoTotal()),
    );
  }

  $renderArrayChart = createPieChart();

  $arrayData = array();
  $xLabels = array();
  foreach ($gestorSimulaciones->getSimulaciones() as $simulacion) {
    $arrayData[] = count($simulacion->retrieveListOfPartidas());
    $xLabels[] = $simulacion->getNombreSimulacion();
  }

  $renderArrayChart2 = createColumnChart($arrayData, $xLabels);

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
          '#element' => 0
        ),
      );
  }

  $finalRenderArray = array(
    '#theme' => 'lista_partidas',
    '#bloque_superior' => array($renderArrayChart,$renderArrayChart2),
    '#bloque_inferior' => array($renderArrayTablePartidas)
  );

  return $finalRenderArray;
}

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