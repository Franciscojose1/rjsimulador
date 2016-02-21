<?php
module_load_include('inc', 'rjsimulador', 'rjsimulador.forms');

/* *************************************************************************** */
/* *                        PÁGINA DE SIMULACIONES                             */
/* *************************************************************************** */
function rjsimulador_simulaciones_page($uid = NULL, $adminMode = FALSE) {
  // Provider de datos
  $provider = FactoryDataProvider::createDataProvider();
  // Recuperamos el usuario
  $usuarioSimulacion = $provider->loadSimulatorUser($uid);

  try {
    $gestorSimulaciones = new GestorSimulaciones($usuarioSimulacion);
  } catch (LogicException $le) {
    $renderArray = array('#markup' => '<p>' . $le->getMessage() . '</p>');
    return $renderArray;
  }

  $renderUpperLeftForm = drupal_get_form('rjsim_simulaciones_upper_left_form', $gestorSimulaciones);

  $renderUpperRightForm = drupal_get_form('rjsim_simulaciones_upper_right_form', $gestorSimulaciones);

  $renderMainForm = drupal_get_form('rjsim_simulaciones_main_form', $gestorSimulaciones);

  // Creamos la tabla a sacar para mostrar las partidas
  // Headers array
  $headers = array(
    array('data' => t('No. of Simulation')),
    array('data' => t('Simulation Name')),
    array('data' => t('Links'))
  );

  $rows = array();
  foreach ($gestorSimulaciones->getUsuarioActual()->getListaSimulaciones() as $simulacion) {
    $rows[] = array(
      array('data' => $simulacion->getIdSimulacion()),
      array('data' => $simulacion->getNombreSimulacion()),
      array('data' => $simulacion->getURLToSimulacionPage($adminMode, 'html_link'))
    );
  }

  $renderArrayTableSimulaciones = NULL;
  if (!empty($rows)) {
    $limit = 5;
    $page = pager_default_initialize(count($rows), $limit, 0);
    $offset = $limit * $page;
    $renderArrayTableSimulaciones =
      array(
        array(
          '#theme' => 'table',
          '#header' => $headers,
          '#rows' => array_slice($rows, $offset, $limit),
          '#empty' => t('There are no stored simulations in the server.')
        ),
        array(
          '#theme' => 'pager',
          '#element' => 0,
        ),
      );
  }

  $renderArrayFinal = array(
    '#theme' => 'lista_simulaciones_partidas',
    '#upper_left' => $renderUpperLeftForm,
    '#upper_right' => $renderUpperRightForm,
    '#main_content' => array($renderMainForm, $renderArrayTableSimulaciones)
  );

  return $renderArrayFinal;
}

/* *************************************************************************** */
/* *             PÁGINA DE PARTIDAS POR TIPO DE SIMULACIÓN                     */
/* *************************************************************************** */
function rjsimulador_partidas_simulacion_page($id_simulacion, $uid = NULL, $adminMode = FALSE) {
  // Provider de datos
  $provider = FactoryDataProvider::createDataProvider();
  // Recuperamos el usuario
  $usuarioSimulacion = $provider->loadSimulatorUser($uid);

  try {
    $gestorSimulaciones = new GestorSimulaciones($usuarioSimulacion);
  } catch (LogicException $le) {
    $renderArray = array('#markup' => '<p>' . $le->getMessage() . '</p>');
    return $renderArray;
  }

  $listadoPartidas = $gestorSimulaciones->getUsuarioActual()->retrieveAllPartidasByIdSimulacion($id_simulacion);

  if ($listadoPartidas->count() == 0) {
    $renderArray = array('#markup' => '<span>' . t('You have no stored Partidas for this Simulation.') . '</span>');
    return $renderArray;
  }

  $renderUpperLeftForm = drupal_get_form('rjsim_partidas_simulacion_upper_left_form', $gestorSimulaciones, $id_simulacion);

  $renderUpperRightForm = drupal_get_form('rjsim_partidas_simulacion_upper_right_form', $gestorSimulaciones, $id_simulacion);

  $renderUpperMainForm = drupal_get_form('rjsim_partidas_simulacion_main_form', $gestorSimulaciones, $id_simulacion);


  // Creamos la tabla a sacar para mostrar las partidas
  // Headers array
  $headers = array(
    array('data' => t('Date'), 'field' => 'Fecha', 'sort' => 'desc'),
    array('data' => t('Simulation Name')),
    array('data' => t('Average Consumption')),
    array('data' => t('Total Consumption')),
    array('data' => t('Total Time')),
    array('data' => t('Links'))
  );

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
      array('data' => $partida->getURLToPartidaPage($adminMode, 'html_link'))
    );
  }

  $renderArrayTablePartidas = NULL;
  if (!empty($rows)) {
    $limit = 5;
    $page = pager_default_initialize(count($rows), $limit, 0);
    $offset = $limit * $page;
    $renderArrayTablePartidas =
      array(
        array(
          '#theme' => 'table',
          '#header' => $headers,
          '#rows' => array_slice($rows, $offset, $limit),
          '#empty' => t('You have no stored Partidas for this Simulation.')
        ),
        array(
          '#theme' => 'pager',
          '#element' => 0,
          '#parameters' => array('filter' => isset($form_state['storage']['name']) ? $form_state['storage']['name'] : "")
        ),
      );
  }

  $renderArrayFinal = array(
    '#theme' => 'lista_simulaciones_partidas',
    '#upper_left' => $renderUpperLeftForm,
    '#upper_right' => $renderUpperRightForm,
    '#main_content' => array($renderUpperMainForm, $renderArrayTablePartidas)
  );

  return $renderArrayFinal;
}

