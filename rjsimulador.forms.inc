<?php
/* *************************************************************************** */
/* *                           LISTA SIMULACIONES                              */
/* *************************************************************************** */
function rjsim_simulaciones_upper_left_form($form, &$form_state, GestorSimulaciones $gestorSimulaciones) {
  /* **************************************************************** */
  /* *        Gráfico nº de partidas jugadas por simulación         * */
  /* **************************************************************** */
  $arrayData = array();
  $xLabels = array();

  foreach ($gestorSimulaciones->getUsuarioActual()->getListaSimulaciones() as $simulacion) {
    $arrayData[] = $simulacion->getListaPartidas()->count();
    $xLabels[] = $simulacion->getNombreSimulacion();
  }

  $form['chart_partidas_simulacion'] = array(
    '#type' => 'chart',
    '#chart_type' => 'column',
    '#chart_library' => 'highcharts',
    '#legend' => FALSE,
    '#title' => t('No. of Partidas played by Simulation'),
    '#title_font_weight' => 'bold',
    'simulation' => array(
      '#type' => 'chart_data',
      '#title' => 'Number of played Partidas',
      '#data' => $arrayData,
    ),
    'xaxis' =>
      array(
        '#type' => 'chart_xaxis',
        '#labels' => $xLabels
      ),
    'yaxis' =>
      array(
        '#type' => 'chart_yaxis',
        '#title' => 'Number of Partidas',
      ),
  );

  /* ****************************************************************************** */
  /* *       Gráfico nº de partidas jugadas por simulación (últimos días)         * */
  /* ****************************************************************************** */
  $ultimosXDias = isset($form_state['values']['select_ultimos_dias']) ? $form_state['values']['select_ultimos_dias'] : 7;
  $fechaFin = new DateTime();
  $fechaInicio = new DateTime($fechaFin->format('Y-m-d'));
  $fechas = array('fecha_inicio' => $fechaInicio, 'fecha_fin' => $fechaFin);

  $arrayPartidasJugadasPorDia = array();
  foreach ($gestorSimulaciones->getArraySimulaciones() as $idSimulacion => $nombreSimulacion) {
    $arrayPartidasJugadasPorDia[$idSimulacion][] = $gestorSimulaciones->getUsuarioActual()
      ->retrieveAllPartidasByIdSimulacion($idSimulacion)
      ->filterBy(new FilterByFecha($fechas))
      ->count();
  }
  $xDayLabels[] = $fechaInicio->format('d-m-Y');

  for ($index = 0; $index < $ultimosXDias; $index++) {
    $fechaFin = new DateTime($fechaInicio->format('Y-m-d'));
    $fechaInicio->modify('-1 day');
    $fechas = array('fecha_inicio' => $fechaInicio, 'fecha_fin' => $fechaFin);
    foreach ($gestorSimulaciones->getArraySimulaciones() as $idSimulacion => $nombreSimulacion) {
      $arrayPartidasJugadasPorDia[$idSimulacion][] = $gestorSimulaciones->getUsuarioActual()
        ->retrieveAllPartidasByIdSimulacion($idSimulacion)
        ->filterBy(new FilterByFecha($fechas))
        ->count();
    }
    $xDayLabels[] = $fechaInicio->format('d-m-Y');
  }

  $xDayLabels = array_reverse($xDayLabels);

  $form['chart_partidas_ultimos_dias'] = array(
    '#type' => 'chart',
    '#chart_type' => 'line',
    '#chart_library' => 'highcharts',
    '#title' => t('Number of played Partidas by Simulation in the last ') . $ultimosXDias . t(' days'),
    '#title_font_weight' => 'bold',
    '#prefix' => '<div id="div-chart-partidas-ultimos-dias">',
    '#suffix' => '</div>',
    'xaxis' =>
      array(
        '#type' => 'chart_xaxis',
        '#title' => t('Date'),
        '#labels' => $xDayLabels,
      ),
    'yaxis' =>
      array(
        '#type' => 'chart_yaxis',
        '#title' => t('Nº Partidas'),
      ),
  );

  foreach ($arrayPartidasJugadasPorDia as $idSimulacion => $arrayPartidasPorSimulacion) {
    $arrayPartidasPorSimulacion = array_reverse($arrayPartidasPorSimulacion);
    $form['chart_partidas_ultimos_dias'][$idSimulacion] = array(
      '#type' => 'chart_data',
      '#title' => t('Simulation ') . $idSimulacion,
      '#data' => $arrayPartidasPorSimulacion,
    );
  }

  $form['select_ultimos_dias'] = array(
    '#type' => 'select',
    '#title' => t('Días'),
    '#options' => array(
      7 => t('Last 7 days'),
      15 => t('Last 15 days'),
    ),
    '#default_value' => 7,
    '#description' => t('Select number of days to show in graph.'),
    '#ajax' => array(
      'callback' => 'ajax_partidas_ultimos_dias_callback',
      'wrapper' => 'div-chart-partidas-ultimos-dias',
    ),
  );

  return $form;
}

