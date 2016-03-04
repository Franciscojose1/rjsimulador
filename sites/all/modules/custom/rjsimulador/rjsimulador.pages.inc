<?php
module_load_include('inc', 'rjsimulador', 'rjsimulador.forms');

/* *************************************************************************** */
/* *                        PÁGINA DE SIMULACIONES                             */
/* *************************************************************************** */
function rjsimulador_simulaciones_page($uid = NULL, $adminMode = FALSE) {
  // Provider de datos
  $provider = FactoryDataManager::createDataProvider();
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
  foreach ($gestorSimulaciones->getUsuarioActual()
             ->getListaSimulaciones() as $simulacion) {
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
          '#empty' => t('There are no stored simulations in the server.'),
          '#attributes' => array('class' => array('rjsim-tabla-top'))
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
    '#main_content' => array($renderMainForm, $renderArrayTableSimulaciones),
    '#attached' => array(
      'css' => array(drupal_get_path('module', 'rjsimulador') . '/css/rjsimulador.css')
    )
  );

  return $renderArrayFinal;
}

/* *************************************************************************** */
/* *             PÁGINA DE PARTIDAS POR TIPO DE SIMULACIÓN                     */
/* *************************************************************************** */
function rjsimulador_partidas_simulacion_page($id_simulacion, $uid = NULL, $adminMode = FALSE) {
  // Provider de datos
  $provider = FactoryDataManager::createDataProvider();
  // Recuperamos el usuario
  $usuarioSimulacion = $provider->loadSimulatorUser($uid);

  try {
    $gestorSimulaciones = new GestorSimulaciones($usuarioSimulacion);
  } catch (LogicException $le) {
    $renderArray = array('#markup' => '<p>' . $le->getMessage() . '</p>');
    return $renderArray;
  }

  $listadoPartidas = $gestorSimulaciones->getUsuarioActual()
    ->retrieveAllPartidasByIdSimulacion($id_simulacion);

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
          '#empty' => t('You have no stored Partidas for this Simulation.'),
          '#attributes' => array('class' => array('rjsim-tabla-top'))
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
    '#main_content' => array($renderUpperMainForm, $renderArrayTablePartidas),
    '#attached' => array(
      'css' => array(drupal_get_path('module', 'rjsimulador') . '/css/rjsimulador.css')
    )
  );

  return $renderArrayFinal;
}

/* *************************************************************************** */
/* *                          PÁGINA DE PARTIDA                                */
/* *************************************************************************** */
function rjsimulador_partida_page($id_partida) {
  // Recuperamos variables locales
  $provider = FactoryDataManager::createDataProvider();
  $miPartida = Partida::loadById($id_partida);
  // Cargamos el usuario de la partida
  $usuario = $provider->loadSimulatorUser($miPartida->getUserUid());
  $listaPartidasUsuarioSimulacion = $usuario->retrieveAllPartidasByIdSimulacion($miPartida->getIdSimulacion());

  // Creamos la tabla con los datos de resumen de la partida
  $renderArrayTableDatosEstadisticos = generarTablaResumenDatosPartida($miPartida, $listaPartidasUsuarioSimulacion);

  // Creamos la tabla con los datos de RPMs por marcha
  $renderArrayTableRpmMarchas = generarTablaRpmMarchas($miPartida);

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
          '#empty' => t('There is no data for this Partida.'),
          '#attributes' => array('class' => array('rjsim-tabla'))
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
          '#empty' => t('There is no data for this Partida.'),
          '#attributes' => array('class' => array('rjsim-tabla'))
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
    '#upper_content' => array(
      $renderArrayTableDatosEstadisticos,
      $renderArrayTableRpmMarchas
    ),
    '#upper_left' => $renderArrayTableDatos,
    '#upper_right' => $renderArrayTableInfracciones,
    '#main_content' => $link,
    '#attached' => array(
      'js' => array(drupal_get_path('module', 'rjsimulador') . '/js/rjsimulador.js'),
      'css' => array(drupal_get_path('module', 'rjsimulador') . '/css/rjsimulador.css')
    )
  );

  return $renderArrayFinal;
}

