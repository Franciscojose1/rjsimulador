<?php
module_load_include('inc', 'rj_simulador', 'rj_simulador.forms');

/* *************************************************************************** */
/* *                        PÁGINA DE SIMULACIONES                             */
/* *************************************************************************** */
function rj_simulador_simulaciones_page($uid = NULL, $adminMode = FALSE) {
  // Recuperamos el usuario original
  if (isset($uid)) {
    $user = user_uid_optional_load($uid);
  }
  else {
    $user = user_uid_optional_load();
  }

  $usuarioSimulacion = new UsuarioSimulacion($user);

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
    array('data' => t('Nº de simulación')),
    array('data' => t('Nombre Simulación')),
    array('data' => t('Enlaces'))
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
          '#empty' => t('No existen simulaciones almacenadas en el servidor.')
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
function rj_simulador_partidas_simulacion_page($id_simulacion, $uid = NULL, $adminMode = FALSE) {
  // Recuperamos el usuario original
  if (isset($uid)) {
    $user = user_uid_optional_load($uid);
  }
  else {
    $user = user_uid_optional_load();
  }

  $usuarioSimulacion = new UsuarioSimulacion($user);

  try {
    $gestorSimulaciones = new GestorSimulaciones($usuarioSimulacion);
  } catch (LogicException $le) {
    $renderArray = array('#markup' => '<p>' . $le->getMessage() . '</p>');
    return $renderArray;
  }

  $listadoPartidas = $gestorSimulaciones->getUsuarioActual()->retrieveAllPartidasByIdSimulacion($id_simulacion);

  if ($listadoPartidas->count() == 0) {
    $renderArray = array('#markup' => '<span>No dispones de partidas almacenadas para esta simulación.</span>');
    return $renderArray;
  }

  $renderUpperLeftForm = drupal_get_form('rjsim_partidas_simulacion_upper_left_form', $gestorSimulaciones, $id_simulacion);

  $renderUpperRightForm = drupal_get_form('rjsim_partidas_simulacion_upper_right_form', $gestorSimulaciones, $id_simulacion);

  $renderUpperMainForm = drupal_get_form('rjsim_partidas_simulacion_main_form', $gestorSimulaciones, $id_simulacion);


  // Creamos la tabla a sacar para mostrar las partidas
  // Headers array
  $headers = array(
    array('data' => t('Fecha'), 'field' => 'Fecha', 'sort' => 'desc'),
    array('data' => t('Nombre Simulación')),
    array('data' => t('Consumo Medio')),
    array('data' => t('Consumo Total')),
    array('data' => t('Tiempo Total')),
    array('data' => t('Enlaces'))
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
          '#empty' => t('No tienes partidas almacenadas en el servidor.')
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
function rj_simulador_partida_page($id_partida) {
  drupal_add_js(drupal_get_path('module', 'rj_simulador') . '/js/rj_simulador.js');
  $miPartida = Partida::loadById($id_partida);

  // Creamos la tabla a sacar para mostrar las partidas
  // Headers array
  $headers = array(
    array('data' => t('Dato Nº')),
    array('data' => t('Instante')),
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
    $limit = 10;
    $page = pager_default_initialize(count($rows), $limit, 0);
    $offset = $limit * $page;
    $renderArrayTableDatos =
      array(
        array(
          '#theme' => 'table',
          '#header' => $headers,
          '#rows' => array_slice($rows, $offset, $limit),
          '#empty' => t('No hay datos para esta partida.')
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
    array('data' => t('Infracción Nº')),
    array('data' => t('Instante')),
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
    $limit = 10;
    $page = pager_default_initialize(count($rows), $limit, 1);
    $offset = $limit * $page;
    $renderArrayTableInfracciones =
      array(
        array(
          '#theme' => 'table',
          '#header' => $headers,
          '#rows' => array_slice($rows, $offset, $limit),
          '#empty' => t('No hay infracciones para esta partida.')
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
    '#upper_left' => $renderArrayTableDatos,
    '#upper_right' => $renderArrayTableInfracciones,
    '#main_content' => $link,
  );

  return $renderArrayFinal;
}