function ajax_partidas_ultimos_dias_callback($form, &$form_state) {
  return $form['chart_partidas_ultimos_dias'];
}

function rjsim_simulaciones_upper_right_form($form, &$form_state, GestorSimulaciones $gestorSimulaciones) {
  /* **************************************************************************** */
  /* *         Gráfico nº medio de infracciones por partida y simulación        * */
  /* *        Gráfico tiempo medio total por partida y tipo de simulación       * */
  /* **************************************************************************** */
  $labelXAxis = array();

  // Creamos array de datos
  $dataUsuarioInfracciones = array();
  $dataTotalInfracciones = array();

  $dataUsuarioTiempo = array();
  $dataTotalTiempo = array();

  foreach ($gestorSimulaciones->getArraySimulaciones() as $idSimulacion => $nombreSimulacion) {
    // Datos del eje X. Nombres de simulaciones.
    $labelXAxis[] = $nombreSimulacion;

    $listaPartidasUsuarioPorSimulacion = $gestorSimulaciones->getUsuarioActual()->retrieveAllPartidasByIdSimulacion($idSimulacion);
    $dataUsuarioTiempo[] = $listaPartidasUsuarioPorSimulacion->calculateData(new CalculateAverageData(CalculateAverageData::TIEMPO_TOTAL));

    // Datos de las infracciones del usuario actual.
    $listaTodasInfraccionesUsuarioPorSimulacion = new ListaInfracciones();
    foreach ($listaPartidasUsuarioPorSimulacion as $partida) {
      $listaTodasInfraccionesUsuarioPorSimulacion->mergeList($partida->getListaInfracciones());
    }

    if ($listaPartidasUsuarioPorSimulacion->count() > 0) {
      $dataUsuarioInfracciones[] = $listaTodasInfraccionesUsuarioPorSimulacion->count() / $listaPartidasUsuarioPorSimulacion->count();
    }
    else {
      $dataUsuarioInfracciones[] = 0;
    }

    $dataRetriever = new ListaUsuariosDataRetriever($gestorSimulaciones->getListaTodosUsuarios());
    // Datos de las infracciones de todos los usuarios.
    $listaPartidasPorSimulacion = $dataRetriever->retrieveAllPartidasByIdSimulacion($idSimulacion);
    $dataTotalTiempo[] = $listaPartidasPorSimulacion->calculateData(new CalculateAverageData(CalculateAverageData::TIEMPO_TOTAL));

    $listaTodasInfraccionesPorSimulacion = new ListaInfracciones();
    foreach ($listaPartidasPorSimulacion as $partida) {
      $listaTodasInfraccionesPorSimulacion->mergeList($partida->getListaInfracciones());
    }

    if ($listaPartidasPorSimulacion->count() > 0) {
      $dataTotalInfracciones[] = $listaTodasInfraccionesPorSimulacion->count() / $listaPartidasPorSimulacion->count();
    }
    else {
      $dataTotalInfracciones[] = 0;
    }
  }

  $form['chart_infracciones_simulacion'] = array(
    '#type' => 'chart',
    '#chart_type' => 'column',
    '#chart_library' => 'highcharts',
    '#title' => t('Average Infractions by Partida and Simulation type'),
    '#title_font_weight' => 'bold',
    'infracciones_usuario' => array(
      '#type' => 'chart_data',
      '#title' => t('User ') . $gestorSimulaciones->getUsuarioActual()
          ->getName(),
      '#data' => $dataUsuarioInfracciones,
    ),
    'infracciones_totales' => array(
      '#type' => 'chart_data',
      '#title' => t('All users'),
      '#data' => $dataTotalInfracciones,
    ),
    'xaxis' =>
      array(
        '#type' => 'chart_xaxis',
        '#labels' => $labelXAxis
      ),
    'yaxis' =>
      array(
        '#type' => 'chart_yaxis',
        '#title' => 'Average number of infractions by Partida'
      )
  );


  $form['chart_tiempo_por_partida_simulacion'] = array(
    '#type' => 'chart',
    '#chart_type' => 'column',
    '#chart_library' => 'highcharts',
    '#title' => t('Average times by Partida and Simulation type'),
    '#title_font_weight' => 'bold',
    'tiempo_usuario' => array(
      '#type' => 'chart_data',
      '#title' => t('User ') . $gestorSimulaciones->getUsuarioActual()
          ->getName(),
      '#data' => $dataUsuarioTiempo,
    ),
    'tiempos_totales' => array(
      '#type' => 'chart_data',
      '#title' => t('All users'),
      '#data' => $dataTotalTiempo,
    ),
    'xaxis' =>
      array(
        '#type' => 'chart_xaxis',
        '#labels' => $labelXAxis
      ),
    'yaxis' =>
      array(
        '#type' => 'chart_yaxis',
        '#title' => 'Avergae time by Partida (s.)'
      )
  );

  return $form;
}