function generarTablaResumenDatosPartida(Partida $partida, ListaPartidas $listaPartidasSimulacion) {
  // Recuperamos los datos de esta partida
  $consumoMedio = $partida->getConsumoMedio();
  $tiempoTotal = $partida->getTiempoTotal();
  $velocidadMedia = $partida->getVelocidadMedia();
  $desviacionVelocidad = $partida->getDesviacionTipicaVelocidad();
  $rpmMedia = $partida->getRpmMedia();
  $desviacionRpm = $partida->getDesviacionTipicaRpm();

  // Recuperamos datos medios de todas las partidas de la misma simulación que esta para comparar
  $mediaConsumoMedio = $listaPartidasSimulacion->calculateData(new CalculateAverageData(CalculateAverageData::CONSUMO_MEDIO));
  $mediaTiempoTotal = $listaPartidasSimulacion->calculateData(new CalculateAverageData(CalculateAverageData::TIEMPO_TOTAL));
  $mediaVelocidadMedia = $listaPartidasSimulacion->calculateData(new CalculateAverageData(CalculateAverageData::VELOCIDAD));
  $mediaDesviacionVelocidad = $listaPartidasSimulacion->calculateData(new CalculateAverageData(CalculateAverageData::DESVIACION_VELOCIDAD));
  $mediaRpmMedia = $listaPartidasSimulacion->calculateData(new CalculateAverageData(CalculateAverageData::RPM));
  $mediaDesviacionRpm = $listaPartidasSimulacion->calculateData(new CalculateAverageData(CalculateAverageData::DESVIACION_RPM));

  // Creamos la tabla con los datos de resumen de la partida
  $headers = array(
    array('data' => t('Average Consumption')),
    array('data' => t('Total Time')),
    array('data' => t('Average Speed')),
    array('data' => t('Speed Typical Deviation')),
    array('data' => t('Average RPM')),
    array('data' => t('RPM Typical Deviation'))
  );

  $rows =
    array(
      'data' =>
        array(
          array('data' => renderElementDataDifference($consumoMedio, $mediaConsumoMedio)),
          array('data' => renderElementDataDifference($tiempoTotal, $mediaTiempoTotal)),
          array('data' => renderElementDataDifference($velocidadMedia, $mediaVelocidadMedia)),
          array('data' => renderElementDataDifference($desviacionVelocidad, $mediaDesviacionVelocidad)),
          array('data' => renderElementDataDifference($rpmMedia, $mediaRpmMedia)),
          array('data' => renderElementDataDifference($desviacionRpm, $mediaDesviacionRpm)),
        ),
    );

  $renderArrayTableDatosEstadisticos =
    array(
      array(
        '#theme' => 'table',
        '#header' => $headers,
        '#rows' => $rows,
        '#empty' => t('There is no data for this Partida.'),
        '#caption' => '<h3>' . t('General data from Partida') . '</h3>',
        '#attributes' => array('class' => array('rjsim-tabla'))
      ),
    );

  return $renderArrayTableDatosEstadisticos;
}

function generarTablaRpmMarchas(Partida $partida) {
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
          array(
            'data' => round($partida->getListaDatos()
              ->filterBy(new FilterByEquality(array(1),
                FilterByEquality::DATO_MARCHA))
              ->calculateData(new CalculateAverageData(CalculateAverageData::RPM)), 2)
          ),
          array(
            'data' => round($partida->getListaDatos()
              ->filterBy(new FilterByEquality(array(2),
                FilterByEquality::DATO_MARCHA))
              ->calculateData(new CalculateAverageData(CalculateAverageData::RPM)), 2)
          ),
          array(
            'data' => round($partida->getListaDatos()
              ->filterBy(new FilterByEquality(array(3),
                FilterByEquality::DATO_MARCHA))
              ->calculateData(new CalculateAverageData(CalculateAverageData::RPM)), 2)
          ),
          array(
            'data' => round($partida->getListaDatos()
              ->filterBy(new FilterByEquality(array(4),
                FilterByEquality::DATO_MARCHA))
              ->calculateData(new CalculateAverageData(CalculateAverageData::RPM)), 2)
          ),
          array(
            'data' => round($partida->getListaDatos()
              ->filterBy(new FilterByEquality(array(5),
                FilterByEquality::DATO_MARCHA))
              ->calculateData(new CalculateAverageData(CalculateAverageData::RPM)), 2)
          ),
        ),
    );

  $renderArrayTableRpmMarchas =
    array(
      array(
        '#theme' => 'table',
        '#header' => $headers,
        '#rows' => $rows,
        '#empty' => t('There is no data for this Partida.'),
        '#caption' => '<h3>' . t('Average RPMs by gear') . '</h3>',
        '#attributes' => array('class' => array('rjsim-tabla'))
      ),
    );

  return $renderArrayTableRpmMarchas;
}

function renderElementDataDifference($actualData, $averageData) {
  $diferencia = round($actualData - $averageData, 2);

  $element = '<span>' . round($actualData, 2) .'</span>';
  if ($diferencia != 0) {
    $element .= '<span ' . drupal_attributes(array('class' => array(getElementDiffClass($actualData, $averageData)))) . '>';
    $element .= ($diferencia >= 0) ? ' + ' . abs($diferencia) :  ' - ' . abs($diferencia);
    $element .= '</span>';
  }

  return $element;
}

function getElementDiffClass($actualData, $averageData) {
  // Devolvemos una clase en función de los datos pasados:
  if ($actualData > $averageData) {
    // Nuestros media es peor que la general
    return "rjsim-red";
  }
  elseif ($actualData < $averageData) {
    // Nuestra media es mejor que la general
    return "rjsim-green";
  }
  else {
    // Nuestra media es la misma que la general
    return "rjsim-blue";
  }
}