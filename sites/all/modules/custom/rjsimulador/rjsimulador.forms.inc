<?php
/* *************************************************************************** */
/* *                           LISTA SIMULACIONES                              */
/* *************************************************************************** */
function rjsim_simulaciones_upper_left_form($form, &$form_state, GestorSimulaciones $gestorSimulaciones) {
  // Creamos el gráfico para mostrar el número de partidas jugadas por simulación
  $arrayData = array();
  $xLabels = array();

  foreach ($gestorSimulaciones->getUsuarioActual()
             ->getListaSimulaciones() as $simulacion) {
    $arrayData[] = $simulacion->getListaPartidas()->count();
    $xLabels[] = $simulacion->getNombreSimulacion();
  }

  $form['chart_partidas_simulacion'] = array(
    '#type' => 'chart',
    '#chart_type' => 'column',
    '#chart_library' => 'highcharts',
    '#legend' => FALSE,
    '#title' => t('Número de partidas jugadas a cada Simulación'),
    '#title_font_weight' => 'bold',
    'simulation' => array(
      '#type' => 'chart_data',
      '#title' => 'Nº partidas jugadas',
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
        '#title' => 'Nº partidas',
      ),
  );

  // Creamos el gráfico para ver el número de partidas jugadas por día la última
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
    '#title' => t('Nº partidas jugadas por Simulación en los últimos ') . $ultimosXDias . t(' días'),
    '#title_font_weight' => 'bold',
    '#prefix' => '<div id="div-chart-partidas-ultimos-dias">',
    '#suffix' => '</div>',
    'xaxis' =>
      array(
        '#type' => 'chart_xaxis',
        '#title' => t('Fecha'),
        '#labels' => $xDayLabels,
      ),
    'yaxis' =>
      array(
        '#type' => 'chart_yaxis',
        '#title' => t('Nº partidas'),
      ),
  );

  foreach ($arrayPartidasJugadasPorDia as $idSimulacion => $arrayPartidasPorSimulacion) {
    $arrayPartidasPorSimulacion = array_reverse($arrayPartidasPorSimulacion);
    $form['chart_partidas_ultimos_dias'][$idSimulacion] = array(
      '#type' => 'chart_data',
      '#title' => t('Simulación ') . $idSimulacion,
      '#data' => $arrayPartidasPorSimulacion,
    );
  }

  $form['select_ultimos_dias'] = array(
    '#type' => 'select',
    '#title' => t('Días'),
    '#options' => array(
      7 => t('Últimos 7 días'),
      15 => t('Últimos 15 días'),
    ),
    '#default_value' => 7,
    '#description' => t('Set this to <em>Yes</em> if you would like this category to be selected by default.'),
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
  // Creamos el gráfico para mostrar el número medio de infracciones por partida y simulación
  $labelXAxis = array();

  // Creamos array de datos
  $dataUsuarioInfracciones = array();
  $dataTotalInfracciones = array();

  $dataUsuarioTiempo = array();
  $dataTotalTiempo = array();

  foreach ($gestorSimulaciones->getArraySimulaciones() as $idSimulacion => $nombreSimulacion) {
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

  $form['chart_infracciones_simulacion'] = array(
    '#type' => 'chart',
    '#chart_type' => 'column',
    '#chart_library' => 'highcharts',
    '#title' => t('Infracciones medias por partida y tipo de Simulación'),
    '#title_font_weight' => 'bold',
    'infracciones_usuario' => array(
      '#type' => 'chart_data',
      '#title' => t('Usuario ') . $gestorSimulaciones->getUsuarioActual()
          ->getName(),
      '#data' => $dataUsuarioInfracciones,
    ),
    'infracciones_totales' => array(
      '#type' => 'chart_data',
      '#title' => t('Todos los usuarios'),
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
        '#title' => 'Nº medio de infracciones por partida'
      )
  );


  $form['chart_tiempo_por_partida_simulacion'] = array(
    '#type' => 'chart',
    '#chart_type' => 'column',
    '#chart_library' => 'highcharts',
    '#title' => t('Tiempos medios por partida y tipo de Simulación'),
    '#title_font_weight' => 'bold',
    'tiempo_usuario' => array(
      '#type' => 'chart_data',
      '#title' => t('Usuario ') . $gestorSimulaciones->getUsuarioActual()
          ->getName(),
      '#data' => $dataUsuarioTiempo,
    ),
    'tiempos_totales' => array(
      '#type' => 'chart_data',
      '#title' => t('Todos los usuarios'),
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
        '#title' => 'Tiempo medio por partida (sg.)'
      )
  );

  return $form;
}

function rjsim_simulaciones_main_form($form, &$form_state, GestorSimulaciones $gestorSimulaciones) {
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
    $data[] = $listaResultados->filterBy(new FilterByID(array($id)))->count();
  }

  $form['chart_infracciones'] = array(
    '#type' => 'chart',
    '#title' => t("Porcentaje infracciones cometidas totales"),
    '#title_font_weight' => 'bold',
    '#chart_type' => 'pie',
    '#chart_library' => 'highcharts',
    '#legend_position' => 'right',
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

  return $form;
}

/* *************************************************************************** */
/* *                    PARTIDAS POR TIPO DE SIMULACIÓN                        */
/* *************************************************************************** */
function rjsim_partidas_simulacion_upper_left_form($form, &$form_state, GestorSimulaciones $gestorSimulaciones, $id_simulacion) {
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
    '#title' => t('Consumo medio usuario vs Consumo medio Simulación'),
    'usuario' =>
      array(
        '#type' => 'chart_data',
        '#title' => t('Usuario'),
        '#data' => $dataUsuario,
      ),
    'todos' =>
      array(
        '#type' => 'chart_data',
        '#title' => t('Todos'),
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
    '#title' => t('Tiempo medio usuario vs Tiempo medio por Simulación'),
    'usuario' =>
      array(
        '#type' => 'chart_data',
        '#title' => t('Usuario'),
        '#data' => $dataUsuario,
      ),
    'todos' =>
      array(
        '#type' => 'chart_data',
        '#title' => t('Todos'),
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
        ->filterBy(new FilterByID(array($id)))
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
    '#title' => t('Nº de infracciones en las últimas partidas'),
    'xaxis' =>
      array(
        '#type' => 'chart_xaxis',
        '#labels' => $labelAxisX
      ),
    'yaxis' =>
      array(
        '#type' => 'chart_yaxis',
        '#title' => 'Nº Infracciones'
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