function rjsim_simulaciones_main_form($form, &$form_state, GestorSimulaciones $gestorSimulaciones) {
  /* **************************************************************** */
  /* *        Gráfico porcentaje de infracciones totales            * */
  /* **************************************************************** */
  $provider = FactoryDataProvider::createDataProvider();
  $idsInfracciones = $provider->loadAllIdsInfracciones();

  $listaPartidas = $gestorSimulaciones->getUsuarioActual()->retrieveAllPartidas();

  $listaResultados = new ListaInfracciones();
  foreach ($listaPartidas as $partida) {
    $listaResultados->mergeList($partida->getListaInfracciones());
  }

  $labels = array();
  $data = array();
  foreach ($idsInfracciones as $id => $nombre_infraccion) {
    $labels[] = check_plain($nombre_infraccion);
    $data[] = $listaResultados->filterBy(new FilterByEquality(array($id), FilterByEquality::INFRACCION_ID))->count();
  }

  $form['chart_infracciones'] = array(
    '#type' => 'chart',
    '#title' => t('Percentage of total committed infractions'),
    '#title_font_weight' => 'bold',
    '#chart_type' => 'pie',
    '#chart_library' => 'highcharts',
    '#legend_position' => 'right',
    '#data_labels' => TRUE,
    '#tooltips' => TRUE,
    'pie_data' =>
      array(
        '#type' => 'chart_data',
        '#title' => t('Infractions'),
        '#labels' => $labels,
        '#data' => $data,
      )
  );

  return $form;
}

/* *************************************************************************** */
/* *                    PARTIDAS POR TIPO DE SIMULACIÓN                        */
/* *************************************************************************** */
function rjsim_partidas_simulacion_upper_left_form($form, &$form_state, GestorSimulaciones $gestorSimulaciones, $id_simulacion) {
  /* ************************************************************************************ */
  /* *        Gráfico comparación consumo medio última partida con consumo medio        * */
  /* *        de todas las partidas anteriores de todos los usuarios                    * */
  /* ************************************************************************************ */
  // Lista de partidas del usuario actual para la simulacion
  $listaPartidasUsuario = $gestorSimulaciones->getUsuarioActual()->retrieveAllPartidasByIdSimulacion($id_simulacion);
  $listaPartidasUsuario->sortBy('Fecha', 'ASC');

  // Lista de partidas de todos los usuarios para la simulación
  $dataRetriever = new ListaUsuariosDataRetriever($gestorSimulaciones->getListaTodosUsuarios());
  $listaPartidasTodos = $dataRetriever->retrieveAllPartidasByIdSimulacion($id_simulacion);

  $labelAxisX = array();
  $dataUsuario = array();
  $dataTodos = array();

  $ultimasPartidas = ($listaPartidasUsuario->count() - 10 > 0) ? $listaPartidasUsuario->count() - 10 : 0;
  foreach (new LimitIterator($listaPartidasUsuario, $ultimasPartidas) as $partida) {
    $labelAxisX[] = 'Partida ' . $partida->getFechaAsObject()->format('d-m-Y');

    $dataUsuario[] = $partida->getConsumoMedio();

    $listaResultado = $listaPartidasTodos->filterBy(new FilterByFecha(array('fecha_fin' => $partida->getFechaAsObject())));

    $dataTodos[] = $listaResultado->calculateData(new CalculateAverageData(CalculateAverageData::CONSUMO_MEDIO));
  }

  $form['chart_consumo_medio'] = array(
    '#type' => 'chart',
    '#chart_type' => 'line',
    '#chart_library' => 'highcharts',
    '#title' => t('User average consumption vs Simulation average consumption'),
    '#title_font_weight' => 'bold',
    'usuario' =>
      array(
        '#type' => 'chart_data',
        '#title' => t('User'),
        '#data' => $dataUsuario,
      ),
    'todos' =>
      array(
        '#type' => 'chart_data',
        '#title' => t('All users'),
        '#data' => $dataTodos,
      ),
    'xaxis' =>
      array(
        '#type' => 'chart_xaxis',
        '#labels' => $labelAxisX,
      ),
  );

  return $form;
}

