<?php

function rj_simulador_simulaciones_page()
{
  // Recuperamos el usuario original
  $usuario = user_uid_optional_load();
  
  // Lanzamos el GestorSimulaciones y cargamos las Simulaciones pasadas
  $idsSimulaciones = array(GestorSimulaciones::SIM_UNO, GestorSimulaciones::SIM_DOS, GestorSimulaciones::SIM_TRES,
                           GestorSimulaciones::SIM_CUATRO, GestorSimulaciones::SIM_CINCO);

  $gestorSimulaciones = new GestorSimulaciones($idsSimulaciones, $usuario);

  module_load_include('inc', 'rj_simulador', 'rj_simulador.forms');
  $renderUpperLeftForm = drupal_get_form('rj_simulador_simulaciones_upper_left_form', $gestorSimulaciones);

  $renderUpperRightForm = drupal_get_form('rj_simulador_simulaciones_upper_right_form', $gestorSimulaciones);

  // Creamos la tabla a sacar para mostrar las partidas
  // Headers array
  $headers = array(
    array('data' => t('Nº de simulación')),
    array('data' => t('Nombre Simulación')),
    array('data' => t('Enlaces'))
  );


  $rows = array();
  foreach ($gestorSimulaciones->getListaSimulacionesByUser($usuario) as $simulacion) {
    $rows[] = array(
      array('data' => $simulacion->getIdSimulacion()),
      array('data' => $simulacion->getNombreSimulacion()),
      array('data' => $simulacion->getURLToSimulacionPage('html_link'))
    );
  }

  $renderArrayTableSimulaciones = null;
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
          '#parameters' => array('filter' => isset($form_state['storage']['name']) ? $form_state['storage']['name'] : "")
        ),
      );
  }

  $renderArrayFinal = array(
    '#theme' => 'lista_simulaciones',
    '#upper_left' => $renderUpperLeftForm,
    '#upper_right' => $renderUpperRightForm,
    '#main_content' => array($renderUpperLeftForm, $renderArrayTableSimulaciones)
  );

  return $renderArrayFinal;
}

function rj_simulador_partidas_simulacion_page($id_simulacion)
{
  // Recuperamos el usuario actual
  $usuario = user_uid_optional_load();

  $simulacion = new Simulacion($id_simulacion, $usuario);

  $listadoPartidas = $simulacion->getListaPartidas();

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
    '#theme' => 'lista_simulaciones',
    '#upper_left' => null,
    '#upper_right' => null,
    '#main_content' => array($renderArrayTablePartidas)
  );

  return $renderArrayFinal;
}

function rj_simulador_partida_page($id_partida)
{
  $miPartida = Partida::loadById($id_partida);

  // Creamos la tabla a sacar para mostrar las partidas
  // Headers array
  $headers = array(
    array('data' => t('Instante')),
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
    array('data' => t('Instante')),
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
    '#options' => array('attributes' => array('title' => 'back',
      'style' => 'padding:8px;appearance:button;-webkit-appearance:button;text-decoration:none;'), 'html' => TRUE)
  );

  $a = array(
    '#type' => 'container',
    '#attributes' => array('id' => 'my-id', 'style' => array('text-align:center;', 'color:red;')),
    'enlace' => $link
  );

  $renderArrayFinal = array(
    '#type' => 'container',
    'content' => array($renderArrayTableDatos, $renderArrayTableInfracciones, $a)
  );

  return $renderArrayFinal;
}