/* *************************************************************************** */
/* *                          PÁGINA DE PARTIDA                                */
/* *************************************************************************** */
function rjsimulador_partida_page($id_partida) {
  drupal_add_js(drupal_get_path('module', 'rjsimulador') . '/js/rjsimulador.js');
  $miPartida = Partida::loadById($id_partida);

  // Creamos la tabla con los datos de resumen de la partida
  $headers = array(
    array('data' => t('Average Consumption')),
    array('data' => t('Total Time')),
    array('data' => t('Average Speed')),
    array('data' => t('Speed Typical Deviation')),
    array('data' => t('Average RPM')),
    array('data' => t('RPM Typical Deviation'))
  );

  $rows = array();
  $rows[] =
    array(
      'data' =>
        array(
          array('data' => $miPartida->getConsumoMedio()),
          array('data' => $miPartida->getTiempoTotal()),
          array('data' => round($miPartida->getVelocidadMedia(),2)),
          array('data' => round($miPartida->getDesviacionTipicaVelocidad(),2)),
          array('data' => round($miPartida->getRpmMedia(),2)),
          array('data' => round($miPartida->getDesviacionTipicaRpm(),2)),
        ),
    );

  $renderArrayTableDatosEstadisticos =
    array(
      array(
        '#theme' => 'table',
        '#header' => $headers,
        '#rows' => $rows,
        '#empty' => t('There is no data for this Partida.'),
        '#caption' => '<h3>' . t('General data from Partida') . '</h3>'
      ),
    );

  // Creamos la tabla con los datos de RPMs por marcha
  $headers = array(
    array('data' => t('Gear 1')),
    array('data' => t('Gear 2')),
    array('data' => t('Gear 3')),
    array('data' => t('Gear 4')),
    array('data' => t('Gear 5')),
  );

  $rows = array();
  $rows[] =
    array(
      'data' =>
        array(
          array('data' => round($miPartida->getListaDatos()->filterBy(new FilterByMarcha(array(1)))->calculateData(new CalculateAverageData(CalculateAverageData::RPM)),2)),
          array('data' => round($miPartida->getListaDatos()->filterBy(new FilterByMarcha(array(2)))->calculateData(new CalculateAverageData(CalculateAverageData::RPM)),2)),
          array('data' => round($miPartida->getListaDatos()->filterBy(new FilterByMarcha(array(3)))->calculateData(new CalculateAverageData(CalculateAverageData::RPM)),2)),
          array('data' => round($miPartida->getListaDatos()->filterBy(new FilterByMarcha(array(4)))->calculateData(new CalculateAverageData(CalculateAverageData::RPM)),2)),
          array('data' => round($miPartida->getListaDatos()->filterBy(new FilterByMarcha(array(5)))->calculateData(new CalculateAverageData(CalculateAverageData::RPM)),2)),
        ),
    );

  $renderArrayTableRpmMarchas =
    array(
      array(
        '#theme' => 'table',
        '#header' => $headers,
        '#rows' => $rows,
        '#empty' => t('There is no data for this Partida.'),
        '#caption' => '<h3>' . t('Average RPMs by gear') . '</h3>'
      ),
    );

  // Creamos la tabla a sacar para mostrar los datos de la Partida
  // Headers array
  $headers = array(
    array('data' => t('Data Number')),
    array('data' => t('Instant')),
  );

  $rows = array();
  $contador = 1;
  foreach ($miPartida->getListaDatos() as $dato) {
    $rows[] =
      array(
        'data' =>
          array(
            array('data' => $contador),
            array('data' => $dato->getInstante()),
          ),
        'class' => array('fila-datos'),
      );
    $contador++;
  }

  $renderArrayTableDatos = NULL;
  if (!empty($rows)) {
    $renderArrayTableDatos =
      array(
        array(
          '#theme' => 'table',
          '#header' => $headers,
          '#rows' => $rows,
          '#empty' => t('There is no data for this Partida.')
        ),
      );
  }

  // Creamos la tabla a sacar para mostrar las infracciones de la partida
  // Headers array
  $headers = array(
    array('data' => t('Infraction Number')),
    array('data' => t('Instant')),
  );

  $rows = array();
  $contador = 1;
  foreach ($miPartida->getListaInfracciones() as $elemento_lista) {
    $rows[] =
      array(
        'data' =>
          array(
            array('data' => $contador),
            array('data' => $elemento_lista->getInstante()),
          ),
        'class' => array('fila-infraccion'),
      );
    $contador++;
  }

  $renderArrayTableInfracciones = NULL;
  if (!empty($rows)) {
    $renderArrayTableInfracciones =
      array(
        array(
          '#theme' => 'table',
          '#header' => $headers,
          '#rows' => $rows,
          '#empty' => t('There is no data for this Partida.')
        ),
      );
  }

  $link = array(
    '#type' => 'link',
    '#title' => t('Go Back'),
    '#href' => 'simulaciones/' . $miPartida->getIdSimulacion() . '/partidas',
    '#options' => array(
      'attributes' => array(
        'title' => 'back',
        'style' => 'padding:8px;appearance:button;-webkit-appearance:button;text-decoration:none;'
      ),
      'html' => TRUE
    )
  );

  $renderArrayFinal = array(
    '#theme' => 'partida',
    '#upper_content' => array($renderArrayTableDatosEstadisticos, $renderArrayTableRpmMarchas),
    '#upper_left' => $renderArrayTableDatos,
    '#upper_right' => $renderArrayTableInfracciones,
    '#main_content' => $link,
  );

  return $renderArrayFinal;
}