function rjsim_partidas_simulacion_upper_right_form($form, &$form_state, GestorSimulaciones $gestorSimulaciones, $id_simulacion) {
  /* ************************************************************************************ */
  /* *     Gráfico comparación tiempo total última partida con tiempo total medio       * */
  /* *     de todas las partidas anteriores de todos los usuarios                       * */
  /* ************************************************************************************ */
  $listaPartidasUsuario = $gestorSimulaciones->getUsuarioActual()
    ->retrieveAllPartidasByIdSimulacion($id_simulacion);
  $listaPartidasUsuario->sortBy('Fecha', 'ASC');

  $dataRetriever = new ListaUsuariosDataRetriever($gestorSimulaciones->getListaTodosUsuarios());
  $listaPartidasTodos = $dataRetriever->retrieveAllPartidasByIdSimulacion($id_simulacion);

  $labelAxisX = array();
  $dataUsuario = array();
  $dataTodos = array();

  $ultimasPartidas = ($listaPartidasUsuario->count() - 10 > 0) ? $listaPartidasUsuario->count() - 10 : 0;
  foreach (new LimitIterator($listaPartidasUsuario, $ultimasPartidas) as $partida) {
    $labelAxisX[] = 'Partida ' . $partida->getFechaAsObject()->format('d-m-Y');

    $dataUsuario[] = $partida->getTiempoTotal();

    $listaResultado = $listaPartidasTodos->filterBy(new FilterByFecha(array('fecha_fin' => $partida->getFechaAsObject())));

    $dataTodos[] = $listaResultado->calculateData(new CalculateAverageData(CalculateAverageData::TIEMPO_TOTAL));
  }

  $form['chart_tiempo_medio'] = array(
    '#type' => 'chart',
    '#chart_type' => 'line',
    '#chart_library' => 'highcharts',
    '#title' => t('User average total time vs Simulation average total time'),
    '#title_font_weight' => 'bold',
    'usuario' =>
      array(
        '#type' => 'chart_data',
        '#title' => t('User'),
        '#data' => $dataUsuario,
      ),
    'todos' =>
      array(
        '#type' => 'chart_data',
        '#title' => t('All users'),
        '#data' => $dataTodos,
      ),
    'xaxis' =>
      array(
        '#type' => 'chart_xaxis',
        '#labels' => $labelAxisX,
      ),
  );

  return $form;
}

function rjsim_partidas_simulacion_main_form($form, &$form_state, GestorSimulaciones $gestorSimulaciones, $id_simulacion) {
  /* ************************************************************************************ */
  /* *               Gráfico infracciones en las últimas partidas                       * */
  /* ************************************************************************************ */
  // Provider de datos
  $provider = FactoryDataProvider::createDataProvider();
  $idsInfracciones = $provider->loadAllIdsInfracciones();

  $listaPartidas = $gestorSimulaciones->getUsuarioActual()->retrieveAllPartidasByIdSimulacion($id_simulacion);
  $listaPartidas->sortBy('Fecha', 'DESC');

  $labelAxisX = array();
  $data = array();
  foreach (new LimitIterator($listaPartidas, 0, 10) as $partida) {
    foreach ($idsInfracciones as $id => $nombre_infraccion) {
      $data[$id][] = $partida->getListaInfracciones()
        ->filterBy(new FilterByEquality(array($id), FilterByEquality::INFRACCION_ID))
        ->count();
    }

    $labelAxisX[] = 'Partida ' . $partida->getFechaAsObject()
        ->format('d-m-Y H:i:s');
  }

  $labelAxisX = array_reverse($labelAxisX);

  $form['chart_infracciones_simulacion'] = array(
    '#type' => 'chart',
    '#chart_type' => 'column',
    '#stacking' => TRUE,
    '#chart_library' => 'highcharts',
    '#title' => t('Number of infractions in last Partidas'),
    '#title_font_weight' => 'bold',
    'xaxis' =>
      array(
        '#type' => 'chart_xaxis',
        '#labels' => $labelAxisX
      ),
    'yaxis' =>
      array(
        '#type' => 'chart_yaxis',
        '#title' => 'Number of Infractions'
      )
  );

  foreach ($idsInfracciones as $id => $nombre_infraccion) {
    $data[$id] = array_reverse($data[$id]);
    $form['chart_infracciones_simulacion'][$id] =
      array(
        '#type' => 'chart_data',
        '#title' => t($nombre_infraccion),
        '#data' => $data[$id],
      );
  }

  return $form;
}