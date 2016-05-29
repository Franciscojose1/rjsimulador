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

  foreach ($gestorSimulaciones->getUsuarioActual()
             ->getListaSimulaciones() as $simulacion) {
    $arrayData[] = $simulacion->getListaPartidas()->count();
    $xLabels[] = $simulacion->getNombreSimulacion();
  }

  $form['rjsim_fieldset_partidas_simulacion'] = array(
    '#type' => 'fieldset',
    '#title' => t('Total number of games played'),
    '#collapsible' => FALSE,
    '#description' => t('Total number of games played for each simulation.'),
    '#attributes' => array('class' => array('rjsim-margin'))
  );

  $form['rjsim_fieldset_partidas_simulacion']['chart_partidas_simulacion'] = array(
    '#type' => 'chart',
    '#chart_type' => 'column',
    '#chart_library' => 'highcharts',
    '#legend' => FALSE,
    '#title' => t('No. of games played for each simulation'),
    '#title_font_weight' => 'bold',
    'simulation' => array(
      '#type' => 'chart_data',
      '#title' => t('Number of played games'),
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
        '#title' => 'Number of games',
      ),
  );

  /* ****************************************************************************** */
  /* *       Gráfico nº de partidas jugadas por simulación (últimos días)         * */
  /* ****************************************************************************** */
  $ultimosXDias = isset($form_state['values']['select_ultimos_dias']) ? $form_state['values']['select_ultimos_dias'] : 7;
  $fechaFin = new DateTime();
  $fechaInicio = new DateTime($fechaFin->format('Y-m-d'));
  $fechas = array(
    FilterByFecha::FECHA_INICIO => $fechaInicio,
    FilterByFecha::FECHA_FIN => $fechaFin
  );

  // Recuperamos tipos de simulaciones
  $tiposSimulacion = Constants::getTiposSimulacion();
  $arrayPartidasJugadasPorDia = array();
  foreach ($tiposSimulacion as $idSimulacion => $nombreSimulacion) {
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
    foreach ($tiposSimulacion as $idSimulacion => $nombreSimulacion) {
      $arrayPartidasJugadasPorDia[$idSimulacion][] = $gestorSimulaciones->getUsuarioActual()
        ->retrieveAllPartidasByIdSimulacion($idSimulacion)
        ->filterBy(new FilterByFecha($fechas))
        ->count();
    }
    $xDayLabels[] = $fechaInicio->format('d-m-Y');
  }

  $xDayLabels = array_reverse($xDayLabels);

  $form['rjsim_fieldset_partidas_ultimos_dias'] = array(
    '#type' => 'fieldset',
    '#title' => t('Played games by simulation in the last days'),
    '#collapsible' => FALSE,
    '#description' => t('Graph with number of last played games by simulation.'),
    '#attributes' => array('class' => array('rjsim-margin'))
  );

  $form['rjsim_fieldset_partidas_ultimos_dias']['chart_partidas_ultimos_dias'] = array(
    '#type' => 'chart',
    '#chart_type' => 'line',
    '#chart_library' => 'highcharts',
    '#title' => t('Number of played games by simulation in the last @numberDays days', array('@numberDays' => $ultimosXDias)),
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
        '#title' => t('No. of games'),
      ),
  );

  foreach ($arrayPartidasJugadasPorDia as $idSimulacion => $arrayPartidasPorSimulacion) {
    $arrayPartidasPorSimulacion = array_reverse($arrayPartidasPorSimulacion);
    $form['rjsim_fieldset_partidas_ultimos_dias']['chart_partidas_ultimos_dias'][$idSimulacion] = array(
      '#type' => 'chart_data',
      '#title' => t('Simulation @idSimulation', array('@idSimulacion' => $idSimulacion)),
      '#data' => $arrayPartidasPorSimulacion,
    );
  }

  $form['rjsim_fieldset_partidas_ultimos_dias']['select_ultimos_dias'] = array(
    '#type' => 'select',
    '#title' => t('Days'),
    '#options' => array(
      7 => t('Last 7 days'),
      15 => t('Last 15 days'),
      30 => t('Last 30 days')
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
  return $form['rjsim_fieldset_partidas_ultimos_dias']['chart_partidas_ultimos_dias'];
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

  // Recuperar tipos de simulacion
  $tiposSimulacion = Constants::getTiposSimulacion();
  foreach ($tiposSimulacion as $idSimulacion => $nombreSimulacion) {
    // Datos del eje X. Nombres de simulaciones.
    $labelXAxis[] = $nombreSimulacion;

    $listaPartidasUsuarioPorSimulacion = $gestorSimulaciones->getUsuarioActual()
      ->retrieveAllPartidasByIdSimulacion($idSimulacion);
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

  $form['rjsim_fieldset_infracciones_simulacion'] = array(
    '#type' => 'fieldset',
    '#title' => t('Average number infractions by simulation'),
    '#collapsible' => FALSE,
    '#description' => t('Graph with average number of infractions committed by simulation.'),
    '#attributes' => array('class' => array('rjsim-margin'))
  );

  $form['rjsim_fieldset_infracciones_simulacion']['chart_infracciones_simulacion'] = array(
    '#type' => 'chart',
    '#chart_type' => 'column',
    '#chart_library' => 'highcharts',
    '#title' => t('Average infractions by game and simulation type'),
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
        '#title' => 'Average number of infractions by played game'
      )
  );

  $form['rjsim_fieldset_tiempo_partida'] = array(
    '#type' => 'fieldset',
    '#title' => t('Average number infractions by simulation'),
    '#collapsible' => FALSE,
    '#description' => t('Graph with average number of infractions committed by simulation.'),
    '#attributes' => array('class' => array('rjsim-margin'))
  );

  $form['rjsim_fieldset_tiempo_partida']['chart_tiempo_por_partida_simulacion'] = array(
    '#type' => 'chart',
    '#chart_type' => 'column',
    '#chart_library' => 'highcharts',
    '#title' => t('Average times by Partida and Simulation type'),
    '#title_font_weight' => 'bold',
    'tiempo_usuario' => array(
      '#type' => 'chart_data',
      '#title' => t('User @userName (UID:@userUID)', array(
        '@userName' => $gestorSimulaciones->getUsuarioActual()->getName(),
        '@userUID' => $gestorSimulaciones->getUsuarioActual()->getUid()
      )),
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
        '#title' => 'Average time by played game (s.)'
      )
  );

  return $form;
}

function rjsim_simulaciones_main_form($form, &$form_state, GestorSimulaciones $gestorSimulaciones) {
  /* **************************************************************** */
  /* *        Gráfico porcentaje de infracciones totales            * */
  /* **************************************************************** */
  $listaPartidas = $gestorSimulaciones->getUsuarioActual()
    ->retrieveAllPartidas();

  // Recuperamos los tipos de infracciones
  $tiposInfracciones = Constants::getTiposInfracciones();

  $listaResultados = new ListaInfracciones();
  foreach ($listaPartidas as $partida) {
    $listaResultados->mergeList($partida->getListaInfracciones());
  }

  $labels = array();
  $data = array();
  foreach ($tiposInfracciones as $id => $nombre_infraccion) {
    $labels[] = check_plain($nombre_infraccion);
    $data[] = $listaResultados->filterBy(new FilterByEquality(array($id), FilterByEquality::INFRACCION_ID))
      ->count();
  }

  $form['rjsim_fieldset_infracciones'] = array(
    '#type' => 'fieldset',
    '#title' => t('Infractions percentage for all simulations.'),
    '#collapsible' => FALSE,
    '#description' => t('Shows most committed infractions in the Simulator.'),
    '#attributes' => array('class' => array('rjsim-margin'))
  );

  $form['rjsim_fieldset_infracciones']['chart_infracciones'] = array(
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
  $listaPartidasUsuario = $gestorSimulaciones->getUsuarioActual()
    ->retrieveAllPartidasByIdSimulacion($id_simulacion);
  $listaPartidasUsuario->sortBy('Fecha', 'ASC');

  // Lista de partidas de todos los usuarios para la simulación
  $dataRetriever = new ListaUsuariosDataRetriever($gestorSimulaciones->getListaTodosUsuarios());
  $listaPartidasTodos = $dataRetriever->retrieveAllPartidasByIdSimulacion($id_simulacion);

  $labelAxisX = array();
  $dataUsuario = array();
  $dataTodos = array();

  $ultimasPartidas = ($listaPartidasUsuario->count() - 10 > 0) ? $listaPartidasUsuario->count() - 10 : 0;
  foreach (new LimitIterator($listaPartidasUsuario, $ultimasPartidas) as $partida) {
    $labelAxisX[] = t('Game @fecha', array(
      '@fecha' => $partida->getFechaAsObject()
        ->format('d-m-Y H:i:s')
    ));

    $dataUsuario[] = $partida->getConsumoMedio();

    $listaResultado = $listaPartidasTodos->filterBy(new FilterByFecha(array(FilterByFecha::FECHA_FIN => $partida->getFechaAsObject())));

    $dataTodos[] = $listaResultado->calculateData(new CalculateAverageData(CalculateAverageData::CONSUMO_MEDIO));
  }

  $form['rjsim_fieldset_consumo_medio'] = array(
    '#type' => 'fieldset',
    '#title' => t('Average consumption comparation.'),
    '#collapsible' => FALSE,
    '#description' => t('Comparation graph between average consumption by user and average consumption for simulation.'),
    '#attributes' => array('class' => array('rjsim-margin'))
  );

  $form['rjsim_fieldset_consumo_medio']['chart_consumo_medio'] = array(
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
    $labelAxisX[] = t('Game @fecha', array(
      '@fecha' => $partida->getFechaAsObject()
        ->format('d-m-Y H:i:s')
    ));

    $dataUsuario[] = $partida->getTiempoTotal();

    $listaResultado = $listaPartidasTodos->filterBy(new FilterByFecha(array(FilterByFecha::FECHA_FIN => $partida->getFechaAsObject())));

    $dataTodos[] = $listaResultado->calculateData(new CalculateAverageData(CalculateAverageData::TIEMPO_TOTAL));
  }

  $form['rjsim_fieldset_tiempo_medio'] = array(
    '#type' => 'fieldset',
    '#title' => t('Average time by Partida comparation.'),
    '#collapsible' => FALSE,
    '#description' => t('Comparation graph between average total time by user and all users.'),
    '#attributes' => array('class' => array('rjsim-margin'))
  );

  $form['rjsim_fieldset_tiempo_medio']['chart_tiempo_medio'] = array(
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
  $listaPartidas = $gestorSimulaciones->getUsuarioActual()
    ->retrieveAllPartidasByIdSimulacion($id_simulacion);
  $listaPartidas->sortBy('Fecha', 'ASC');

  $tiposInfracciones = Constants::getTiposInfracciones();

  $labelAxisX = array();
  $data = array();

  //Recuperamos las X últimas partidas
  $numeroPartidasAMostrar = 10;
  $partidaInicio = ($listaPartidas->count() - $numeroPartidasAMostrar < 0) ? 0 : $listaPartidas->count() - $numeroPartidasAMostrar;

  foreach (new LimitIterator($listaPartidas, $partidaInicio) as $partida) {
    foreach ($tiposInfracciones as $id => $nombre_infraccion) {
      $data[$id][] = $partida->getListaInfracciones()
        ->filterBy(new FilterByEquality(array($id), FilterByEquality::INFRACCION_ID))
        ->count();
    }

    $labelAxisX[] = t('Game @fecha', array(
      '@fecha' => $partida->getFechaAsObject()
        ->format('d-m-Y H:i:s')
    ));
  }

  $form['rjsim_fieldset_infracciones_simulacion'] = array(
    '#type' => 'fieldset',
    '#title' => t('Number of Infractions charts.'),
    '#collapsible' => FALSE,
    '#description' => t('Number of infractions in column stacked charts in last played games.'),
    '#attributes' => array('class' => array('rjsim-margin'))
  );

  $form['rjsim_fieldset_infracciones_simulacion']['chart_infracciones_simulacion'] = array(
    '#type' => 'chart',
    '#chart_type' => 'column',
    '#stacking' => TRUE,
    '#chart_library' => 'highcharts',
    '#title' => t('Number of infractions in last played games'),
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

  foreach ($tiposInfracciones as $id => $nombre_infraccion) {
    $form['rjsim_fieldset_infracciones_simulacion']['chart_infracciones_simulacion'][$id] =
      array(
        '#type' => 'chart_data',
        '#title' => t('@nombreInfraccion', array('@nombreInfraccion' => $nombre_infraccion)),
        '#data' => $data[$id],
      );
  }

  return $form;
}